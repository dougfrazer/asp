//
//  networkthread.h
//  aspserver
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#ifndef aspserver_networkthread_h
#define aspserver_networkthread_h

#include <pthread.h>

struct sockaddr_in;
struct ASP_PACKET;

class NetworkThread {
public:
    NetworkThread(int sockfd, struct sockaddr_in Address);
    ~NetworkThread();
    
public:
    void Start();
    bool IsDone();
    
private:
    // functions
    cerrno RecievePacket(ASP_PACKET** Packet, long* len);
    cerrno ParsePacket(ASP_PACKET* Packet, long len);
    void   SendKeepalive();
    void   UpdateTimers();
    
    // data
    struct {
        time_t Keepalive;
        time_t CurrentTime;
        time_t PreviousTime;
        time_t KeepaliveTimeout;
    } Timers;
    
    struct {
        int sockfd;
        sockaddr_in address;
    } Data;
    
    pthread_t Thread;
    
    char buffer[MAX_RECV_LEN];
    
    bool Terminated;
    
private:
    static const uint SLEEP_TIMER = 1; // seconds
    static const float KEEPALIVE_TIMER = 5.0; // seconds
    static const float KEEPALIVE_TIMEOUT = 20.0; // seconds
};

#endif
