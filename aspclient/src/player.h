//******************************************************************************
// Player
// ------
//   A player represents a human-controlled player.
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#include "actor.h"

class PLAYER : public ACTOR
{
public:
	PLAYER(u32 UserId);
	virtual ~PLAYER();
	
public:
    void Update(float DeltaTime);
    void Draw();

public:
	u32     Id;
	vector4 Position;
	PLAYER* Next;
	PLAYER* Prev;
};

void Player_SetPrimaryPlayer(u32 UserId);
bool Player_GetPrimaryPlayerPosition(vector4* Position);
void Player_SetPosition(float x, float y, float z, u32 UserId);
PLAYER* Player_GetFirst();
PLAYER* Player_GetNext(PLAYER* Player);
