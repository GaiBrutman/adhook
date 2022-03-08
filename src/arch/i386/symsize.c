#include "arch/symsize.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// TODO: Detect more function return instructions.
#define x86_RET_OPCODE (0xc3)

void *x86_search_for_ret(void *func)
{
    uint8_t *ptr = (uint8_t *)func;
    while (*ptr != x86_RET_OPCODE)
    {
        ptr++;
    }
    return ptr;
}

size_t calc_function_size(void *func)
{
    void *end = x86_search_for_ret(func);
    return (size_t)end - (size_t)func;
}
