
#include "ASPLib.h"
#include "string.h"
#include "stdio.h"

int main()
{
    for(uint i = 0; i < 100; i++) {
        printf("%d log 2 = %d\n", i, Log2(i));
    }
}
