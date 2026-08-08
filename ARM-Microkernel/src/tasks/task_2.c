#include "../../inc/task_2.h"
#include <stdio.h>
#include <stdint.h>
/*
Tarea 2
    Realiza el decremento de una variable local y de otra global
    12 Ticks

*/
__attribute__((section(".task_2_var")))int global_2=0;

__attribute__((section(".task_2_code")))void task_2(void)
{


    while(1)
    {
        asm("SVC 90");
    }
}