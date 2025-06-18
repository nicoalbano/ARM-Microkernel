#include "../../inc/task_1.h"
#include "../../inc/task_2.h"
#include "../../inc/task_3.h"
#include "../../inc/idle.h"
#include "../../inc/scheduler_manager.h"
#include <stddef.h>
#include <stdint.h>


extern uint32_t __IRQ_IDLE_stack_top__;
extern uint32_t __IRQ_TASK1_stack_top__;
extern uint32_t __IRQ_TASK2_stack_top__;

extern uint32_t __SVC_IDLE_stack_top__;
extern uint32_t __SVC_TASK1_stack_top__;
extern uint32_t __SVC_TASK2_stack_top__;

extern uint32_t __tarea_1_usr_stack_top_vma__;
extern uint32_t __tarea_2_usr_stack_top_vma__;



__attribute__((section(".bss"))) tcb_t tcb_tasks[3];
__attribute__((section(".bss"))) sch_m scheduler_struct;
__attribute__((section(".data"))) int ft=0;



__attribute__((section(".text")))void TCB_init(void)
{
    int i=0;
    uint32_t SVC_SPSR;

    scheduler_struct.task_running=0;
    scheduler_struct.ticks_exec=0;


    void(*idle_ptr)(void)=&idle;
    void(*task_1_ptr)(void)=&task_1;
    void(*task_2_ptr)(void)=&task_2;


    // Tengo la direccion de cada sp en un puntero
    //MODO IRQ
    tcb_tasks[0].sp_irq=(uint32_t*)&__IRQ_IDLE_stack_top__;
    tcb_tasks[1].sp_irq= (uint32_t*)&__IRQ_TASK1_stack_top__;
    tcb_tasks[2].sp_irq=(uint32_t*)&__IRQ_TASK2_stack_top__;

    //MODO SVC
    tcb_tasks[0].sp_svc=&__SVC_IDLE_stack_top__;
    tcb_tasks[1].sp_svc=&__SVC_TASK1_stack_top__;
    tcb_tasks[2].sp_svc=&__SVC_TASK2_stack_top__;

    tcb_tasks[0].sp_usr=NULL;
    tcb_tasks[1].sp_usr=&__tarea_1_usr_stack_top_vma__;
    tcb_tasks[2].sp_usr=&__tarea_2_usr_stack_top_vma__;



    // Cargo las estructuras de los TCB de cada tarea

    tcb_tasks[0].ptr_task=(uint32_t )idle_ptr;
    tcb_tasks[1].ptr_task=(uint32_t )task_1_ptr;
    tcb_tasks[2].ptr_task=(uint32_t )task_2_ptr;


    tcb_tasks[0].cant_ticks=CANT_TICKS_IDLE;
    tcb_tasks[1].cant_ticks=CANT_TICKS_TASK_1;
    tcb_tasks[2].cant_ticks=CANT_TICKS_TASK_2;


    tcb_tasks[0].task_id=IDLE_ID;
    tcb_tasks[1].task_id=TASK_1_ID;
    tcb_tasks[2].task_id=TASK_2_ID;


    // Guardo el puntero a funcion de cada tarea en memoria

    *--tcb_tasks[0].sp_irq=(uint32_t)idle_ptr;
    *--tcb_tasks[1].sp_irq=(uint32_t)task_1_ptr;
    *--tcb_tasks[2].sp_irq=(uint32_t)task_2_ptr;


    // Cargo todos los registros en 0 del r0-r12
    for(i=0;i<=12;i++)
    {
        *--tcb_tasks[0].sp_irq=0;
        *--tcb_tasks[1].sp_irq=0;
        *--tcb_tasks[2].sp_irq=0;
    }

    
    uint32_t spr0_0=(uint32_t)tcb_tasks[0].sp_irq;
    uint32_t spr0_1=(uint32_t)tcb_tasks[1].sp_irq;
    uint32_t spr0_2=(uint32_t)tcb_tasks[2].sp_irq;

    // Me falta el spsr


    asm("MRS %0,CPSR":"=r"(SVC_SPSR));

    // Despues modifico el bit para habilitar las interrupciones y lo pusheo 
    SVC_SPSR=SVC_SPSR&~(1<<7);

    *--tcb_tasks[0].sp_irq=SVC_SPSR;

    // Limpiar modo (0-4), FIQ (6), IRQ (7), y poner modo User
    SVC_SPSR &= ~(0x1F | 0xC0);
    SVC_SPSR |= 0x10;  // modo User
    *--tcb_tasks[1].sp_irq=SVC_SPSR;
    *--tcb_tasks[2].sp_irq=SVC_SPSR;


    // Guardo el stack pointer que apunta a r0
    *--tcb_tasks[0].sp_irq=spr0_0;
    *--tcb_tasks[1].sp_irq=spr0_1;
    *--tcb_tasks[2].sp_irq=spr0_2;

    tcb_tasks[0].TTBR0=TTBR0_IDLE;
    tcb_tasks[1].TTBR0=TTBR0_T1;
    tcb_tasks[2].TTBR0=TTBR0_T2;

    return;


}