//******************************************************************************
// AI Public
// ---------
//   Common definitions of AI objects that many different components are
//   interested in.
//
//   Eventually this will get removed, its just a scratchpad until I have a
//   better understanding of all the components necessary.
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#ifndef __AI_PUBLIC_H__
#define __AI_PUBLIC_H__

enum SKELETON_NODE
{
    NECK,
    BACK,

    LEFT_SHOULDER,
    LEFT_ELBOW,
    LEFT_WRIST,
    
    RIGHT_SHOULDER,
    RIGHT_ELBOW,
    RIGHT_WRIST,

    TORSO,

    LEFT_THIGH,
    LEFT_KNEE,
    LEFT_FOOT,

    RIGHT_THIGH,
    RIGHT_KNEE,
    RIGHT_FOOT,

    MAX_SKELETON_NODES,
};

struct CONNECTED_NODE {
    SKELETON_NODE First;
    SKELETON_NODE Second;
};

// TODO: quaternions?
struct ORIENTATION
{
    float rotx;
    float roty;
    float rotz;
    
    float posx;
    float posy;
    float posz;
};

struct SKELETON {
    ORIENTATION Nodes[MAX_SKELETON_NODES];
    CONNECTED_NODE* ConnectedNodes;
};

#endif
