#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/PKCS7/PKCS7.h"

#define BLOCK_SIZE PKCS7_BLOCK_SIZE_256_BIT

const char *TEST_STRING = "12345";
const char *TEST_NAME = "PKCS7 test with input size: 5 - block size: 256 bits";
char input_string[PKCS7_MAX_BLOCK_SIZE_BYTES];

size_t input_string_len, comparison;
size_t output_len;

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    input_string_len = strlen(TEST_STRING);
    memset(input_string, 0, PKCS7_MAX_BLOCK_SIZE_BYTES);
    memcpy(input_string,TEST_STRING,input_string_len); 
    
    printf("Input: <<%s>> Length: %u\n",input_string,input_string_len);
    printf("Block size: %u bytes\n",BLOCK_SIZE);
    PKCS7_padding_t padding_struct;
    PKCS7_unpadding_t unpadding_struct;
    PKCS7_error_t code = PKCS7_add_padding(&padding_struct, input_string, input_string_len, PKCS7_BLOCK_SIZE_256_BIT);

    uint8_t *p = padding_struct.data_with_padding;

    printf(" PKCS7 scheme:\n");
    for(uint8_t i=0; i!= PKCS7_BLOCK_SIZE_256_BIT; i++ ){
        printf("%02X ",p[i]);
    }
    printf("\n");

    code = PKCS7_remove_padding(&unpadding_struct, padding_struct.data_with_padding, padding_struct.data_len_with_padding);

    p = unpadding_struct.data_without_padding;
    printf(" PKCS7 scheme w/o padding:\n");
    for(uint8_t i=0; i!= BLOCK_SIZE; i++ ){
        printf("%02X ",p[i]);
    }
    printf("\n");

    comparison = memcmp(unpadding_struct.data_without_padding, input_string, input_string_len);

    printf("%s %s",TEST_NAME, comparison == 0? "OK":"not OK");
    return comparison == 0? EXIT_SUCCESS:EXIT_FAILURE;

}