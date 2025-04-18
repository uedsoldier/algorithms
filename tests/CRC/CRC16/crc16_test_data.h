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
     (const uint8_t *)"\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 8},

    /* New complex test cases */
    {"Large block (256 bytes)", (const uint8_t *)
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
        "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
        "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
        "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
        "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
        "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
        "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
        "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
        "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
        "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
        "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
        "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
        "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
        "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
        "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
        "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF", 256},

    {"Repeating pattern", (const uint8_t *)
        "\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5\xA5", 16},

    {"Edge transition pattern", (const uint8_t *)
        "\x00\xFF\x00\xFF\x00\xFF\x00\xFF\x00\xFF\x00\xFF\x00\xFF\x00\xFF", 16},

    {"ASCII printable characters", (const uint8_t *)
        "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", 95},

    {"Polynomial-like sequence", (const uint8_t *)
        "\x01\x02\x04\x08\x10\x20\x40\x80\x1B\x36\x6C\xD8\xAB\x4D\x9A\x2F", 16},

    {"UTF-8 encoded characters", (const uint8_t *)
        "\xE2\x82\xAC\xC2\xA9\xE2\x84\xA2\xC2\xAE\xE2\x85\x93", 13},

    {"Network packet header simulation", (const uint8_t *)
        "\x45\x00\x00\x73\x00\x00\x40\x00\x40\x11\x00\x00\xC0\xA8\x00\x01", 16},

    {"Fibonacci sequence bytes", (const uint8_t *)
        "\x01\x01\x02\x03\x05\x08\x0D\x15\x22\x37\x59\x90\xE9\x79\x62\xDB", 16},

    {"Mixed binary patterns", (const uint8_t *)
        "\xAA\x55\x0F\xF0\xA5\x5A\x3C\xC3\x69\x96\xC6\x3C\x35\x53\xCC\x33", 16}};

#define TOTAL_TESTS (sizeof(test_inputs) / sizeof(test_inputs[0]))

/**
 * @brief Expected CRC values for CRC16-XMODEM
 */
const uint16_t expected_crc16_xmodem_crcs[] = {
    0x4FD6,  // "Hello, World!"
    0x0000,  // Empty string
    0x58E5,  // "A"
    0x31C3,  // "123456789"
    0x0000,  // All zeros (8 bytes)
    0xA6E1,  // All ones (8 bytes)
    0x56C9,  // Alternating pattern (0x55, 0xAA)
    0x26B3,  // Binary sequence
    0x1EF0,  // Single byte 0xFF
    0x341A,  // Random bytes sequence
    0x7E55,  // Large block (256 bytes)
    0xC063,  // Repeating pattern
    0xFCD2,  // Edge transition pattern
    0xAD61,  // ASCII printable characters
    0xB83F,  // Polynomial-like sequence
    0xFF83,  // UTF-8 encoded characters
    0xB61F,  // Network packet header simulation
    0xB338,  // Fibonacci sequence bytes
    0xC003   // Mixed binary patterns
};

#endif /* CRC16_TEST_DATA_H */
