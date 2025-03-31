#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AES192.h"
#include "PKCS7.h"
#include "aes_test_data.h"
#include "test_utils.h"

static const char *TEST_NAME = "AES-192 CBC tester";

/**
 * @brief Structure for test state
 */
typedef struct {
    AES192_ctx_t ctx;
    size_t input_len;
    size_t output_len;
    uint8_t encrypt_buffer[AES192_MAX_BUFFER_SIZE];
    uint8_t decrypt_buffer[AES192_MAX_BUFFER_SIZE];
} TestState;

static bool run_single_test(const AES_TestInput *test, size_t test_number) {
    // Verify key size
    assert(test->key_size == AES192_FIXED_KEY_SIZE && "Invalid key size for AES192");
    
    TestState state = {0};
    bool test_passed = true;

    printf("\n--- Test %zu: %s ---\n", test_number + 1, test->description);

    // Initialize input parameters
    state.input_len = strlen(test->input_string);

    printf("Input string (%zu bytes): %s\n", state.input_len,
           test->input_string);
    printf("Key: ");
    print_hex(test->key, AES192_FIXED_KEY_SIZE);
    printf("IV: ");
    print_hex(test->init_vector, AES_BLOCK_LEN);
    printf("PKCS7 Padding: %s\n", test->usePKCS7 ? "Enabled" : "Disabled");

    // Initialize AES context
    AES192_init_ctx(&state.ctx, test->key, test->init_vector);

    // Encrypt
    AES_errcode_t err_code =
        AES192_CBC_encrypt(&state.ctx, test->input_string, state.encrypt_buffer,
                           state.input_len, &state.output_len, test->usePKCS7);

    if (err_code != AES_CODE_OK) {
        printf("Encryption failed with code: %d\n", err_code);
        return false;
    }

    printf("Encrypted (%zu bytes): ", state.output_len);
    print_hex(state.encrypt_buffer, state.output_len);

    // Re-initialize context for decryption
    AES192_init_ctx(&state.ctx, test->key, test->init_vector);

    // Decrypt
    size_t decrypted_len;
    err_code = AES192_CBC_decrypt(&state.ctx, state.encrypt_buffer,
                                  state.decrypt_buffer, state.output_len,
                                  &decrypted_len, test->usePKCS7);

    if (err_code != AES_CODE_OK) {
        printf("Decryption failed with code: %d\n", err_code);
        return false;
    }

    printf("Decrypted (%zu bytes): ", decrypted_len);
    print_hex(state.decrypt_buffer, decrypted_len);
    printf("Decrypted text: %s\n", state.decrypt_buffer);

    // Verify result
    if (test->usePKCS7) {
        test_passed = (decrypted_len == state.input_len) &&
                      (memcmp(test->input_string, state.decrypt_buffer,
                              state.input_len) == 0);
    } else {
        test_passed = (decrypted_len >= state.input_len) &&
                      (memcmp(test->input_string, state.decrypt_buffer,
                              state.input_len) == 0);
    }

    printf("Test %zu result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");

    return test_passed;
}

int main(void) {
    printf("%s\n\n", TEST_NAME);
    bool all_tests_passed = true;

    // Get AES192-specific tests
    const AES_TestInput *aes192_tests = get_aes192_tests();

    // Run AES192 tests only
    for (size_t i = 0; i < AES192_TESTS_COUNT; i++) {
        if (!run_single_test(&aes192_tests[i], i)) {
            all_tests_passed = false;
        }
    }

    // Print final summary
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", AES192_TESTS_COUNT);
    printf("Final result: %s\n",
           all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
