/**
 * @file test_hook.cpp
 * @author Gai Brutman
 * @brief Base hooking mechanism testing.
 * @version 0.1
 * @date 2022-03-12
 */

#include <stdlib.h>
#include <cstddef>

#include "gtest/gtest.h"

extern "C"
{
#include "hook.h"
}

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

void empty_func()
{
}

TEST(hook, init)
{
    struct hook hook = {0};
    ASSERT_EQ(NULL, hook.symbol);
    ASSERT_EQ(false, hook.attached);
    ASSERT_EQ(NULL, hook.handles);
    ASSERT_EQ(NULL, hook.current_handle);
    ASSERT_EQ(NULL, hook.last_handle);
    ASSERT_EQ(NULL, hook.priv);
}

TEST(hook, subscribe)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    // we only have one handle
    ASSERT_EQ(handle, hook.handles);
    ASSERT_EQ(handle, hook.current_handle);
    ASSERT_EQ(handle, hook.last_handle);
}

TEST(hook, subscribe_null)
{
    struct hook hook = {0};
    ASSERT_EQ(NULL, hook_subscribe(&hook, NULL));
    ASSERT_EQ(NULL, hook.handles);
    ASSERT_EQ(NULL, hook.current_handle);
    ASSERT_EQ(NULL, hook.last_handle);
}

TEST(hook, subscribe_twice)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    ASSERT_EQ(handle, hook.handles);
    ASSERT_EQ(handle, hook.current_handle);
    ASSERT_EQ(handle2, hook.last_handle);
}

TEST(hook, unsubscribe)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(NULL, hook.handles);
}

TEST(hook, unsubscribe_null)
{
    struct hook hook = {0};
    ASSERT_EQ(EINVAL, hook_unsubscribe(&hook, NULL));
    ASSERT_EQ(NULL, hook.handles);
}

TEST(hook, unsubscribe_not_subscribed)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    ASSERT_EQ(ESRCH, hook_unsubscribe(&hook, (struct handle *)(~(size_t)handle)));
    ASSERT_EQ(handle, hook.handles);
}

TEST(hook, unsibscribe_twice)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(NULL, hook.handles);
    ASSERT_EQ(NULL, hook.current_handle);
    ASSERT_EQ(NULL, hook.last_handle);

    ASSERT_EQ(ESRCH, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(NULL, hook.handles);
    ASSERT_EQ(NULL, hook.current_handle);
    ASSERT_EQ(NULL, hook.last_handle);
}

#pragma GCC diagnostic pop
