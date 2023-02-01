#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CHECKSUM8/checksum8.h"
#include "../minunit.h"

const char *test_string = "test checksum8";
uint8_t expected_checksum8_xor = 0x03;
uint8_t expected_checksum8_modulo256 = 0x6B;
uint8_t expected_checksum8_2complement = 0x95;

int tests_run = 0;
int total_tests = 3;

#define INPUT_STRING_SIZE 32

char input_string[INPUT_STRING_SIZE];

static uint8_t perform_checksum(CHECKSUM8_t checksum8_type, void *input_data)
{
    memset(input_string, 0, sizeof(input_string));
    sprintf(input_string, "%s", input_data);
    printf("Input string: <<%s>>\r\n", input_string);
    uint16_t input_str_len = strlen(input_string);
    printf("Input string size: %u\r\n", input_str_len);
    uint8_t checksum = checksum8(input_string, sizeof(input_string), checksum8_type);
    printf("Checksum %s: 0x%02X\r\n", checksum8_implementations[checksum8_type], checksum);
    return checksum;
}

static char *test_checksum8_xor()
{
    mu_assert("Incorrect checksum8 result\r\n", perform_checksum(CHECKSUM8_XOR, test_string) == expected_crc8_xor);
    return 0;
}

static char *test_checksum8_2complement()
{
    mu_assert("Incorrect checksum8 result\r\n", perform_checksum(CHECKSUM8_2complement, test_string) == expected_crc8_2complement);
    return 0;
}

static char *test_checksum8_modulo256()
{
    mu_assert("Incorrect checksum8 result\r\n", perform_checksum(CHECKSUM8_modulo256, test_string) == expected_crc8_modulo256);
    return 0;
}

static char *all_tests()
{
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("Expected checksum value: 0x%02X\r\n", expected_crc8_xor);
    mu_run_test(test_checksum8_xor);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("Expected checksum value: 0x%02X\r\n", expected_crc8_2complement);
    mu_run_test(test_checksum8_2complement);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("Expected checksum value: 0x%02X\r\n", expected_crc8_modulo256);
    mu_run_test(test_checksum8_modulo256);
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