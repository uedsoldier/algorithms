#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/BASE64/base64.h"
#include "../minunit.h"

#define ASCII_STRING_MAX_SIZE 64
#define BUFFER_SIZE (ASCII_STRING_MAX_SIZE * 2)

char input_string[ASCII_STRING_MAX_SIZE];
char base64_buffer[BUFFER_SIZE];
char binary_buffer[BUFFER_SIZE];

int tests_run = 0;
int total_tests = 2;

const char *ASCII_STRING1 = "Base64 test string";
const char *BASE64_STRING1 = "QmFzZTY0IHRlc3Qgc3RyaW5n";

static char *test_bin_to_base64()
{
    memset(input_string, 0, sizeof(input_string));
    sprintf(input_string, "%s", ASCII_STRING1);
    uint16_t input_str_len = strlen(ASCII_STRING1);
    uint16_t output_str_len = strlen(BASE64_STRING1);
    printf("\r\nASCII input: <<%s>>\r\n", ASCII_STRING1);
    printf("Input string length: %u\r\n", input_str_len);
    printf("Expected base64 output: <<%s>>\r\n", BASE64_STRING1);
    printf("Expected base64 string length: %u\r\n", output_str_len);
    bin_to_base64(base64_buffer, input_string, input_str_len);
    printf("Output base64 string: <<%s>>\r\n", base64_buffer);
    int result = strcmp(base64_buffer, BASE64_STRING1);
    mu_assert("Error: incorrect bin to base64 conversion", result == 0);
}

static char *test_base64_to_bin()
{
    memset(input_string, 0, sizeof(input_string));
    sprintf(input_string, "%s", BASE64_STRING1);
    uint16_t input_str_len = strlen(BASE64_STRING1);
    uint16_t output_str_len = strlen(ASCII_STRING1);
    printf("\r\nBase64 input: <<%s>>\r\n", BASE64_STRING1);
    printf("Input string length: %u\r\n", input_str_len);
    printf("Expected ASCII output: <<%s>>\r\n", ASCII_STRING1);
    printf("Expected ASCII string length: %u\r\n", output_str_len);
    base64_to_bin(binary_buffer, input_string, input_str_len);
    printf("Output ASCII string: <<%s>>\r\n", binary_buffer);
    int result = strcmp(binary_buffer, ASCII_STRING1);
    mu_assert("Error: incorrect base64 to bin conversion", result == 0);
}

static char *all_tests()
{
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    mu_run_test(test_bin_to_base64);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    mu_run_test(test_base64_to_bin);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    return 0;
}

int main(int argc, char *argv[])
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %u/%u\n", tests_run, total_tests);

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}