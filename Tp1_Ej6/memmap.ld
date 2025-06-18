OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_reset_vector)

/* Direcciones base */
_PUBLIC_RAM_INIT = 0x70010000;
_PUBLIC_STACK_INIT = 0x70020000;

C_STACK_SIZE = 128;
IRQ_STACK_SIZE = 1024;
FIQ_STACK_SIZE = 128;
SVC_STACK_SIZE = 1024;
ABT_STACK_SIZE = 128;
UND_STACK_SIZE = 128;
SYS_STACK_SIZE = 128;

/* Tamaño de stack a las tareas */

/* IDLE */

TAREA_IDLE_IRQ_STACK_SIZE = 256;
TAREA_IDLE_SVC_STACK_SIZE = 256;

/* Tarea 1*/
TAREA_1_IRQ_STACK_SIZE  = 1024;
TAREA_1_FIQ_STACK_SIZE  = 128;
TAREA_1_SVC_STACK_SIZE  = 1024;
TAREA_1_ABT_STACK_SIZE  = 128;
TAREA_1_UND_STACK_SIZE  = 128;
TAREA_1_SYS_STACK_SIZE  = 128;
TAREA_1_USR_STACK_SIZE = 1024;

/* Tarea 2*/
TAREA_2_IRQ_STACK_SIZE  = 1024;
TAREA_2_FIQ_STACK_SIZE  = 128;
TAREA_2_SVC_STACK_SIZE  = 1024;
TAREA_2_ABT_STACK_SIZE  = 128;
TAREA_2_UND_STACK_SIZE  = 128;
TAREA_2_SYS_STACK_SIZE  = 128;
TAREA_2_USR_STACK_SIZE = 1024;


/* Kernel */
_KERNEL_BSS_PHY         = 0x70022000;
_KERNEL_RODATA_PHY      = 0x70023000;
_KERNEL_TXT_PHY         = 0x70030000;
_KERNEL_DATA_PHY        = 0x70032000;

/* Tareas Fisicas */

_IDLE_TXT_PHY         = 0x70034000;

_TASK_1_TXT_PHY            = 0x80750000;
_TASK_1_DATA_PHY           = 0x80751000;
_TASK_1_STACK_INIT_PHY     = 0x80752000;
_TASK_1_BSS_PHY            = 0x80753000;
_TASK_1_RODATA_PHY         = 0x80754000;
_TASK_1_READ_AREA_PHY      = 0x80000000;

_TASK_2_TXT_PHY            = 0x80740000;
_TASK_2_DATA_PHY           = 0x80741000;
_TASK_2_STACK_INIT_PHY     = 0X80742000;
_TASK_2_BSS_PHY            = 0x80743000;
_TASK_2_RODATA_PHY         = 0x80744000;
_TASK_2_READ_AREA_PHY      = 0x80010000;

/* VMA */ 

/* Kernel */
_KERNEL_BSS_VMA         = 0x70022000;
_KERNEL_RODATA_VMA      = 0x70023000;
_KERNEL_TXT_VMA         = 0x70030000;
_KERNEL_DATA_VMA        = 0x70032000;

/* IDLE */ 

_IDLE_TXT_VMA = 0x70040000;
 
/* Tarea 1 */ 
_TASK_1_READ_VMA = 0X70A00000;
_TASK_1_TEXT_VMA = 0X70F50000;
_TASK_1_DATA_VMA = 0X70F51000;
_TASK_1_STACK_VMA = 0X70F52000;
_TASK_1_BSS_VMA  = 0X70F53000;
_TASK_1_RODATA_VMA = 0X70F54000;

/* Tarea 2 */

_TASK_2_READ_VMA = 0X70A10000;
_TASK_2_TEXT_VMA = 0X70F40000;
_TASK_2_DATA_VMA = 0X70F41000;
_TASK_2_BSS_VMA  = 0X70F44000;
_TASK_2_STACK_VMA = 0X70F42000;
_TASK_2_RODATA_VMA = 0X70F44000;

