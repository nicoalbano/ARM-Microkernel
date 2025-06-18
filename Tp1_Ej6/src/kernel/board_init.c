#include "../../inc/timer.h"
#include "../../inc/gic.h"
#include "../../inc/scheduler_manager.h"
#include "../../inc/mapeo.h"
#include "../../inc/memcopy.h"
#include "../../inc/idle.h"


/* LMA Y SIZE TAREAS */
extern uint32_t  _TAREA_1_TXT_LMA, __tarea_1_txt_size__;
extern uint32_t  _TAREA_1_DATA_LMA, __tarea_1_data_size__;
extern uint32_t _TAREA_1_RODATA_LMA, __tarea_1_rodata_size__;

extern uint32_t  _TAREA_2_TXT_LMA, __tarea_2_txt_size__;
extern uint32_t  _TAREA_2_DATA_LMA, __tarea_2_data_size__;
extern uint32_t  _TAREA_2_RODATA_LMA, __tarea_2_rodata_size__;


// TAREA IDLE
extern uint32_t _IDLE_TXT_VMA; // VMA 
extern uint32_t __idle_size__; // SIZE    
extern uint32_t _IDLE_TXT_LMA; // LMA
extern uint32_t _IDLE_TXT_PHY; // PHy

// TAREA 1 FISICA

extern uint32_t _TASK_1_TXT_PHY;          
extern uint32_t _TASK_1_DATA_PHY;
extern uint32_t _TASK_1_STACK_INIT_PHY;
extern uint32_t _TASK_1_BSS_PHY;
extern uint32_t _TASK_1_RODATA_PHY;
extern uint32_t _TASK_1_READ_AREA_PHY;

// TAREA 2 FISICA
extern uint32_t _TASK_2_TXT_PHY;
extern uint32_t _TASK_2_DATA_PHY;
extern uint32_t _TASK_2_STACK_INIT_PHY;
extern uint32_t _TASK_2_BSS_PHY;
extern uint32_t _TASK_2_RODATA_PHY;
extern uint32_t _TASK_2_READ_AREA_PHY;

/* STACK VMA */
extern uint32_t __tarea_1_usr_stack_top_vma__;
extern uint32_t __tarea_2_usr_stack_top_vma__;

extern uint32_t __tarea_1_usr_stack_top_phy__;
extern uint32_t __tarea_2_usr_stack_top_phy__;



 
/* Tarea 1 */ 
extern uint32_t  _TASK_1_READ_VMA;
extern uint32_t  _TASK_1_TEXT_VMA;
extern uint32_t  _TASK_1_DATA_VMA ;
extern uint32_t  _TASK_1_STACK_VMA ;
extern uint32_t  _TASK_1_BSS_VMA ;
extern uint32_t  _TASK_1_RODATA_VMA ;

/* Tarea 2 */

extern uint32_t _TASK_2_READ_VMA;
extern uint32_t _TASK_2_TEXT_VMA;
extern uint32_t _TASK_2_DATA_VMA;
extern uint32_t _TASK_2_BSS_VMA;
extern uint32_t _TASK_2_STACK_VMA;
extern uint32_t  _TASK_2_RODATA_VMA;

extern uint32_t _tarea_1_stack_size_; 
extern uint32_t _tarea_2_stack_size_; 

extern uint32_t _tarea_1_stack_phy_size_;



extern uint32_t _TAREA_1_BSS_LMA;
extern uint32_t __tarea_1_bss_size__;

extern uint32_t _TAREA_1_DATA_LMA;
extern uint32_t __tarea_1_data_size__;

extern uint32_t _TAREA_1_RODATA_LMA;
extern uint32_t __tarea_1_rodata_size__;

extern uint32_t _TAREA_2_BSS_LMA;
extern uint32_t __tarea_2_bss_size__;

extern uint32_t _TAREA_2_DATA_LMA;
extern uint32_t __tarea_2_data_size__;

extern uint32_t _TAREA_2_RODATA_LMA;
extern uint32_t __tarea_2_rodata_size__;






extern int ft;
extern int num_table_2;

