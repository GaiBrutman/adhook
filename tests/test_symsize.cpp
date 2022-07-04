/**
 * @file test_symsize.cpp
 * @author Gai Brutman
 * @brief Symbol size calculation testing.
 * @version 0.1
 * @date 2022-03-12
 */

#include <cstddef>
#include <stdlib.h>

#include <gtest/gtest.h>

extern "C" {
#include <adhook/arch/symsize.h>
}

bool is_prime(int n)
{
    if (n <= 1) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int is_prime_delim()
{
    return 0;
}

TEST(symsize, prime)
{
    ASSERT_EQ((size_t)is_prime_delim - (size_t)is_prime, calc_function_size((void *)is_prime));
}
