/**
 * @file jump.h
 * @author Gai Brutman
 * @brief Branch instruction patching mechanism.
 * @version 0.1
 * @date 2022-03-09
 */
#ifndef ADHOOK_ARCH_JUMP_H //NOLINT
#define ADHOOK_ARCH_JUMP_H

#include <stdlib.h>

/**
 * @brief Patch a jump instruction on a symbol.
 *
 * @param[in,out] symbol The symbol to patch.
 * @param[in] size The size of the symbol.
 * @param[in] target The target address of the jump.
 * @return int 0 on success, other on failure.
 */
int patch_jump(void *symbol, size_t size, void *target);

/**
 * @brief Get the payload size of a jump instruction.
 * @return The size of the jump instruction.
 */
size_t get_payload_size(void);

#endif // ADHOOK_ARCH_JUMP_H
