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

private:
    void HandleKeepalive();
    void HandleDirection(ASP_DIRECTION_PACKET* Data);
    void HandleLogin(ASP_LOGIN_PACKET* Data);

private:
    int UserId;
    sockaddr Address;
    socklen_t AddressLength;
};

#endif
