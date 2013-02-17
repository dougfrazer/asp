//
//  network.cpp
//  aspserver
//
//  Created by Douglas Frazer on 1/2/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "common_include.h"

#include "network.h"
#include "asppacket.h"
#include "client.h" 

//*******************************************************************************
// Singleton Instance
//*******************************************************************************
static NETWORK Network;

//*******************************************************************************
// Constructor/Deconstructor
//*******************************************************************************
NETWORK::NETWORK()
{
    // what to do...
    sockfd = -1;
}
//*******************************************************************************
NETWORK::~NETWORK()
{
    // never called, this is a global singleton
}
//*******************************************************************************

//*******************************************************************************
// Public Interface
//*******************************************************************************
void NETWORK::Init()
{
    printf("Initializing Network...\n");
    cerrno rc = EOK;
    struct sockaddr_in Server;
    
    zero(&Server);
    Server.sin_family = AF_INET;
    Server.sin_port = htons(PORTNUM);
    Server.sin_addr.s_addr = INADDR_ANY;
    
    // establish the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(errno != EOK || sockfd <= 0) {
        exit(EXIT_FAILURE);
    }
    
    // bind the socket
    if(bind(sockfd, (struct sockaddr*)&Server, sizeof(Server)) != EOK) {
        exit(EXIT_FAILURE);
    }
    
    printf("Listening for new connections\n");

}
//*******************************************************************************
void NETWORK::Deinit()
{
    
}
//*******************************************************************************
void NETWORK::Update(float DeltaTime)
{
    NetworkKey Key;
    long len = 0;
    struct sockaddr_in ClientAddr;
    socklen_t socksize = sizeof(struct sockaddr_in);
    
    // see if we got any data
    len = recvfrom(sockfd, Buffer, sizeof(Buffer), 0, (struct sockaddr*)&ClientAddr, &socksize);
    if(len == 0) {
        return;
    } else if(len == -1) {
        printf("Error recieving on main socket %d (%s)\n", errno, strerror(errno));
        return;
    }
    
    // Lookup who this data belongs to
    Key.Address = ClientAddr.sin_addr;
    Key.Port = ClientAddr.sin_port;
    MapIterator Found = HashMap.find(Key);
    if(Found != HashMap.end()) {
        printf("Incoming packet for %s (%ld bytes)\n", inet_ntoa(ClientAddr.sin_addr), len);
        Found->second->ProcessData(Buffer, len); 
    } else {
        printf("Incoming connection from %s (%ld bytes)\n", inet_ntoa(ClientAddr.sin_addr), len);
        CLIENT* Client = new CLIENT((struct sockaddr*)&ClientAddr, socksize);
        HashMap.insert( { Key, Client } );
        Client->ProcessData(Buffer, len);
    }
}
//*******************************************************************************
void NETWORK::SendPacket(char* buffer, size_t size, sockaddr* dest, socklen_t addrlen)
{
    long len = sendto(sockfd, buffer, size, 0, dest, addrlen);
    if(len == -1 || errno != 0) {
        printf("Error writing to network device %d (%s)\n", errno, strerror(errno));
        return;
    }
}
//*******************************************************************************

//*******************************************************************************
// C-Style interface
//*******************************************************************************
void Network_Init()
{
    Network.Init();
}
//*******************************************************************************
void Network_Deinit()
{
    Network.Deinit();
}
//*******************************************************************************
void Network_Update(float DeltaTime)
{
    Network.Update(DeltaTime);
}
//*******************************************************************************
void Network_SendPacket(char* buffer, size_t size, sockaddr* dest, socklen_t addrlen)
{
    Network.SendPacket(buffer, size, dest, addrlen);
}
//*******************************************************************************
