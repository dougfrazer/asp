

#include "network/packets/login_ack.h"

#include "network.h"

void LOGIN_ACK_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    Network_ProcessLoginAckPacket(Data);
}
