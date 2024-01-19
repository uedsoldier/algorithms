#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/AES/AES256.h"
#include "../../src/AES/AES_common.h"
#include "../../src/PKCS7/PKCS7.h"


const char *TEST_STRING = "12345678901234567890123456789012";
const char TEST_KEY[AES256_FIXED_KEY_SIZE] = {
"12345678901234567890123456789012"
};

const char *TEST_NAME = "AES-256 ECB";


size_t test_string_len, test_string_len_normalized, key_len, comparison;
size_t output_len;

AES256_ctx_t AES256_ctx;

AES_errcode_t AES256_code;

#if defined(AES256_DYNAMIC_MEMORY) && (AES256_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *AES256_encrypt_buffer;
uint8_t *AES256_decrypt_buffer;
#else
uint8_t input_string[AES256_MAX_BUFFER_SIZE];
uint8_t AES256_encrypt_buffer[AES256_MAX_BUFFER_SIZE];
uint8_t AES256_decrypt_buffer[AES256_MAX_BUFFER_SIZE];
#endif

int main(int argc, char *argv[]){

    printf("%s testing\n",TEST_NAME);
    test_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", test_string_len);
    test_string_len_normalized = AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(test_string_len);
    if(test_string_len == test_string_len_normalized){
        test_string_len_normalized += AES_BLOCK_LEN;
    }
    printf("Test string length (normalized to nearest multiple of 16 and incremented if multiple of 16): %u\n", test_string_len_normalized);
    key_len = strlen(TEST_KEY);

    memset(input_string, 0, test_string_len_normalized);
    memset(AES256_encrypt_buffer, 0, test_string_len_normalized);
    memset(AES256_decrypt_buffer, 0, test_string_len_normalized);

    memcpy(input_string,TEST_STRING,test_string_len);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");
    
    AES256_init_ctx(&AES256_ctx, TEST_KEY, NULL);
    AES256_code = AES256_ECB_encrypt(&AES256_ctx, input_string, AES256_encrypt_buffer, test_string_len, &output_len);

    printf("AES output ( %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES256_encrypt_buffer[i]);
    }
    printf("]\n");

    AES256_code = AES256_ECB_decrypt(&AES256_ctx, AES256_encrypt_buffer, AES256_decrypt_buffer, test_string_len, &output_len);
    printf("Back to ASCII (%u bytes):\n<<", output_len, AES256_decrypt_buffer);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES256_decrypt_buffer[i]);
    }
    printf(">>\n");

    comparison = memcmp(TEST_STRING, AES256_decrypt_buffer, test_string_len);
    if(comparison == 0){
        printf("%s OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }
    else{
        printf("%s not OK\n",TEST_NAME);
        return EXIT_SUCCESS;
    }

}