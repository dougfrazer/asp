

#ifndef __COMMON_INCLUDE_H__
#define __COMMON_INCLUDE_H__

// macros
#define zero(a) memset(a, '\0', sizeof(*a))

/* LinearInterpolate:
 * take the value X and translate (within the range x0-x1) to the range y0-y1
 *
 *  y - y0     y1 - y0
 * -------- = ---------
 *  x - x0     x1 - x0
 */
#define LinearInterpolate(x, x0, x1, y0, y1) ( y0+ ( (float)(y1-y0) * ( (float)(x-x0)/(float)(x1-x0) ) ) )
#define clamp(x, min, max) ( x > max ? max : x < min ? min : x )
#define countof(x) (sizeof(x)/sizeof(x[0])

// definitions
static const unsigned int PORTNUM=12345;
static const unsigned int MAX_RECV_LEN=1024;
static const unsigned int WORLD_SIZE=5;

// errors
#define EOK 0
#define ASP_ESLEEP 0x123001
#define ASP_EMORE  0x123002

// types
#ifndef CERRNO
#define CERRNO
typedef int cerrno;
#endif

// enums
enum ASP_DIRECTION
{
    NORTH,
    EAST,
    WEST,
    SOUTH,
};

#endif
