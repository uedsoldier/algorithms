#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "AES192.h"
#include "AES_common.h"
#include "PKCS7.h"


const char *TEST_STRING = "1234567890123456";
const char TEST_KEY[AES192_FIXED_KEY_SIZE] = {
"123456789012345678901234"
};

const char *TEST_NAME = "AES-192 ECB";


size_t input_string_len, key_len, comparison;
size_t output_len;

AES192_ctx_t AES192_ctx;

AES_errcode_t AES192_code;

uint8_t input_string[AES192_MAX_BUFFER_SIZE];
uint8_t AES192_encrypt_buffer[AES192_MAX_BUFFER_SIZE];
uint8_t AES192_decrypt_buffer[AES192_MAX_BUFFER_SIZE];

int main(int argc, char *argv[]){

    printf("%s testing\n",TEST_NAME);
    input_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", input_string_len);

    memset(input_string, 0, AES192_MAX_BUFFER_SIZE);
    memset(AES192_encrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);
    memset(AES192_decrypt_buffer, 0, AES192_MAX_BUFFER_SIZE);

    memcpy(input_string,TEST_STRING,input_string_len);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i < input_string_len; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");
    
    AES192_init_ctx(&AES192_ctx, TEST_KEY, NULL);
    AES192_code = AES192_ECB_encrypt(&AES192_ctx, input_string, AES192_encrypt_buffer, input_string_len, &output_len);

    printf("AES output ( %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES192_encrypt_buffer[i]);
    }
    printf("]\n");

    AES192_code = AES192_ECB_decrypt(&AES192_ctx, AES192_encrypt_buffer, AES192_decrypt_buffer, input_string_len, &output_len);
    printf("Back to ASCII (%u bytes):\n<<", output_len, AES192_decrypt_buffer);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES192_decrypt_buffer[i]);
    }
    printf(">>\n");

    comparison = memcmp(TEST_STRING, AES192_decrypt_buffer, input_string_len);
    if(comparison == 0){
        printf("%s OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }
    else{
        printf("%s not OK\n",TEST_NAME);
        return EXIT_FAILURE;
    }

}