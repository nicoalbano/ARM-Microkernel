#include <stddef.h>
#include <stdint.h>

#include "../../inc/gic.h"
#include "../../inc/timer.h"
#include "../../inc/scheduler_manager.h"

uint32_t ticks=0;

extern uint32_t *actual_sp;

__attribute__((section(".text"))) uint32_t* _handler_irq(ctx_t* ctx)
{
    uint32_t* sp;
    // Leemos el IAR para identificar int
    _gicc_t* const GICC0 = (_gicc_t*)GICC0_ADDR;
    _timer_t* const TIMER0 = (_timer_t*)TIMER0_ADDR;

    //Guardo el sp actual en una variable global
    //actual_sp=(ctx->sp);

    uint32_t id=GICC0->IAR;

    // Manejo de interrupcion y limpiamos la fuente
    switch(id){

        case GIC_SOURCE_TIMER0 : {

            // Llamo al scheduler que devuelve mi nuevo sp en caso de cambiar
            sp=scheduler(ctx);

            TIMER0->Timer1IntClr=0x01;
            break;
        }
        default : {
            break;
        }
    }

    GICC0->EOIR= id; 
    return sp;

}