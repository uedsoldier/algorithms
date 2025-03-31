#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AES256.h"
#include "PKCS7.h"
#include "test_utils.h"

static const char *TEST_NAME = "AES-256 ECB tester";

typedef struct {
    AES256_ctx_t AES256_ctx;
    AES_errcode_t AES256_code;
    const char *input_string;
    const uint8_t key[AES256_FIXED_KEY_SIZE];  // Changed to uint8_t
    const char *description;
    bool usePKCS7;
    size_t input_string_len;
    size_t key_len;
    size_t output_len;
    uint8_t AES256_encrypt_buffer[AES256_MAX_BUFFER_SIZE];
    uint8_t AES256_decrypt_buffer[AES256_MAX_BUFFER_SIZE];
} TestCase;

static const TestCase test_cases[] = {
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "This is a test string for AES-256 ECB mode!",
     .key = {'A', 'E', 'S', '2', '5', '6', '_', 'S', 'E', 'C', 'R',
             'E', 'T', '_', 'K', 'E', 'Y', '_', '3', '2', 'B', 'Y',
             'T', 'E', '_', 'S', 'E', 'C', 'U', 'R', 'E', '_'},
     .description = "Standard test with text that requires padding",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "ExactBlock16BytesX",
     .key = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
             'b', 'c', 'd', 'e', 'f', '0', '1', '2', '3', '4', '5',
             '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
     .description = "Exact block size test (16 bytes)",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "Short text",
     .key = {'S', 'E', 'C', 'R', 'E', 'T', 'K', 'E', 'Y', '1', '2',
             '3', '4', '5', '6', '7', '2', 'S', 'E', 'C', 'R', 'E',
             'T', 'K', 'E', 'Y', '1', '2', '3', '4', '5', '7'},
     .description = "Short text test",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "This is a multi-block input that will require more than "
                     "one block of AES encryption to fully process.",
     .key = {'T', 'e', 's', 't', 'K', 'e', 'y', '1', '2', '3', '4',
             '5', '6', '7', '8', '9', '0', 'T', 'e', 's', 't', 'K',
             'e', 'y', '1', '2', '3', '4', '5', '6', '7', '8'},
     .description = "Multi-block test with padding",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "ExactBlock16BytesXExactBlock16BytesY",
     .key = {'S', 'e', 'c', 'u', 'r', 'i', 't', 'y', 'K', 'e', 'y',
             '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1',
             '2', 'S', 'e', 'c', 'u', 'r', 'i', 't', 'y', 'K'},
     .description = "Multiple exact blocks test (32 bytes)",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "Special chars: !@#$%^&*()_+-=[]{}|;':\",./<>?",
     .key = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     .description = "Special characters test",
     .usePKCS7 = true,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string =
         "This is a test string for AES-256 ECB mode without padding!",
     .key = {'A', 'E', 'S', '2', '5', '6', '_', 'S', 'E', 'C', 'R',
             'E', 'T', '_', 'K', 'E', 'Y', '_', '3', '2', 'B', 'Y',
             'T', 'E', '_', 'S', 'E', 'C', 'U', 'R', 'E', '_'},
     .description = "Test with padding disabled",
     .usePKCS7 = false,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}},
    {.AES256_ctx = {0},
     .AES256_code = AES_CODE_OK,
     .input_string = "ExactBlock16BytesXExactBlock16BytesY",
     .key = {'S', 'E', 'C', 'R', 'E', 'T', 'K', 'E', 'Y', '1', '2',
             '3', '4', '5', '6', '7', '2', 'S', 'E', 'C', 'R', 'E',
             'T', 'K', 'E', 'Y', '1', '2', '3', '4', '5', '7'},
     .description = "Exact multiple blocks with padding disabled",
     .usePKCS7 = false,
     .input_string_len = 0,
     .key_len = AES256_FIXED_KEY_SIZE,
     .output_len = 0,
     .AES256_encrypt_buffer = {0},
     .AES256_decrypt_buffer = {0}}};

#define TOTAL_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

int main(void) {
    printf("%s\n\n", TEST_NAME);
    bool all_tests_passed = true;

    // Run tests
    for (size_t i = 0; i < TOTAL_TESTS; i++) {
        assert(i < TOTAL_TESTS);

        TestCase test = test_cases[i];  // Create a modifiable copy

        printf("\n--- Test %u: %s ---\n", i + 1, test.description);

        // Initialize input parameters
        test.input_string_len = strlen(test.input_string);
        test.key_len = AES256_FIXED_KEY_SIZE;

        printf("Input string (%u bytes): %s\n", test.input_string_len,
               test.input_string);
        printf("Key: ");
        print_hex(test.key, test.key_len);
        printf("PKCS7 Padding: %s\n", test.usePKCS7 ? "Enabled" : "Disabled");

        // Clear buffers
        memset(test.AES256_encrypt_buffer, 0, AES256_MAX_BUFFER_SIZE);
        memset(test.AES256_decrypt_buffer, 0, AES256_MAX_BUFFER_SIZE);

        // Initialize AES context (ECB mode doesn't use IV)
        AES256_init_ctx(&test.AES256_ctx, test.key, NULL);

        // Encrypt
        test.AES256_code = AES256_ECB_encrypt(
            &test.AES256_ctx, test.input_string, test.AES256_encrypt_buffer,
            test.input_string_len, &test.output_len, test.usePKCS7);

        if (test.AES256_code != AES_CODE_OK) {
            printf("Encryption failed with code: %d\n", test.AES256_code);
            all_tests_passed = false;
            continue;
        }

        printf("Encrypted (%u bytes): ", test.output_len);
        print_hex(test.AES256_encrypt_buffer, test.output_len);

        // Decrypt
        size_t decrypted_len;
        test.AES256_code =
            AES256_ECB_decrypt(&test.AES256_ctx, test.AES256_encrypt_buffer,
                               test.AES256_decrypt_buffer, test.output_len,
                               &decrypted_len, test.usePKCS7);

        if (test.AES256_code != AES_CODE_OK) {
            printf("Decryption failed with code: %d\n", test.AES256_code);
            all_tests_passed = false;
            continue;
        }

        printf("Decrypted (%u bytes): ", decrypted_len);
        print_hex(test.AES256_decrypt_buffer, decrypted_len);
        printf("Decrypted text: %s\n", test.AES256_decrypt_buffer);

        // Verify result
        bool test_passed = false;

        if (test.usePKCS7) {
            test_passed = (decrypted_len == test.input_string_len) &&
                          (memcmp(test.input_string, test.AES256_decrypt_buffer,
                                  test.input_string_len) == 0);
        } else {
            test_passed = (decrypted_len >= test.input_string_len) &&
                          (memcmp(test.input_string, test.AES256_decrypt_buffer,
                                  test.input_string_len) == 0);
        }

        printf("Test %u result: %s\n", i + 1,
               test_passed ? "PASSED" : "FAILED");

        if (!test_passed) {
            all_tests_passed = false;
        }
    }

    printf("\n=== Test Summary ===\n");
    printf("Total tests: %u\n", TOTAL_TESTS);
    printf("Final result: %s\n",
           all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
