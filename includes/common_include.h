

#ifndef __COMMON_INCLUDE_H__
#define __COMMON_INCLUDE_H__

// definitions
static const unsigned int PORTNUM=12345;
static const unsigned int MAX_RECV_LEN=1024;
static const unsigned int WORLD_SIZE=100;
static const float SEND_RATE = 1.0/20.0;

// errors
#define EOK 0
#define ASP_ESLEEP 0x123001
#define ASP_EMORE  0x123002

#endif
