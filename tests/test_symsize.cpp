/**
 * @file test_symsize.cpp
 * @author Gai Brutman
 * @brief Symbol size calculation testing.
 * @version 0.1
 * @date 2022-03-12
 */

#include <stdlib.h>
#include <cstddef>

#include "gtest/gtest.h"

extern "C"
{
#include "arch/symsize.h"
}

volatile bool is_prime(int n)
{
    if (n <= 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

volatile int is_prime_delim()
{
    return 0;
}

TEST(symsize, prime)
{
    ASSERT_EQ((size_t)is_prime_delim - (size_t)is_prime, calc_function_size((void *)is_prime));
}