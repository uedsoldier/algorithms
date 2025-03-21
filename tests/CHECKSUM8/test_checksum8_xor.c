#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checksum8.h"

#define TEST_NAME "Checksum8 (XOR) tester"

/**
 * @brief Test case structure for CHECKSUM8 XOR tests
 */
typedef struct {
    const char *description;  // Test case description
    const uint8_t *input;     // Input data to encode
    size_t input_len;         // Length of input data
    uint8_t expected_checksum_output;
} TestCase;

/**
 * @brief Test cases for CHECKSUM8 XOR calculation
 */
static const TestCase test_cases[] = {
    // Basic cases
    {
        .description = "Simple sequence",
        .input = (const uint8_t[]){0x01, 0x02, 0x03, 0x04},
        .input_len = 4,
        .expected_checksum_output = 0x04  // 0x01 ^ 0x02 ^ 0x03 ^ 0x04
    },
    {
        .description = "All 0xFF",
        .input = (const uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF},
        .input_len = 4,
        .expected_checksum_output = 0x00  // 0xFF ^ 0xFF ^ 0xFF ^ 0xFF
    },
    {
        .description = "All zeros",
        .input = (const uint8_t[]){0x00, 0x00, 0x00, 0x00},
        .input_len = 4,
        .expected_checksum_output = 0x00  // 0x00 ^ 0x00 ^ 0x00 ^ 0x00
    },
    {
        .description = "Alternating bits",
        .input = (const uint8_t[]){0xAA, 0x55},
        .input_len = 2,
        .expected_checksum_output = 0xFF  // 0xAA ^ 0x55
    },
    {
        .description = "Five byte sequence",
        .input = (const uint8_t[]){0x12, 0x34, 0x56, 0x78, 0x9A},
        .input_len = 5,
        .expected_checksum_output = 0x92  // 0x12 ^ 0x34 ^ 0x56 ^ 0x78 ^ 0x9A
    },
    // Edge cases
    {
        .description = "Single byte",
        .input = (const uint8_t[]){0x42},
        .input_len = 1,
        .expected_checksum_output = 0x42  // Single byte is its own checksum
    },
    {
        .description = "Alternating 0x00 and 0xFF",
        .input = (const uint8_t[]){0x00, 0xFF, 0x00, 0xFF},
        .input_len = 4,
        .expected_checksum_output = 0x00  // 0x00 ^ 0xFF ^ 0x00 ^ 0xFF
    },
    {
        .description = "Incremental sequence",
        .input =
            (const uint8_t[]){0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08},
        .input_len = 8,
        .expected_checksum_output =
            0x08  // 0x01 ^ 0x02 ^ 0x03 ^ 0x04 ^ 0x05 ^ 0x06 ^ 0x07 ^ 0x08
    },
    // Additional test cases
    {
        .description = "Palindrome sequence",
        .input = (const uint8_t[]){0x12, 0x34, 0x56, 0x56, 0x34, 0x12},
        .input_len = 6,
        .expected_checksum_output =
            0x00  // 0x12 ^ 0x34 ^ 0x56 ^ 0x56 ^ 0x34 ^ 0x12
    },
    {
        .description = "All same non-zero value",
        .input = (const uint8_t[]){0x55, 0x55, 0x55, 0x55, 0x55},
        .input_len = 5,
        .expected_checksum_output = 0x55  // 0x55 ^ 0x55 ^ 0x55 ^ 0x55 ^ 0x55
    },
    {
        .description = "Fibonacci sequence bytes",
        .input =
            (const uint8_t[]){0x01, 0x01, 0x02, 0x03, 0x05, 0x08, 0x0D, 0x15},
        .input_len = 8,
        .expected_checksum_output =
            0x14  // 0x01 ^ 0x01 ^ 0x02 ^ 0x03 ^ 0x05 ^ 0x08 ^ 0x0D ^ 0x15
    },
    {
        .description = "Powers of 2",
        .input =
            (const uint8_t[]){0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80},
        .input_len = 8,
        .expected_checksum_output = 0xFF  // XOR of powers of 2
    },
    {
        .description = "ASCII string",
        .input =
            (const uint8_t[]){0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21},  // "Hello!"
        .input_len = 6,
        .expected_checksum_output = 0x63  // H(0x48) ^ e(0x65) ^ l(0x6C) ^
                                          // l(0x6C) ^ o(0x6F) ^ !(0x21) = 0x63
    },
    {
        .description = "Decremental sequence",
        .input = (const uint8_t[]){0xFF, 0xFE, 0xFD, 0xFC},
        .input_len = 4,
        .expected_checksum_output = 0x00  // 0xFF ^ 0xFE ^ 0xFD ^ 0xFC
    },
    {
        .description = "Alternating nibbles",
        .input = (const uint8_t[]){0xA5, 0x5A, 0xA5, 0x5A},
        .input_len = 4,
        .expected_checksum_output = 0x00  // 0xA5 ^ 0x5A ^ 0xA5 ^ 0x5A
    },
    {
        .description = "Binary counting pattern",
        .input =
            (const uint8_t[]){0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F},
        .input_len = 8,
        .expected_checksum_output =
            0x55  // 0x00 ^ 0x01 ^ 0x03 ^ 0x07 ^ 0x0F ^ 0x1F ^ 0x3F ^ 0x7F
    },
    {
        .description = "Large byte sequence",
        .input =
            (const uint8_t[]){0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                              0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00},
        .input_len = 16,
        .expected_checksum_output = 0x00  // XOR of all bytes
    },
    {
        .description = "Repeated pattern",
        .input =
            (const uint8_t[]){0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A},
        .input_len = 8,
        .expected_checksum_output = 0x00  // XOR of repeated pattern
    },
    {
        .description = "Prime numbers",
        .input =
            (const uint8_t[]){0x02, 0x03, 0x05, 0x07, 0x0B, 0x0D, 0x11, 0x13},
        .input_len = 8,
        .expected_checksum_output =
            0x07  // 0x02 ^ 0x03 ^ 0x05 ^ 0x07 ^ 0x0B ^ 0x0D ^ 0x11 ^ 0x13
    }};
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
 * @brief Run a single checksum test case
 */
static bool run_single_test(const TestCase *test, size_t test_number) {
    printf("\n--- Test %u: %s ---\n", test_number + 1, test->description);

    // Print input data
    printf("Input length: %u bytes\n", test->input_len);
    printf("Input hex:    ");
    print_hex(test->input, test->input_len);

    // Calculate checksum - pass pointer to first element instead of array
    uint8_t calculated_checksum =
        checksum8((void *)&test->input[0], test->input_len, CHECKSUM8_XOR);

    printf("Expected checksum: 0x%02X\n", test->expected_checksum_output);
    printf("Calculated checksum: 0x%02X\n", calculated_checksum);

    bool test_passed = (calculated_checksum == test->expected_checksum_output);
    printf("Test %u result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");

    return test_passed;
}

int main(void) {
    size_t total_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    size_t passed_tests = 0;

    printf("Running %u %s...\n", total_tests, TEST_NAME);

    // Run all tests
    for (size_t i = 0; i < total_tests; i++) {
        if (run_single_test(&test_cases[i], i)) {
            passed_tests++;
        }
    }

    // Print summary
    printf("\nTest Summary:\n");
    printf("Passed: %u/%u\n", passed_tests, total_tests);
    printf("Failed: %u/%u\n", total_tests - passed_tests, total_tests);

    // Add final result line to match CMake's expected output
    printf("Final result: %s\n", (passed_tests == total_tests)
                                     ? "ALL TESTS PASSED"
                                     : "SOME TESTS FAILED");

    return (passed_tests == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}
