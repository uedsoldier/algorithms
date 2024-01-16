#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/PKCS7/PKCS7.h"
#include "../minunit.h"

char test_string[128];

size_t test_string_len, test_string_len_normalized, key_len, comparison;
size_t output_len;

#define BLOCK_SIZE 16

int main(int argc, char *argv[])
{
    printf("PKCS7 testing\n");
    if(argc != 2) {
        printf("Must have one argument\n");
        return EXIT_FAILURE;
    }
    
    memcpy(test_string,argv[1],strlen(argv[1])); 
    test_string_len = strlen(test_string);
    printf("Input: <<%s>> Length: %u\n",test_string,test_string_len);
    printf("Block size: %u bytes\n",BLOCK_SIZE);
    PKCS7_padding_t padding_struct;
    PKCS7_unpadding_t unpadding_struct;
    PKCS7_error_t code = PKCS7_add_padding(&padding_struct, test_string, test_string_len, PKCS7_BLOCK_SIZE_128_BIT);

    uint8_t *p = padding_struct.data_with_padding;

    printf(" PKCS7 scheme:\n");
    for(uint8_t i=0; i!= PKCS7_BLOCK_SIZE_128_BIT; i++ ){
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
    return EXIT_SUCCESS;
}