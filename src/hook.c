#include "adhook/hook.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

int hook_attach(struct hook *hook, void *symbol)
{
    if (NULL == hook || NULL == symbol) {
        return EINVAL;
    }
    if (hook->attached) {
        return EALREADY;
    }

    hook->symbol = symbol;
    hook->attached = true;
    return 0;
}

int hook_detach(struct hook *hook)
{
    if (NULL == hook) {
        return EINVAL;
    }
    if (!hook->attached) {
        return EALREADY;
    }

    hook->symbol = NULL;
    hook->attached = false;
    return 0;
}

struct handle *hook_subscribe(struct hook *hook, void *callback)
{
    if (NULL == hook || NULL == callback) {
        errno = EINVAL;
        return NULL;
    }
    if (hook->attached) {
        errno = EBUSY;
        return NULL;
    }

    struct handle *handle = malloc(sizeof(struct handle));
    if (NULL == handle) {
        return NULL;
    }
    handle->callback = callback;
    handle->next = NULL;

    if (NULL == hook->handles) {
        // First handle.
        hook->handles = handle;
        hook->current_handle = handle;
    } else {
        // Link handle to hook.
        hook->last_handle->next = handle;
    }
    hook->last_handle = handle;

    return handle;
}

int hook_unsubscribe(struct hook *hook, struct handle *handle)
{
    if (NULL == hook || NULL == handle) {
        return EINVAL;
    }
    if (hook->attached) {
        return EBUSY;
    }

    struct handle *prev = NULL;
    struct handle *curr = hook->handles;
    while (NULL != curr) {
        if (curr == handle) {
            // Unlink handle from hook.
            if (NULL == prev) {
                hook->handles = hook->current_handle = curr->next;
            } else {
                prev->next = curr->next;
            }

            // unlink last handle
            if (curr == hook->last_handle) {
                hook->last_handle = prev;
            }

            // Free handle.
            free(handle);
            return 0;
        }

        prev = curr;
        curr = curr->next;
    }
    hook->current_handle = hook->handles;

    // did not find handle
    return ESRCH;
}

int hook_clear(struct hook *hook)
{
    if (NULL == hook) {
        return EINVAL;
    }
    if (hook->attached) {
        return EALREADY;
    }

    struct handle *handle = hook->handles;
    while (NULL != handle) {
        struct handle *next = handle->next;
        free(handle);
        handle = next;
    }

    hook->handles = NULL;
    hook->last_handle = NULL;

    return 0;
}
