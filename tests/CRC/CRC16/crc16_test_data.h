#ifndef CRC16_TEST_DATA_H
#define CRC16_TEST_DATA_H

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
 * @brief Common test input data for all CRC16 variants
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
 * @brief Expected CRC values for CRC16-XMODEM
 */
const uint8_t expected_crc16_xmodem_crcs[] = {
    0x0000,  // "Hello, World!"
    0x0000,  // Empty string
    0x0000,  // "A"
    0x0000,  // "123456789"
    0x0000,  // All zeros
    0x0000,  // All ones
    0x0000,  // Alternating pattern
    0x0000,  // Binary sequence
    0x0000,  // Single byte 0xFF
    0x0000   // Random bytes
};

#endif /* CRC16_TEST_DATA_H */
