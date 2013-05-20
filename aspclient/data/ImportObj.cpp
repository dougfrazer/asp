//******************************************************************************
//  Import Object Files
//  -------------------
//    This file reads in a .obj file and creates a binary structure representation
//    of it that we can load in the game.  The binary structure is as follows:
//
//     +-------------------------------------------------+
//     | NumVertices (4 bytes)  |  NumObjects (4 bytes)  | Header
//     | Vertices* (4 bytes)    |  Objects* (4 bytes)    |
//     +------------------------+------------------------+
//     | Vertex Data (sizeof(vertex) * NumVertices)      | Vertex Data
//     | ...                                             |
//     +-------------------------------------------------+
//     | object data (sizeof(object)* NumObjects         | Object Data 
//     | ...                                             |
//     +-------------------------------------------------+
//     | face data (sizeof(face) * NumFaces              | Face Data
//     | ...                                             |
//     +-------------------------------------------------+
//
//   (c) May 2013
//   @author Doug Frazer
//******************************************************************************
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "stdint.h"

#include <list>

#define null 0

#include "obj.h"

    struct Object {
        std::list<face> FaceList;
    };

int main(int argc, char** argv)
{
    assert(argc == 2);
    printf("Reading %s...\n", argv[1]);

    FILE* file = fopen(argv[1], "r");
    assert(file != null);

    char buffer[256];
    // TODO: support objects
    std::list<vertex> VertexList;
    std::list<Object> ObjectList;
    while(fgets(buffer, 256, file)) {
        char* pch = strtok(buffer, " ");
        if(strcmp(pch, "v") == 0) {
            vertex NewVert;
            pch = strtok(null, " ");
            NewVert.x = atof(pch);
            pch = strtok(null, " ");
            NewVert.y = atof(pch);
            pch = strtok(null, " ");
            NewVert.z = atof(pch);
            //printf("Read vertex ( %f, %f, %f )\n", NewVert.x, NewVert.y, NewVert.z);
            VertexList.push_back(NewVert);
        } else if(strcmp(pch, "f") == 0) {
            // TODO: support f int/int/int ... meaning vertex/texture/normal
            face NewFace;
            pch = strtok(null, " ");
            NewFace.v0 = atoi(pch) - 1;
            pch = strtok(null, " ");
            NewFace.v1 = atoi(pch) - 1;
            pch = strtok(null, " ");
            if(pch == null) {
                NewFace.v2 = -1;
                NewFace.v3 = -1;
            } else {
                NewFace.v2 = atoi(pch) - 1;
                pch = strtok(null, " ");
                if(pch == null) {
                    NewFace.v3 = -1;
                } else {
                    NewFace.v3 = atoi(pch) - 1;
                }
            }
            ObjectList.back().FaceList.push_back(NewFace);
        } else if(strcmp(pch, "o") == 0) {
            Object o;
            ObjectList.push_back(o);
        } else if(strcmp(pch, "vt") == 0) {
            // TODO
        }
    }
    fclose(file);
    
    int NumVertices = VertexList.size();
    int NumObjects = ObjectList.size();
    int NumFaces = 0;
    for(std::list<Object>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++) {
        NumFaces += it->FaceList.size();
    }
    assert(NumVertices < 0xffff); // Need to use u32 not u16 for faces
    printf("Got %d vertices...\n", NumVertices);
    printf("Got %d objects...\n", NumObjects);
    printf("Got %d faces...\n", NumFaces);

    char OutputFileName[256];
    strncpy(OutputFileName, argv[1], strlen(argv[1]) - 3);
    strncpy(&OutputFileName[strlen(OutputFileName)], "data\0", 5);
    FILE* out = fopen(OutputFileName, "w");
    assert(out != null);

    // Allocate one big block of memory that will exactly replicate our data
    // so that we can set up relative pointers
    printf("Setting up the data\n");
    size_t BufferSize;
    BufferSize = sizeof(ObjData);
    BufferSize += NumVertices * sizeof(vertex);
    BufferSize += NumObjects * sizeof(object);
    BufferSize += NumFaces * sizeof(face);
    void* Data = malloc(BufferSize);
    assert(Data != null);
    
    // Set up the header
    ObjData* Header = (ObjData*)Data;
    Header->NumVertices = NumVertices;
    Header->NumObjects = NumObjects;

    Header->Vertices = (vertex*)(Header + 1);
    Header->Objects = (object*)(&Header->Vertices[NumVertices]); 

    // Set up the vertices
    int Index = 0;
    for(std::list<vertex>::iterator it = VertexList.begin(); it != VertexList.end(); it++) {
        Header->Vertices[Index] = *it;
        Index++;
    }
    
    // Set up the objects
    Index = 0;
    int FaceCounter = 0;
    for(std::list<Object>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++) {
        printf("Beginning object Index=%d with face-offset=%d\n", Index, FaceCounter);
        Header->Objects[Index].NumFaces = it->FaceList.size();
        Header->Objects[Index].Faces = (face*)(&Header->Objects[NumObjects]);
        Header->Objects[Index].Faces = &Header->Objects[Index].Faces[FaceCounter];
        int FaceIndex = 0;
        for(std::list<face>::iterator fit = it->FaceList.begin(); fit != it->FaceList.end(); fit++) {
            Header->Objects[Index].Faces[FaceIndex] = *fit;
            FaceIndex++;
        }
        Index++;
        FaceCounter += it->FaceList.size();
    }

    // Make all pointers relative
    for(int i = 0; i < Header->NumObjects; i++) {
        pointer_make_relative((void**)&Header->Objects[i].Faces);
    }
    pointer_make_relative((void**)&Header->Vertices);
    pointer_make_relative((void**)&Header->Objects);
    
    // Write out the file
    printf("Printing binary file to %s\n", OutputFileName);
    assert(fwrite(Data, BufferSize, 1, out));
}
