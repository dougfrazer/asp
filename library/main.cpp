
#include "ASPLib.h"
#include "string.h"
#include "stdio.h"

#include "PacketHandler.h"
#include "PacketStream.h"

class PACKET_HANDLER_TEST : public PACKET_HANDLER
{
public:
    struct DATA {
        u32 Value;
        u16 AnotherValue;
        u64 MoreValues;
    };
public:
    PACKET_HANDLER_TEST() {}
    ~PACKET_HANDLER_TEST() {}
public:
    u32 GetId()   { return StringHash("PACKET_HANDLER_TEST"); }
    u32 GetSize() { return sizeof(DATA); }

    void Recieve(void* Buffer, void* Context)
    {
        DATA* Data = (DATA*)Buffer;
        printf("[Data] %x\n", Data->Value);
        printf("[Data] %x\n", Data->AnotherValue);
        printf("[Data] %lx\n", Data->MoreValues);
    }
};

int main()
{
    PACKET_HANDLER_TEST Test;
    Test.Register();

    PACKET_STREAM Stream;
    PACKET_HANDLER_TEST::DATA Data;

    Data.Value = 0xabababab;
    Data.AnotherValue = 0xcdcd;
    Data.MoreValues = 0xefefefefefefefef;
    Stream.AddPacket(&Test, &Data);
    
    Data.Value = 0x12121212;
    Data.AnotherValue = 0x2323;
    Data.MoreValues = 0x3434343434343434;
    Stream.AddPacket(&Test, &Data);
    
    Data.Value = 0x56565656;
    Data.AnotherValue = 0x6767;
    Data.MoreValues = 0x8989898989898989;
    Stream.AddPacket(&Test, &Data);
    Stream.RecievePackets(&Stream.Data[0], Stream.DataOffset, null);
}
