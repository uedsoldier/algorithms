#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "AES256.h"
#include "AES_common.h"
#include "PKCS7.h"

const char IV[AES_BLOCK_LEN] = "1234567890123456";    // 16-byte init vector  
const char *TEST_STRING = "1234567890123456";
const char TEST_KEY[AES256_FIXED_KEY_SIZE] = {
"12345678901234567890123456789012"
};

const char *TEST_NAME = "AES-256 CBC";


size_t input_string_len, key_len, comparison;
size_t output_len;

AES256_ctx_t AES256_ctx;

AES_errcode_t AES256_code;

uint8_t input_string[AES256_MAX_BUFFER_SIZE];
uint8_t AES256_encrypt_buffer[AES256_MAX_BUFFER_SIZE];
uint8_t AES256_decrypt_buffer[AES256_MAX_BUFFER_SIZE];

int main(int argc, char *argv[]){

    printf("%s testing\n",TEST_NAME);
    input_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", input_string_len);

    memset(input_string, 0, AES256_MAX_BUFFER_SIZE);
    memset(AES256_encrypt_buffer, 0, AES256_MAX_BUFFER_SIZE);
    memset(AES256_decrypt_buffer, 0, AES256_MAX_BUFFER_SIZE);

    memcpy(input_string,TEST_STRING,input_string_len);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i < input_string_len; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");
    
    AES256_init_ctx(&AES256_ctx, TEST_KEY, IV);
    AES256_code = AES256_CBC_encrypt(&AES256_ctx, input_string, AES256_encrypt_buffer, input_string_len, &output_len);

    printf("AES output ( %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES256_encrypt_buffer[i]);
    }
    printf("]\n");

    AES256_code = AES256_CBC_decrypt(&AES256_ctx, AES256_encrypt_buffer, AES256_decrypt_buffer, input_string_len, &output_len);
    printf("Back to ASCII (%u bytes):\n<<", output_len, AES256_decrypt_buffer);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES256_decrypt_buffer[i]);
    }
    printf(">>\n");
    
    comparison = memcmp(TEST_STRING, AES256_decrypt_buffer, input_string_len);
    if(comparison == 0){
        printf("%s OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }
    else{
        printf("%s not OK\n",TEST_NAME);
        return EXIT_FAILURE;
    }

}