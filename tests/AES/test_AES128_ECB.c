#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/AES/AES128.h"
#include "../../src/AES/AES_common.h"
#include "../../src/PKCS7/PKCS7.h"


const char *TEST_STRING = "1234567890123456";
const char TEST_KEY[AES128_FIXED_KEY_SIZE] = {
"1234567890123456"
};

const char *TEST_NAME = "AES-128 ECB";


size_t test_string_len, key_len, comparison;
size_t output_len;

AES128_ctx_t AES128_ctx;

AES_errcode_t AES128_code;

uint8_t input_string[AES128_MAX_BUFFER_SIZE];
uint8_t AES128_encrypt_buffer[AES128_MAX_BUFFER_SIZE];
uint8_t AES128_decrypt_buffer[AES128_MAX_BUFFER_SIZE];

int main(int argc, char *argv[]){

    printf("%s testing\n",TEST_NAME);
    test_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", test_string_len);

    memset(input_string, 0, AES128_MAX_BUFFER_SIZE);
    memset(AES128_encrypt_buffer, 0, AES128_MAX_BUFFER_SIZE);
    memset(AES128_decrypt_buffer, 0, AES128_MAX_BUFFER_SIZE);

    memcpy(input_string,TEST_STRING,test_string_len);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i != test_string_len; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");
    
    AES128_init_ctx(&AES128_ctx, TEST_KEY, NULL);
    AES128_code = AES128_ECB_encrypt(&AES128_ctx, input_string, AES128_encrypt_buffer, test_string_len, &output_len);

    printf("AES output ( %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES128_encrypt_buffer[i]);
    }
    printf("]\n");

    AES128_code = AES128_ECB_decrypt(&AES128_ctx, AES128_encrypt_buffer, AES128_decrypt_buffer, test_string_len, &output_len);
    printf("Back to ASCII (%u bytes):\n<<", output_len, AES128_decrypt_buffer);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES128_decrypt_buffer[i]);
    }
    printf(">>\n");

    comparison = memcmp(TEST_STRING, AES128_decrypt_buffer, test_string_len);
    if(comparison == 0){
        printf("%s OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }
    else{
        printf("%s not OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }

}