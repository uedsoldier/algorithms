/**
 * @file base64.h
 * @brief Header file for Base64 encoding and decoding using a struct.
 * @version 0.2
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef BASE64_H
#define BASE64_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Enum for special base64 values.
 */
enum special_e
{
    notabase64 = 64, // Value to return when a non base64 digit is found.
    terminator = 65, // Value to return when the character '=' is found.
};

/**
 * @brief Lookup table that converts a base64 digit to integer.
 */
static const uint8_t digit2bin[] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 65, 64, 64,
    64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};

/**
 * @brief Lookup table that converts an integer to a base64 digit.
 */
static const uint8_t bin2digit[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * @brief Structure representing a 3-byte block for base64 encoding/decoding.
 */
typedef struct
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
} block_t;

/**
 * @brief Context structure for base64 operations.
 */
typedef struct
{
    const uint8_t *src;
    uint8_t *dest;
    size_t size;
} base64_context_t;

uint8_t *bin_to_base64(uint8_t *dest, const void *src, size_t size);
void *base64_to_bin(uint8_t *dest, const uint8_t *src, size_t size);

#endif /*BASE64_H*/
