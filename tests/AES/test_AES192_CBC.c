#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "AES192.h"
#include "PKCS7.h"

static const char *TEST_NAME = "AES-192 CBC tester";

typedef struct
{
    AES192_ctx_t AES192_ctx;
    AES_errcode_t AES192_code;
    const uint8_t init_vector[AES_BLOCK_LEN];
    const char *input_string;
    const uint8_t key[AES192_FIXED_KEY_SIZE];
    const char *description;
    bool usePKCS7;
    size_t input_string_len;
    size_t key_len;
    size_t output_len;
    uint8_t AES192_encrypt_buffer[AES192_MAX_BUFFER_SIZE];
    uint8_t AES192_decrypt_buffer[AES192_MAX_BUFFER_SIZE];
} TestCase;

static const TestCase test_cases[] = {
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'},
        .input_string = "This is a test string for AES-192 CBC mode!",
        .key = {'A','E','S','1','9','2','_','S','E','C','R','E','T','_','K','E','Y','_','2','4','B','Y','T','E'},
        .description = "Standard test with text that requires padding",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'},
        .input_string = "ExactBlock16BytesX",
        .key = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','0','1','2','3','4','5','6','7'},
        .description = "Exact block size test (16 bytes)",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .input_string = "Short text",
        .key = {'S','E','C','R','E','T','K','E','Y','1','2','3','4','5','6','7','2','4','B','Y','T','E','S','!'},
        .description = "Short text with zero IV",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .input_string = "This is a multi-block input that will require more than one block of AES encryption to fully process.",
        .key = {'T','e','s','t','K','e','y','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7'},
        .description = "Multi-block test with padding",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {'1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f'},
        .input_string = "ExactBlock16BytesXExactBlock16BytesY",
        .key = {'S','e','c','u','r','i','t','y','K','e','y','1','2','3','4','5','6','7','8','9','0','1','2','3'},
        .description = "Multiple exact blocks test (32 bytes)",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p'},
        .input_string = "Special chars: !@#$%^&*()_+-=[]{}|;':\",./<>?",
        .key = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .description = "Special characters test",
        .usePKCS7 = true,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'},
        .input_string = "This is a test string for AES-192 CBC mode without padding!",
        .key = {'A','E','S','1','9','2','_','S','E','C','R','E','T','_','K','E','Y','_','2','4','B','Y','T','E'},
        .description = "Test with padding disabled",
        .usePKCS7 = false,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    },
    {
        .AES192_ctx = {0},
        .AES192_code = AES_CODE_OK,
        .init_vector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .input_string = "ExactBlock16BytesXExactBlock16BytesY",
        .key = {'S','E','C','R','E','T','K','E','Y','1','2','3','4','5','6','7','2','4','B','Y','T','E','S','!'},
        .description = "Exact multiple blocks with padding disabled",
        .usePKCS7 = false,
        .input_string_len = 0,
        .key_len = AES192_FIXED_KEY_SIZE,
        .output_len = 0,
        .AES192_encrypt_buffer = {0},
        .AES192_decrypt_buffer = {0}
    }
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
        test.key_len = AES192_FIXED_KEY_SIZE;

        printf("Input string (%u bytes): %s\n", test.input_string_len, test.input_string);
        printf("Key: ");
        print_hex(test.key, test.key_len);
        printf("IV: ");
        print_hex(test.init_vector, AES_BLOCK_LEN);
        printf("PKCS7 Padding: %s\n", test.usePKCS7 ? "Enabled" : "Disabled");

        // Clear buffers
        memset(test.AES192_encrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);
        memset(test.AES192_decrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);

        // Initialize AES context
        AES192_init_ctx(&test.AES192_ctx, test.key, test.init_vector);

        // Encrypt
        test.AES192_code = AES192_CBC_encrypt(
            &test.AES192_ctx,
            (const uint8_t *)test.input_string,
            test.AES192_encrypt_buffer,
            test.input_string_len,
            &test.output_len,
            test.usePKCS7);

        if (test.AES192_code != AES_CODE_OK)
        {
            printf("Encryption failed with code: %d\n", test.AES192_code);
            all_tests_passed = false;
            continue;
        }

        printf("Encrypted (%u bytes): ", test.output_len);
        print_hex(test.AES192_encrypt_buffer, test.output_len);

        // Re-initialize context with same key and IV for decryption
        AES192_init_ctx(&test.AES192_ctx, test.key, test.init_vector);

        // Decrypt
        size_t decrypted_len;
        test.AES192_code = AES192_CBC_decrypt(
            &test.AES192_ctx,
            test.AES192_encrypt_buffer,
            test.AES192_decrypt_buffer,
            test.output_len,
            &decrypted_len,
            test.usePKCS7);

        if (test.AES192_code != AES_CODE_OK)
        {
            printf("Decryption failed with code: %d\n", test.AES192_code);
            all_tests_passed = false;
            continue;
        }

        printf("Decrypted (%u bytes): ", decrypted_len);
        print_hex(test.AES192_decrypt_buffer, decrypted_len);
        printf("Decrypted text: %s\n", test.AES192_decrypt_buffer);

        // Verify result
        bool test_passed = false;
        
        if (test.usePKCS7) {
            test_passed = (decrypted_len == test.input_string_len) &&
                         (memcmp(test.input_string, test.AES192_decrypt_buffer, test.input_string_len) == 0);
        } else {
            test_passed = (decrypted_len >= test.input_string_len) &&
                         (memcmp(test.input_string, test.AES192_decrypt_buffer, test.input_string_len) == 0);
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