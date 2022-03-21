#include "arch/jump.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

// #define JUMP_SIZE 12

#define MOV_RAX "\x48\xb8"
#define PUSH_RAX "\x50"
#define RET "\xc3"
#define MOV_RAX_IMM64(imm64) MOV_RAX imm64
#define ADDR_PLACEHOLDER "\x00\x00\x00\x00\x00\x00\x00\x00"

#define JUMP_SIZE (sizeof(MOV_RAX) + sizeof(PUSH_RAX) + sizeof(RET) + sizeof(ADDR_PLACEHOLDER) - 4)

// TODO: Optimize for near jumps
int patch_jump(void *symbol, size_t size, void *target)
{
    char *payload = MOV_RAX_IMM64(ADDR_PLACEHOLDER) PUSH_RAX RET;

    if (size < JUMP_SIZE)
    {
        return EINVAL;
    }

    memcpy(symbol, payload, JUMP_SIZE);
    *(uint64_t *)((uint8_t *)symbol + sizeof(MOV_RAX) - 1) = (uint64_t)target;
    return 0;
}

size_t get_payload_size(void)
{
    return JUMP_SIZE;
}
