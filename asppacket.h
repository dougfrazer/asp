//*******************************************************************************
// ASP Packet
// 
//  Defines the structure of different packets we send over the wire.
//  Each packet will contain a header, which will contain a "length" which specifies
//  the number of bytes that follow for the body.  A header will also contain a
//  "type" field which will be used to describe the contents of the body.
//
//  It is valid for a packet to have no body, in which case the "length" is zero.
//
//  The current implementation only supports player movement.
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
    LOGIN,
    LOGIN_ACK,
    DIRECTION,
    DIRECTION_ACK,
    CLOSE_CONNECTION,
    PLAYER_UPDATE,
    KEEPALIVE,
};

/*******************************************************************************
 * struct ASP_HEADER
 *
 * This will appear at the head of every ASP packet, effectively acting as a
 * layer 5 header.  It will defined the type and length of data that remains in
 * the datagram.
 ******************************************************************************/
struct ASP_HEADER
{
    uint32_t     Type   : 8;
    uint32_t     Length : 8;
    uint32_t     Unused : 16;
};

/*******************************************************************************
 * struct ASP_PACKET
 *
 * The general structure of all packets.  A static-sized header followed by a
 * dynamically-sized body.  The size of the body is specified in the header.
 ******************************************************************************/
struct ASP_PACKET
{
    ASP_HEADER Header;
    char       Body[];
};


/*******************************************************************************
 * struct ASP_LOGIN_PACKET
 *
 * Ask the server to login using a specific user id
 ******************************************************************************/
struct ASP_LOGIN_PACKET
{
    uint32_t    UserId;
};

/*******************************************************************************
 * struct ASP_LOGIN_ACK_PACKET
 *
 * Server acknowledges your login request, returns whether it was successful
 * or not, potential error codes, and the userid you are logged in with.
 * Also, tell the client their initial position.
 ******************************************************************************/
struct ASP_LOGIN_ACK_PACKET
{
    uint32_t       UserId;
    uint32_t       Successful  : 1;
    uint32_t       Error       : 15;
    uint32_t       Unused      : 16;
    uint32_t       x;
    uint32_t       y;
};

/*******************************************************************************
 * struct ASP_DIRECTION_ACK_PACKET
 *
 * Sent from the server to acknowledge a directional packet and tell the client
 * what the server thinks its current position is.
 ******************************************************************************/
struct ASP_DIRECTION_ACK_PACKET
{
    uint32_t    x;
    uint32_t    y;
    uint32_t    UserId;
};

/*******************************************************************************
 * struct ASP_DIRECTION_PACKET
 *
 * A request from the client to move a certain direction and magnitude.  The
 * client should recieve a DIRECTION_ACK packet following this request.
 ******************************************************************************/
struct ASP_DIRECTION_PACKET
{
    uint8_t    Direction;
    uint8_t    Magnitude;
};

#endif
