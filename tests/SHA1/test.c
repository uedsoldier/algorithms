#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/SHA1/SHA1.h"
#include "../minunit.h"

const size_t TEST_LENGTHS[] = {
    4, 23
};

const uint8_t *TEST_STRINGS[] = {
    "Hola","La vaquita es fenomenal"
};

const uint8_t *EXPECTED_HASHES[SHA1_DIGEST_SIZE_BYTES] = {
    "\x4E\x46\xDC\x09\x69\xE6\x62\x1F\x2D\x61\xD2\x22\x8E\x3C\xD9\x1B\x75\xCD\x9E\xDC",
    "\xA7\x93\x6A\x16\x03\x2F\x1F\x2B\x5F\x16\xCE\x72\xFE\x9A\xE5\x6D\x87\x3E\x8C\xEC",
};
// Copy hash from SHA1_test.py script while using the TEST_STRINGS elements as input to the script

size_t comparison;

SHA1_ctx_t ctx;

int tests_run = 0;
int total_tests = 1;

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *output_hash;
#else
uint8_t input_string[SHA1_MAX_BUFFER_SIZE_BYTES];
uint8_t output_hash[SHA1_DIGEST_SIZE_BYTES];
#endif

static void perform_comparison(uint8_t test_num)
{
    comparison = memcmp(EXPECTED_HASHES[test_num], output_hash, SHA1_DIGEST_SIZE_BYTES);
}

static void perform_SHA1(uint8_t test)
{
    printf("Expected hash:\n<<");
    for (size_t i = 0; i != SHA1_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",EXPECTED_HASHES[test][i]);
    }
    printf(">>\n");

    sha1(TEST_STRINGS[test],output_hash,TEST_LENGTHS[test]);

    printf("Computed hash:\n<<");
    for (size_t i = 0; i != SHA1_DIGEST_SIZE_BYTES; i++)
    {
        printf("%02X ",output_hash[i]);
    }
    printf(">>\n");

}

void fill_input_data(uint8_t test)
{
    memcpy(input_string,TEST_STRINGS[test],TEST_LENGTHS[test]);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i != TEST_LENGTHS[test]; i++)
    {
        printf("%02X ",input_string[i]);
    }
    printf(">>\n");

}

void reset_data()
{
    printf("Resetting buffers...\n");
    memset(input_string,0,SHA1_MAX_BUFFER_SIZE_BYTES);
    memset(output_hash,0,SHA1_DIGEST_SIZE_BYTES);
}

void test_init(size_t len){
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII hex in (%u bytes):\n<<",len);
    for (size_t i = 0; i != len; i++)
    {
        printf("%02x ",input_string[i]);
    }
    printf(">>\n");
}

static char *test_1(){
    reset_data();
    fill_input_data(0);
    perform_SHA1(0);
    perform_comparison(0);
    mu_assert("SHA1 for test 1 not OK\n", comparison == 0);
    return 0;
}

static char *test_2(){
    reset_data();
    fill_input_data(1);
    perform_SHA1(1);
    perform_comparison(1);
    mu_assert("SHA1 for test 2 not OK\n", comparison == 0);
    return 0;
}



static char *all_tests()
{
    // Test 1
    test_init(TEST_LENGTHS[0]);
    mu_run_test(test_1);
    printf("Test passed\n");
    //Test 2
    test_init(TEST_LENGTHS[1]);
    mu_run_test(test_2);
    printf("Test passed\n");
    // Test 'n'
    // test_init();
    // mu_run_test(test_n);
    // printf("Test passed\n");
    
    return 0;
}

int main(int argc, char *argv[])
{
    printf("SHA1 testing\n");
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %u/%u\n", tests_run, total_tests);

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
    free(input_string);
    free(AES_encrypt_buffer);
    free(AES_decrypt_buffer);
#endif

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}