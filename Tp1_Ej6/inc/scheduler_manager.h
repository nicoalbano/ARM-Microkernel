

#include <stddef.h>
#include <stdint.h>


#define CANT_TICKS_IDLE 8
#define CANT_TICKS_TASK_1 1
#define CANT_TICKS_TASK_2 1

#define TTBR0_IDLE 0x70080000
#define TTBR0_T1 0x70088000
#define TTBR0_T2 0x70090000

#define IDLE_ID 0
#define TASK_1_ID 1
#define TASK_2_ID 2
#define CANT_TASK 3

typedef struct ctx_t{
    uint32_t *sp,spsr,gpr[13],*lr;

}ctx_t;

typedef struct tcb_t{
        uint32_t* sp_irq;
        uint32_t* sp_svc;
        uint32_t* ptr_task;
        uint32_t cant_ticks;
        uint32_t task_id;
        uint32_t TTBR0;
        uint32_t* sp_usr;
}tcb_t;

typedef struct sch_m{
    uint32_t task_running;
    uint32_t ticks_exec;
    uint32_t * sp_c;
}sch_m;

__attribute__((section(".text"))) uint32_t* _handler_irq(ctx_t * ctx);
__attribute__((section(".text"))) uint32_t* scheduler(ctx_t *ctx);
__attribute__((section(".text"))) void TCB_init(void);