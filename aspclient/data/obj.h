#include "../../library/ASPLib.h"


#ifndef __OBJ_H__
#define __OBJ_H__

typedef struct { float x; float y; float z; } vertex;
typedef struct { u16 v0; u16 v1; u16 v2; u16 v3; } face;

struct object
{
    int   NumFaces;
    face* Faces;
};

struct ObjData
{
    int     NumVertices;
    int     NumObjects;
    vertex* Vertices;
    object* Objects;
};

#endif
