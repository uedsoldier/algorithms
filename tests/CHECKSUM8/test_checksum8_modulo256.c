#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CHECKSUM8/checksum8.h"

const char *TEST_STRING = "test checksum8";
const char *TEST_NAME = "checksum8-modulo256";
const CHECKSUM8_t checksum_type = CHECKSUM8_modulo256;

const uint8_t expected_checksum8_modulo256 = 0x6B;

#define MAX_INPUT_STRING_SIZE 32
char input_string[MAX_INPUT_STRING_SIZE];

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    uint16_t input_str_len = strlen(TEST_STRING);
    memset(input_string, 0, MAX_INPUT_STRING_SIZE);
    memcpy(input_string,TEST_STRING,input_str_len);
    printf("Input string: <<%s>> Size: %u bytes\n", input_string, input_str_len);
    
    uint8_t checksum = checksum8(input_string, sizeof(input_string), checksum_type);
    printf("Checksum %s: 0x%02X - Expected: 0x%02X\n", checksum8_implementations[checksum_type], checksum, expected_checksum8_modulo256);

    printf("%s %s",TEST_NAME, checksum == expected_checksum8_modulo256? "OK":"not OK");
    return checksum == expected_checksum8_modulo256? EXIT_SUCCESS:EXIT_FAILURE;
}