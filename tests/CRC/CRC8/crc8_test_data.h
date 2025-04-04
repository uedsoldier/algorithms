#ifndef CRC8_TEST_DATA_H
#define CRC8_TEST_DATA_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Common test input data structure
 */
typedef struct {
    const char *description;  // Test case description
    const uint8_t *input;     // Input data to encode
    size_t input_len;         // Length of input data
} TestInput;

/**
 * @brief Common test input data for all CRC8 variants
 */
static const TestInput test_inputs[] = {
    {"Test case 1: Basic test", (const uint8_t *)"Hello, World!", 13},
    {"Test case 2: Empty string", (const uint8_t *)"", 0},
    {"Test case 3: Single byte", (const uint8_t *)"A", 1},
    {"Standard test vector '123456789'", (const uint8_t *)"123456789", 9},
    {"All zeros (8 bytes)", (const uint8_t *)"\x00\x00\x00\x00\x00\x00\x00\x00",
     8},
    {"All ones (8 bytes)", (const uint8_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
     8},
    {"Alternating pattern (0x55, 0xAA)", (const uint8_t *)"\x55\xAA\x55\xAA",
     4},
    {"Binary sequence", (const uint8_t *)"\x00\x01\x02\x03\x04\x05\x06\x07", 8},
    {"Single byte (0xFF)", (const uint8_t *)"\xFF", 1},
    {"Random bytes sequence",
     (const uint8_t *)"\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 8}};

#define TOTAL_TESTS (sizeof(test_inputs) / sizeof(test_inputs[0]))

/**
 * @brief Expected CRC values for CRC8-CCITT
 */
const uint8_t expected_crc8_ccitt_crcs[] = {
    0x87,  // "Hello, World!"
    0x00,  // Empty string
    0xC0,  // "A"
    0xF4,  // "123456789"
    0x00,  // All zeros
    0xD7,  // All ones
    0x6F,  // Alternating pattern
    0xD8,  // Binary sequence
    0xF3,  // Single byte 0xFF
    0xE7   // Random bytes
};

/**
 * @brief Expected CRC values for CRC8-CDMA2000
 */
static const uint8_t expected_crc8_cdma2000_crcs[] = {
    0x56,  // "Hello, World!"
    0xFF,  // Empty string
    0x28,  // "A"
    0xDA,  // "123456789"
    0xD4,  // All zeros
    0x8B,  // All ones
    0xFA,  // Alternating pattern
    0x78,  // Binary sequence
    0x00,  // Single byte 0xFF
    0x39   // Random bytes
};

/**
 * @brief Expected CRC values for CRC8-MAXIM
 */
static const uint8_t expected_crc8_maxim_crcs[] = {
    0x9C,  // "Hello, World!"
    0x00,  // Empty string
    0x18,  // "A"
    0xA1,  // "123456789"
    0x00,  // All zeros
    0xC9,  // All ones
    0x9C,  // Alternating pattern
    0x0F,  // Binary sequence
    0x35,  // Single byte 0xFF
    0xF0   // Random bytes
};

/**
 * @brief Expected CRC values for CRC8-DARC
 */
static const uint8_t expected_crc8_darc_crcs[] = {
    0x8C,  // "Hello, World!"
    0x00,  // Empty string
    0x3c,  // "A"
    0x15,  // "123456789"
    0x00,  // All zeros
    0x0F,  // All ones
    0x72,  // Alternating pattern
    0xA5,  // Binary sequence
    0xC6,  // Single byte 0xFF
    0x4C   // Random bytes
};

#endif /* CRC8_TEST_DATA_H */
