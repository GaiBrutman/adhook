#include "adhook/arch/symsize.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Detect more function return instructions.
#define x86_RET_OPCODE (0xc3)
#define x86_RET_OPCODE_SIZE (1)

// FIXME: There can be more than one return instruction.
void *x86_search_for_ret(void *func)
{
    uint8_t *ptr = (uint8_t *)func;
    while (*ptr != x86_RET_OPCODE) {
        ptr++;
    }
    return ptr;
}

size_t calc_function_size(void *func)
{
    void *end = x86_search_for_ret(func);
    return ((size_t)end - (size_t)func) + x86_RET_OPCODE_SIZE;
}
