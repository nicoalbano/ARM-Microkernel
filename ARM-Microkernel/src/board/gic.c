#include "../../inc/gic.h"

__attribute__((section(".text"))) void __gic_init()
    {
        //Guardo en un puntero la direccion de mi interfaz de registros del GIC 0 
        _gicc_t* const GICC0 = (_gicc_t*)GICC0_ADDR;
        // Guardo en un puntero la direccion de mi distribuidor de registros del GIC 0
        _gicd_t* const GICD0 = (_gicd_t*)GICD0_ADDR;

        // PONEMOS 0XF EN EL REGISTRO DE PRIORITY MASK PARA QUE LAS INTERRUPCIONES CON PRIORIDAD 0XF SEAN ENMASCARADAS, NO ASI LAS DE PRIORIDAD 0XE O 0X0
        GICC0->PMR  = 0x000000F0;
        // Habilita la interrupcion de timer 0 
        GICD0->ISENABLER[1] |= 0x00000010;
        // Habilita la UART0
        GICD0->ISENABLER[1] |= 0x00001000;
        // HABILITA LA INTERFAZ DEL CPU PARA ESTE GIC0
        GICC0->CTLR         = 0x00000001;
        // HABILITA LA INTERRUPCIONES DEL GIC
        GICD0->CTLR         = 0x00000001;

    }