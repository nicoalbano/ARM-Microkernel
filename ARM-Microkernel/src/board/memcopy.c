#include <stdint.h>
#include <string.h>
#include "../../inc/memcopy.h"

void memcopy(uint32_t* dst, const uint32_t* src, uint32_t size_bytes) 
{
    uint32_t * pf= src;
    for (uint32_t i = 0; i < size_bytes / 4; i++) 
    {
        *(dst+i) = *(pf);
        pf++;
    }
    return;
}