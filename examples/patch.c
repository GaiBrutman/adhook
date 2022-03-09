#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

#include "patch.h"
#include "hook.h"
#include "arch/symsize.h"

struct user
{
    char name[20];
    int age;
};

struct user g_admin_user = {
    .name = "admin",
    .age = 18,
};

static bool is_admin(struct user *user)
{
    if (NULL == user)
        return false;

    if (user->age < g_admin_user.age)
        return false;

    // No way the admin has aged that much since the program started.
    if (user->age > g_admin_user.age + 5)
        return false;

    return 0 == strcmp(user->name, g_admin_user.name);
}

struct hook g_is_admin_hook = {0};

static bool my_is_admin(struct user *user)
{
    printf("I just hooked is_admin :)\n");
    printf("I'm %s, %d years old.\n", user->name, user->age);

    user->age = 1000;

    call_next(&g_is_admin_hook, user);
    return true; // Fallback if no other handle is subscribed.
}

static bool this_is_another_is_admin_hook(struct user *user)
{
    printf("I just hooked is_admin as well :)\n");
    printf("I'm %s, %d years old.\n", user->name, user->age);
    return true;
}

int install_is_admin_hook(void)
{
    int ret = 0;
    struct handle *handle = NULL;
    size_t func_size = calc_function_size(is_admin);
    printf("func_size: %zu\n", func_size);

    handle = hook_subscribe(&g_is_admin_hook, my_is_admin);
    if (NULL == handle)
    {
        ret = ENOMEM;
        goto out;
    }

    handle = hook_subscribe(&g_is_admin_hook, this_is_another_is_admin_hook);
    if (NULL == handle)
    {
        ret = ENOMEM;
        goto out;
    }

    ret = patch_install(&g_is_admin_hook, is_admin, func_size);

out:
    return ret;
}

void uninstall_is_admin_hook(void)
{
    patch_uninstall(&g_is_admin_hook);
    hook_clear(&g_is_admin_hook);
}

#define PAGE_SIZE (size_t)4096
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define PAGE_ALIGN(addr) ((addr)&PAGE_MASK)

int main(void)
{
    struct user user = {
        .name = "guest",
        .age = 20};
    size_t is_admin_addr = (size_t)is_admin;

    // Set is_admin memory protection to RWX.
    if (0 != mprotect((void *)PAGE_ALIGN(is_admin_addr), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC))
    {
        printf("mprotect failed: %s\n", strerror(errno));
        return errno;
    }

    install_is_admin_hook();

    if (is_admin(&user))
    {
        printf("Welcome, admin!\n");
    }
    else
    {
        printf("Welcome, guest!\n");
    }

    uninstall_is_admin_hook();

    return 0;
}
