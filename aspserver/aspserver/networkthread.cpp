//
//  networkthread.cpp
//  aspserver
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "common_include.h"

#include "network.h"
#include "networkthread.h"
#include "asppacket.h"


// *******************************************************************************
static void* NetworkThread_StartFunction(void* ptr)
{
    NetworkThread* thread = (NetworkThread*)ptr;
    thread->Start();
    return NULL;
}
// *******************************************************************************
NetworkThread::NetworkThread(int sockfd, struct sockaddr_in Address)
{
    printf("[%lu] Creating thread for sockfd=%d\n", (unsigned long)pthread_self(), sockfd);
    cerrno rc = EOK;
    
    Data.sockfd = sockfd;
    Data.address = Address;

    rc = pthread_create(&Thread, NULL, NetworkThread_StartFunction, (void*)this);
    assert(rc == EOK);
}
// *******************************************************************************
NetworkThread::~NetworkThread()
{
    printf("[%lu] Killing thread for sockfd=%d\n", (unsigned long)pthread_self(), Data.sockfd);
    cerrno rc = EOK;
    rc = pthread_cancel(Thread);
    if(rc != EOK) {
        printf("[%lu] Error destroying NetworkThread rc=%d (%s) errno=%d (%s)\n", (unsigned long)pthread_self(), rc, strerror(rc), errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
// *******************************************************************************
void NetworkThread::Start()
{
    cerrno rc = EOK;
    ASP_PACKET* Packet = NULL;
    long len = -1;
    Timers.Keepalive = KEEPALIVE_TIMER;
    Timers.KeepaliveTimeout = KEEPALIVE_TIMEOUT;
    Timers.PreviousTime = time(NULL);
    
    while(true) {
        // handle keeplive
        //printf("Updating thread=%lu\n", (unsigned long)pthread_self());
        UpdateTimers();
        
        // see if we have a packet to be recieved
        rc = RecievePacket(&Packet, &len);
        if(rc == ASP_ESLEEP || errno == EWOULDBLOCK) {
            sleep(SLEEP_TIMER);
            continue;
        } else if(rc != EOK && rc != ASP_EMORE) {
            printf("[%s:%d] Error %d (%s)", __FILE__, __LINE__, errno, strerror(errno));
            Terminated = true;
            break;
        }
        
        // parse the recieved packet
        while(len > 0) {
            assert(Packet->Header.Length > 0);
            rc = ParsePacket(Packet, len);
            if(rc != EOK) {
                printf("[%lu] Error parsing packet error %d (%s)", (unsigned long)pthread_self(), errno, strerror(errno));
                Terminated = true;
                break;
            }
            Packet += Packet->Header.Length;
            len -= Packet->Header.Length; 
        } 
    }
}
// *******************************************************************************
cerrno NetworkThread::RecievePacket(ASP_PACKET** OutPacket, long* outlen)
{
    zero(buffer);
    long len = read(Data.sockfd, (void *)buffer, sizeof(buffer));
    
    if(len == -1) {
        return ENOTCONN;
    } else if(len == 0) {
        return ASP_ESLEEP;
    }
    
    *OutPacket = (ASP_PACKET*)buffer;
    *outlen = len;
    return len == sizeof(buffer) ? ASP_EMORE : EOK;
}
// *******************************************************************************
cerrno NetworkThread::ParsePacket(ASP_PACKET* Packet, long len)
{
    ASP_DIRECTION_PACKET* DirectionPacket;
    assert(len >= Packet->Header.Length);
    switch(Packet->Header.Type)
    {
    case CLOSE_CONNECTION:
        printf("[%lu] Got a CLOSE_CONNECTION packet (%ld bytes)\n", (unsigned long)pthread_self(), len);
        return ENOTCONN;
            
    case DIRECTION:
        DirectionPacket = (ASP_DIRECTION_PACKET*)&Packet->Body;
        printf("[%lu] Got a directional packet (%ld bytes): %d %d %d %d\n", 
                (unsigned long)pthread_self(),
                len,
                Packet->Header.Type, 
                Packet->Header.Length,
                DirectionPacket->Direction,
                DirectionPacket->Magnitude);
        break;
            
    case KEEPALIVE:
        printf("[%lu] Got a keepalive packet (%ld bytes)\n", (unsigned long)pthread_self(), len);
        Timers.KeepaliveTimeout = KEEPALIVE_TIMEOUT;
        break;
            
    default:
        printf("[%lu] Got a packet of type %d (%ld bytes)\n", (unsigned long)pthread_self, Packet->Header.Type, len);
        break;
    }
    return EOK;
}
// *******************************************************************************
void NetworkThread::UpdateTimers()
{
    Timers.CurrentTime = time(NULL);
    time_t DeltaTime = difftime(Timers.CurrentTime, Timers.PreviousTime);
    Timers.Keepalive -= DeltaTime;
    Timers.PreviousTime = Timers.CurrentTime;
    if(Timers.Keepalive < 0.0) {
        SendKeepalive();
        Timers.Keepalive = KEEPALIVE_TIMER;
    }
    
    time_t PrevTimeout = Timers.KeepaliveTimeout;
    Timers.KeepaliveTimeout -= DeltaTime;
    if(PrevTimeout >= 0.0 && Timers.KeepaliveTimeout < 0.0) {
        printf("[%lu] Thread exceeded Keepalive timeout\n", (unsigned long)pthread_self());
    }
}
// *******************************************************************************
void NetworkThread::SendKeepalive()
{
    ASP_PACKET Packet;
    Packet.Header.Type = KEEPALIVE;
    Packet.Header.Length = sizeof(ASP_PACKET);
    send(Data.sockfd, (void*)&Packet, sizeof(Packet), 0);
}
// *******************************************************************************
bool NetworkThread::IsDone()
{
    return Terminated || Timers.KeepaliveTimeout < 0.0;
}
// *******************************************************************************
