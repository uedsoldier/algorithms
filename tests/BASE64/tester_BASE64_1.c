#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/BASE64/base64.h"

#define ASCII_STRING_MAX_SIZE 64
#define BASE64_BUFFER_SIZE (ASCII_STRING_MAX_SIZE * 2)

char input_string[ASCII_STRING_MAX_SIZE];
char base64_buffer[BASE64_BUFFER_SIZE];
char binary_buffer[BASE64_BUFFER_SIZE];

const char *TEST_NAME = "BASE64 - test 1";
const char *ASCII_STRING = "Base64 test string";

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    uint16_t input_str_len = strlen(ASCII_STRING);
    printf("\r\nASCII input: <<%s>>\r\n", ASCII_STRING);
    printf("Input string length: %u\r\n", input_str_len);
    memset(input_string, 0, sizeof(input_string));
    memset(base64_buffer,0,BASE64_BUFFER_SIZE);
    memcpy(input_string,ASCII_STRING,strlen(ASCII_STRING));


    bin_to_base64(base64_buffer, input_string, input_str_len);
    printf("Output base64 string: <<%s>>\r\n", base64_buffer);

    base64_to_bin(binary_buffer, base64_buffer, strlen(base64_buffer));
    printf("Back to ASCII string: <<%s>>\r\n", binary_buffer);

    int result = strcmp(binary_buffer, ASCII_STRING);

    printf("%s %s\n", TEST_NAME, (result==0)? "OK":"not OK");
    return (result==0)? EXIT_SUCCESS:EXIT_FAILURE;
    
}