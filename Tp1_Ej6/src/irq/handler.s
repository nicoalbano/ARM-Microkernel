.global  _undefined_vector
.global  _SVC_vector
.global  _prefetch_vector
.global  _data_a_vector
.global  _reserved_vector
.global  _IRQ_vector

.extern _handler_irq
.extern _rec_kernel

.code 32

.section .handlers , "ax"

_undefined_vector: 
    B . 
_SVC_vector: 

    sub lr,lr,#4
    stmfd sp!, {r0-r12,lr}
    bl _rec_kernel
    ldmfd sp!, { r0-r12, pc}^

    

_prefetch_vector: 
    mrc p15, 0, R3, c6, c0, 0
    mrc p15, 0, R4, c5, c0, 1
    B . 
_data_a_vector:

    mrc p15, 0, R1, c6, c0, 0
    mrc p15, 0, R2, c5, c0, 0

    B .
_reserved_vector: 
    B . 
_IRQ_vector:

    SUB lr,lr,#4
    stmfd sp!,{ r0-r12, lr} /*store multiple full decending, empieza a guardar los registros y actualiza el sp al ultimo de la pila */
    mov r7, sp  // 0x7002048
    mrs r8, spsr /* guardo el sp del modo guardado*/ 

    push {r7, r8} 

    mov r0, sp // 0x7002040 este es el sp despues de pushear el sp y el spsr, lo paso como parametro a la funcion
    bl _handler_irq

    mov sp,r0
    pop {r7, r8}

    mov sp,r7
    msr spsr, r8
 
    ldmfd sp!, { r0-r12, pc}^ /*Lo que hace el ^ es que el cpsr se carga con el spsr */
    .end
