#include "../../inc/idle.h"


__attribute__((section(".idle_text")))void idle(void)
{
    while(1)
    {
        asm("WFI");
    }
}