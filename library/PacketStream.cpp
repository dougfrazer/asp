//*****************************************************************************
// Packet Stream
// -------------
//   See PacketStream.h
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************

#include "PacketStream.h"

//*****************************************************************************
void PACKET_STREAM::AddPacket(PACKET_HANDLER* Packet, void* Buffer)
{
    HEADER Header;
    Header.Size = Packet->GetSize();
    Header.Id   = Packet->GetId();
  
    assert(DataOffset + sizeof(Header) + Header.Size < sizeof(Data));

    Memcpy(&Data[DataOffset], &Header, sizeof(Header));
    DataOffset += sizeof(Header);
    Memcpy(&Data[DataOffset], &Header, Header.Size);
    DataOffset += Header.Size;
}
//*****************************************************************************
void PACKET_STREAM::RecievePackets(void* Buffer, size_t size)
{
    PACKET_HANDLER* Handler = null;

    while(size > 0) {
        Handler = PACKET_HANDLER::FindHandler(((HEADER*)Buffer)->Id);
        size -= sizeof(HEADER);
        Buffer += sizeof(HEADER);
        if(Handler == null) {
            error("Got a packet without a matching handler id=%d", Header->id);
        }
        Handler->Recieve(Buffer);
        size -= Handler->GetSize();
        Buffer += Handler->GetSize();
    }
    assert(size == 0);
}
//*****************************************************************************
