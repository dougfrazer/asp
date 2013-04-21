
#include "ASPLib.h"
#include "string.h"
#include "stdio.h"

#include "PacketHandler.h"
#include "PacketStream.h"


#include "packets/direction.h"

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

class PACKET_HANDLER_TEST_TWO : public PACKET_HANDLER
{
public:
    struct DATA {
        char Name[10];
    };
public:
    PACKET_HANDLER_TEST_TWO() {}
    ~PACKET_HANDLER_TEST_TWO() {}
public:
    u32 GetId()   { return StringHash("PACKET_HANDLER_TEST_TWO"); }
    u32 GetSize() { return sizeof(DATA); }

    void Recieve(void* Buffer, void* Context)
    {
        DATA* Data = (DATA*)Buffer;
        printf("[Data2] %s\n", Data->Name);
    }
};


void DIRECTION_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    printf("[Direction] x=%u y=%u UserId=%u\n", Data->x, Data->y, Data->UserId);
}

int main()
{
    PACKET_HANDLER_TEST Test;
    PACKET_HANDLER_TEST_TWO Test2;
    DIRECTION_PACKET_HANDLER Direction;
    Test.Register();
    Test2.Register();
    Direction.Register();

    PACKET_STREAM Stream;
    PACKET_HANDLER_TEST::DATA Data;
    PACKET_HANDLER_TEST_TWO::DATA Data2;
    DIRECTION_PACKET_HANDLER::DATA DirectionData;

    Data.Value = 0xabababab;
    Data.AnotherValue = 0xcdcd;
    Data.MoreValues = 0xefefefefefefefef;
    Stream.AddPacket(&Test, &Data);

    DirectionData.x = 10;
    DirectionData.y = 200;
    DirectionData.UserId = 100000;
    Stream.AddPacket(&Direction, &DirectionData);
    
    Data.Value = 0x12121212;
    Data.AnotherValue = 0x2323;
    Data.MoreValues = 0x3434343434343434;
    Stream.AddPacket(&Test, &Data);

    strncpy(&Data2.Name[0], "Test123", sizeof(Data2.Name));;
    Stream.AddPacket(&Test2, &Data2);
    
    Data.Value = 0x56565656;
    Data.AnotherValue = 0x6767;
    Data.MoreValues = 0x8989898989898989;
    Stream.AddPacket(&Test, &Data);
    Stream.RecievePackets(&Stream.Data[0], Stream.DataOffset, null);
}
