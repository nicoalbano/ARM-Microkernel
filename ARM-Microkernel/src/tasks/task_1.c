#include "../../inc/task_1.h"
#include <stdio.h>
#include <stdint.h>

__attribute__((section(".task_1_data")))int global_1=0;

__attribute__((section(".task_1_code")))void task_1(void)
{

    while(1)
    {
        asm("SVC 90");
    }
    
}