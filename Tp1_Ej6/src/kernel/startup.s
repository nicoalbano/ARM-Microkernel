.extern _reset_vector
.extern _undefined_vector
.extern _SVC_vector
.extern _prefetch_vector
.extern _data_a_vector
.extern _reserved_vector
.extern _IRQ_vector
.extern __irq_stack_top__
.extern __fiq_stack_top__
.extern __svc_stack_top__
.extern __abt_stack_top__
.extern __und_stack_top__
.extern __sys_stack_top__
.extern idle

.extern TCB_init

.extern __board_init




.equ USR_MODE, 0x10
.equ FIQ_MODE, 0x11
.equ IRQ_MODE, 0x12
.equ SVC_MODE, 0x13
.equ ABT_MODE, 0x17
.equ UND_MODE, 0x1B
.equ SYS_MODE, 0x1F
.equ DIR_FISICA2, 0x70800000

.equ I_BIT, 0x80
.equ F_BIT, 0x40

.code 32

.global _start
.global _table_start

.section .text

_table_start:
    LDR PC, add_reset_vector
    LDR PC, add_undefined_vector
    LDR PC, add_SVC_vector
    LDR PC, add_prefetch_vector
    LDR PC, add_data_a_vector
    LDR PC, add_reserved_vector
    LDR PC, add_IRQ_vector



add_reset_vector: .word _reset_vector
add_undefined_vector: .word _undefined_vector
add_SVC_vector: .word _SVC_vector
add_prefetch_vector: .word _prefetch_vector
add_data_a_vector: .word _data_a_vector
add_reserved_vector: .word _reserved_vector
add_IRQ_vector: .word _IRQ_vector 


_start: 
    _TABLE_COPY:
        MOV R0, #0
        LDR R1, =_table_start
        LDR R2, =_start
    _TABLE_LOOP:
        LDR R3, [R1], #4
        STR R3, [ R0 ], #4
    
        CMP R1, R2
        BNE _TABLE_LOOP

_STACK_INIT: 
    MSR cpsr_c,#(IRQ_MODE | I_BIT | F_BIT)
    LDR SP,=__irq_stack_top__

    MSR cpsr_c,#(FIQ_MODE | I_BIT | F_BIT)
    LDR SP,=__fiq_stack_top__

    MSR cpsr_c,#(ABT_MODE | I_BIT | F_BIT)
    LDR SP,=__abt_stack_top__

    MSR cpsr_c,#(UND_MODE | I_BIT | F_BIT)
    LDR SP,=__und_stack_top__

    MSR cpsr_c,#(SYS_MODE | I_BIT | F_BIT)

    LDR SP,=__sys_stack_top__

    MSR cpsr_c,#(SVC_MODE | I_BIT | F_BIT)
    LDR SP,=__svc_stack_top__


    LDR R0, =DIR_FISICA2
      LDR R1, =0x12345678
      STR R1, [R0]

 init_board:
    LDR R10,= __board_init
    MOV LR, PC
    BX R10

        // Habilitar MMU
    MRC p15, 0,R1, c1, c0, 0    // Leer reg. control.
    ORR R1, R1, #0x1             // Bit 0 es habilitación de MMU.
    MCR p15, 0, R1, c1, c0, 0   // Escribir reg. control.


    // Activo las interrupciones
    CPSIE i

    idle_:
        WFI
        B idle_

    .end