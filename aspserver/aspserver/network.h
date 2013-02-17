//*******************************************************************************
// NETWORK
//   Handles the main network connection, listens on the known server port/ip
//   and dispatches messages to the connected clients
//   
// @author Doug Frazer
// December 2012
//*******************************************************************************

#include <unordered_map>
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
    typedef std::unordered_map<NetworkKey,CLIENT*>::const_iterator MapIterator;
    std::unordered_map<NetworkKey,CLIENT*,NetworkKeyHash,NetworkKeyEq> HashMap;
    int sockfd;
    char Buffer[1500];
};

// C-style interface
void Network_Init();
void Network_Deinit();
void Network_Update(float DeltaTime);
void Network_SendPacket(char* buffer, size_t size, sockaddr* dest, socklen_t addrlen);

#endif
