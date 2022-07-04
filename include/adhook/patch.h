/**
 * @file patch.h
 * @author Gai Brutman
 * @brief Branch instruction patching mechanism.
 * @version 0.1
 * @date 2022-03-09
 */
#ifndef ADHOOK_PATCH_H
#define ADHOOK_PATCH_H

#include <adhook/hook.h>
#include <stdlib.h>

/**
 * @brief Hook a symbol by patching it's begining with a jump.
 *
 * @param[in,out] hook The hook to install.
 * @param[in] symbol The symbol to hook.
 * @param[in] size The size of the symbol.
 * @return int 0 on success, other on failure.
 */
int patch_install(struct hook *hook, void *symbol, size_t size);

/**
 * @brief Uninstall a hook installed by a jump patch.
 *
 * @param[in,out] hook The hook to uninstall.
 */
void patch_uninstall(struct hook *hook);

#endif // ADHOOK_PATCH_H
