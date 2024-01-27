#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/SHA256/SHA256.h"

const char *TEST_NAME = "SHA256 - test1";

const uint8_t *TEST_STRING = "La vaquita es fenomenal";      
    
const uint8_t EXPECTED_HASH[SHA256_DIGEST_SIZE_BYTES] = "\x95\xFE\x15\x40\x96\x10\x80\x91\x2A\xDA\x2C\x7B\xAE\xD2\x9F\x96\xFB\xA9\xD0\x8A\x24\x3F\x89\xE1\xEF\xE6\x3D\x7D\x6F\x0F\x5D\x22";
// Copy hash from SHA256_test.py script while using the TEST_STRINGS elements as input to the script

size_t comparison;
size_t test_string_len;

SHA256_ctx_t ctx;

uint8_t input_string[SHA256_MAX_BUFFER_SIZE_BYTES];
uint8_t output_hash[SHA256_DIGEST_SIZE_BYTES];

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    test_string_len = strlen(TEST_STRING);
    memset(input_string,0,SHA256_MAX_BUFFER_SIZE_BYTES);
    memset(output_hash,0,SHA256_DIGEST_SIZE_BYTES);
    memcpy(input_string,TEST_STRING,test_string_len);

    printf("\nFILL INPUT DATA (%u bytes)\n<<",test_string_len);
    for (size_t i = 0; i != test_string_len; i++)
    {
        printf("%02x ",input_string[i]);
    }
    printf(">>\n");

    printf("Expected hash:\n<<");
    for (size_t i = 0; i != SHA256_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",EXPECTED_HASH[i]);
    }
    printf(">>\n");

    SHA256(TEST_STRING,output_hash,test_string_len);

    printf("Computed hash:\n<<");
    for (size_t i = 0; i != SHA256_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",output_hash[i]);
    }
    printf(">>\n");

    comparison = memcmp(EXPECTED_HASH, output_hash, SHA256_DIGEST_SIZE_BYTES);

    printf("%s %s",TEST_NAME,comparison==0? "OK":"not OK");
    return comparison == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}