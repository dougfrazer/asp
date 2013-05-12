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

static CONNECTED_NODE TestSkeletonConnectedNodes[] =
{
    { NECK, BACK },
    { BACK, TORSO },
    { BACK, LEFT_SHOULDER },
    { BACK, RIGHT_SHOULDER },
    { LEFT_SHOULDER, LEFT_ELBOW },
    { LEFT_ELBOW, LEFT_WRIST },
    { RIGHT_SHOULDER, RIGHT_ELBOW },
    { RIGHT_ELBOW, RIGHT_WRIST },
    { TORSO, LEFT_THIGH },
    { TORSO, RIGHT_THIGH },
    { LEFT_THIGH, LEFT_KNEE },
    { LEFT_KNEE, LEFT_FOOT },
    { RIGHT_THIGH, RIGHT_KNEE },
    { RIGHT_KNEE, RIGHT_FOOT },
    { MAX_SKELETON_NODES, MAX_SKELETON_NODES },
};

static SKELETON TestSkeleton = {
    { 
        { 0.0, 0.0, 0.0, 0.0, 2.0, 0.0 },     // NECK
        { 0.0, 0.0, 0.0, 0.0, 1.5, 0.0 },     // BACK

        { 0.0, 0.0, 0.0, 0.25, 1.75, 0.0 },  // LEFT SHOULDER
        { 0.0, 0.0, 0.0, 0.35, 1.5,  0.0 },  // LEFT ELBOW
        { 0.0, 0.0, 0.0, 0.25, 1.25, 0.2 },  // LEFT WRIST

        { 0.0, 0.0, 0.0, -0.25, 1.75, 0.0 },  // RIGHT SHOULDER
        { 0.0, 0.0, 0.0, -0.35, 1.5,  0.0 },  // RIGHT ELBOW
        { 0.0, 0.0, 0.0, -0.25, 1.25, 0.2 },  // RIGHT WRIST

        { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 },    // TORSO

        { 0.0, 0.0, 0.0, 0.25, 0.9,  0.0 },   // LEFT THIGH
        { 0.0, 0.0, 0.0, 0.25, 0.4,  0.1 },   // LEFT KNEE
        { 0.0, 0.0, 0.0, 0.25, 0.01, 0.0 },  // LEFT FOOT

        { 0.0, 0.0, 0.0, -0.25, 0.9,  0.0 },   // RIGHT THIGH
        { 0.0, 0.0, 0.0, -0.25, 0.4,  0.1 },   // RIGHT KNEE
        { 0.0, 0.0, 0.0, -0.25, 0.01, 0.0 },  // RIGHT FOOT
    },

    &TestSkeletonConnectedNodes[0],
};

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
    Skeleton = &TestSkeleton;
}
//******************************************************************************
void Actor_Update(float DeltaTime)
{
    for(ACTOR* Actor = Actor_GetFirst(); Actor != null; Actor = Actor_GetNext(Actor)) {
        Actor->Update(DeltaTime);
        if(Actor->Animation != null) {
            Actor->Animation->Update(DeltaTime, Actor->Skeleton);
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