/* Regiones de memoria */
MEMORY
{
    public_ram    : org = _PUBLIC_RAM_INIT,     len = 512M
    public_stack  : org = _PUBLIC_STACK_INIT,   len = 64K
    tarea_1_stack_vma    : org = _TASK_1_STACK_VMA, len = 0x1000
    tarea_1_stack_phy    : org = _TASK_1_STACK_INIT_PHY, len = 0x1000
    tarea_2_stack_vma    : org = _TASK_2_STACK_VMA, len = 0x1000
    tarea_2_stack_phy    : org = _TASK_2_STACK_INIT_PHY, len = 0x1000

}

/* Secciones */
SECTIONS
{
    . = _PUBLIC_RAM_INIT;

    /* Kernel code */
    .text : {
        *(.reset*)
        *(.text*)
        *(.handler*)
    } > public_ram

    /* Read-only data (e.g., const) */
    .rodata : {
        *(.rodata*)
    } > public_ram

    /* Initialized data */
    .data : {
        *(.data*)
        *(.kernel_data*)
    } > public_ram

    /* BSS */
    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
    } > public_ram

    /* ================= Kernel ================= */
        /* Código de kernel */    
    _KERNEL_TXT_LMA = .;  /* Digo que quiero que me lo ubique a continuacion */ 
    .kernel_text _KERNEL_TXT_VMA : AT (_KERNEL_TXT_LMA)  /* Que la seccion se ensamble a continuacion pero que se execute en esa dir virtual*/ 
    { 
        *(.kernel_text*);
        . = ALIGN(4); 
    } > public_ram
    __kernel_size__ = SIZEOF(.kernel_text);


        /* Código de tarea idle */    
    _IDLE_TXT_LMA = __kernel_size__ + LOADADDR(.kernel_text); /* Defino donde empieza la localizacion de idle que es el tamaño de la seccion anterior mas la dir base*/
    .idle_text _IDLE_TXT_VMA : AT (_IDLE_TXT_LMA)
    { 
        *(.idle_text*);
        . = ALIGN(4); 
    } > public_ram
    __idle_size__ = SIZEOF(.idle_text);


        /* BSS de kernel */ 
    _KERNEL_BSS_LMA = __idle_size__ + LOADADDR(.idle_text);
    .kernel_bss _KERNEL_BSS_VMA : AT(_KERNEL_BSS_LMA)
    {
        *(.kernel_bss*);
        . = ALIGN(4); 
    } > public_ram
    __kernel_bss_size__ = SIZEOF(.kernel_bss);

    /* Data de kernel */ 
    _KERNEL_DATA_LMA = __kernel_bss_size__ + LOADADDR(.kernel_bss);
    .kernel_data _KERNEL_DATA_VMA : AT(_KERNEL_DATA_LMA)
    {
        *(.kernel_data*);
        . = ALIGN(4);
    } > public_ram
    __kernel_data_size__ = SIZEOF(.kernel_data);

    /* Read only data de kernel */ 
    _KERNEL_RODATA_LMA = __kernel_data_size__ + LOADADDR(.kernel_data);
    .kernel_rodata _KERNEL_RODATA_VMA : AT(_KERNEL_RODATA_LMA)
    {
        *(.kernel_rodata*);
        . = ALIGN(4);
    } > public_ram
    __kernel_rodata_size__ = SIZEOF(.kernel_rodata);



    /* ================= Tareas ================= */

    /* =============================== Tarea 1 =============================== */

    /* Código de Tarea 1 */ 
    _TAREA_1_TXT_LMA = __kernel_rodata_size__ + LOADADDR(.kernel_rodata);
    .task_1_code _TASK_1_TEXT_VMA : AT(_TAREA_1_TXT_LMA)
    {
        *(.task_1_code*);
        . = ALIGN(4);
    } > public_ram
    __tarea_1_txt_size__ = SIZEOF(.task_1_code);    

    /* BSS de Tarea 1 */ 
    _TAREA_1_BSS_LMA = __tarea_1_txt_size__ + LOADADDR(.task_1_code);
    .bss_tarea_1 _TASK_1_BSS_VMA : AT(_TAREA_1_BSS_LMA)
    {
        *(.task_1_bss*);
        . = ALIGN(4); 
    } > public_ram
    __tarea_1_bss_size__ = SIZEOF(.bss_tarea_1);

    /* Data de Tarea 1 */ 
    _TAREA_1_DATA_LMA = __tarea_1_bss_size__ + LOADADDR(.bss_tarea_1);
    .data_tarea_1 _TASK_1_DATA_VMA : AT(_TAREA_1_DATA_LMA)
    {
        *(.task_1_data*);
        . = ALIGN(4);
    } > public_ram
    __tarea_1_data_size__ = SIZEOF(.data_tarea_1);

    /* Read only data de la tarea 1 */ 
    _TAREA_1_RODATA_LMA = __tarea_1_data_size__ + LOADADDR(.data_tarea_1);
    .tarea_1_rodata _TASK_1_RODATA_VMA : AT(_TAREA_1_RODATA_LMA)
    {
        *(.tarea_1_rodata*);
        . = ALIGN(4);
    } > public_ram
    __tarea_1_rodata_size__ = SIZEOF(.tarea_1_rodata);



    /* =================== TAREA 2 ===================== * / 

        /* Código de Tarea 2 */ 
    _TAREA_2_TXT_LMA = __tarea_1_rodata_size__ + LOADADDR(.tarea_1_rodata);
    .txt_tarea_2 _TASK_2_TEXT_VMA : AT(_TAREA_2_TXT_LMA)
    {
        *(.task_2_code*);
        . = ALIGN(4);
    } > public_ram
    __tarea_2_txt_size__ = SIZEOF(.txt_tarea_2);    

    /* BSS de Tarea 2 */ 
    _TAREA_2_BSS_LMA = __tarea_2_txt_size__ + LOADADDR(.txt_tarea_2);
    .bss_tarea_2 _TASK_2_BSS_VMA : AT(_TAREA_2_BSS_LMA)
    {
        *(.tarea_2_bss*);
        . = ALIGN(4);
    } > public_ram
    __tarea_2_bss_size__ = SIZEOF(.bss_tarea_2);

    /* Data de Tarea 2 */ 
    _TAREA_2_DATA_LMA = __tarea_2_bss_size__ + LOADADDR(.bss_tarea_2);
    .data_tarea_2 _TASK_2_DATA_VMA : AT(_TAREA_2_DATA_LMA)
    {
        *(.tarea_2_data*);
        . = ALIGN(4);
    } > public_ram
    __tarea_2_data_size__ = SIZEOF(.data_tarea_2);

    /* Read only data de la tarea 2 */ 
    _TAREA_2_RODATA_LMA = __tarea_2_data_size__ + LOADADDR(.data_tarea_2);
    .tarea_2_rodata _TASK_2_RODATA_VMA : AT(_TAREA_2_RODATA_LMA)
    {
        *(.tarea_2_rodata*);
        . = ALIGN(4);
    } > public_ram
    __tarea_2_rodata_size__ = SIZEOF(.tarea_2_rodata);




        /* Stacks */
    . = _PUBLIC_STACK_INIT;
    .stack : 
    {
        . += TAREA_IDLE_IRQ_STACK_SIZE;
        . = ALIGN(4);
        __IRQ_IDLE_stack_top__ = .;

        . += (IRQ_STACK_SIZE - TAREA_IDLE_IRQ_STACK_SIZE);
        . = ALIGN(4);
        __irq_stack_top__ = .;
        
        . += FIQ_STACK_SIZE;
        . = ALIGN(4);
        __fiq_stack_top__ = .;

        . += TAREA_IDLE_SVC_STACK_SIZE;
        . = ALIGN(4);
        __SVC_IDLE_stack_top__ = .;

        . += (SVC_STACK_SIZE - TAREA_IDLE_SVC_STACK_SIZE);
        . = ALIGN(4);
        __svc_stack_top__ = .;

        . += ABT_STACK_SIZE;
        . = ALIGN(4);
        __abt_stack_top__ = .;

        . += UND_STACK_SIZE;
        . = ALIGN(4);
        __und_stack_top__ = .;

        . += SYS_STACK_SIZE;
        . = ALIGN(4);
        __sys_stack_top__ = .;
    } > public_stack

        . = _TASK_1_STACK_VMA;
    .tarea_1_stack_vma :
    {
        . += TAREA_1_IRQ_STACK_SIZE;
        . = ALIGN(4);
        __IRQ_TASK1_stack_top__ = .;   /* Asigno donde empieza el stack de irq, recordar que es full descending (\uparrow)*/

        . += TAREA_1_SVC_STACK_SIZE;
        . = ALIGN(4);
        __SVC_TASK1_stack_top__ = .;

        . += TAREA_1_ABT_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_abt_stack_top_vma__ = .;

        . += TAREA_1_UND_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_und_stack_top_vma__ = .;

        . += TAREA_1_SYS_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_sys_stack_top_vma__ = .;

        . += TAREA_1_USR_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_usr_stack_top_vma__ = .;


    } > tarea_1_stack_vma

    _tarea_1_stack_size_ = SIZEOF(.tarea_1_stack_vma);

    . = _TASK_2_STACK_VMA;
    .tarea_2_stack_vma :
    {
        . += TAREA_2_IRQ_STACK_SIZE;
        . = ALIGN(4);
        __IRQ_TASK2_stack_top__ = .;

        . += TAREA_2_SVC_STACK_SIZE;
        . = ALIGN(4);
        __SVC_TASK2_stack_top__ = .;

        . += TAREA_2_ABT_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_abt_stack_top_vma__ = .;

        . += TAREA_2_UND_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_und_stack_top_vma__ = .;

        . += TAREA_2_SYS_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_sys_stack_top_vma__ = .;

        . += TAREA_2_USR_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_usr_stack_top_vma__ = .;

    } > tarea_2_stack_vma

    _tarea_2_stack_size_ = SIZEOF(.tarea_2_stack_vma);
    
    . = _TASK_1_STACK_INIT_PHY;
    .tarea_1_stack_phy :
    {
        . += TAREA_1_IRQ_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_irq_stack_top_phy__ = .;   /* Asigno donde empieza el stack de irq, recordar que es full descending (\uparrow)*/

        . += TAREA_1_SVC_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_svc_stack_top_phy__ = .;

        . += TAREA_1_ABT_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_abt_stack_top_phy__ = .;

        . += TAREA_1_UND_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_und_stack_top_phy__ = .;

        . += TAREA_1_SYS_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_sys_stack_top_phy__ = .;

        . += TAREA_1_USR_STACK_SIZE;
        . = ALIGN(4);
        __tarea_1_usr_stack_top_phy__ = .;

    } > tarea_1_stack_phy
    _tarea_1_stack_phy_size_ = SIZEOF(.tarea_1_stack_phy);

    . = _TASK_2_STACK_INIT_PHY;
    .tarea_2_stack_phy :
    {
        . += TAREA_2_IRQ_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_irq_stack_top_phy__ = .;

        . += TAREA_2_SVC_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_svc_stack_top_phy__ = .;

        . += TAREA_2_ABT_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_abt_stack_top_phy__ = .;

        . += TAREA_2_UND_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_und_stack_top_phy__ = .;

        . += TAREA_2_SYS_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_sys_stack_top_phy__ = .;

        . += TAREA_2_USR_STACK_SIZE;
        . = ALIGN(4);
        __tarea_2_usr_stack_top_phy__ = .;

    } > tarea_2_stack_phy
    _tarea_2_stack_phy_size_ = SIZEOF(.tarea_2_stack_phy);

    
}
