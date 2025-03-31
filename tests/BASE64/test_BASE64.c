#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "test_utils.h"

#define TEST_BUFFER_SIZE 2048
#define TEST_NAME "BASE64 tester"

/**
 * @brief Test case structure for Base64 encoding/decoding tests
 */
typedef struct {
    const char *description;    // Test case description
    const uint8_t *input;       // Input data to encode
    size_t input_len;           // Length of input data
    const uint8_t *expect_enc;  // Expected Base64 encoded output
    size_t expect_enc_len;      // Expected encoded length
    const uint8_t *expect_dec;  // Expected decoded output
    size_t expect_dec_len;      // Expected decoded length
} TestCase;

/**
 * @brief Test cases for Base64 encoding/decoding
 */
static const TestCase test_cases[] = {
    // Basic cases
    {.description = "Empty string",
     .input = (const uint8_t *)"",
     .input_len = 0,
     .expect_enc = (const uint8_t *)"",
     .expect_enc_len = 0,
     .expect_dec = (const uint8_t *)"",
     .expect_dec_len = 0},
    {.description = "Single character",
     .input = (const uint8_t *)"A",
     .input_len = 1,
     .expect_enc = (const uint8_t *)"QQ==",
     .expect_enc_len = 4,
     .expect_dec = (const uint8_t *)"A",
     .expect_dec_len = 1},
    {.description = "Two characters",
     .input = (const uint8_t *)"AB",
     .input_len = 2,
     .expect_enc = (const uint8_t *)"QUI=",
     .expect_enc_len = 4,
     .expect_dec = (const uint8_t *)"AB",
     .expect_dec_len = 2},
    {.description = "Three characters",
     .input = (const uint8_t *)"ABC",
     .input_len = 3,
     .expect_enc = (const uint8_t *)"QUJD",
     .expect_enc_len = 4,
     .expect_dec = (const uint8_t *)"ABC",
     .expect_dec_len = 3},
    {.description = "Four characters",
     .input = (const uint8_t *)"ABCD",
     .input_len = 4,
     .expect_enc = (const uint8_t *)"QUJDRA==",
     .expect_enc_len = 8,
     .expect_dec = (const uint8_t *)"ABCD",
     .expect_dec_len = 4},
    // Special character cases
    {
        .description = "Single percent character",
        .input = (const uint8_t *)"%",  // Input is just the '%' character
        .input_len = 1,                 // Length of the input string is 1 byte
        .expect_enc = (const uint8_t *)"JQ==",  // Base64 encoding of '%'
        .expect_enc_len = 4,  // The length of the Base64 encoded string is 4
        .expect_dec =
            (const uint8_t *)"%",  // Decoding it back should give us '%'
        .expect_dec_len = 1        // The length of the decoded string is 1 byte
    },
    {.description = "Special character mix",
     .input = (const uint8_t *)"!@#$%^&*()_+-=",
     .input_len = 14,
     .expect_enc = (const uint8_t *)"IUAjJCVeJiooKV8rLT0=",
     .expect_enc_len = 20,
     .expect_dec = (const uint8_t *)"!@#$%^&*()_+-=",
     .expect_dec_len = 14},
    {.description = "URL special characters",
     .input = (const uint8_t *)"https://example.com",
     .input_len = 18,
     .expect_enc = (const uint8_t *)"aHR0cHM6Ly9leGFtcGxlLmNvbQ==",
     .expect_enc_len = 24,
     .expect_dec = (const uint8_t *)"https://example.com",
     .expect_dec_len = 18},
    // Whitespace and control characters
    {.description = "String with spaces",
     .input = (const uint8_t *)"Hello World 123",
     .input_len = 14,
     .expect_enc = (const uint8_t *)"SGVsbG8gV29ybGQgMTI=",
     .expect_enc_len = 20,
     .expect_dec = (const uint8_t *)"Hello World 123",
     .expect_dec_len = 14},
    {.description = "String with tabs and newlines",
     .input = (const uint8_t *)"Line1\tTab\nLine2",
     .input_len = 15,
     .expect_enc = (const uint8_t *)"TGluZTEJVGFiCkxpbmUy",
     .expect_enc_len = 20,
     .expect_dec = (const uint8_t *)"Line1\tTab\nLine2",
     .expect_dec_len = 15},
    // Binary data
    {.description = "Binary zeros",
     .input = (const uint8_t *)"\x00\x00\x00\x00",
     .input_len = 4,
     .expect_enc = (const uint8_t *)"AAAAAA==",
     .expect_enc_len = 8,
     .expect_dec = (const uint8_t *)"\x00\x00\x00\x00",
     .expect_dec_len = 4},
    {.description = "Binary data mix",
     .input = (const uint8_t *)"\xFF\x00\xFF\x00",
     .input_len = 4,
     .expect_enc = (const uint8_t *)"/wD/AA==",
     .expect_enc_len = 8,
     .expect_dec = (const uint8_t *)"\xFF\x00\xFF\x00",
     .expect_dec_len = 4},
    // Edge cases
    {.description = "All ASCII printable characters",
     .input = (const uint8_t *)" !\"#$%&'()*+,-./"
                               "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]"
                               "^_`abcdefghijklmnopqrstuvwxyz{|}~",
     .input_len = 95,
     .expect_enc =
         (const uint8_t *)"ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9AQUJDREVG"
                          "R0hJSktMTU5PUFFSU1RVVldYWVpbXF1eX2BhYmNkZWZnaGlqa2xt"
                          "bm9wcXJzdHV2d3h5ent8fX4=",
     .expect_enc_len = 128,
     .expect_dec =
         (const uint8_t *)" !\"#$%&'()*+,-./"
                          "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                          "abcdefghijklmnopqrstuvwxyz{|}~",
     .expect_dec_len = 95},
    {.description = "Repeated characters",
     .input = (const uint8_t *)"AAAAAAAAAA",
     .input_len = 10,
     .expect_enc = (const uint8_t *)"QUFBQUFBQUFBQQ==",
     .expect_enc_len = 16,
     .expect_dec = (const uint8_t *)"AAAAAAAAAA",
     .expect_dec_len = 10},
    // Long string test
    {.description = "Long string with mixed content",
     .input = (const uint8_t *)"The quick brown fox jumps over the lazy dog "
                               "1234567890 !@#$%^&*()",
     .input_len = 63,
     .expect_enc =
         (const uint8_t *)"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5"
                          "IGRvZyAxMjM0NTY3ODkwICFAIyQlXiYqKCk=",
     .expect_enc_len = 84,
     .expect_dec = (const uint8_t *)"The quick brown fox jumps over the lazy "
                                    "dog 1234567890 !@#$%^&*()",
     .expect_dec_len = 63}};

