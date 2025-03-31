#ifndef AES_TEST_DATA_H
#define AES_TEST_DATA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "AES_common.h"

/**
 * @brief Common test input data structure for all AES variants
 */
typedef struct {
    const char* description;
    const char* input_string;
    const uint8_t* key;  // Pointer to key of appropriate size
    size_t key_size;     // Size of the key in bytes
    const uint8_t init_vector[AES_BLOCK_LEN];
    bool usePKCS7;
} AES_TestInput;

/* Key definitions for different AES variants */
static const uint8_t aes128_key1[16] = {'A', 'E', 'S', '1', '2', '8', '_', 'S',
                                        'E', 'C', 'R', 'E', 'T', 'K', 'E', 'Y'};
static const uint8_t aes128_key2[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
static const uint8_t aes128_key3[16] = {'S', 'M', 'A', 'L', 'L', '_', 'K', 'E',
                                        'Y', '1', '2', '3', '4', '5', '6', '7'};
static const uint8_t aes128_key4[16] = {'N', 'O', '_', 'P', 'A', 'D', '_', 'K',
                                        'E', 'Y', '1', '2', '3', '4', '5', '6'};
static const uint8_t aes128_key5[16] = {'S', 'P', 'E', 'C', '_', 'C', 'H', 'A',
                                        'R', 'S', '_', 'K', 'E', 'Y', '!', '@'};
static const uint8_t aes128_key6[16] = {'N', 'U', 'M', 'B', 'E', 'R', 'S', '_',
                                        'K', 'E', 'Y', '1', '2', '3', '4', '5'};
static const uint8_t aes128_key7[16] = {'L', 'O', 'N', 'G', '_', 'T', 'E', 'X',
                                        'T', '_', 'K', 'E', 'Y', '!', '@', '#'};

/* AES-192 keys can be defined similarly */
static const uint8_t aes192_key1[24] = {'A', 'E', 'S', '1', '9', '2', '_', 'S',
                                        'E', 'C', 'R', 'E', 'T', '_', 'K', 'E',
                                        'Y', '_', '2', '4', 'B', 'Y', 'T', 'E'};
static const uint8_t aes192_key2[24] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
                                        '0', '1', '2', '3', '4', '5', '6', '7'};
static const uint8_t aes192_key3[24] = {'S', 'E', 'C', 'U', 'R', 'E', '_', 'K',
                                        'E', 'Y', '_', '1', '9', '2', '_', 'B',
                                        'I', 'T', 'S', '!', '@', '#', '$', '%'};
static const uint8_t aes192_key4[24] = {'T', 'E', 'S', 'T', '_', 'K', 'E', 'Y',
                                        '_', 'F', 'O', 'R', '_', 'A', 'E', 'S',
                                        '1', '9', '2', '_', 'C', 'B', 'C', '!'};
static const uint8_t aes192_key5[24] = {'S', 'P', 'E', 'C', 'I', 'A', 'L', '_',
                                        'C', 'H', 'A', 'R', 'S', '_', '1', '9',
                                        '2', '@', '#', '$', '%', '^', '&', '*'};
static const uint8_t aes192_key6[24] = {'Z', 'E', 'R', 'O', '_', 'P', 'A', 'D',
                                        'D', 'I', 'N', 'G', '_', 'T', 'E', 'S',
                                        'T', '_', 'K', 'E', 'Y', '1', '9', '2'};
static const uint8_t aes192_key7[24] = {'M', 'U', 'L', 'T', 'I', '_', 'B', 'L',
                                        'O', 'C', 'K', '_', 'T', 'E', 'S', 'T',
                                        '_', 'K', 'E', 'Y', '1', '9', '2', '!'};

/* AES-256 keys can be defined similarly */
static const uint8_t aes256_key1[32] = {'A', 'E', 'S', '2', '5', '6', '_', 'S',
                                        'E', 'C', 'R', 'E', 'T', '_', 'K', 'E',
                                        'Y', '_', '3', '2', 'B', 'Y', 'T', 'E',
                                        'S', '_', 'L', 'O', 'N', 'G', '!', '!'};
static const uint8_t aes256_key2[32] = {'L', 'O', 'N', 'G', '_', 'S', 'E', 'C',
                                        'U', 'R', 'E', '_', 'K', 'E', 'Y', '_',
                                        'F', 'O', 'R', '_', 'A', 'E', 'S', '2',
                                        '5', '6', '_', 'T', 'E', 'S', 'T', 'S'};
static const uint8_t aes256_key3[32] = {'S', 'P', 'E', 'C', 'I', 'A', 'L', '_',
                                        'C', 'H', 'A', 'R', 'S', '_', '2', '5',
                                        '6', '_', 'K', 'E', 'Y', '@', '#', '$',
                                        '%', '^', '&', '*', '(', ')', '-', '+'};
static const uint8_t aes256_key4[32] = {'Z', 'E', 'R', 'O', '_', 'P', 'A', 'D',
                                        'D', 'I', 'N', 'G', '_', 'T', 'E', 'S',
                                        'T', '_', 'K', 'E', 'Y', '_', '2', '5',
                                        '6', '_', 'B', 'I', 'T', 'S', '!', '!'};
static const uint8_t aes256_key5[32] = {'M', 'U', 'L', 'T', 'I', '_', 'B', 'L',
                                        'O', 'C', 'K', '_', 'T', 'E', 'S', 'T',
                                        '_', 'K', 'E', 'Y', '_', '2', '5', '6',
                                        '_', 'B', 'I', 'T', 'S', '!', '@', '#'};

