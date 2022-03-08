#pragma once

#include <stdlib.h>

/**
 * @brief Calculate the size of a function.
 *
 * @param func The function to calculate the size of.
 * @return size_t The size of the function.
 */
size_t calc_function_size(void *func);