#define TOTAL_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

/**
 * @brief Run a single Base64 test case
 */
static bool run_single_test(base64_converter_t *converter, const TestCase *test,
                            size_t test_number) {
    printf("\n--- Test %u: %s ---\n", test_number + 1, test->description);

    // Print input data
    printf("Input string:  '%.*s'\n", (int)test->input_len, test->input);
    printf("Input length:  %u bytes\n", test->input_len);
    printf("Input hex:     ");
    print_hex(test->input, test->input_len);

    // Encode
    const uint8_t *encoded_data;
    size_t encoded_size;
    base64_status_t status = base64_encode(
        converter, test->input, test->input_len, &encoded_data, &encoded_size);

    if (status != BASE64_SUCCESS) {
        printf("Encoding failed with status: %s\n",
               base64_get_error_string(status));
        return false;
    }

    printf("Encoded Base64: '%.*s'\n", (int)encoded_size, encoded_data);
    printf("Encoded length: %u bytes\n", encoded_size);
    printf("Encoded hex:    ");
    print_hex(encoded_data, encoded_size);

    // Decode
    uint8_t decoded_buffer[TEST_BUFFER_SIZE];
    size_t decoded_size;
    status = base64_decode(converter, encoded_data, encoded_size,
                           decoded_buffer, &decoded_size);

    if (status != BASE64_SUCCESS) {
        printf("Decoding failed with status: %s\n",
               base64_get_error_string(status));
        return false;
    }

    printf("Decoded string: '%.*s'\n", (int)decoded_size, decoded_buffer);
    printf("Decoded length: %u bytes\n", decoded_size);
    printf("Decoded hex:    ");
    print_hex(decoded_buffer, decoded_size);

    // Verify results
    bool encoding_correct = bytes_equal(encoded_data, encoded_size,
                                        test->expect_enc, test->expect_enc_len);
    bool decoding_correct = bytes_equal(decoded_buffer, decoded_size,
                                        test->expect_dec, test->expect_dec_len);
    bool test_passed = encoding_correct && decoding_correct;

    if (!test_passed) {
        printf("\nMismatch detected:\n");
        if (!encoding_correct) {
            printf("Expected Base64: '%.*s'\n", (int)test->expect_enc_len,
                   test->expect_enc);
            printf("Actual Base64:   '%.*s'\n", (int)encoded_size,
                   encoded_data);
        }
        if (!decoding_correct) {
            printf("Expected string: '%.*s'\n", (int)test->expect_dec_len,
                   test->expect_dec);
            printf("Actual string:   '%.*s'\n", (int)decoded_size,
                   decoded_buffer);
        }
    }

    printf("Test %u result: %s\n", test_number + 1,
           test_passed ? "PASSED" : "FAILED");
    return test_passed;
}

/**
 * @brief Main entry point for Base64 tests
 */
int main(void) {
    printf("%s\n\n", TEST_NAME);
    bool all_tests_passed = true;

    // Print buffer size requirements
    printf("Required buffer size: %d\n", BASE64_MAX_ENCODED_BUFFER);
    printf("Allocated buffer size: %d\n", TEST_BUFFER_SIZE);

    // Initialize converter
    uint8_t buffer[TEST_BUFFER_SIZE];
    base64_converter_t converter;
    base64_status_t status = base64_init(&converter, buffer, sizeof(buffer));

    if (status != BASE64_SUCCESS) {
        printf("Failed to initialize converter: %s\n",
               base64_get_error_string(status));
        printf("Buffer size provided: %u\n", sizeof(buffer));
        printf("Required buffer size: %d\n", BASE64_MAX_ENCODED_BUFFER);
        return EXIT_FAILURE;
    }

    printf("Converter initialized successfully\n");

    // Run all tests
    for (size_t i = 0; i < TOTAL_TESTS; i++) {
        if (!run_single_test(&converter, &test_cases[i], i)) {
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