__attribute__((section(".text")))void __board_init();
__attribute__((section(".text")))void __board_init()
{
    //Inicializo gic , timer y las pilas
    TCB_init();
    __gic_init();
    __timer_init();
    
    // Memcopy de las tareas
    memcopy(&_IDLE_TXT_PHY,&_IDLE_TXT_LMA,&__idle_size__);
    memcopy(&_TASK_1_TXT_PHY,&_TAREA_1_TXT_LMA,&__tarea_1_txt_size__);
    memcopy(&_TASK_2_TXT_PHY,&_TAREA_2_TXT_LMA,&__tarea_2_txt_size__);

    // PREGUNTAR EL TEMA DE LOS STACKS


    /*
    Pasos a hacer en el ejercicio 6

    En mi stack de irq debo pushear mi spsr en modo user para que la tarea al recuperar el contexto se ponga directamente en modo user? 
    esto creo q es asi por ende ahora deberia funcionar solo que en el momento que quiero hacer temas relacionados con modos privilegiados deberia saltar una excepcion

    Desde el lado de la paginacion debo hacer accecibles las zonas de memoria en modo user

    Modificar las tareas para que usen SWI, Que es lo que pasa cuando hago esto ? 


    
    
    */
    memcopy(&_TASK_1_STACK_INIT_PHY,&_TASK_1_STACK_VMA,&_tarea_1_stack_size_);
    memcopy(&_TASK_2_STACK_INIT_PHY,&_TASK_2_STACK_VMA,&_tarea_2_stack_size_);

    memcopy(&_TASK_1_BSS_PHY,&_TAREA_1_BSS_LMA,&__tarea_1_bss_size__);
    memcopy(&_TASK_1_DATA_PHY,&_TAREA_1_DATA_LMA,&__tarea_1_data_size__);
    memcopy(&_TASK_1_RODATA_PHY,&_TAREA_1_RODATA_LMA,&__tarea_1_rodata_size__);

    memcopy(&_TASK_2_BSS_PHY,&_TAREA_2_BSS_LMA,&__tarea_2_bss_size__);
    memcopy(&_TASK_2_DATA_PHY,&_TAREA_2_DATA_LMA,&__tarea_2_data_size__);
    memcopy(&_TASK_2_RODATA_PHY,&_TAREA_2_RODATA_LMA,&__tarea_2_rodata_size__);


    // Tengo que mapear para los 3 ttbr0 el kernel en general 
    // Despues que cada parte de memoria 

    // Tarea IDLE 

    mapeo(TTBR0_IDLE,0x70010000,0x70010000,(0x30+2));

    //Mapeo stack
    mapeo(TTBR0_IDLE,0x7001f000,0x7001f000,(0x30+2));
    mapeo(TTBR0_IDLE,0x7001e000,0x7001e000,(0x30+2));
    mapeo(TTBR0_IDLE,0x70020000,0x70020000,(0x30+2));

    mapeo(TTBR0_IDLE,0x7001d000,0x7001d000,(0x30+2));
    mapeo(TTBR0_IDLE,0x70011000,0x70011000,(0x30+2));
    mapeo(TTBR0_IDLE,0x70012000,0x70012000,0);

    //Mapeo vector de interrupciones
    mapeo(TTBR0_IDLE,0x0,0x0,(0x30+2));

    mapeo(TTBR0_IDLE,0x1E000000,0x1E000000,(0x30+2));

    //Mapeo de timer 
    mapeo(TTBR0_IDLE,0x10011000,0x10011000,(0x30+2));
    mapeo(TTBR0_IDLE,0x20000000,0x20000000,(0x30+2));

    mapeo(TTBR0_IDLE,&_IDLE_TXT_VMA,&_IDLE_TXT_PHY,(0x32));
    //mapeo(TTBR0_IDLE,0x70f523c0,0x70f523c0,0x32);

    //mapeo el stack


    num_table_2=0;

    // Tarea 1

    mapeo(TTBR0_T1,&_TASK_1_TEXT_VMA,&_TASK_1_TXT_PHY,0x22);
    mapeo(TTBR0_T1,&_TASK_1_BSS_VMA,&_TASK_1_BSS_PHY,0x22);
    mapeo(TTBR0_T1,&_TASK_1_DATA_VMA,&_TASK_1_DATA_PHY,0x22);
    mapeo(TTBR0_T1,&_TASK_1_STACK_VMA,&_TASK_1_STACK_INIT_PHY,0x32);
    mapeo(TTBR0_T1,&_TASK_1_READ_VMA,&_TASK_1_READ_AREA_PHY,0x22);
    //mapeo(TTBR0_T1,0x70f52d7c,0x70f52d7c,0x22);
    //mapeo(TTBR0_T1,&__tarea_1_usr_stack_top_vma__,&__tarea_1_usr_stack_top_phy__,0x22);

    mapeo(TTBR0_T1,0x70010000,0x70010000,(0x30+2));

    //Mapeo stack

    mapeo(TTBR0_T1,0x7001f000,0x7001f000,(0x30+2));
    mapeo(TTBR0_T1,0x7001e000,0x7001e000,(0x30+2));
    mapeo(TTBR0_T1,0x70020000,0x70020000,(0x30+2));

    mapeo(TTBR0_T1,0x7001d000,0x7001d000,(0x30+2));
    mapeo(TTBR0_T1,0x70011000,0x70011000,(0x30+2));
    mapeo(TTBR0_T1,0x70012000,0x70012000,0x32);

    //Mapeo vector de interrupciones
    mapeo(TTBR0_T1,0x0,0x0,(0x30+2));

    mapeo(TTBR0_T1,0x1E000000,0x1E000000,(0x30+2));

    //Mapeo de timer 
    mapeo(TTBR0_T1,0x10011000,0x10011000,(0x30+2));
    mapeo(TTBR0_T1,0x20000000,0x20000000,(0x30+2));



    num_table_2=0;
    // Tarea 2 

    mapeo(TTBR0_T2,&_TASK_2_TEXT_VMA,&_TASK_2_TXT_PHY,0x22);
    mapeo(TTBR0_T2,&_TASK_2_BSS_VMA,&_TASK_2_BSS_PHY,0x22);
    mapeo(TTBR0_T2,&_TASK_2_DATA_VMA,&_TASK_2_DATA_PHY,0x22);
    mapeo(TTBR0_T2,&_TASK_2_STACK_VMA,&_TASK_2_STACK_INIT_PHY,0x32);
    mapeo(TTBR0_T2,&_TASK_2_READ_VMA,&_TASK_2_READ_AREA_PHY,0x22);
    
    mapeo(TTBR0_T2,0x70010000,0x70010000,(0x30+2));

    //Mapeo stack
    //mapeo(TTBR0_T2,&__tarea_2_usr_stack_top_vma__,&__tarea_2_usr_stack_top_phy__,0x22);
    mapeo(TTBR0_T2,0x7001f000,0x7001f000,(0x30+2));
    mapeo(TTBR0_T2,0x7001e000,0x7001e000,(0x30+2));
    mapeo(TTBR0_T2,0x70020000,0x70020000,(0x30+2));

    mapeo(TTBR0_T2,0x7001d000,0x7001d000,(0x30+2));
    mapeo(TTBR0_T2,0x70011000,0x70011000,(0x30+2));
    mapeo(TTBR0_T2,0x70012000,0x70012000,0x32);

    //Mapeo vector de interrupciones
    mapeo(TTBR0_T2,0x0,0x0,(0x30+2));

    mapeo(TTBR0_T2,0x1E000000,0x1E000000,(0x30+2));

    //Mapeo de timer 
    mapeo(TTBR0_T2,0x10011000,0x10011000,(0x30+2));
    mapeo(TTBR0_T2,0x20000000,0x20000000,(0x30+2));

    num_table_2=0;
    // Falta separar el codigo y data de cada tarea

    // Cargo el TTBR0 
    asm("LDR R0,=0x70080000");
    asm("MCR p15, 0, R0, c2, c0, 0");
    // Todos los dominios van a ser manager.
    asm( "LDR R0, =0x55555555");
    asm("MCR p15, 0, R0, c3, c0, 0");

    


}