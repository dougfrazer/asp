#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "stdint.h"

#include <list>

#define null 0

#include "obj.h"

int main(int argc, char** argv)
{
    assert(argc == 2);
    printf("Reading %s...\n", argv[1]);

    FILE* file = fopen(argv[1], "r");
    assert(file != null);

    char buffer[256];
    // TODO: support objects
    std::list<vertex> VertexList;
    std::list<face> FaceList;
    while(fgets(buffer, 256, file)) {
        char* pch = strtok(buffer, " ");
        if(*pch == 'v') {
            vertex NewVert;
            pch = strtok(null, " ");
            NewVert.x = atof(pch);
            pch = strtok(null, " ");
            NewVert.y = atof(pch);
            pch = strtok(null, " ");
            NewVert.z = atof(pch);
            //printf("Read vertex ( %f, %f, %f )\n", NewVert.x, NewVert.y, NewVert.z);
            VertexList.push_back(NewVert);
        } else if(*pch == 'f') {
            face NewFace;
            pch = strtok(null, " ");
            NewFace.v0 = atoi(pch) - 1;
            pch = strtok(null, " ");
            NewFace.v1 = atoi(pch) - 1;
            pch = strtok(null, " ");
            NewFace.v2 = atoi(pch) - 1;
            pch = strtok(null, " ");
            if(pch == null) {
                printf("Got a triangle with 3 args at index %ld\n", FaceList.size());
                NewFace.v3 = -1;
            } else {
                NewFace.v3 = atoi(pch) - 1;
            }
            FaceList.push_back(NewFace);
        }
    }

    printf("Got %ld vertices...\n", VertexList.size());
    printf("Got %ld faces...\n", FaceList.size());

    fclose(file);

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
    BufferSize += VertexList.size() * sizeof(vertex);
    BufferSize += FaceList.size() * sizeof(face);
    void* Data = malloc(BufferSize);
    assert(Data != null);
    
    // Set up the header
    ObjData* Header = (ObjData*)Data;
    Header->NumVertices = VertexList.size();
    Header->NumFaces = FaceList.size();
    Header->Vertices = (vertex*)(Header + 1);
    Header->Faces = (face*)(&Header->Vertices[Header->NumVertices]);

    // Set up the vertices
    int Index = 0;
    for(std::list<vertex>::iterator it = VertexList.begin(); it != VertexList.end(); it++) {
        Header->Vertices[Index] = *it;
        Index++;
    }
    
    // Set up the faces
    Index = 0;
    for(std::list<face>::iterator it = FaceList.begin(); it != FaceList.end(); it++) {
        Header->Faces[Index] = *it;
        Index++;
    }

    // Make all pointers relative
    pointer_make_relative((void**)&Header->Vertices);
    pointer_make_relative((void**)&Header->Faces);
    
    // Write out the file
    printf("Printing binary file to %s\n", OutputFileName);
    assert(fwrite(Data, BufferSize, 1, out));
}
