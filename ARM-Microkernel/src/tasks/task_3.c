#include "../../inc/task_3.h"

/*
Tarea 3
    Decrementa la variable global que la tarea 1 incrementa y incrementa la que la tarea 2 decrementa
    5 Ticks

*/
extern int global_1;
extern int global_2;


__attribute__((section(".task")))void task_3(void)
{
    while(1)
    {
        global_1-=5;
        global_2+=2;
    }
}