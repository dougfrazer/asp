

#ifndef __COMMON_INCLUDE_H__
#define __COMMON_INCLUDE_H__

// macros
#define zero(a) memset(a, '\0', sizeof(*a))

#define clamp(x, min, max) ( x > max ? max : x < min ? min : x )
#define countof(x) (sizeof(x)/sizeof(x[0])

// definitions
static const unsigned int PORTNUM=12345;
static const unsigned int MAX_RECV_LEN=1024;
static const unsigned int WORLD_SIZE=10;
static const float SEND_RATE = 1.0/20.0;

// errors
#define EOK 0
#define ASP_ESLEEP 0x123001
#define ASP_EMORE  0x123002

// enums
enum ASP_DIRECTION
{
    NORTH,
    EAST,
    WEST,
    SOUTH,
};

struct Position {
    int x;
    int y;
};

#endif
