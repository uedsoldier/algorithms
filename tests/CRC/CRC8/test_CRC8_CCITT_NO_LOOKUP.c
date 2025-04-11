#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crc.h"
#include "crc8_test_data.h"
#include "test_utils.h"

#define TEST_NAME "CRC8-CCITT (without lookup table) tester"

/**
 * @brief Print CRC configuration details
 * @param type CRC type enumeration
 */
void print_crc8_config(crc_t type) {
    printf("=== CRC8 Configuration ===\n");

    printf("Type:           %s\n", get_crc_implementation_name(type));
    printf("Polynomial:     0x%02X\n", CRC8_getPoly(type));
    printf("Initial value:  0x%02X\n", CRC8_getSeed(type));
    printf("Final XOR:      0x%02X\n", CRC8_getFinalXOR(type));
    printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" : "No");
    printf("Reflect output: %s\n", CRC_getOutputReflected(type) ? "Yes" : "No");
    printf("CRC8 lookup table:    %s\n", CRC8_USE_LOOKUP_TABLE ? "Yes" : "No");

    printf("====================\n\n");
}

/**
 * @brief Run a single Base64 test case
 */
static bool run_single_test(const TestInput *test, uint8_t expected_crc,
                            size_t test_number) {
    printf("\n--- Test %u: %s ---\n", test_number + 1, test->description);

    bool test_passed = true;
    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %u bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Get CRC8
    test_passed = test_crc8(test->input, test->input_len, expected_crc, CRC8_CCITT);

    printf("Test %u result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");
    return test_passed;
}

/**
 * @brief Print the status of CRC-related macro settings
 */
void crc8_print_macro_settings(void) {
    printf("=== Macro Settings ===\n");
#if defined(CRC8_USE_LOOKUP_TABLE)
    printf("CRC8_USE_LOOKUP_TABLE is defined as: %d\n", CRC8_USE_LOOKUP_TABLE);
#else
    printf("CRC8_USE_LOOKUP_TABLE is not defined\n");
#endif

#if defined(CRC_USE_IMPLEMENTATION_NAMES)
    printf("CRC_USE_IMPLEMENTATION_NAMES is defined as: %d\n",
           CRC_USE_IMPLEMENTATION_NAMES);
#else
    printf("CRC_USE_IMPLEMENTATION_NAMES is not defined\n");
#endif
    printf("====================\n\n");
}

/**
 * @brief Main entry point for Base64 tests
 */
int main(void) {
    /* Runtime check for array size match */
    assert(sizeof(expected_crc8_ccitt_crcs) /
                   sizeof(expected_crc8_ccitt_crcs[0]) ==
               TOTAL_TESTS &&
           "Expected CRC count must match test input count");

    printf("%s\n\n", TEST_NAME);
    crc8_print_macro_settings();
    // Print CRC8 configuration
    print_crc8_config(CRC8_CCITT);
    bool all_tests_passed = true;

    // Run all tests
    for (size_t i = 0; i < TOTAL_TESTS; i++) {
        if (!run_single_test(&test_inputs[i], expected_crc8_ccitt_crcs[i], i)) {
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