/**
 * @brief Common test input data for all AES variants
 */
static const AES_TestInput test_inputs[] = {
    {"Standard test with text that requires padding",
     "This is a test string for AES mode!",
     aes128_key1,
     16,
     {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5',
      '6'},
     true},
    {"Exact block size test (16 bytes)",
     "ExactBlock16BytesX",
     aes128_key2,
     16,
     {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
      'P'},
     true},
    {"Small text test",
     "Small text!",
     aes128_key3,
     16,
     {'s', 'm', 'a', 'l', 'l', '_', 'i', 'v', '1', '2', '3', '4', '5', '6', '7',
      '8'},
     true},
    {"Test without PKCS7 padding (multiple of block size)",
     "16 byte block one16 byte block two",
     aes128_key4,
     16,
     {'n', 'o', '_', 'p', 'a', 'd', '_', 'i', 'v', '1', '2', '3', '4', '5', '6',
      '7'},
     false},
    {"Special characters test",
     "Special @#$%^&* chars!",
     aes128_key5,
     16,
     {'s', 'p', 'e', 'c', '_', 'c', 'h', 'a', 'r', 's', '_', 'i', 'v', '!', '@',
      '#'},
     true},
    {"Numeric content test",
     "123456789 123456789",
     aes128_key6,
     16,
     {'n', 'u', 'm', '_', 'i', 'v', '_', '1', '2', '3', '4', '5', '6', '7', '8',
      '9'},
     true},
    {"Long text test (multiple blocks)",
     "This is a longer test string that will require multiple blocks of AES "
     "encryption!",
     aes128_key7,
     16,
     {'l', 'o', 'n', 'g', '_', 't', 'e', 'x', 't', '_', 'i', 'v', '!', '@', '#',
      '$'},
     true},

    /* AES-192 test cases */
    {"AES-192 standard test",
     "This is a test string for AES-192 mode!",
     aes192_key1,
     24,
     {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5',
      '6'},
     true},
    {"AES-192 exact block size test",
     "ExactBlock16BytesX",
     aes192_key2,
     24,
     {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
      'P'},
     true},
    {"AES-192 small text test",
     "Small text!",
     aes192_key3,
     24,
     {'s', 'm', 'a', 'l', 'l', '_', 'i', 'v', '1', '9', '2', '_', 't', 'e', 's',
      't'},
     true},
    {"AES-192 special characters test",
     "Special @#$%^&* chars for 192!",
     aes192_key4,
     24,
     {'s', 'p', 'e', 'c', '_', '1', '9', '2', '_', 'i', 'v', '!', '@', '#', '$',
      '%'},
     true},
    {"AES-192 numeric content test",
     "123456789 987654321 192",
     aes192_key5,
     24,
     {'n', 'u', 'm', '_', '1', '9', '2', '_', 'i', 'v', '_', 't', 'e', 's', 't',
      '!'},
     true},
    {"AES-192 test without padding",
     "Exact 32 byte text for no padding test",
     aes192_key6,
     24,
     {'n', 'o', '_', 'p', 'a', 'd', '_', '1', '9', '2', '_', 'i', 'v', '!', '@',
      '#'},
     false},
    {"AES-192 long text test",
     "This is a longer test string that will require multiple blocks for "
     "AES-192 encryption!",
     aes192_key7,
     24,
     {'l', 'o', 'n', 'g', '_', '1', '9', '2', '_', 'i', 'v', '_', 't', 'e', 's',
      't'},
     true},

    /* AES-256 test cases */
    {"AES-256 standard test",
     "This is a test string for AES-256 mode!",
     aes256_key1,
     32,
     {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5',
      '6'},
     true},
    {"AES-256 exact block size test",
     "ExactBlock16BytesX",
     aes256_key2,
     32,
     {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
      'P'},
     true},
    {"AES-256 special characters test",
     "Special @#$%^&* chars for 256!",
     aes256_key3,
     32,
     {'s', 'p', 'e', 'c', '_', '2', '5', '6', '_', 'i', 'v', '!', '@', '#', '$',
      '%'},
     true},
    {"AES-256 test without padding",
     "Exact 32 byte text for no padding test",
     aes256_key4,
     32,
     {'n', 'o', '_', 'p', 'a', 'd', '_', '2', '5', '6', '_', 'i', 'v', '!', '@',
      '#'},
     false},
    {"AES-256 long text test",
     "This is a longer test string that will require multiple blocks for "
     "AES-256 encryption!",
     aes256_key5,
     32,
     {'l', 'o', 'n', 'g', '_', '2', '5', '6', '_', 'i', 'v', '_', 't', 'e', 's',
      't'},
     true}};

#define TOTAL_TESTS (sizeof(test_inputs) / sizeof(test_inputs[0]))

/* Helper macros to get tests for specific AES variants */
#define AES128_TESTS_COUNT 7
#define AES192_TESTS_COUNT 7
#define AES256_TESTS_COUNT 5

/* Helper functions to filter tests by key size */
static inline const AES_TestInput* get_aes128_tests(void) {
    return &test_inputs[0];
}

static inline const AES_TestInput* get_aes192_tests(void) {
    return &test_inputs[AES128_TESTS_COUNT];
}

static inline const AES_TestInput* get_aes256_tests(void) {
    return &test_inputs[AES128_TESTS_COUNT + AES192_TESTS_COUNT];
}

#endif /* AES_TEST_DATA_H */
