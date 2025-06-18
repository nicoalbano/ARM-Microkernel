#include "../../inc/timer.h"

__attribute__((section(".text"))) void __timer_init()
    {
        // GUARDO EN UN PUNTERO LA
        _timer_t* const TIMER0 = ( _timer_t* )TIMER0_ADDR;

        // Carga la cantidad de ticks
        TIMER0->Timer1Load     = 0x00010000;
        // Selecciona el timer de 32 bits
        TIMER0->Timer1Ctrl     = 0x00000002;
        // Selecciona el modo periodico
        TIMER0->Timer1Ctrl    |= 0x00000040;
        // Habilita la interrupcion 
        TIMER0->Timer1Ctrl    |= 0x00000020;
        // Habilita el timer
        TIMER0->Timer1Ctrl    |= 0x00000080;

    }