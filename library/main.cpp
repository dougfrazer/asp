
#include "ASPLib.h"
#include "string.h"
#include "stdio.h"

int main()
{
    char Data[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    char Data2[10];

    Memcpy(Data2, Data, sizeof(Data));

    printf("Data = 0x%08x\n", &Data[0]);
    for(uint i = 0; i < countof(Data2); i++) {
        printf("%c", Data2[i]);
    }
    printf("\n");
}
