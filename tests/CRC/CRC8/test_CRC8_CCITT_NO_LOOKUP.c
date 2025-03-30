#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crc.h"

#define TEST_NAME "CRC8-CCITT (without lookup table) tester"

/**
 * @brief Print the status of CRC-related macro settings
 */
static void print_macro_settings(void) {
    printf("=== Macro Settings ===\n");
#ifdef CRC8_USE_LOOKUP_TABLE
    printf("CRC8_USE_LOOKUP_TABLE is defined as: %d\n", CRC8_USE_LOOKUP_TABLE);
#else
    printf("CRC8_USE_LOOKUP_TABLE is not defined\n");
#endif

#ifdef CRC_USE_IMPLEMENTATION_NAMES
    printf("CRC_USE_IMPLEMENTATION_NAMES is defined as: %d\n",
           CRC_USE_IMPLEMENTATION_NAMES);
#else
    printf("CRC_USE_IMPLEMENTATION_NAMES is not defined\n");
#endif
    printf("====================\n\n");
}

/**
 * @brief Test case structure for CRC8 encoding/decoding tests
 */
typedef struct {
    const char *description;     // Test case description
    const uint8_t *input;        // Input data to encode
    size_t input_len;            // Length of input data
    crc_t crc_type;              // CRC algorithm variant to use
    const uint8_t expected_crc;  // Expected CRC8
} TestCase;

/**
 * @brief Test cases for CRC8 encoding/decoding
 */
static const TestCase test_cases[] = {
    {
        "Test case 1: Basic test",
        (const uint8_t *)"Hello, World!",
        13,
        CRC8_CCITT,
        0x87,
    },
    {
        "Test case 2: Empty string",
        (const uint8_t *)"",
        0,
        CRC8_CCITT,
        0x00,
    },
    {
        "Test case 3: Single byte",
        (const uint8_t *)"A",
        1,
        CRC8_CCITT,
        0xC0,
    },
    // Additional test cases
    {
        "Standard test vector '123456789'",
        (const uint8_t *)"123456789",
        9,
        CRC8_CCITT,
        0xF4,
    },
    {
        "All zeros (8 bytes)",
        (const uint8_t *)"\x00\x00\x00\x00\x00\x00\x00\x00",
        8,
        CRC8_CCITT,
        0x00,
    },
    {
        "All ones (8 bytes)",
        (const uint8_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
        8,
        CRC8_CCITT,
        0xD7,
    },
    {
        "Alternating pattern (0x55, 0xAA)",
        (const uint8_t *)"\x55\xAA\x55\xAA",
        4,
        CRC8_CCITT,
        0x6F,
    },
    {
        "Binary sequence",
        (const uint8_t *)"\x00\x01\x02\x03\x04\x05\x06\x07",
        8,
        CRC8_CCITT,
        0xD8,
    },
    {
        "Single byte (0xFF)",
        (const uint8_t *)"\xFF",
        1,
        CRC8_CCITT,
        0xF3,
    },
    {
        "Random bytes sequence",
        (const uint8_t *)"\x12\x34\x56\x78\x9A\xBC\xDE\xF0",
        8,
        CRC8_CCITT,
        0xE7,
    }};

#define TOTAL_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

/**
 * @brief Print data in hexadecimal format
 */
static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

/**
 * @brief Run a single Base64 test case
 */
static bool run_single_test(const TestCase *test, size_t test_number) {
    printf("\n--- Test %u: %s ---\n", test_number + 1, test->description);

    bool test_passed = true;
    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %u bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Get CRC8
    uint8_t calculated_crc = CRC8(test->input, test->input_len, test->crc_type);
    printf("CRC8 type:    %s\n", crc8_implementations[test->crc_type]);

    // Compare CRC8 with expected value
    bool crc_matches = (calculated_crc == test->expected_crc);
    test_passed = crc_matches;

    // Print info
    printf("Expected CRC8: 0x%02X\n", test->expected_crc);
    printf("Calculated CRC8: 0x%02X\n", calculated_crc);
    printf("CRC8 matches: %s\n", crc_matches ? "YES" : "NO");

    printf("Test %u result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");
    return test_passed;
}

/**
 * @brief Main entry point for Base64 tests
 */
int main(void) {
    printf("%s\n\n", TEST_NAME);
    print_macro_settings();
    bool all_tests_passed = true;

    // Run all tests
    for (size_t i = 0; i < TOTAL_TESTS; i++) {
        if (!run_single_test(&test_cases[i], i)) {
            all_tests_passed = false;
        }
    }

    // Print final summary
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %u\n", TOTAL_TESTS);
    printf("Final result: %s\n",
           all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
