

#include "packets/login_ack_packet.h"

void LOGIN_ACK_PACKET::Recieve(void* Buffer, void* Context)
{
    error("Should not recieve Login Ack packets on server");
}
