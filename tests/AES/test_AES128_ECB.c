
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "AES128.h"
#include "PKCS7.h"

static const char *TEST_NAME = "AES-128 ECB tester";

// Test cases structure for better readability and maintenance
typedef struct
{
    AES128_ctx_t AES128_ctx;
    AES_errcode_t AES128_code;
    const char *input_string;
    const uint8_t key[AES128_FIXED_KEY_SIZE];  // Changed to uint8_t
    const char *description;
    bool usePKCS7;
    size_t input_string_len;
    size_t key_len;
    size_t output_len;
    uint8_t AES128_encrypt_buffer[AES128_MAX_BUFFER_SIZE];
    uint8_t AES128_decrypt_buffer[AES128_MAX_BUFFER_SIZE];
} TestCase;

static const TestCase test_cases[] = {
    {
        .AES128_ctx = {0},
        .AES128_code = AES_CODE_OK,
        .input_string = "This is a test string for AES-128 ECB mode!",
        .key = {'A','E','S','1','2','8','_','S','E','C','R','E','T','K','E','Y'},
        .description = "Standard test with text that requires padding",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES128_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES128_encrypt_buffer = {0},
        .AES128_decrypt_buffer = {0}
    },
    {
        .AES128_ctx = {0},
        .AES128_code = AES_CODE_OK,
        .input_string = "ExactBlock16BytesX",
        .key = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'},
        .description = "Exact block size test (16 bytes)",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES128_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES128_encrypt_buffer = {0},
        .AES128_decrypt_buffer = {0}
    },
    {
        .AES128_ctx = {0},
        .AES128_code = AES_CODE_OK,
        .input_string = "Short text",
        .key = {'S','E','C','R','E','T','K','E','Y','1','2','3','4','5','6','7'},
        .description = "Short text test",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES128_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES128_encrypt_buffer = {0},
        .AES128_decrypt_buffer = {0}
    },
    // ... Add other test cases with the same pattern
};

#define TOTAL_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

// Helper function to print hex representation of data
void print_hex(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void)
{
    printf("%s\n\n", TEST_NAME);
    bool all_tests_passed = true;

    // Run tests
    for (size_t i = 0; i < TOTAL_TESTS; i++)
    {
        assert(i < TOTAL_TESTS);

        TestCase test = test_cases[i]; // Create a modifiable copy

        printf("\n--- Test %u: %s ---\n", i + 1, test.description);

        // Initialize input parameters
        test.input_string_len = strlen(test.input_string);
        test.key_len = AES128_FIXED_KEY_SIZE;

        printf("Input string (%u bytes): %s\n", test.input_string_len, test.input_string);
        printf("Key: ");
        print_hex(test.key, test.key_len);
        printf("PKCS7 Padding: %s\n", test.usePKCS7 ? "Enabled" : "Disabled");

        // Clear buffers
        memset(test.AES128_encrypt_buffer, 0, AES128_MAX_BUFFER_SIZE);
        memset(test.AES128_decrypt_buffer, 0, AES128_MAX_BUFFER_SIZE);

        // Initialize AES context (ECB mode doesn't use IV)
        AES128_init_ctx(&test.AES128_ctx, test.key, NULL);  // Changed: removed cast, using uint8_t key directly

        // Encrypt
        test.AES128_code = AES128_ECB_encrypt(
            &test.AES128_ctx,
            (const uint8_t *)test.input_string,
            test.AES128_encrypt_buffer,
            test.input_string_len,
            &test.output_len,
            test.usePKCS7);

        if (test.AES128_code != AES_CODE_OK)
        {
            printf("Encryption failed with code: %d\n", test.AES128_code);
            all_tests_passed = false;
            continue;
        }

        printf("Encrypted (%u bytes): ", test.output_len);
        print_hex(test.AES128_encrypt_buffer, test.output_len);

        // Decrypt
        size_t decrypted_len;
        test.AES128_code = AES128_ECB_decrypt(
            &test.AES128_ctx,
            test.AES128_encrypt_buffer,
            test.AES128_decrypt_buffer,
            test.output_len,
            &decrypted_len,
            test.usePKCS7);

        if (test.AES128_code != AES_CODE_OK)
        {
            printf("Decryption failed with code: %d\n", test.AES128_code);
            all_tests_passed = false;
            continue;
        }

        printf("Decrypted (%u bytes): ", decrypted_len);
        print_hex(test.AES128_decrypt_buffer, decrypted_len);
        printf("Decrypted text: %s\n", test.AES128_decrypt_buffer);

        // Verify result
        bool test_passed = false;
        
        if (test.usePKCS7) {
            test_passed = (decrypted_len == test.input_string_len) &&
                         (memcmp(test.input_string, test.AES128_decrypt_buffer, test.input_string_len) == 0);
        } else {
            test_passed = (decrypted_len >= test.input_string_len) &&
                         (memcmp(test.input_string, test.AES128_decrypt_buffer, test.input_string_len) == 0);
        }

        printf("Test %u result: %s\n", i + 1, test_passed ? "PASSED" : "FAILED");

        if (!test_passed)
        {
            all_tests_passed = false;
        }
    }

    printf("\n=== Test Summary ===\n");
    printf("Total tests: %u\n", TOTAL_TESTS);
    printf("Final result: %s\n", all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
