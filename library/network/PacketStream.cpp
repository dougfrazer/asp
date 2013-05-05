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

#include "sys/socket.h"

//*****************************************************************************
void PACKET_STREAM::AddPacket(PACKET_HANDLER* Packet, void* Buffer)
{
    AddPacket(Packet->GetId(), Packet->GetSize(), Buffer);    
}
//*****************************************************************************
void PACKET_STREAM::AddPacket(u32 Id, u8 Size, void* Buffer)
{
    HEADER Header;
    Header.Size = Size;
    Header.Id   = Id;
  
    assert(DataOffset + sizeof(Header) + Header.Size < sizeof(Data));

    Memcpy(&Data[DataOffset], &Header, sizeof(Header));
    DataOffset += sizeof(Header);
    Memcpy(&Data[DataOffset], Buffer, Header.Size);
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
        Buffer = (u8*)Buffer + sizeof(HEADER);
        size -= sizeof(HEADER);
        if(!Handler) {
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
void PACKET_STREAM::Transmit(int sockfd, sockaddr* Address)
{
    ssize_t size = sendto(sockfd, (void*)&Data[0], DataOffset, 0, Address, sizeof(*Address));
    assert(size == DataOffset);
    DataOffset = 0;
}
//*****************************************************************************
