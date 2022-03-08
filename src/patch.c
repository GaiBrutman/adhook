#include "patch.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "arch/jump.h"
#include "hook.h"

struct patch_priv
{
    void *symbol_backup;
    size_t backup_size;
};

static int prepare_patch(struct hook *hook, void *symbol, size_t size)
{
    int ret = 0;

    // Allocate private struct.
    struct patch_priv *priv = malloc(sizeof(struct patch_priv));
    if (NULL == priv)
    {
        ret = ENOMEM;
        goto out;
    }

    // Backup the original code.
    priv->symbol_backup = malloc(size);
    priv->backup_size = size;
    if (NULL == priv->symbol_backup)
    {
        ret = ENOMEM;
        goto free_priv;
    }
    memcpy(priv->symbol_backup, symbol, size);

    hook->priv = (void *)priv;
    goto out;

free_priv:
    free(priv);
out:
    return ret;
}

int patch_install(struct hook *hook, void *symbol, size_t size)
{
    int ret = 0;

    // Hook must have at least one handle when installing.
    if (NULL == hook || NULL == hook->handles)
    {
        ret = EINVAL;
        goto out;
    }

    if (hook->attached)
    {
        ret = EALREADY;
        goto out;
    }

    ret = prepare_patch(hook, symbol, size);
    // Patch jump to first handle.
    patch_jump(symbol, size, hook->handles->callback);

    hook->symbol = symbol;
    hook->attached = true;

out:
    return ret;
}

void patch_uninstall(struct hook *hook)
{
    struct patch_priv *priv = NULL;

    if (NULL == hook || !hook->attached || NULL == hook->priv)
    {
        return;
    }

    priv = (struct patch_priv *)hook->priv;
    if (NULL == priv->symbol_backup)
    {
        // ? Should we set hook->attached to false?
        goto free_priv;
    }

    // Restore the original code.
    memcpy(hook->symbol, priv->symbol_backup, get_payload_size());
    hook->attached = false;

    free(priv->symbol_backup);

free_priv:
    free(priv);
}