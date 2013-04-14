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
void PACKET_STREAM::RecievePackets(void* Buffer, size_t size, void* Context)
{
    HEADER* Header = null;
    PACKET_HANDLER* Handler = null;

    while(size > 0) {
        Header = (HEADER*)Buffer;
        Handler = PACKET_HANDLER::FindHandler(Header->Id);
        Buffer = Header + 1;
        size -= sizeof(HEADER);
        if(Handler == null) {
            error("Got a packet without a matching handler id=%d", Header->id);
        } else {
            Handler->Recieve(Buffer, Context);
            assert(Handler->GetSize() == Header->Size);
        }
        Buffer = (u8*)Buffer + Header->Size;
        size -= Header->Size;
    }
    assert(size == 0);
}
//*****************************************************************************
