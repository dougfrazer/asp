//******************************************************************************
// Actor
// -----
//   A general representation of a character.  All characters, NPCs, etc. will
//   extend from this base class.  They will be responsible for defining their
//   own:
//     Animations
//     Physics
//     Behaviors
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#include "ai_public.h"
#include "library/ASPLib.h"

class ANIMATION;
class PHYSICS;

class ACTOR
{
public:
	ACTOR();
	virtual ~ACTOR() {};

public:
    virtual void Update(float DeltaTime) = 0;
    virtual void Draw() = 0;

    SKELETON       Skeleton;

    ANIMATION*     Animation;
    PHYSICS*       Physics;

	ACTOR*         Prev;
	ACTOR*         Next;
};

void Actor_Draw();
void Actor_Update(float DeltaTime);
ACTOR* Actor_GetFirst();
ACTOR* Actor_GetNext(ACTOR* Actor);
