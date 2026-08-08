#include "../../inc/mapeo.h"


#define L2_SMALL_PAGE        (0b10)          // bits [1:0]
#define L2_AP_RW_PRIV        (0 << 8) | (1 << 4)  // AP[2]=0, AP[1:0]=01 → solo kernel R/W
#define L2_CACHEABLE         (1 << 3)
#define L2_BUFFERABLE        (1 << 2)



__attribute__((aligned(0x4000)))  // 16 KB
uint32_t tabla_l1[4096] __attribute__((section(".tablas"), aligned(0x400)));                       // Aca le tengo que poner 


uint32_t tablas_l2[NUM_TABLAS_L2][256]__attribute__((section(".tablas"), aligned(0x400)));// 1 KB c/u

#define DESC_TYPE_PAGE_TABLE  0x01  // tipo para tabla de segundo nivel (nivel 1)
#define DESC_TYPE_SMALL_PAGE  0x02  // tipo para small page (nivel 2)

int num_table_2=0;

// tengo q modificar este codigo y usar el ttbr0 que me dan, no un parametro fisico 

__attribute__((section(".text")))void mapeo(uint32_t* ttbr0, uint32_t vir_address, uint32_t phy_address, uint32_t flags)
{
    uint32_t flags_ = L2_SMALL_PAGE | L2_AP_RW_PRIV | L2_CACHEABLE | L2_BUFFERABLE;
    uint32_t entry_l1= ( vir_address>>20 ) & 0xFFF; // Me quedo con los bits 31-20
    uint32_t entry_l2= ( vir_address>>12 ) & 0xFF; // Me quedo con los bits 20-12

    uint32_t *chq_if= (uint32_t) ttbr0 + (entry_l1 * 4);

    // Verifico que mi entrada de la tabla 1 no este en uso
    if((*(chq_if)& 0x3) !=0x1) // Pregunto si tengo que asignar una pagina nueva
    {
        if(num_table_2>= NUM_TABLAS_L2)
        {
            return; // No pudo asignar una pagina
        }

        // guardo el valor de la  nueva tabla correspondiente 
        uint32_t* tabla_l2_asignar= (uint32_t) (ttbr0) + 0x4000 + (num_table_2 * 0x400);

         // Cargo en la level 1
        *(ttbr0+entry_l1)=((uint32_t)tabla_l2_asignar & 0xFFFFFC00) | 0x01; // dejo los bits del 31-10 y pongo que sea una small page
        // incremento el contador de numero de paginas
        num_table_2++;

    }
    else
    {
        uint32_t* aux= (uint32_t) ttbr0 + (entry_l1 * 4);
        uint32_t *p_tabla_level_2= (*(aux) & 0xFFFFFC00);
        // Calcular la dirección exacta 
        uint32_t* entrada_l2 = p_tabla_level_2 + (entry_l2 );
        *entrada_l2= (phy_address & 0xFFFFF000)| (flags & 0xFFF);

        return;

    }
    uint32_t *puntero_aux= (uint32_t) (ttbr0) +0x4000 + (num_table_2-1)*0x400 + (entry_l2 * 4);
    
    *puntero_aux= (phy_address & 0xFFFFF000)|(flags & 0xFFF);// cargo en la tabla l2  
}