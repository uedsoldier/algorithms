#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "crc.h"
#include "crc32_test_data.h"
#include "crc32_variants.h"
#include "test_utils.h"

// Test configuration based on compile-time definitions
#ifndef CRC32_VARIANT
#error "CRC32_VARIANT not defined. Please define it to one of the supported variants."
#endif

// Get the correct CRC type and expected array based on the variant
#if CRC32_VARIANT == V_D
    #define CURRENT_CRC32_TYPE CRC32_D
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc32_d_crcs
#else
#error "Unsupported CRC32_VARIANT specified"
#endif

/**
 * @brief Print CRC configuration details
 * @param type CRC type enumeration
 */
void print_crc32_config(crc_t type) {
    printf("=== CRC32 Configuration ===\n");

    printf("Type:           %s\n", get_crc_implementation_name(type));
    printf("Polynomial:     0x%08X\n", CRC32_getPoly(type));
    printf("Initial value:  0x%08X\n", CRC32_getSeed(type));
    printf("Final XOR:      0x%08X\n", CRC32_getFinalXOR(type));
    printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" : "No");
    printf("Reflect output: %s\n", CRC_getOutputReflected(type) ? "Yes" : "No");
    printf("CRC32 lookup table:    %s\n", CRC32_USE_LOOKUP_TABLE ? "Yes" : "No");

    printf("====================\n\n");
}

/**
 * @brief Run a single CRC32 test case
 */
static bool run_single_test(const TestInput *test, uint32_t expected_crc,
                            size_t test_number) {
    printf("\n--- Test %zu: %s ---\n", test_number + 1, test->description);

    bool test_passed = true;
    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %zu bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Get CRC32 using the variant specified by CRC32_VARIANT
    test_passed = test_crc32(test->input, test->input_len, expected_crc, CURRENT_CRC32_TYPE);

    printf("Test %zu result: %s\n", test_number + 1, test_passed ? "PASSED" : "FAILED");
    return test_passed;
}

/**
 * @brief Print the status of CRC-related macro settings
 */
void crc32_print_macro_settings(void) {
    printf("=== Macro Settings ===\n");
#if defined(CRC32_USE_LOOKUP_TABLE)
    printf("CRC32_USE_LOOKUP_TABLE is defined as: %d\n", CRC32_USE_LOOKUP_TABLE);
#else
    printf("CRC32_USE_LOOKUP_TABLE is not defined\n");
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
    printf("=== CRC32 Test ===\n");

    crc32_print_macro_settings();
    print_crc32_config(CURRENT_CRC32_TYPE);

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
