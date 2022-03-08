/**
 * @file hook.h
 * @author Gai Brutman
 * @brief Base hooking mechanism.
 * @version 0.1
 * @date 2022-03-09
 */

#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Call the next handle in the hook.
 */
#define call_next(hook, ...)                                             \
    ({                                                                   \
        if ((hook)->handles->next)                                       \
        {                                                                \
            (hook)->current_handle = (hook)->handles->next;              \
            void *__ret = (hook)->current_handle->callback(__VA_ARGS__); \
        }                                                                \
    })

/**
 * @brief Hook subscription handle.
 */
struct handle
{
    struct handle *next; // Next handle in the list.
    void *(*callback)(); // Callback function.
};

/**
 * @brief Hook structure.
 */
struct hook
{
    void *symbol;                  // Hooked symbol.
    bool attached;                 // Whether the hook is attached.
    struct handle *handles;        // Linked list of handles.
    struct handle *current_handle; // Current handle.
    struct handle *last_handle;    // Last handle.
    void *priv;                    // Private data.
};

/**
 * @brief Subscribe to a hook.
 *
 * @param hook The hook to subscribe to.
 * @param callback The callback to call when the hook is triggered.
 * @return struct handle* The handle to use when unsubscribing.
 */
struct handle *hook_subscribe(struct hook *hook, void *callback);

/**
 * @brief Unsubscribe from a hook.
 *
 * @param hook The hook to unsubscribe from.
 * @param handle The handle to unsubscribe.
 * @return int 0 on success, other on failure.
 */
int hook_unsubscribe(struct hook *hook, struct handle *handle);

/**
 * @brief Clear all handles from a hook.
 *
 * @param hook The hook to clear.
 * @return int 0 on success, other on failure.
 */
int hook_clear(struct hook *hook);
