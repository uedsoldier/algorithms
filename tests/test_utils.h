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

/**
 * @brief Test CRC8 checksum
 * @param data Pointer to input data buffer
 * @param len Length of input data in bytes
 * @param expected_crc Expected CRC8 checksum
 * @param crc_type CRC algorithm variant to use
 * @return true if the calculated CRC8 matches the expected value, false
 */
bool test_crc8(const uint8_t *data, size_t len, uint8_t expected_crc,
               crc_t crc_type);

/**
 * @brief Test CRC16 checksum
 * @param data Pointer to input data buffer
 * @param len Length of input data in bytes
 * @param expected_crc Expected CRC16 checksum
 * @param crc_type CRC algorithm variant to use
 * @return true if the calculated CRC16 matches the expected value, false
 */
bool test_crc16(const uint8_t *data, size_t len, uint16_t expected_crc,
                crc_t crc_type);

/**
 * @brief Test CRC32 checksum
 * @param data Pointer to input data buffer
 * @param len Length of input data in bytes
 * @param expected_crc Expected CRC32 checksum
 * @param crc_type CRC algorithm variant to use
 * @return true if the calculated CRC32 matches the expected value, false
 */
bool test_crc32(const uint8_t *data, size_t len, uint32_t expected_crc,
                crc_t crc_type);

#endif /* TEST_UTILS_H */
