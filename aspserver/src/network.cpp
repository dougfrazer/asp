//*****************************************************************************
// Network
// -------
//   See network.h
//
// (c) Janurary 2013
//
// @author Doug Frazer
//*****************************************************************************

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

#include "includes/common_include.h"

#include "network.h"
#include "includes/asppacket.h"
#include "client.h" 

#include <list>

//*******************************************************************************
// Static Data
//*******************************************************************************
typedef std::list<CLIENT*>::iterator ListIterator;
std::list<CLIENT*> ClientList;
char RecvBuffer[1500];

static struct {
    int sockfd;
    float Timer;
} NetworkData;

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static void Network_TransmitPackets();
static CLIENT* Network_FindClient(sockaddr_in Addr);
static void Network_AddClient(CLIENT* Client);
//*******************************************************************************

//*******************************************************************************
// C-Style interface
//*******************************************************************************
void Network_Init()
{
    printf("Initializing Network...\n");
    struct sockaddr_in Server;
    
    Memset(&Server, (u8)0, sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_port = htons(PORTNUM);
    Server.sin_addr.s_addr = INADDR_ANY;
    
    // establish the socket
    NetworkData.sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
    if(errno != EOK || NetworkData.sockfd <= 0) {
		printf("Failed to create socket\n");
        exit(EXIT_FAILURE);
    }
    
    // bind the socket
    if(bind(NetworkData.sockfd, (struct sockaddr*)&Server, sizeof(Server)) != EOK) {
		printf("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
    NetworkData.Timer = SEND_RATE;

    printf("Listening for new connections\n");

    PacketHandler_RegisterAll();
}
//*******************************************************************************
void Network_Deinit()
{
}
//*******************************************************************************
void Network_Update(float DeltaTime)
{
    long        len;
    sockaddr_in ClientAddr;
    socklen_t   socksize = sizeof(struct sockaddr_in);
    
    // See if its time to transmit our update of the world
    NetworkData.Timer -= DeltaTime;
    if(NetworkData.Timer < 0) {
        NetworkData.Timer = SEND_RATE;
        Network_TransmitPackets();
    }

    // see if we got any data
	do {
		len = recvfrom(NetworkData.sockfd, RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr*)&ClientAddr, &socksize);
		if(len == 0) {
			return;
		} else if(len == -1) {
			if(errno != EAGAIN) {
				printf("Error recieving on main socket %d (%s)\n", errno, strerror(errno));
			}
			return;
		}

		// Lookup who this data belongs to
        CLIENT* Client = Network_FindClient(ClientAddr);
		if(Client != null) {
			printf("Incoming packet for %s (%ld bytes)\n", inet_ntoa(ClientAddr.sin_addr), len);
			Client->ProcessData(RecvBuffer, len); 
		} else {
			printf("New connection from %s:%d (%ld bytes)\n", inet_ntoa(ClientAddr.sin_addr), ClientAddr.sin_port, len);
			CLIENT* Client = new CLIENT((struct sockaddr*)&ClientAddr, socksize);
            Network_AddClient(Client);
			Client->ProcessData(RecvBuffer, len);
		}
	} while(len > 0);
}
//*******************************************************************************
void Network_RequestTransmit()
{
    // TODO
}
//*******************************************************************************


//*******************************************************************************
// Private Interface
//*******************************************************************************
static void Network_TransmitPackets()
{
    for(ListIterator it = ClientList.begin(); it != ClientList.end(); it++) {
        CLIENT* Client = *it;
        Client->GetWorldState();
        Client->Transmit(NetworkData.sockfd);
    }
}
//*******************************************************************************
static CLIENT* Network_FindClient(sockaddr_in Addr)
{
    // TODO: use the hashmap in the library
    for(ListIterator it = ClientList.begin(); it != ClientList.end(); it++) {
        CLIENT* Client = *it;
        sockaddr_in* InAddr = (sockaddr_in*)&Client->Address;
        if((InAddr->sin_addr.s_addr == Addr.sin_addr.s_addr) && (InAddr->sin_port == Addr.sin_port)) {
            return Client;
        }
    }
    return null;
}
//*******************************************************************************
static void Network_AddClient(CLIENT* Client)
{
    ClientList.push_front( Client );
}
//*******************************************************************************
