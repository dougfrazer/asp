//*******************************************************************************
// NETWORK
//   Handles the main network connection, listens on the known server port/ip
//   and dispatches messages
//   
// @author Doug Frazer
// January 2013
//*******************************************************************************

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "includes/common_include.h"
#include "library/network/packets/login_ack.h"

void Network_Init();
void Network_Update(float DeltaTime);
void Network_Deinit();

bool Network_IsUserLocal(u32 UserId);
void Network_ProcessLoginAckPacket(LOGIN_ACK_PACKET_HANDLER::DATA* Data);

#endif
