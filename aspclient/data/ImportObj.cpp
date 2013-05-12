#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "stdint.h"

#include <list>

#define null 0

#include "obj.h"

inline void pointer_make_relative(void** x) { *x = (void*)((intptr_t)*x - (intptr_t)x); }

int main(int argc, char** argv)
{
    assert(argc == 2);
    printf("Reading %s...\n", argv[1]);

    FILE* file = fopen(argv[1], "r");
    assert(file != null);

    char buffer[256];
    std::list<vertex> VertexList;
    bool InterestingObject = false;
    while(fgets(buffer, 256, file)) {
        char* pch = strtok(buffer, " ");
        if(*pch == 'v' && InterestingObject) {
            vertex NewVert;
            pch = strtok(null, " ");
            NewVert.x = atof(pch);
            pch = strtok(null, " ");
            NewVert.y = atof(pch);
            pch = strtok(null, " ");
            NewVert.z = atof(pch);
            //printf("Read vertex ( %f, %f, %f )\n", NewVert.x, NewVert.y, NewVert.z);
            VertexList.push_back(NewVert);
        } else if(*pch == 'o') {
            pch = strtok(null, " ");
            if(strcmp(pch, "female_body_basemesh\n") == 0) {
                //printf("Found an interesting object: %s\n", pch);
                InterestingObject = true;
            } else {
                //printf("Found a not interesting object: %s\n", pch);
                InterestingObject = false;
            }
        }
    }

    printf("Got %ld vertices...\n", VertexList.size());

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
    void* Data = malloc(BufferSize);
    assert(Data != null);
    
    // Set up the header
    ObjData* Header = (ObjData*)Data;
    Header->NumVertices = VertexList.size();
    Header->Vertices = (vertex*)(Header + 1);

    // Set up the vertices
    int Index = 0;
    for(std::list<vertex>::iterator it = VertexList.begin(); it != VertexList.end(); it++) {
        Header->Vertices[Index].x = it->x;
        Header->Vertices[Index].y = it->y;
        Header->Vertices[Index].z = it->z;
        Index++;
    }

    // Make all pointers relative
    pointer_make_relative((void**)&Header->Vertices);
    
    // Write out the file
    printf("Printing binary file to %s\n", OutputFileName);
    assert(fwrite(Data, BufferSize, 1, out));
}
