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
#include <sys/poll.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network.h"
#include "includes/common_include.h"
#include "includes/asppacket.h"
#include "keyboard.h"
#include "world.h"

// TODO: get rid of static memory
static struct {
    int sockfd;
    struct sockaddr_in Destination;
    socklen_t          DestinationLength;
    struct sockaddr_in RecievedFrom;
    socklen_t          RecievedFromLength;
    uint32_t LoginId;
    bool LoginConfirmed;
} NetworkData;

static void Network_SendBack()    { Network_SendDirectionPacket(SOUTH, 1); }
static void Network_SendForward() { Network_SendDirectionPacket(NORTH, 1); }
static void Network_SendLeft()    { Network_SendDirectionPacket(WEST,  1); }
static void Network_SendRight()   { Network_SendDirectionPacket(EAST,  1); } 
// *****************************************************************************
static void Network_SendKeepaliveResponse()
{
    ASP_PACKET Packet;
    Packet.Header.Type = KEEPALIVE;
    Packet.Header.Length = 0; // no body

    ssize_t size = sendto(NetworkData.sockfd,
                            (void*)&Packet, 
                            sizeof(Packet), 
                            0, 
                            (struct sockaddr*)&NetworkData.Destination, 
                            sizeof(NetworkData.Destination));
    assert(size > 0);
}
// *****************************************************************************
static void Network_SendLoginRequest(uint32_t UserId)
{
    char Buffer[1024];
    ASP_PACKET* Packet = (ASP_PACKET*)Buffer;
    Packet->Header.Type = LOGIN;
    Packet->Header.Length = sizeof(ASP_LOGIN_PACKET);

    ASP_LOGIN_PACKET* LoginPacket = (ASP_LOGIN_PACKET*)&Packet->Body;
    LoginPacket->UserId = UserId;

    ssize_t size = sendto(NetworkData.sockfd, 
                        (void*)&Buffer, 
                        sizeof(ASP_HEADER) + sizeof(ASP_LOGIN_PACKET),
                        0,
                        (struct sockaddr*)&NetworkData.Destination,
                        sizeof(NetworkData.Destination));
    assert(size > 0);
}
// *****************************************************************************
static void Network_ProcessDirectionAckPacket(ASP_DIRECTION_ACK_PACKET* Data)
{
    printf("Recieved a DirectionAck packet [User=%d | x=%d | y=%d]\n", Data->UserId, Data->x, Data->y);
    World_SetPosition( Data->x, Data->y, Data->UserId );
}
// *****************************************************************************
static void Network_ProcessLoginAckPacket(ASP_LOGIN_ACK_PACKET* Data)
{
    printf("Recieved a LOGIN_ACK Packet\n");
    if(Data->Success) {
        printf("Login successful as userid=%d position=(%d,%d)\n", Data->UserId, Data->x, Data->y);
        NetworkData.LoginId = Data->UserId;
        NetworkData.LoginConfirmed = true;
        World_SetPosition(Data->x, Data->y, Data->UserId);
    } else {
        printf("Login unnsuccessful, (error %d) trying with a userid=%d\n", Data->Error, Data->UserId+1);
        Network_SendLoginRequest( Data->UserId + 1 );
    }
}
// *****************************************************************************
static void Network_ProcessKeepalivePacket()
{
    printf("Recieved a keepalive packet\n");
    Network_SendKeepaliveResponse();
}
// *****************************************************************************
void Network_Init()
{
    NetworkData.sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
    assert(NetworkData.sockfd > 0);
    
    zero(&NetworkData.Destination);
    NetworkData.Destination.sin_family = AF_INET;
    NetworkData.Destination.sin_addr.s_addr = inet_addr("127.0.0.1");
    NetworkData.Destination.sin_port = htons(PORTNUM);
    
    connect(NetworkData.sockfd, (struct sockaddr*)&NetworkData.Destination, sizeof(NetworkData.Destination));

    Keyboard_RegisterEvent('a', Network_SendLeft);
    Keyboard_RegisterEvent('s', Network_SendBack);
    Keyboard_RegisterEvent('d', Network_SendRight);
    Keyboard_RegisterEvent('w', Network_SendForward);
}
// *****************************************************************************
static void Network_Read(char* buffer, int size, ssize_t* outlen)
{
    *outlen = recvfrom(NetworkData.sockfd,
                        (void*)buffer,
                        size,
                        0,
                        (struct sockaddr*)&NetworkData.RecievedFrom,
                        &NetworkData.RecievedFromLength);

    if(errno == EAGAIN) {
        return;
    } else {
        printf("Error occured recieving data %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
// *****************************************************************************
static void Network_ProcessPackets(char* Data, long Size)
{
    ASP_PACKET* Packet = (ASP_PACKET*)(Data);
    while(Size > 0) {
        Size -= sizeof(ASP_HEADER) + Packet->Header.Length;
        switch(Packet->Header.Type)
        {
            case KEEPALIVE:
                {
                    Network_ProcessKeepalivePacket();
                    Packet = Packet + 1;
                } break;
            case LOGIN_ACK:
                {
                    ASP_LOGIN_ACK_PACKET* Data = (ASP_LOGIN_ACK_PACKET*)(Packet + 1);
                    Network_ProcessLoginAckPacket(Data);
                    Packet = (ASP_PACKET*)(Data + 1);
                } break;
            case DIRECTION_ACK:
                {
                    ASP_DIRECTION_ACK_PACKET* Data = (ASP_DIRECTION_ACK_PACKET*)(Packet + 1);
                    Network_ProcessDirectionAckPacket(Data);
                    Packet = (ASP_PACKET*)(Data + 1);
                } break;
            case NONE:
            case LOGIN:
            case DIRECTION:
                printf("Error client sent a server-only packet type=%d\n", Packet->Header.Type);
                return;
            default:
                break;
        }
    }
    assert(Size == 0);
}
// *****************************************************************************
void Network_Update(float DeltaTime)
{
    if(NetworkData.LoginId == 0) {
        Network_SendLoginRequest(1);
        NetworkData.LoginId = 1;
        NetworkData.LoginConfirmed = false;
    }
    char buffer[MAX_RECV_LEN];
    ssize_t len = 0;
    //Network_SendTestPacket();
    do {
		Network_Read(buffer, sizeof(buffer), &len);    
		if(len > 0) Network_ProcessPackets(buffer, len);
    } while( len > 0);
}
// *****************************************************************************
void Network_Deinit()
{
    // stop both transmission and reception
    shutdown(NetworkData.sockfd, 2);
}
// *****************************************************************************
void Network_SendDirectionPacket(ASP_DIRECTION direction, int magnitude)
{
    uint32_t x, y;
    char buffer[MAX_RECV_LEN];
    
    bool MoveSuccessful = World_AttemptMovement(direction, magnitude, NetworkData.LoginId, &x, &y);
    if(!MoveSuccessful) return; // TODO: send it anyway?

    ASP_PACKET* Packet = (ASP_PACKET*)&buffer;
    ASP_DIRECTION_PACKET* DirectionalPacket = (ASP_DIRECTION_PACKET*)&Packet->Body;
    Packet->Header.Type = DIRECTION;
    Packet->Header.Length = sizeof(ASP_DIRECTION_PACKET);
    DirectionalPacket->x = x;
    DirectionalPacket->y = y;
    
    ssize_t size = sendto(NetworkData.sockfd, 
                            buffer, 
                            sizeof(ASP_HEADER) + sizeof(ASP_DIRECTION_PACKET), 
                            0,
                            (struct sockaddr*)&NetworkData.Destination, 
                            sizeof(NetworkData.Destination));
    if(size <= 0) {
        printf("Error sending direction packet %d (%s) sockfd=%d\n", errno, strerror(errno), NetworkData.sockfd);
    }
    assert(size > 0);
}
// *****************************************************************************
