#include <stddef.h>
#include <stdint.h>

#include "../../inc/task_1.h"
#include "../../inc/task_2.h"
#include "../../inc/task_3.h"
#include "../../inc/scheduler_manager.h"



extern tcb_t tcb_tasks[4];
extern sch_m scheduler_struct;
extern int ft;
__attribute__((section(".data")))uint32_t* actual_sp=0;


__attribute__((section(".text"))) uint32_t* scheduler(ctx_t *ctx)
{
    uint32_t* sp_irq=tcb_tasks[scheduler_struct.task_running].sp_irq;
    uint32_t* SVC_SP=tcb_tasks[scheduler_struct.task_running].sp_svc;


    if(scheduler_struct.ticks_exec<tcb_tasks[scheduler_struct.task_running].cant_ticks)
    {

        scheduler_struct.ticks_exec+=1;
    }
    else
    {
        //cambio a svc
        asm("CPS 0x13");
        //guardo el sp_svc de la que estaba ejecutando
        asm("MOV %0,SP":"=r"(tcb_tasks[scheduler_struct.task_running].sp_svc));

        if(scheduler_struct.task_running<CANT_TASK-1)
        {
            scheduler_struct.task_running++;
        }
        else
        {
            scheduler_struct.task_running=IDLE_ID;
        }
        // tengo que cambiar el ttbr0 
        asm volatile(
            "MCR p15, 0, %0, c2, c0, 0\n" // Escribe en TTBR0
            :
            : "r" (tcb_tasks[scheduler_struct.task_running].TTBR0)
            : "memory"
        );
        // Cargo el SP_SVC de la nueva tarea
        asm("MOV SP,%0"::"r"(tcb_tasks[scheduler_struct.task_running].sp_svc));

        if(scheduler_struct.task_running>0)
        {
            asm("CPS 0x1F"); // paso a modo sys
            asm("MOV SP,%0"::"r"(tcb_tasks[scheduler_struct.task_running].sp_usr));
        }

        //Cambio a IRQ
        asm("CPS 0x12");
        //Cargo SP_IRQ nueva tarea
        asm("MOV SP,%0"::"r"(tcb_tasks[scheduler_struct.task_running].sp_irq));
        sp_irq=tcb_tasks[scheduler_struct.task_running].sp_irq;
        scheduler_struct.sp_c=sp_irq;
        scheduler_struct.ticks_exec=0;
        
    }
    if(ft==0)
    {
        // la primera vez que entro cargo mi sp de modos svc
        ft++;
        asm("CPS #0x13");
        asm("mov sp, %0"::"r"(tcb_tasks[scheduler_struct.task_running].sp_svc));
        asm("CPS #0x12");
    }

    return sp_irq;

}