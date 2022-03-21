/**
 * @file symsize.h
 * @author Gai Brutman
 * @brief Symbol size calculation.
 * @version 0.1
 * @date 2022-03-09
 */

#pragma once

#include <stdlib.h>

/**
 * @brief Calculate the size of a function.
 *
 * @param[in] func The function to calculate the size of.
 * @return size_t The size of the function.
 */
size_t calc_function_size(void *func);
