#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "crc.h"
#include "crc32_test_data.h"
#include "test_utils.h"

// Test data structure for validation tests
typedef struct {
    const char *description;
    const uint8_t *data;
    size_t data_len;
    uint32_t appended_crc;
    bool should_pass;
} ValidationTestCase;

// Test data - includes both valid and invalid cases
static const uint8_t test_data[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
static const uint8_t single_byte[] = {'X'};
static const uint8_t repeated_pattern[] = {'A', 'A', 'A', 'A', 'A'}; // Testing with repeated pattern
static const uint8_t special_chars[] = {'!', '@', '#', '$', '%', '^', '&', '*'}; // Testing with special characters
static const uint8_t long_data[] = "This is a longer test string to validate CRC with more data"; // Testing with longer data

// Helper function to create test buffer with appended CRC
static uint8_t* create_test_buffer(const uint8_t* data, size_t data_len, uint32_t crc) {
    uint8_t* buffer = malloc(data_len + 4); // +4 for CRC32 bytes
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, data, data_len);
    // Store CRC32 in big-endian format
    buffer[data_len] = (uint8_t)(crc >> 24);     // Highest byte
    buffer[data_len + 1] = (uint8_t)(crc >> 16); // Second highest byte
    buffer[data_len + 2] = (uint8_t)(crc >> 8);  // Second lowest byte
    buffer[data_len + 3] = (uint8_t)(crc);       // Lowest byte
    return buffer;
}

// Run a single validation test case
static bool run_validation_test(const ValidationTestCase* test, crc_t crc_type) {
    printf("\n--- Test: %s ---\n", test->description);

    // Create test buffer with appended CRC
    uint8_t* test_buffer = create_test_buffer(test->data, test->data_len, test->appended_crc);
    if (test_buffer == NULL) {
        printf("Failed to allocate test buffer\n");
        return false;
    }

    // Run validation
    crc_error_t result = CRC32_ValidateAppended(test_buffer, test->data_len + 4, crc_type);
    bool test_passed = (result == CRC_SUCCESS) == test->should_pass;

    // Print test details
    printf("Data: ");
    print_hex(test->data, test->data_len);
    printf("Appended CRC: 0x%08X\n", test->appended_crc);
    printf("Expected validation: %s\n", test->should_pass ? "PASS" : "FAIL");
    printf("Actual validation: %s\n", (result == CRC_SUCCESS) ? "PASS" : "FAIL");
    printf("Test result: %s\n", test_passed ? "PASSED" : "FAILED");

    free(test_buffer);
    return test_passed;
}

int main(void) {
    printf("=== CRC32 D Validation Tests ===\n\n");

    // Create test cases
    ValidationTestCase test_cases[] = {
        // Test case with correct CRC
        {
            .description = "Valid CRC32 D",
            .data = test_data,
            .data_len = sizeof(test_data),
            .appended_crc = 0x87315576,  // Known good CRC32-D for "123456789"
            .should_pass = true
        },
        // Test case with incorrect CRC
        {
            .description = "Invalid CRC32 D",
            .data = test_data,
            .data_len = sizeof(test_data),
            .appended_crc = 0x00000000,  // Wrong CRC
            .should_pass = false
        },
        // Test with empty data
        {
            .description = "Empty data with valid CRC",
            .data = (const uint8_t*)"",
            .data_len = 0,
            .appended_crc = 0x00000000,  // CRC32 D for empty string
            .should_pass = true
        },
        // Single byte tests
        {
            .description = "Single byte with valid CRC",
            .data = single_byte,
            .data_len = sizeof(single_byte),
            .appended_crc = CRC32(single_byte, sizeof(single_byte), CRC32_D),
            .should_pass = true
        },
        {
            .description = "Single byte with invalid CRC",
            .data = single_byte,
            .data_len = sizeof(single_byte),
            .appended_crc = 0xFFFFFFFF,  // Wrong CRC
            .should_pass = false
        },
        // Pattern tests
        {
            .description = "Repeated pattern with valid CRC",
            .data = repeated_pattern,
            .data_len = sizeof(repeated_pattern),
            .appended_crc = CRC32(repeated_pattern, sizeof(repeated_pattern), CRC32_D),
            .should_pass = true
        },
        // Special character tests
        {
            .description = "Special characters with valid CRC",
            .data = special_chars,
            .data_len = sizeof(special_chars),
            .appended_crc = CRC32(special_chars, sizeof(special_chars), CRC32_D),
            .should_pass = true
        },
        // Long data tests
        {
            .description = "Long data with valid CRC",
            .data = long_data,
            .data_len = strlen((const char*)long_data),
            .appended_crc = CRC32(long_data, strlen((const char*)long_data), CRC32_D),
            .should_pass = true
        },
        {
            .description = "Long data with invalid CRC",
            .data = long_data,
            .data_len = strlen((const char*)long_data),
            .appended_crc = 0x12345678,  // Wrong CRC
            .should_pass = false
        }
    };

    size_t total_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    size_t passed_tests = 0;

    // Run all test cases
    for (size_t i = 0; i < total_tests; i++) {
        if (run_validation_test(&test_cases[i], CRC32_D)) {
            passed_tests++;
        }
    }

    // Print summary
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %zu\n", total_tests);
    printf("Passed tests: %zu\n", passed_tests);
    printf("Failed tests: %zu\n", total_tests - passed_tests);
    printf("Final result: %s\n",
           (passed_tests == total_tests) ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return (passed_tests == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}
