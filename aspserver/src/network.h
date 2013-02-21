//*******************************************************************************
// NETWORK
//   Handles the main network connection, listens on the known server port/ip
//   and dispatches messages to the connected clients
//   
// @author Doug Frazer
// December 2012
//*******************************************************************************

#include <list>
#include <tr1/unordered_map>
#include "hashmap.h"

#ifndef __NETWORK_H__
#define __NETWORK_H__

class CLIENT;

class NETWORK
{
public:
    NETWORK();
    ~NETWORK();

public:
    void Init();
    void Deinit();
    void Update(float DeltaTime);
    void SendPacket(char* buffer, size_t size, sockaddr* dest, socklen_t addrlen);

private:
    void TransmitPackets();

    typedef std::tr1::unordered_map<NetworkKey,CLIENT*>::const_iterator MapIterator;
    std::tr1::unordered_map<NetworkKey,CLIENT*,NetworkKeyHash,NetworkKeyEq> HashMap;

    std::list<CLIENT*> ClientList;
    typedef std::list<CLIENT*>::iterator ListIterator;

    int sockfd;
    char RecvBuffer[1500];
    float Timer;
};

// C-style interface
void Network_Init();
void Network_Deinit();
void Network_Update(float DeltaTime);
void Network_SendPacket(char* buffer, size_t size, sockaddr* dest, socklen_t addrlen);
void Network_RequestTransmit();

#endif
