/**
 * @file jump.h
 * @author Gai Brutman
 * @brief Branch instruction patching mechanism.
 * @version 0.1
 * @date 2022-03-09
 */

#pragma once

#include <stdlib.h>

/**
 * @brief Patch a jump instruction on a symbol.
 *
 * @param symbol The symbol to patch.
 * @param size The size of the symbol.
 * @param target The target address of the jump.
 * @return int 0 on success, other on failure.
 */
int patch_jump(void *symbol, size_t size, void *target);

/**
 * @brief Get the payload size of a jump instruction.
 * @return The size of the jump instruction.
 */
size_t get_payload_size(void);
