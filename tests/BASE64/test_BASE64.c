#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "base64.h"

static const char *TEST_NAME = "BASE 64 tester";  // Fixed test name

// Test cases structure for better readability and maintenance
typedef struct
{
    const char *description;
    const char *input_string;
    const char *output_base64;
    const char *back_to_string;
    // More stuff
} TestCase;

static const TestCase test_cases[] = {
    {
        "Empty string",
        "",
        "",
        ""
    },
    {
        "Single character",
        "A",
        "QQ==",
        "A"
    },
    {
        "Two characters",
        "AB",
        "QUI=",
        "AB"
    },
    {
        "Three characters",
        "ABC",
        "QUJD",
        "ABC"
    },
    {
        "Four characters",
        "ABCD",
        "QUJDRA==",
        "ABCD"
    },
    {
        "Special characters",
        "!@#",
        "IUAj",
        "!@#"
    },
    {
        "Long string",
        "The quick brown fox jumps over the lazy dog",
        "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==",
        "The quick brown fox jumps over the lazy dog"
    }
};

#define TOTAL_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

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

        // Encode input string to base64
        uint8_t encoded[128] = {0};
        bin_to_base64(encoded, test.input_string, strlen(test.input_string));

        // Decode base64 back to string
        uint8_t decoded[128] = {0};
        base64_to_bin(decoded, encoded, strlen((char *)encoded));

        // Verify result
        bool test_passed = strcmp((char *)encoded, test.output_base64) == 0 &&
                           strcmp((char *)decoded, test.back_to_string) == 0;
        
        if (!test_passed) {
            printf("Expected base64: '%s'\n", test.output_base64);
            printf("Actual base64:   '%s'\n", (char *)encoded);
            printf("Expected string: '%s'\n", test.back_to_string);
            printf("Actual string:   '%s'\n", (char *)decoded);
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
