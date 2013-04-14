//******************************************************************************
// Login Packet
// ------------
//   Handles login packets
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "login_packet.h"
#include "login_ack_packet.h"

#include "client.h"
#include "world.h"

void LOGIN_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    printf("[%d] Got a login request\n", UserId);
    struct DATA* Data = (DATA*)Buffer;
    CLIENT* Client = (CLIENT*)Context;
    if(Client == null) {
        error("Invalid client context");
        return;
    }

    LOGIN_ACK_PACKET_HANDLER::DATA AckData;
    if(Ack == null) {
        error("Failed to get login ack packet handler\n");
        return;
    }
    if(Client_FindClient(Data->UserId)) {
        printf("Client already logged in with id=%d\n", Data->UserId);
        AckData.Success = false;
        AckData.UserId = UserId;
        AckData.x = 0;
        AckData.y = 0;
        AckData.Error = 1; // TODO: enumerate errors
    } else {
        printf("Logging in client=%p to userid=%d\n", Context, Data->UserId);
        Client->UserId = Data->UserId;
        World_SetInitialPosition(UserId);
        AckData.Success = true;
        AckData.UserId = UserId;
        AckData.x = 0;
        AckData.y = 0;
    }

    PACKET_HANDLER* LoginAckHandler = PacketHandler_FindHandler(StringHash("LOGIN_ACK"), buffer);
    Client->Stream.AddPacket(LoginAckHandler, &AckData);
}

