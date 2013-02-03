//*******************************************************************************
// ASP Packet
//
// @author Doug Frazer
// copyright (c) Doug Frazer
// December 2012
//*******************************************************************************V

#include <stdint.h>

#ifndef __ASPPACKET_H__
#define __ASPPACKET_H__

enum ASP_PACKET_TYPE
{
    NONE,
    DIRECTION,
    CLOSE_CONNECTION,
    KEEPALIVE,
};

struct ASP_HEADER
{
    uint32_t     Type   : 8;
    uint32_t     Length : 8;
    uint32_t     Unused : 16;
};

struct ASP_ACK_PACKET
{
    ASP_PACKET_TYPE  Response;
};

struct ASP_DIRECTION_PACKET
{
    uint8_t    Direction;
    uint8_t    Magnitude;
};

struct ASP_PACKET
{
    ASP_HEADER Header;
    char       Body[];
};

#endif
