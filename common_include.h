

#ifndef __COMMON_INCLUDE_H__
#define __COMMON_INCLUDE_H__

// macros
#define zero(a) memset(a, '\0', sizeof(*a))

// definitions
static const unsigned int PORTNUM=12345;
static const unsigned int MAX_RECV_LEN=1024;

// errors
#define EOK 0
#define ASP_ESLEEP 0x123001
#define ASP_EMORE  0x123002

// types
#ifndef CERRNO
#define CERRNO
typedef int cerrno;
#endif

#endif
