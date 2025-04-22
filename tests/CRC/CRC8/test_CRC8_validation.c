
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "crc.h"
#include "test_utils.h"

// Test data structure for validation tests
typedef struct {
    const char *description;
    const uint8_t *data;
    size_t data_len;
    uint8_t appended_crc;
    bool should_pass;
} ValidationTestCase;

// Test data - includes both valid and invalid cases
static const uint8_t test_data[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
static const uint8_t single_byte[] = {'X'};
static const uint8_t repeated_pattern[] = {'A', 'A', 'A', 'A', 'A'}; // Testing with repeated pattern
static const uint8_t special_chars[] = {'!', '@', '#', '$', '%', '^', '&', '*'}; // Testing with special characters
static const uint8_t long_data[] = "This is a longer test string to validate CRC with more data"; // Testing with longer data

// Helper function to create test buffer with appended CRC
static uint8_t* create_test_buffer(const uint8_t* data, size_t data_len, uint8_t crc) {
    uint8_t* buffer = malloc(data_len + 1); // +1 for CRC byte
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, data, data_len);
    buffer[data_len] = crc;
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
    crc_error_t result = CRC8_ValidateAppended(test_buffer, test->data_len + 1, crc_type);
    bool test_passed = (result == CRC_SUCCESS) == test->should_pass;

    // Print test details
    printf("Data: ");
    print_hex(test->data, test->data_len);
    printf("Appended CRC: 0x%02X\n", test->appended_crc);
    printf("Expected validation: %s\n", test->should_pass ? "PASS" : "FAIL");
    printf("Actual validation: %s\n", (result == CRC_SUCCESS) ? "PASS" : "FAIL");
    printf("Test result: %s\n", test_passed ? "PASSED" : "FAILED");

    free(test_buffer);
    return test_passed;
}

int main(void) {
    printf("=== CRC8 Validation Tests ===\n\n");

    // Create test cases
    ValidationTestCase test_cases[] = {
        // Original test cases
        {
            .description = "Valid CRC8 CCITT",
            .data = test_data,
            .data_len = sizeof(test_data),
            .appended_crc = 0xF4,  // Known good CRC8-CCITT for "123456789"
            .should_pass = true
        },
        {
            .description = "Invalid CRC8 CCITT",
            .data = test_data,
            .data_len = sizeof(test_data),
            .appended_crc = 0x00,  // Wrong CRC
            .should_pass = false
        },
        {
            .description = "Empty data with valid CRC",
            .data = (const uint8_t*)"",
            .data_len = 0,
            .appended_crc = CRC8("", 0, CRC8_CCITT),
            .should_pass = true
        },
        // New test cases
        {
            .description = "Single byte with valid CRC",
            .data = single_byte,
            .data_len = sizeof(single_byte),
            .appended_crc = CRC8(single_byte, sizeof(single_byte), CRC8_CCITT),
            .should_pass = true
        },
        {
            .description = "Single byte with invalid CRC",
            .data = single_byte,
            .data_len = sizeof(single_byte),
            .appended_crc = 0xFF,  // Wrong CRC
            .should_pass = false
        },
        {
            .description = "Repeated pattern with valid CRC",
            .data = repeated_pattern,
            .data_len = sizeof(repeated_pattern),
            .appended_crc = CRC8(repeated_pattern, sizeof(repeated_pattern), CRC8_CCITT),
            .should_pass = true
        },
        {
            .description = "Special characters with valid CRC",
            .data = special_chars,
            .data_len = sizeof(special_chars),
            .appended_crc = CRC8(special_chars, sizeof(special_chars), CRC8_CCITT),
            .should_pass = true
        },
        {
            .description = "Long data with valid CRC",
            .data = long_data,
            .data_len = strlen((const char*)long_data),
            .appended_crc = CRC8(long_data, strlen((const char*)long_data), CRC8_CCITT),
            .should_pass = true
        },
        {
            .description = "Long data with invalid CRC",
            .data = long_data,
            .data_len = strlen((const char*)long_data),
            .appended_crc = 0x42,  // Wrong CRC
            .should_pass = false
        }
    };

    size_t total_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    size_t passed_tests = 0;

    // Run all test cases
    for (size_t i = 0; i < total_tests; i++) {
        if (run_validation_test(&test_cases[i], CRC8_CCITT)) {
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
