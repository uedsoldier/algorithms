#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "crc.h"
#include "crc16_test_data.h"
#include "crc16_variants.h"
#include "test_utils.h"

// Test configuration based on compile-time definitions
#ifndef CRC16_VARIANT
#error "CRC16_VARIANT not defined. Please define it to one of the supported variants."
#endif

// Get the correct CRC type and expected array based on the variant
#if CRC16_VARIANT == V_XMODEM
    #define CURRENT_CRC16_TYPE CRC16_XMODEM
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc16_xmodem_crcs
#else
#error "Unsupported CRC16_VARIANT specified"
#endif

/**
 * @brief Print CRC configuration details
 * @param type CRC type enumeration
 */
void print_crc16_config(crc_t type) {
    printf("=== CRC16 Configuration ===\n");

    printf("Type:           %s\n", get_crc_implementation_name(type));
    printf("Polynomial:     0x%04X\n", CRC16_getPoly(type));
    printf("Initial value:  0x%04X\n", CRC16_getSeed(type));
    printf("Final XOR:      0x%04X\n", CRC16_getFinalXOR(type));
    printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" : "No");
    printf("Reflect output: %s\n", CRC_getOutputReflected(type) ? "Yes" : "No");
    printf("CRC16 lookup table:    %s\n", CRC16_USE_LOOKUP_TABLE ? "Yes" : "No");

    printf("====================\n\n");
}

/**
 * @brief Run a single CRC16 test case
 */
static bool run_single_test(const TestInput *test, uint16_t expected_crc,
                            size_t test_number) {
    printf("\n--- Test %zu: %s ---\n", test_number + 1, test->description);

    bool test_passed = true;
    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %zu bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Get CRC16 using the variant specified by CRC16_VARIANT
    test_passed = test_crc16(test->input, test->input_len, expected_crc, CURRENT_CRC16_TYPE);

    printf("Test %zu result: %s\n", test_number + 1, test_passed ? "PASSED" : "FAILED");
    return test_passed;
}

/**
 * @brief Print the status of CRC-related macro settings
 */
void crc16_print_macro_settings(void) {
    printf("=== Macro Settings ===\n");
#if defined(CRC16_USE_LOOKUP_TABLE)
    printf("CRC16_USE_LOOKUP_TABLE is defined as: %d\n", CRC16_USE_LOOKUP_TABLE);
#else
    printf("CRC16_USE_LOOKUP_TABLE is not defined\n");
#endif

#if defined(CRC_USE_IMPLEMENTATION_NAMES)
    printf("CRC_USE_IMPLEMENTATION_NAMES is defined as: %d\n", CRC_USE_IMPLEMENTATION_NAMES);
#else
    printf("CRC_USE_IMPLEMENTATION_NAMES is not defined\n");
#endif
    printf("====================\n\n");
}

/**
 * @brief Main entry point for CRC8 tests
 */
int main(void) {
    /* Runtime check for array size match */
    assert(sizeof(CURRENT_EXPECTED_CRC_ARRAY) /
                   sizeof(CURRENT_EXPECTED_CRC_ARRAY[0]) ==
               TOTAL_TESTS &&
           "Expected CRC count must match test input count");

    // Print macro settings and CRC configuration
    printf("=== CRC16 Test ===\n");

    crc16_print_macro_settings();
    print_crc16_config(CURRENT_CRC16_TYPE);

    bool all_tests_passed = true;

    // Run all tests
    for (size_t i = 0; i < TOTAL_TESTS; i++) {
        if (!run_single_test(&test_inputs[i], CURRENT_EXPECTED_CRC_ARRAY[i], i)) {
            all_tests_passed = false;
        }
    }

    // Print final summary
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %zu\n", TOTAL_TESTS);
    printf("Final result: %s\n",
           all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
