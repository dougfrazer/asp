//*******************************************************************************
// NETWORK
//   
// @author Doug Frazer
// January 2013
//*******************************************************************************

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "network.h"
#include "keyboard.h"
#include "world.h"
#include "player.h"

#include "library/network/PacketStream.h"

#include "library/network/packets/login.h"
#include "library/network/packets/direction.h"

// TODO: get rid of static memory
static struct {
    int sockfd;
    struct sockaddr_in Destination;
    socklen_t          DestinationLength;
    struct sockaddr_in RecievedFrom;
    socklen_t          RecievedFromLength;
    u32 LoginId;
} NetworkData;

static PACKET_STREAM Stream;

static void Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::DIRECTION Direction);
static void Network_SendBack()    { Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::SOUTH); }
static void Network_SendForward() { Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::NORTH); }
static void Network_SendLeft()    { Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::WEST); }
static void Network_SendRight()   { Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::EAST); } 
static void Network_Transmit();
// *****************************************************************************
// *****************************************************************************
static void Network_SendLoginRequest(u32 UserId)
{
    LOGIN_PACKET_HANDLER::DATA Data;
    Data.UserId = UserId;
    Stream.AddPacket(StringHash("LOGIN"), sizeof(Data), &Data);
}
// *****************************************************************************
void Network_ProcessLoginAckPacket(LOGIN_ACK_PACKET_HANDLER::DATA* Data)
{
    printf("Recieved a LOGIN_ACK Packet\n");
    if(Data->Success) {
        printf("Login successful as userid=%d position=(%f,%f,%f)\n", 
			Data->UserId, Data->x, Data->y, Data->z);
		Player_SetPrimaryPlayer(Data->UserId);
        NetworkData.LoginId = Data->UserId;
        Player_SetPosition(Data->x, Data->y, Data->z, Data->UserId);
    } else {
        printf("Login unnsuccessful, (error %d) trying with a userid=%d\n", Data->Error, Data->UserId+1);
        Network_SendLoginRequest( Data->UserId + 1 );
    }
}
// *****************************************************************************
void Network_Init()
{
    NetworkData.sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
    assert(NetworkData.sockfd > 0);
    
    Memset(&NetworkData.Destination, (u8)0, sizeof(NetworkData.Destination));
    NetworkData.Destination.sin_family = AF_INET;
    NetworkData.Destination.sin_addr.s_addr = inet_addr("127.0.0.1");
    NetworkData.Destination.sin_port = htons(PORTNUM);
    
    connect(NetworkData.sockfd, (struct sockaddr*)&NetworkData.Destination, sizeof(NetworkData.Destination));

    Keyboard_RegisterEvent('a', Network_SendLeft);
    Keyboard_RegisterEvent('s', Network_SendBack);
    Keyboard_RegisterEvent('d', Network_SendRight);
    Keyboard_RegisterEvent('w', Network_SendForward);

    PacketHandler_RegisterAll();
}
// *****************************************************************************
static void Network_Read(char* buffer, int size, ssize_t* outlen)
{
    *outlen = recvfrom(NetworkData.sockfd,
                        (void*)buffer,
                        size,
                        0,
                        (struct sockaddr*)&NetworkData.RecievedFrom,
                        &NetworkData.RecievedFromLength);

    if(errno == EAGAIN) {
        return;
    } else {
        printf("Error occured recieving data %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
// *****************************************************************************
void Network_Update(float DeltaTime)
{
    if(NetworkData.LoginId == 0) {
        Network_SendLoginRequest(1);
        NetworkData.LoginId = 1;
    }
    char buffer[MAX_RECV_LEN];
    ssize_t len = 0;
    
    do {
		Network_Read(buffer, sizeof(buffer), &len);    
		if(len > 0) Stream.RecievePackets(buffer, len, null);
    } while( len > 0);
    Network_Transmit();
}
// *****************************************************************************
void Network_Deinit()
{
    // stop both transmission and reception
    shutdown(NetworkData.sockfd, 2);
}
// *****************************************************************************
static void Network_SendDirectionPacket(DIRECTION_PACKET_HANDLER::DIRECTION Direction)
{
    DIRECTION_PACKET_HANDLER::DATA Data;
    Data.UserId = NetworkData.LoginId;
    Data.Direction = Direction;
    Stream.AddPacket(StringHash("DIRECTION"), sizeof(Data), &Data);
}
// *****************************************************************************
static void Network_Transmit()
{
    Stream.Transmit(NetworkData.sockfd, (struct sockaddr*)&NetworkData.Destination);
}
// *****************************************************************************
bool Network_IsUserLocal(u32 UserId)
{
    return UserId == NetworkData.LoginId;
}
