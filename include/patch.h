/**
 * @file patch.h
 * @author Gai Brutman
 * @brief Branch instruction patching mechanism.
 * @version 0.1
 * @date 2022-03-09
 */

#pragma once

#include "hook.h"

#include <stdlib.h>

/**
 * @brief Hook a symbol by patching it's begining with a jump.
 *
 * @param hook The hook to install.
 * @param symbol The symbol to hook.
 * @param size The size of the symbol.
 * @return int 0 on success, other on failure.
 */
int patch_install(struct hook *hook, void *symbol, size_t size);

/**
 * @brief Uninstall a hook installed by a jump patch.
 *
 * @param hook The hook to uninstall.
 */
void patch_uninstall(struct hook *hook);
