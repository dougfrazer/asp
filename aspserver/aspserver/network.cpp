//
//  network.cpp
//  aspserver
//
//  Created by Douglas Frazer on 1/2/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <assert.h>
#include <list>
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
#include "networkthread.h"
#include "asppacket.h"

static struct {
    int sockfd;
    std::list<NetworkThread*> Threads;
} NetworkData;


//*******************************************************************************
// Public Interface
//*******************************************************************************
void Network_Init()
{
    cerrno rc = EOK;
    NetworkData.sockfd = -1;
    struct sockaddr_in Server;
    
    zero(&Server);
    Server.sin_family = AF_INET;
    Server.sin_port = htons(PORTNUM);
    Server.sin_addr.s_addr = INADDR_ANY;
    
    // establish the socket
    NetworkData.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( fcntl(NetworkData.sockfd, F_SETFL, O_NONBLOCK) < 0 ) {
        printf("error setting non-blocking flag %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    assert(NetworkData.sockfd > 0 && errno == EOK);
    
    // bind the socket
    rc = bind(NetworkData.sockfd, (struct sockaddr*)&Server, sizeof(Server));
    assert(rc >= 0 && errno == EOK);
    
    // listen for incoming messages, up to 1 pending connection
    listen(NetworkData.sockfd, 1);
}
//*******************************************************************************
void Network_Update(float DeltaTime)
{
    struct sockaddr_in Client;
    socklen_t socksize = sizeof(struct sockaddr_in);
    
    // update existing connections
    for (std::list<NetworkThread*>::iterator it=NetworkData.Threads.begin(); it!=NetworkData.Threads.end(); ++it) {
        NetworkThread* thread = *it;
        if(thread->IsDone()) {
            delete(thread);
            NetworkData.Threads.remove(thread);
        }
    }
    
    // listen for new connections
    int ConnectedSocket = accept(NetworkData.sockfd, (struct sockaddr*)&Client, &socksize);
    if(errno == EWOULDBLOCK) return;
    if(ConnectedSocket < 0) {
        printf("[%s:%d] Error %d (%s)\n", __FILE__, __LINE__, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("Incoming connection from %s\n", inet_ntoa(Client.sin_addr));
    
    // spawn a new thread for the new connection
    NetworkThread* Thread = new NetworkThread(ConnectedSocket, Client);
    assert(Thread != NULL);
    NetworkData.Threads.push_front(Thread);
}
//*******************************************************************************
void Network_Deinit()
{
    
}

