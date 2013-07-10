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

static LIST<ACTOR> ActorList;

//******************************************************************************
ACTOR::ACTOR()
{
    ActorList.Add( this );
    Animation = null;
    Physics = null;
}
//******************************************************************************
ACTOR::~ACTOR()
{
    ActorList.Remove( this );
}
//******************************************************************************
void Actor_Update(float DeltaTime)
{
    for(ACTOR* Actor = Actor_GetFirst(); Actor != null; Actor = Actor_GetNext(Actor)) {
        Actor->Update(DeltaTime);
        if(Actor->Animation != null) {
            Actor->Animation->Update(DeltaTime, null);
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
    return ActorList.GetFirst();
}
//******************************************************************************
ACTOR* Actor_GetNext(ACTOR* Actor)
{
    return ActorList.GetNext( Actor );
}
//******************************************************************************
