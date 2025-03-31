/**
 * @file test_utils.h
 * @brief Common utilities for test cases
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>
#include <stdint.h>

#include "crc.h"

/**
 * @brief Print data in hexadecimal format
 * @param data Pointer to data buffer
 * @param len Length of data
 */
void print_hex(const uint8_t *data, size_t len);

/**
 * @brief Compare two byte arrays for equality
 */
bool bytes_equal(const uint8_t *s1, size_t len1, const uint8_t *s2,
                 size_t len2);

#endif /* TEST_UTILS_H */
