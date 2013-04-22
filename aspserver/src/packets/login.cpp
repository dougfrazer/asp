//******************************************************************************
// Login Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "network/packets/login.h"
#include "network/packets/login_ack.h"

#include "client.h"
#include "world.h"

// TODO: don't depend on STL
#include <list>

static LOGIN_PACKET_HANDLER LoginHandler;

// TODO: messy... static list for connected clients...
typedef std::list<CLIENT*>::iterator ListIterator;
std::list<CLIENT*> ConnectedClients;

static CLIENT* FindConnectedClient(CLIENT* Client)
{
    for(ListIterator it = ConnectedClients.begin(); it != ConnectedClients.end(); it++) {
        if(*it == Client) return *it;
    }
    return null;
}

void LOGIN_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    //printf("[%d] Got a login request\n", UserId);
    DATA* Data = (DATA*)Buffer;
    CLIENT* Client = (CLIENT*)Context;
    if(Data == null || Client == null) {
        error("Invalid client context");
        return;
    }

    LOGIN_ACK_PACKET_HANDLER::DATA AckData;
    if(FindConnectedClient(Client) != null) {
       // printf("Client already logged in with id=%d\n", Data->UserId);
        AckData.Success = false;
        AckData.UserId = Data->UserId;
        AckData.x = 0;
        AckData.y = 0;
        AckData.Error = 1; // TODO: enumerate errors
    } else {
        //printf("Logging in client=%p to userid=%d\n", Context, Data->UserId);
        Client->UserId = Data->UserId;
        World_SetInitialPosition(Data->UserId);
        AckData.Success = true;
        AckData.UserId = Data->UserId;
        AckData.x = 0;
        AckData.y = 0;
        ConnectedClients.push_front(Client);
    }

    Client->Stream.AddPacket(StringHash("LOGIN_ACK"), sizeof(AckData), &AckData);
}

