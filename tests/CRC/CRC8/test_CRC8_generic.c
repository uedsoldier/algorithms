#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CRC.h"
#include "crc8_test_data.h"
#include "crc8_variants.h"
#include "test_utils.h"

// Test configuration based on compile-time definitions
#ifndef CRC8_VARIANT
#error "CRC8_VARIANT not defined. Please define it to one of the supported variants."
#endif

// Get the correct CRC type and expected array based on the variant
#if CRC8_VARIANT == V_CCITT
    #define CURRENT_CRC8_TYPE CRC8_CCITT
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_ccitt_crcs
#elif CRC8_VARIANT == V_CDMA2000
    #define CURRENT_CRC8_TYPE CRC8_CDMA2000
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_cdma2000_crcs
#elif CRC8_VARIANT == V_DARC
    #define CURRENT_CRC8_TYPE CRC8_DARC
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_darc_crcs
#elif CRC8_VARIANT == V_DVB_S2
    #define CURRENT_CRC8_TYPE CRC8_DVB_S2
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_dvb_s2_crcs
#elif CRC8_VARIANT == V_EBU
    #define CURRENT_CRC8_TYPE CRC8_EBU
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_ebu_crcs
#elif CRC8_VARIANT == V_I_CODE
    #define CURRENT_CRC8_TYPE CRC8_I_CODE
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_i_code_crcs
#elif CRC8_VARIANT == V_ITU
    #define CURRENT_CRC8_TYPE CRC8_ITU
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_itu_crcs
#elif CRC8_VARIANT == V_MAXIM
    #define CURRENT_CRC8_TYPE CRC8_MAXIM
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_maxim_crcs
#elif CRC8_VARIANT == V_ROHC
    #define CURRENT_CRC8_TYPE CRC8_ROHC
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_rohc_crcs
#elif CRC8_VARIANT == V_WCDMA
    #define CURRENT_CRC8_TYPE CRC8_WCDMA
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_wcdma_crcs
#elif CRC8_VARIANT == V_SAE_J1850
    #define CURRENT_CRC8_TYPE CRC8_SAE_J1850
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_1850_crcs
#elif CRC8_VARIANT == V_SAE_J1850_ZERO
    #define CURRENT_CRC8_TYPE CRC8_SAE_J1850_ZERO
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_1850_zero_crcs
#elif CRC8_VARIANT == V_AUTOSAR
    #define CURRENT_CRC8_TYPE CRC8_AUTOSAR
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_autosar_crcs
#elif CRC8_VARIANT == V_BLUETOOTH
    #define CURRENT_CRC8_TYPE CRC8_BLUETOOTH
    #define CURRENT_EXPECTED_CRC_ARRAY expected_crc8_bluetooth_crcs
#else
#error "Unsupported CRC8_VARIANT specified"
#endif

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
 * @brief Run a single CRC8 test case
 */
static bool run_single_test(const TestInput *test, uint8_t expected_crc,
                            size_t test_number) {
    printf("\n--- Test %zu: %s ---\n", test_number + 1, test->description);

    bool test_passed = true;
    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %zu bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Get CRC8 using the variant specified by CRC8_VARIANT
    test_passed = test_crc8(test->input, test->input_len, expected_crc, CURRENT_CRC8_TYPE);

    printf("Test %zu result: %s\n", test_number + 1, test_passed ? "PASSED" : "FAILED");
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
    printf("=== CRC8 Test ===\n");

    crc8_print_macro_settings();
    print_crc8_config(CURRENT_CRC8_TYPE);

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
