//*******************************************************************************
// NETWORK
//   Handles the main network connection, listens on the known server port/ip
//   and dispatches messages to the connected clients
//   
// @author Doug Frazer
// December 2012
//*******************************************************************************


#ifndef __NETWORK_H__
#define __NETWORK_H__

// C-style interface
void Network_Init();
void Network_Deinit();
void Network_Update(float DeltaTime);
void Network_RequestTransmit();

#endif
