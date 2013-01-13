//
//  network.cpp
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network.h"
#include "common_include.h"
#include "asppacket.h"
#include "keyboard.h"

static struct {
    int sockfd;
    struct sockaddr_in Destination;
} NetworkData;

static void Network_SendBack()    { Network_SendDirectionPacket(1, 5); }
static void Network_SendForward() { Network_SendDirectionPacket(2, 5); }
static void Network_SendLeft()    { Network_SendDirectionPacket(3, 5); }
static void Network_SendRight()   { Network_SendDirectionPacket(4, 5); } 
// *****************************************************************************
static void Network_SendKeepaliveResponse()
{
    ASP_PACKET Packet;
    Packet.Header.Type = KEEPALIVE;
    Packet.Header.Length = sizeof(ASP_HEADER);

    ssize_t size = send(NetworkData.sockfd, (void*)&Packet, sizeof(Packet), 0);
    assert(size > 0);
}
// *****************************************************************************
static void Network_ProcessPacket(ASP_PACKET* Packet, long len)
{
    switch(Packet->Header.Type)
    {
    case KEEPALIVE: Network_SendKeepaliveResponse(); break;
    default: printf("Recieved a packet of type %d (%ld bytes)\n", Packet->Header.Type, len);
    }
}
// *****************************************************************************
static void Network_SendTestPacket()
{
    Network_SendDirectionPacket(10, 20);
}
// *****************************************************************************
void Network_Init()
{
    NetworkData.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(NetworkData.sockfd > 0);
    
    zero(&NetworkData.Destination);
    NetworkData.Destination.sin_family = AF_INET;
    NetworkData.Destination.sin_addr.s_addr = inet_addr("127.0.0.1");
    NetworkData.Destination.sin_port = htons(PORTNUM);
    
    int rc = connect(NetworkData.sockfd, (struct sockaddr*)&NetworkData.Destination, sizeof(NetworkData.Destination));
    if(errno != EOK || rc == -1) {
        printf("Error connecting to server %d (%s)\n", errno, strerror(errno));
        close(NetworkData.sockfd);
        NetworkData.sockfd = -1;
    }

    int status;
    status = fcntl(NetworkData.sockfd, F_GETFL, 0);
    if(status != -1) {
        status |= O_NONBLOCK;
        status = fcntl(NetworkData.sockfd, F_SETFL, status);
    }
    if(status == -1 || errno != EOK) {
        printf("Error getting file descriptor %d (%s)\n", errno, strerror(errno));
        close(NetworkData.sockfd);
        NetworkData.sockfd = -1;
    }

    printf("Successfully opened TCP connection to server\n");    
    Network_SendTestPacket();

    Keyboard_RegisterEvent('a', Network_SendLeft);
    Keyboard_RegisterEvent('s', Network_SendBack);
    Keyboard_RegisterEvent('d', Network_SendRight);
    Keyboard_RegisterEvent('w', Network_SendForward);
}
// *****************************************************************************
static void Network_Read(char* buffer, int size, ssize_t* outlen)
{
    do {
        *outlen = read(NetworkData.sockfd, (void*)buffer, sizeof(buffer));
    } while(*outlen == -1 && errno == EINTR);

    if(*outlen == -1 && errno != EAGAIN) {
        printf("Error occured recieving data %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    } 
}
// *****************************************************************************
void Network_Update(float DeltaTime)
{
    char buffer[MAX_RECV_LEN];
    ssize_t len;
    Network_Read(buffer, sizeof(buffer), &len);    
    if(len > 0) Network_ProcessPacket((ASP_PACKET*)buffer, len);
}
// *****************************************************************************
void Network_Deinit()
{
    // stop both transmission and reception
    shutdown(NetworkData.sockfd, 2);
}
// *****************************************************************************
void Network_SendDirectionPacket(int direction, int magnitude)
{
    char buffer[MAX_RECV_LEN];
    
    ASP_PACKET Packet;
    ASP_DIRECTION_PACKET DirectionalPacket;
    Packet.Header.Type = DIRECTION;
    Packet.Header.Length = sizeof(ASP_HEADER) + sizeof(ASP_DIRECTION_PACKET);
    DirectionalPacket.Direction = direction;
    DirectionalPacket.Magnitude = magnitude;
    memcpy(&Packet.Body, &DirectionalPacket, sizeof(DirectionalPacket));
    memcpy(buffer, &Packet, sizeof(ASP_HEADER) + sizeof(ASP_DIRECTION_PACKET));
    
    ssize_t size = send(NetworkData.sockfd, buffer, sizeof(ASP_HEADER) + sizeof(ASP_DIRECTION_PACKET), 0);
    assert(size > 0);
}
// *****************************************************************************
