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
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>

#include "common_include.h"

#include "network.h"
#include "networkthread.h"
#include "asppacket.h"
#include "world.h"


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
    
    Timers.Keepalive = KEEPALIVE_TIMER;
    Timers.KeepaliveTimeout = KEEPALIVE_TIMEOUT;
    Timers.PreviousTime = time(NULL);

    Terminated = false;

    UserId = 0;

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
       // printf("Updating thread=%lu\n", (unsigned long)pthread_self());
        UpdateTimers();
        
        // see if we have a packet to be recieved
        rc = RecievePacket(&Packet, &len);
        if(rc == ASP_ESLEEP || errno == EWOULDBLOCK) {
            sleep(SLEEP_TIMER);
            continue;
        } else if(Packet->Header.Type != NONE) {
            rc = ParsePacket(Packet, len);
            if(rc != EOK) {
                printf("[%lu] Error parsing packet error %d (%s)", (unsigned long)pthread_self(), errno, strerror(errno));
                Terminated = true;
                break;
            }
        } else {
            printf("[%s:%d] Error %d (V%s)", __FILE__, __LINE__, errno, strerror(errno));
            Terminated = true;
            break;
        }
    }
}
// *******************************************************************************
cerrno NetworkThread::RecievePacket(ASP_PACKET** OutPacket, long* outlen)
{
    zero(buffer);
    long len;

    struct pollfd fds;
    fds.fd = Data.sockfd;
    fds.events = POLLIN;
    if(poll(&fds, 1, 0) == 0) {
        // no data avaiable
        return ASP_ESLEEP;
    }
    
    // read the header
    len = read(Data.sockfd, (void *)buffer, sizeof(ASP_HEADER));
    if(len == -1) {
        return ENOTCONN;
    } else if(len == 0) {
        return ASP_ESLEEP;
    }

    // read the data if this packet contains any
    printf("[%lu] Recieved a packet of type %d length=%d\n", 
            (unsigned long)pthread_self(), 
            ((ASP_HEADER*)buffer)->Type,
            ((ASP_HEADER*)buffer)->Length); 
    len = read(Data.sockfd, (void*)&(((ASP_PACKET*)buffer)->Body), ((ASP_PACKET*)buffer)->Header.Length);
    if(len == -1) {
        return ENOTCONN;
    }
    
    *OutPacket = (ASP_PACKET*)buffer;
    *outlen = len;
    return ASP_EMORE;
}
// *******************************************************************************
cerrno NetworkThread::ParsePacket(ASP_PACKET* Packet, long len)
{
    if(len < Packet->Header.Length) {
        printf("Length read: %d   Length Expected %d\n", (int)len, (int)Packet->Header.Length);
        exit(EXIT_FAILURE);
    }
    switch(Packet->Header.Type)
    {
    case LOGIN:
        {
            ASP_LOGIN_PACKET* LoginPacket = (ASP_LOGIN_PACKET*)&Packet->Body;
            printf("[%lu] Got a LOGIN packet (%ld bytes): userId=%d\n", 
                    (unsigned long)pthread_self(), 
                    len,
                    LoginPacket->UserId);
            // TODO: this needs to check whether the user is logged in, once we
            //       have a proper user module...
            UserId = LoginPacket->UserId;
            SendLoginAck(true, 0);
        } break;

    case CLOSE_CONNECTION:
        {
            printf("[%lu] Got a CLOSE_CONNECTION packet (%ld bytes)\n",
                    (unsigned long)pthread_self(),
                    len);
        } return ENOTCONN;
            
    case DIRECTION:
        {
            ASP_DIRECTION_PACKET* DirectionPacket = (ASP_DIRECTION_PACKET*)&Packet->Body;
            printf("[%lu] Got a directional packet (%ld bytes): %d %d userid=%d\n", 
                    (unsigned long)pthread_self(),
                    len,
                    DirectionPacket->Direction,
                    DirectionPacket->Magnitude,
                    UserId);
            assert(UserId != 0);
            SendDirectionAck((ASP_DIRECTION)DirectionPacket->Direction, DirectionPacket->Magnitude);
        } break;
            
    case KEEPALIVE:
        {
            printf("[%lu] Got a keepalive packet (%ld bytes)\n",
                    (unsigned long)pthread_self(),
                    len);
            Timers.KeepaliveTimeout = KEEPALIVE_TIMEOUT;
        } break;
            
    default:
        {
            printf("[%lu] Got a packet of type %d (%ld bytes)\n",
                    (unsigned long)pthread_self(),
                    Packet->Header.Type,
                    len);
        }
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
    struct pollfd fds;
    fds.fd = Data.sockfd;
    fds.events = POLLOUT;
    if(poll(&fds, 1, 10) == 0) {
        printf("[%lu] Socket not ready for write after 10ms\n", (unsigned long)pthread_self());
        return;
    }

    printf("[%lu] Sending a keepalive\n", (unsigned long)pthread_self());
    ASP_PACKET Packet;
    Packet.Header.Type = KEEPALIVE;
    Packet.Header.Length = 0;
    write(Data.sockfd, (void*)&Packet, sizeof(Packet));
}
// *******************************************************************************
void NetworkThread::SendLoginAck(bool Success, uint32_t Error)
{
    struct pollfd fds;
    fds.fd = Data.sockfd;
    fds.events = POLLOUT;
    if(poll(&fds, 1, 10) == 0) {
        printf("[%lu] Socket not ready for write after 10ms\n", (unsigned long)pthread_self());
        return;
    }

    ASP_PACKET* Packet = (ASP_PACKET*)buffer;
    Packet->Header.Type = LOGIN_ACK;
    Packet->Header.Length = sizeof(ASP_LOGIN_ACK_PACKET);

    ASP_LOGIN_ACK_PACKET* LoginAck = (ASP_LOGIN_ACK_PACKET*)&Packet->Body;
    World_SetInitialPosition(UserId, &LoginAck->x, &LoginAck->y);
    LoginAck->Successful = Success && UserId != 0;
    LoginAck->Error = Error;
    LoginAck->UserId = UserId;
    printf("[%lu] Sending a LOGIN ACK userId=%d position=(%d,%d)\n", (unsigned long)pthread_self(), UserId, LoginAck->x, LoginAck->y);
    write(Data.sockfd, (void*)buffer, sizeof(ASP_HEADER) + sizeof(ASP_LOGIN_ACK_PACKET));
}
// *******************************************************************************
void NetworkThread::SendDirectionAck(ASP_DIRECTION Direction, uint32_t Magnitude)
{
    struct pollfd fds;
    fds.fd = Data.sockfd;
    fds.events = POLLOUT;
    if(poll(&fds, 1, 10) == 0) {
        printf("[%lu] Socket not ready for write after 10ms\n", (unsigned long)pthread_self());
        return;
    }

    ASP_PACKET* Packet = (ASP_PACKET*)buffer;
    Packet->Header.Type = DIRECTION_ACK;
    Packet->Header.Length = sizeof(ASP_DIRECTION_ACK_PACKET);

    ASP_DIRECTION_ACK_PACKET* DirectionAck = (ASP_DIRECTION_ACK_PACKET*)&Packet->Body;
    World_SetPosition(Direction, Magnitude, UserId, &DirectionAck->x, &DirectionAck->y);
    DirectionAck->UserId = UserId;
    printf("[%lu] Sending a DIRECTION ACK (%d,%d) userId=%d\n", (unsigned long)pthread_self(), DirectionAck->x, DirectionAck->y, UserId);
    write(Data.sockfd, (void*)buffer, sizeof(ASP_HEADER) + sizeof(ASP_DIRECTION_ACK_PACKET));
}
// *******************************************************************************
bool NetworkThread::IsDone()
{
    return Terminated || Timers.KeepaliveTimeout < 0.0;
}
// *******************************************************************************
