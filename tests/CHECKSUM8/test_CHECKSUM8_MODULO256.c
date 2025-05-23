#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checksum8.h"
#include "test_utils.h"

#define TEST_NAME "Checksum8 (modulo 256) tester"

/**
 * @brief Test case structure for CHECKSUM8 modulo 256 tests
 */
typedef struct {
    const char *description;  // Test case description
    const uint8_t *input;     // Input data to encode
    size_t input_len;         // Length of input data
    uint8_t expected_checksum_output;
} TestCase;

/**
 * @brief Test cases for CHECKSUM8 modulo 256 calculation
 */
static const TestCase test_cases[] = {
    // Basic cases
    {
        .description = "Simple sequence",
        .input = (const uint8_t[]){0x01, 0x02, 0x03, 0x04},
        .input_len = 4,
        .expected_checksum_output = 0x0A  // (0x01 + 0x02 + 0x03 + 0x04) % 256
    },
    {
        .description = "All 0xFF",
        .input = (const uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF},
        .input_len = 4,
        .expected_checksum_output = 0xFC  // (0xFF + 0xFF + 0xFF + 0xFF) % 256
    },
    {
        .description = "All zeros",
        .input = (const uint8_t[]){0x00, 0x00, 0x00, 0x00},
        .input_len = 4,
        .expected_checksum_output = 0x00  // (0x00 + 0x00 + 0x00 + 0x00) % 256
    },
    {
        .description = "Alternating bits",
        .input = (const uint8_t[]){0xAA, 0x55},
        .input_len = 2,
        .expected_checksum_output = 0xFF  // (0xAA + 0x55) % 256
    },
    {
        .description = "Five byte sequence",
        .input = (const uint8_t[]){0x12, 0x34, 0x56, 0x78, 0x9A},
        .input_len = 5,
        .expected_checksum_output =
            0xAE  // (0x12 + 0x34 + 0x56 + 0x78 + 0x9A) % 256
    },
    // Edge cases
    {
        .description = "Single byte",
        .input = (const uint8_t[]){0x42},
        .input_len = 1,
        .expected_checksum_output = 0x42  // (0x42) % 256
    },
    {
        .description = "Alternating 0x00 and 0xFF",
        .input = (const uint8_t[]){0x00, 0xFF, 0x00, 0xFF},
        .input_len = 4,
        .expected_checksum_output = 0xFE  // (0x00 + 0xFF + 0x00 + 0xFF) % 256
    },
    {
        .description = "ASCII string",
        .input =
            (const uint8_t[]){0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21},  // "Hello!"
        .input_len = 6,
        .expected_checksum_output =
            0x15  // ('H' + 'e' + 'l' + 'l' + 'o' + '!') % 256
    },
    {
        .description = "Byte values near overflow",
        .input = (const uint8_t[]){0xFE, 0xFE, 0xFE},
        .input_len = 3,
        .expected_checksum_output = 0xFA  // (0xFE + 0xFE + 0xFE) % 256
    },
    {
        .description = "Maximum byte values",
        .input = (const uint8_t[]){0xFF, 0xFF},
        .input_len = 2,
        .expected_checksum_output = 0xFE  // (0xFF + 0xFF) % 256
    }};

/**
 * @brief Run a single checksum test case
 */
static bool run_single_test(const TestCase *test, size_t test_number) {
    printf("\n--- Test %zu: %s ---\n", test_number + 1, test->description);

    // Print input data
    printf("Input length: %zu bytes\n", test->input_len);
    printf("Input hex:    ");
    print_hex(test->input, test->input_len);

    // Calculate checksum
    uint8_t calculated_checksum = checksum8(
        (void *)&test->input[0], test->input_len, CHECKSUM8_modulo256);

    printf("Expected checksum: 0x%02X\n", test->expected_checksum_output);
    printf("Calculated checksum: 0x%02X\n", calculated_checksum);

    bool test_passed = (calculated_checksum == test->expected_checksum_output);
    printf("Test %zu result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");

    return test_passed;
}

int main(void) {
    size_t total_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    size_t passed_tests = 0;

    printf("Running %zu %s...\n", total_tests, TEST_NAME);

    // Run all tests
    for (size_t i = 0; i < total_tests; i++) {
        if (run_single_test(&test_cases[i], i)) {
            passed_tests++;
        }
    }

    // Print summary
    printf("\nTest Summary:\n");
    printf("Passed: %zu/%zu\n", passed_tests, total_tests);
    printf("Failed: %zu/%zu\n", total_tests - passed_tests, total_tests);

    // Add final result line to match CMake's expected output
    printf("Final result: %s\n", (passed_tests == total_tests)
                                     ? "ALL TESTS PASSED"
                                     : "SOME TESTS FAILED");

    return (passed_tests == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}
