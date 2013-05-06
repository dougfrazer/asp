//******************************************************************************
// Actor
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#include "actor.h"
#include "animation.h"
#include "physics.h"

#include "GL/glut.h"

static ACTOR* ActorList = null;

//******************************************************************************
ACTOR::ACTOR()
{
    // add it to a cyclic list
    if(ActorList == null) {
        ActorList       = this;
        ActorList->Prev = this;
        ActorList->Next = this;
    } else {
        this->Prev = ActorList->Prev;
        this->Next = ActorList->Prev->Next;
        ActorList->Prev->Next = this;
        ActorList->Prev = this;
    }

	Animation = null;
	Physics = null;
}
//******************************************************************************
void Actor_Update(float DeltaTime)
{
	for(ACTOR* Actor = Actor_GetFirst(); Actor != null; Actor = Actor_GetNext(Actor)) {
        Actor->Update(DeltaTime);
		if(Actor->Animation != null) {
			Actor->Animation->Update(DeltaTime);
		}
		if(Actor->Physics != null) {
			Actor->Physics->Update(DeltaTime);
		}
    }
}
//******************************************************************************
void Actor_Draw()
{
	for(ACTOR* Actor = Actor_GetFirst(); Actor != null; Actor = Actor_GetNext(Actor)) {
        glPushMatrix();
        Actor->Draw();
        glPopMatrix();
    }
}
//******************************************************************************
ACTOR* Actor_GetFirst()
{
	return ActorList;
}
//******************************************************************************
ACTOR* Actor_GetNext(ACTOR* Actor)
{
	return Actor->Next == ActorList ? null : Actor->Next;
}
//******************************************************************************
