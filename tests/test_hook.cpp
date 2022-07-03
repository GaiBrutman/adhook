/**
 * @file test_hook.cpp
 * @author Gai Brutman
 * @brief Base hooking mechanism testing.
 * @version 0.1
 * @date 2022-03-12
 */

#include <stdlib.h>
#include <cstddef>

#include <gtest/gtest.h>

extern "C"
{
#include <adhook/hook.h>
}

#pragma GCC diagnostic push
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

TEST(hook, subscribe_unsubscribe_subscribe)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(NULL, hook.handles);
    ASSERT_EQ(NULL, hook.current_handle);
    ASSERT_EQ(NULL, hook.last_handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    ASSERT_EQ(handle2, hook.handles);
    ASSERT_EQ(handle2, hook.current_handle);
    ASSERT_EQ(handle2, hook.last_handle);
}

TEST(hook, unsubscribe_first)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(handle2, hook.handles);
    ASSERT_EQ(handle2, hook.current_handle);
    ASSERT_EQ(handle2, hook.last_handle);
}

TEST(hook, unsubscribe_last)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle2));
    ASSERT_EQ(handle, hook.handles);
    ASSERT_EQ(handle, hook.current_handle);
    ASSERT_EQ(handle, hook.last_handle);
}

TEST(hook, three_handles_unsubscribe_first)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    struct handle *handle3 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle3);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle));
    ASSERT_EQ(handle2, hook.handles);
    ASSERT_EQ(handle2, hook.current_handle);
    ASSERT_EQ(handle3, hook.last_handle);
}

TEST(hook, three_handles_unsubscribe_second)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    struct handle *handle3 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle3);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle2));
    ASSERT_EQ(handle, hook.handles);
    ASSERT_EQ(handle, hook.current_handle);
    ASSERT_EQ(handle3, hook.last_handle);
}

TEST(hook, three_handles_unsubscribe_third)
{
    struct hook hook = {0};
    struct handle *handle = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle);

    struct handle *handle2 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle2);

    struct handle *handle3 = hook_subscribe(&hook, (void *)empty_func);
    ASSERT_NE((void *)NULL, handle3);

    ASSERT_EQ(0, hook_unsubscribe(&hook, handle3));
    ASSERT_EQ(handle, hook.handles);
    ASSERT_EQ(handle, hook.current_handle);
    ASSERT_EQ(handle2, hook.last_handle);
}

TEST(hook, attach)
{
    struct hook hook = {0};
    ASSERT_EQ(hook.attached, false);

    ASSERT_EQ(0, hook_attach(&hook, (void *)empty_func));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);
}

TEST(hook, attach_twice)
{
    struct hook hook = {0};
    ASSERT_EQ(hook.attached, false);

    ASSERT_EQ(0, hook_attach(&hook, (void *)empty_func));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);

    ASSERT_EQ(EALREADY, hook_attach(&hook, (void *)0xDEADBEEF));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);
}

TEST(hook, attach_invalid)
{
    struct hook hook = {0};
    ASSERT_EQ(hook.attached, false);

    ASSERT_EQ(EINVAL, hook_attach(&hook, NULL));

    ASSERT_EQ(hook.attached, false);
    ASSERT_EQ(NULL, hook.symbol);

    ASSERT_EQ(EINVAL, hook_attach(NULL, (void *)empty_func));
}

TEST(hook, attach_then_attach_invalid)
{
    struct hook hook = {0};
    ASSERT_EQ(hook.attached, false);

    ASSERT_EQ(0, hook_attach(&hook, (void *)empty_func));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);

    ASSERT_EQ(EINVAL, hook_attach(&hook, NULL));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);
}

TEST(hook, attach_detach)
{
    struct hook hook = {0};
    ASSERT_EQ(hook.attached, false);

    ASSERT_EQ(0, hook_attach(&hook, (void *)empty_func));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);

    ASSERT_EQ(0, hook_detach(&hook));

    ASSERT_EQ(hook.attached, false);
    ASSERT_EQ(NULL, hook.symbol);
}

TEST(hook, detach_no_attach)
{
    struct hook hook = {0};

    ASSERT_EQ(EALREADY, hook_detach(&hook));

    ASSERT_EQ(hook.attached, false);
    ASSERT_EQ(NULL, hook.symbol);
}

TEST(hook, attach_detach_detach)
{
    struct hook hook = {0};

    ASSERT_EQ(0, hook_attach(&hook, (void *)empty_func));

    ASSERT_EQ(hook.attached, true);
    ASSERT_EQ(empty_func, hook.symbol);

    ASSERT_EQ(0, hook_detach(&hook));

    ASSERT_EQ(hook.attached, false);
    ASSERT_EQ(NULL, hook.symbol);

    ASSERT_EQ(EALREADY, hook_detach(&hook));

    ASSERT_EQ(hook.attached, false);
    ASSERT_EQ(NULL, hook.symbol);
}

#pragma GCC diagnostic pop
