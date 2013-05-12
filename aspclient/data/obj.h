
#ifndef __OBJ_H__
#define __OBJ_H__

typedef struct { float x; float y; float z; } vertex;

struct ObjData
{
    int NumVertices;
    vertex* Vertices;
};

#endif
