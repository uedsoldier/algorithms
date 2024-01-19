#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/AES/AES.h"
#include "../../src/PKCS7/PKCS7.h"


size_t test_string_len, test_string_len_normalized, key_len, comparison;
size_t output_len;

AES_ctx_t AES_ctx;

AES_errcode_t AES_code;

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *AES_encrypt_buffer;
uint8_t *AES_decrypt_buffer;
#else
uint8_t input_string[AES_MAX_BUFFER_SIZE];
uint8_t AES_encrypt_buffer[AES_MAX_BUFFER_SIZE];
uint8_t AES_decrypt_buffer[AES_MAX_BUFFER_SIZE];
#endif

int main(int argc, char *argv[]){
    return EXIT_FAILURE;
}