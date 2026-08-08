#include <stdint.h>

#define NUM_TABLAS_L2 20
#define tabla_primer_nivel 0x70080000

#define TTBR0_IDLE 0x70080000
#define TTBR0_T1 0x70088000
#define TTBR0_T2 0x70090000


// Direcciones Virtuales 

#define DIR_V_IDLE_TEXT 
#define DIR_V_TASK1_TEXT 0x70F50000 
#define DIR_V_TASK2_TEXT 0x70F40000

#define DIR_V_IDLE_DATA
#define DIR_V_TASK1_DATA 0x70F51000 
#define DIR_V_TASK2_DATA 0x70F41000

#define DIR_V_IDLE_STACK
#define DIR_V_TASK1_STACK 0x70F52000
#define DIR_V_TASK2_STACK 0x70F42000 

// Direcciones fisicas

#define DIR_F_IDLE_TEXT
#define DIR_F_TASK1_TEXT 0x80750000
#define DIR_F_TASK2_TEXT 0x80740000

#define DIR_F_IDLE_DATA
#define DIR_F_TASK1_DATA 0x80751000
#define DIR_F_TASK2_DATA 0x80741000

#define DIR_V_IDLE_STACK
#define DIR_F_TASK1_STACK 0x80752000
#define DIR_F_TASK2_STACK 0x80742000


__attribute__((section(".text"))) void mapeo(uint32_t* ttbr0, uint32_t vir_address, uint32_t phy_address, uint32_t flags); 
