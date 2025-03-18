#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "AES192.h" // Changed to AES192 header
#include "PKCS7.h"

static const char *TEST_NAME = "AES-192 ECB tester"; // Changed to AES-192

// Test cases structure for better readability and maintenance
typedef struct
{
    AES192_ctx_t AES192_ctx; // Changed to AES192 context
    AES_errcode_t AES192_code;
    const char *input_string;
    const char key[AES192_FIXED_KEY_SIZE]; // Changed to AES192 key size (24 bytes)
    const char *description;
    bool usePKCS7;
    size_t input_string_len;
    size_t key_len;
    size_t output_len;
    uint8_t AES192_encrypt_buffer[AES192_MAX_BUFFER_SIZE]; // Changed to AES192 buffer size
    uint8_t AES192_decrypt_buffer[AES192_MAX_BUFFER_SIZE];
} TestCase;

static const TestCase test_cases[] = {
    {.input_string = "This is a test string for AES-192 ECB mode!",
     .key = "AES192_SECRET_KEY_24BYTE", // 24-byte key
     .description = "Standard test with text that requires padding",
     .usePKCS7 = true},
    {.input_string = "ExactBlock16BytesX", // Exactly 16 bytes (one block)
     .key = "0123456789abcdef01234567",    // 24-byte key
     .description = "Exact block size test (16 bytes)",
     .usePKCS7 = true},
    {.input_string = "Short text",     // Short text requiring padding
     .key = "SECRETKEY123456724BYTES", // 24-byte key
     .description = "Short text test",
     .usePKCS7 = true},
    {.input_string = "This is a multi-block input that will require more than one block of AES encryption to fully process.",
     .key = "TestKey12345678901234567", // 24-byte key
     .description = "Multi-block test with padding",
     .usePKCS7 = true},
    {.input_string = "ExactBlock16BytesXExactBlock16BytesY", // Exactly 32 bytes (two blocks)
     .key = "SecurityKey123456789012",                       // 24-byte key
     .description = "Multiple exact blocks test (32 bytes)",
     .usePKCS7 = true},
    {.input_string = "Special chars: !@#$%^&*()_+-=[]{}|;':\",./<>?",
     .key = "000000000000000000000000", // 24-byte key of zeros
     .description = "Special characters test",
     .usePKCS7 = true},
    {.input_string = "This is a test string for AES-192 ECB mode without padding!",
     .key = "AES192_SECRET_KEY_24BYTE", // 24-byte key
     .description = "Test with padding disabled",
     .usePKCS7 = false},
    {.input_string = "ExactBlock16BytesXExactBlock16BytesY", // Exactly 32 bytes (two blocks)
     .key = "SECRETKEY123456724BYTES",                       // 24-byte key
     .description = "Exact multiple blocks with padding disabled",
     .usePKCS7 = false}};

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
        test.key_len = AES192_FIXED_KEY_SIZE; // Changed to AES192 key size

        printf("Input string (%u bytes): %s\n", test.input_string_len, test.input_string);
        printf("Key: ");
        print_hex((const uint8_t *)test.key, test.key_len);
        printf("PKCS7 Padding: %s\n", test.usePKCS7 ? "Enabled" : "Disabled");

        // Clear buffers
        memset(test.AES192_encrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);
        memset(test.AES192_decrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);

        // Initialize AES context (ECB mode doesn't use IV)
        AES192_init_ctx(&test.AES192_ctx, (const uint8_t *)test.key, NULL);

        // Encrypt
        test.AES192_code = AES192_ECB_encrypt( // Changed to AES192 function
            &test.AES192_ctx,
            (const uint8_t *)test.input_string,
            test.AES192_encrypt_buffer,
            test.input_string_len,
            &test.output_len,
            test.usePKCS7); // Pass the PKCS7 flag

        if (test.AES192_code != AES_CODE_OK)
        {
            printf("Encryption failed with code: %d\n", test.AES192_code);
            all_tests_passed = false;
            continue;
        }

        printf("Encrypted (%u bytes): ", test.output_len);
        print_hex(test.AES192_encrypt_buffer, test.output_len);

        // Decrypt
        size_t decrypted_len;
        test.AES192_code = AES192_ECB_decrypt( // Changed to AES192 function
            &test.AES192_ctx,
            test.AES192_encrypt_buffer,
            test.AES192_decrypt_buffer,
            test.output_len,
            &decrypted_len,
            test.usePKCS7); // Pass the PKCS7 flag

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

        if (test.usePKCS7)
        {
            // With padding, we expect the decrypted length to match the original input length
            test_passed = (decrypted_len == test.input_string_len) &&
                          (memcmp(test.input_string, test.AES192_decrypt_buffer, test.input_string_len) == 0);
        }
        else
        {
            // Without padding, we need to check if the decrypted data starts with our input
            // (there might be zero padding at the end)
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