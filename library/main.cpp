
#include "ASPLib.h"
#include "string.h"
#include "stdio.h"

int main()
{
    u32 x = StringHash("TEST");
    u32 y = StringHash("TL:KJA:LKJ@LKJ");
    printf("x=0x%08x\ny=0x%08x\n", x, y);
}
