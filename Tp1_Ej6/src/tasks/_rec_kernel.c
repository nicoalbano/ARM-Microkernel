#include "../../inc/_rec_kernel.h"
#include "../../inc/task_1.h"
#include "../../inc/task_2.h"
#include "../../inc/scheduler_manager.h"
#include <stdint.h>
#include <stdio.h>

extern sch_m scheduler_struct;
__attribute__((section(".text")))void _rec_kernel();

__attribute__((section(".text")))void _rec_kernel()
{
    uint32_t aux=0;
    uint32_t * read_write = RAM_START;
    uint32_t * read_toggle = START; 
    switch (scheduler_struct.task_running)
    {
        case 1:

    
        while(read_write < RAM_END)
        {   
            aux= *read_write;
            *read_write= WORD_CHECK;
            read_write++;
        }
        read_write=RAM_START;
        break;

        case 2:

        while(read_toggle < END )
        {
            * read_toggle ^= TOGGLE;
            read_toggle++;
        }
        read_toggle=START;

        break;

    }
    return;
}