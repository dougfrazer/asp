//*******************************************************************************
// CLIENT
//   A container class for an individual client.  A client is defined by their
//   IP address and the main network class will maintain a mapping from IP to
//   CLIENT and pass messages directly to their assosciated clients.
//   
// @author Doug Frazer
// Copyright (c) Doug Frazer
// February 2013
//*******************************************************************************

#include <netinet/in.h>

#ifndef __CLIENT_H__
#define __CLIENT_H__

class CLIENT
{
public:
    CLIENT(sockaddr* Address, socklen_t AddressLength);
    ~CLIENT();

public:
    void ProcessData(char* Buffer, int Size);
    void GetWorldState();
    void GetSendBuffer(char** Buffer, size_t* Size);

    int UserId;
    sockaddr Address;
    socklen_t AddressLength;

private:
    void HandleKeepalive();
    void HandleDirection(ASP_DIRECTION_PACKET* Data);
    void HandleLogin(ASP_LOGIN_PACKET* Data);
    void QueuePacket(char* buffer, size_t size);

private:
    static const int TRANSMIT_SIZE = 1500;
    static const int BUFFER_SIZE = 3000;
    char SendBuffer[BUFFER_SIZE];
    int SendBufferSize;
};

#endif
