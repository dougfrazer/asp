

#include "library/network/packets/login_ack.h"

#include "../network.h"

static LOGIN_ACK_PACKET_HANDLER LoginAckHandler;

void LOGIN_ACK_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    Network_ProcessLoginAckPacket(Data);
}
