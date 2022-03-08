#include "arch/jump.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define JUMP_SIZE 6

// TODO: Optimize for near jumps
int patch_jump(void *symbol, size_t size, void *target)
{
    if (size < JUMP_SIZE)
    {
        return EINVAL;
    }
    memcpy(symbol, "\x68\x00\x00\x00\x00\xc3", JUMP_SIZE);
    *(uint32_t *)((uint8_t *)symbol + 1) = (uint32_t)(size_t)target;
    return 0;
}

size_t get_payload_size(void)
{
    return JUMP_SIZE;
}
