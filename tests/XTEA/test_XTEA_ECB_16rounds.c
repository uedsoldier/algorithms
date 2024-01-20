#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/XTEA/XTEA.h"
#include "../../src/PKCS7/PKCS7.h"

#define NUM_ROUNDS 16
const bool ecb = true;
const char *TEST_NAME = "XTEA ECB with 16 rounds";
const uint8_t IV[XTEA_BLOCK_SIZE] = "12345678";
const char *TEST_STRING = "Mi bb cachorrita";
const char TEST_KEY[XTEA_FIXED_KEY_SIZE] = "1234567890123456";

uint32_t test_string_len, test_string_len_normalized, key_len, comparison;
uint32_t output_len;

xtea_key_t xtea_test_key;
xtea_iv_t iv;
XTEA_t xtea_test;

XTEA_code_t xtea_code;

uint8_t xtea_rounds;

uint8_t input_string[XTEA_MAX_BUFFER_SIZE];
uint8_t XTEA_encrypt_buffer[XTEA_MAX_BUFFER_SIZE];
uint8_t XTEA_decrypt_buffer[XTEA_MAX_BUFFER_SIZE];

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    test_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", test_string_len);

    memset(input_string, 0, XTEA_MAX_BUFFER_SIZE);
    memset(XTEA_encrypt_buffer, 0, XTEA_MAX_BUFFER_SIZE);
    memset(XTEA_decrypt_buffer, 0, XTEA_MAX_BUFFER_SIZE);

    key_len = strlen(TEST_KEY);
    memset(&xtea_test_key, 0, XTEA_FIXED_KEY_SIZE);
    memset(iv.iv_array,0,XTEA_BLOCK_SIZE);
    memcpy(&xtea_test_key, TEST_KEY, XTEA_FIXED_KEY_SIZE);
    memcpy(iv.iv_array, IV, XTEA_BLOCK_SIZE);

    memcpy(input_string,TEST_STRING,test_string_len);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i != test_string_len; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");

    XTEA_init(&xtea_test, NUM_ROUNDS, &xtea_test_key, &iv);
    xtea_code = XTEA_encrypt(&xtea_test, input_string, XTEA_encrypt_buffer, test_string_len,ecb,&output_len);


    printf("XTEA output ( %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", XTEA_encrypt_buffer[i]);
    }
    printf("]\n");

    xtea_code = XTEA_decrypt(&xtea_test, XTEA_encrypt_buffer, XTEA_decrypt_buffer, test_string_len,ecb,&output_len);

    printf("Back to ASCII (%u bytes):\n<<", output_len, XTEA_decrypt_buffer);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", XTEA_decrypt_buffer[i]);
    }
    printf(">>\n");

    comparison = memcmp(TEST_STRING, XTEA_decrypt_buffer, test_string_len);
    printf("%s %s\n",TEST_NAME, comparison==0? "OK":"not OK");

    return comparison==0? EXIT_SUCCESS:EXIT_FAILURE;

}