#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/SHA256/SHA256.h"

const char *TEST_NAME = "SHA256 - test1";

const uint8_t *TEST_STRING = "Hola"; 
    
const uint8_t EXPECTED_HASH[SHA256_DIGEST_SIZE_BYTES] = "\xE6\x33\xF4\xFC\x79\xBA\xDE\xA1\xDC\x5D\xB9\x70\xCF\x39\x7C\x82\x48\xBA\xC4\x7C\xC3\xAC\xF9\x91\x5B\xA6\x0B\x5D\x76\xB0\xE8\x8F";
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