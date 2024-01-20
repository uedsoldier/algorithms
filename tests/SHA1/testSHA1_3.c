#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/SHA1/SHA1.h"

const char *TEST_NAME = "SHA1 - test1";

const uint8_t *TEST_STRING = "Hijos de PERRA";    
    
const uint8_t EXPECTED_HASH[SHA1_DIGEST_SIZE_BYTES] = "\x62\xEA\xE8\x74\xD1\xF9\x3E\x28\x12\x1A\xB4\x15\x6E\xB6\xF6\x65\xB0\x3E\x19\xE7";
// Copy hash from SHA1_test.py script while using the TEST_STRINGS elements as input to the script

size_t comparison;
size_t test_string_len;

SHA1_ctx_t ctx;

uint8_t input_string[SHA1_MAX_BUFFER_SIZE_BYTES];
uint8_t output_hash[SHA1_DIGEST_SIZE_BYTES];

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);
    test_string_len = strlen(TEST_STRING);
    memset(input_string,0,SHA1_MAX_BUFFER_SIZE_BYTES);
    memset(output_hash,0,SHA1_DIGEST_SIZE_BYTES);
    memcpy(input_string,TEST_STRING,test_string_len);

    printf("\nFILL INPUT DATA (%u bytes)\n<<",test_string_len);
    for (size_t i = 0; i != test_string_len; i++)
    {
        printf("%02x ",input_string[i]);
    }
    printf(">>\n");

    printf("Expected hash:\n<<");
    for (size_t i = 0; i != SHA1_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",EXPECTED_HASH[i]);
    }
    printf(">>\n");

    sha1(TEST_STRING,output_hash,test_string_len);

    printf("Computed hash:\n<<");
    for (size_t i = 0; i != SHA1_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",output_hash[i]);
    }
    printf(">>\n");

    comparison = memcmp(EXPECTED_HASH, output_hash, SHA1_DIGEST_SIZE_BYTES);

    printf("%s %s",TEST_NAME,comparison==0? "OK":"not OK");
    return comparison == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}