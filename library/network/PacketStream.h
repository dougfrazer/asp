//*****************************************************************************
// Packet Stream
// -------------
//   A packet stream is a stream of data, represented by packet handlers.
//   
//   On recieve, the PACKET_STREAM will iterate through all the data it is
//   passed in and notify the appropriate handlers of a packet recieved.
//
//   On transmit, it will buffer data until either its buffer size is exceeded
//   or it reaches the end of the frame, when it will transmit the stream of
//   data collectively.
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************

#ifndef __PACKET_STREAM_H__
#define __PACKET_STREAM_H__

#include "ASPLib.h"
#include "PacketHandler.h"

class PACKET_STREAM
{
public:
    PACKET_STREAM() { DataOffset = 0; } 
    virtual ~PACKET_STREAM() {}
public:
    void RecievePackets(void* Buffer, size_t size, void* Context);
    void AddPacket(PACKET_HANDLER* Packet, void* Buffer);
    
    struct HEADER {
        u32 Id;
        u8  Size;
    };
    uint DataOffset;
    char Data[1500];
};

#endif
