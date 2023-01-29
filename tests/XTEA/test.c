#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/XTEA/XTEA.h"
#include "../minunit.h"

uint8_t IV[XTEA_INIT_VECTOR_SIZE] = "vector**";
const uint8_t *TEST_STRING = "Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet.";
const uint8_t *TEST_KEY = "XTEAkey_0123456";

uint32_t test_string_len, test_string_len_normalized, key_len, comparison;
uint32_t output_len;

xtea_key_t xtea_test_key;
XTEA_t xtea_test;

XTEA_code_t xtea_code;

int tests_run = 0;
int total_tests = 10;

uint8_t xtea_rounds;
bool ecb;

#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *XTEA_encrypt_buffer;
uint8_t *XTEA_decrypt_buffer;
#else
uint8_t input_string[XTEA_MAX_BUFFER_SIZE];
uint8_t XTEA_encrypt_buffer[XTEA_MAX_BUFFER_SIZE];
uint8_t XTEA_decrypt_buffer[XTEA_MAX_BUFFER_SIZE];
#endif

static void perform_comparison()
{
    comparison = memcmp(TEST_STRING, XTEA_decrypt_buffer, test_string_len);
}

static void perform_xtea(uint16_t _rounds)
{
    XTEA_init(&xtea_test, _rounds, &xtea_test_key, IV);
    printf("Mode: %s. Rounds: %u\n", ecb ? "ECB" : "CBC", _rounds);
    xtea_code = XTEA_encrypt(&xtea_test, input_string, XTEA_encrypt_buffer, test_string_len, ecb, &output_len);

    printf("XTEA output (block size %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", XTEA_encrypt_buffer[i]);
    }
    printf("]\n");

    xtea_code = XTEA_decrypt(&xtea_test, XTEA_encrypt_buffer, XTEA_decrypt_buffer, test_string_len, ecb, &output_len);
    printf("Back to ASCII (%u bytes):\n<<%s>>\n", strlen(XTEA_decrypt_buffer), XTEA_decrypt_buffer);

}

static void perform_xxtea()
{
    XXTEA_init(&xtea_test, &xtea_test_key, IV);
    printf("Mode: %s.\n", ecb ? "ECB" : "CBC");
    xtea_code = XXTEA_encrypt(&xtea_test, input_string, XTEA_encrypt_buffer, test_string_len, ecb, &output_len);
    
    printf("XTEA output (block size %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", XTEA_encrypt_buffer[i]);
    }
    printf("]\n");

    xtea_code = XXTEA_decrypt(&xtea_test, XTEA_encrypt_buffer, XTEA_decrypt_buffer, test_string_len, ecb, &output_len);
    printf("Back to ASCII (%u bytes):\n<<%s>>\n", strlen(XTEA_decrypt_buffer), XTEA_decrypt_buffer);

}

void fill_input_data()
{
    memcpy(input_string,TEST_STRING,test_string_len);
    //sprintf(input_string, "%s", TEST_STRING);
    printf("\nFILL INPUT DATA\n<<");
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");

}

void reset_data()
{
    memset(input_string, 0, test_string_len_normalized);
    memset(XTEA_encrypt_buffer, 0, test_string_len_normalized);
    memset(XTEA_decrypt_buffer, 0, test_string_len_normalized);

    printf("\nRESET DATA\n");
    printf("input_string\n<<");
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    printf("XTEA_encrypt_buffer\n<<");
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",XTEA_encrypt_buffer[i]);
    }
    printf(">>\n");
    printf("XTEA_decrypt_buffer\n<<");    
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",XTEA_decrypt_buffer[i]);
    }
    printf(">>\n");

}

static char *test_xtea_cbc_8rounds()
{
    xtea_rounds = 8;
    ecb = false;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_cbc_16rounds()
{
    xtea_rounds = 16;
    ecb = false;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_cbc_32rounds()
{
    xtea_rounds = 32;
    ecb = false;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_cbc_64rounds()
{
    xtea_rounds = 64;
    ecb = false;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_ebc_8rounds()
{
    xtea_rounds = 8;
    ecb = true;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_ebc_16rounds()
{
    xtea_rounds = 16;
    ecb = true;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_ebc_32rounds()
{
    xtea_rounds = 32;
    ecb = true;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xtea_ebc_64rounds()
{
    xtea_rounds = 64;
    ecb = true;
    perform_xtea(xtea_rounds);
    perform_comparison();
    mu_assert("XTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xxtea_ecb()
{
    ecb = true;
    perform_xxtea();
    perform_comparison();
    mu_assert("XXTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *test_xxtea_cbc()
{
    ecb = false;
    perform_xxtea();
    perform_comparison();
    mu_assert("XXTEA not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

static char *all_tests()
{
    // Test 1
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_ebc_8rounds);
    printf("Test passed\n");
    // Test 2
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_ebc_16rounds);
    printf("Test passed\n");
    // Test 3
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_ebc_32rounds);
    printf("Test passed\n");
    // Test 4
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_ebc_64rounds);
    printf("Test passed\n");
    // Test 5
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_cbc_8rounds);
    printf("Test passed\n");
    // Test 6
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_cbc_16rounds);
    printf("Test passed\n");
    // Test 7
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_cbc_32rounds);
    printf("Test passed\n");
    // Test 8
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xtea_cbc_64rounds);
    printf("Test passed\n");
    // Test 9
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xxtea_ecb);
    printf("Test passed\n");
    // Test 10
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<",test_string_len);
    for (size_t i = 0; i < test_string_len_normalized; i++)
    {
        printf("%c",input_string[i]);
    }
    printf(">>\n");
    mu_run_test(test_xxtea_cbc);
    printf("Test passed\n");
    printf("-------------------------------------------\n");
    return 0;
}

int main(int argc, char *argv[])
{
    printf("XTEA testing\n");
    test_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", test_string_len);
    test_string_len_normalized = ((test_string_len + 7) & (-8L));
    printf("Test string length (normalized to nearest multiple of 8): %u\n", test_string_len_normalized);
    key_len = strlen(TEST_KEY);
    memcpy(&xtea_test_key, TEST_KEY, key_len);

#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
    input_string = (uint8_t *)malloc(test_string_len_normalized);
    XTEA_encrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
    XTEA_decrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
    if (input_string == NULL || XTEA_encrypt_buffer == NULL || XTEA_decrypt_buffer == NULL)
    {
        printf("NULL pointer\n");
        return EXIT_FAILURE;
    }
#endif
    printf("Input string buffer size (bytes): %u\n",test_string_len_normalized);
    printf("Encrypt buffer size (bytes): %u\n",test_string_len_normalized);
    printf("Decrypt buffer size (bytes): %u\n",test_string_len_normalized);

    #if !defined(XTEA_DYNAMIC_MEMORY) || (XTEA_DYNAMIC_MEMORY == 0)
    if(test_string_len_normalized > XTEA_MAX_BUFFER_SIZE){
        printf("Normalized string length (%u) exceeds possible max buffer size (%u)\n",test_string_len_normalized,XTEA_MAX_BUFFER_SIZE);
        return EXIT_FAILURE;
    }
    #endif


    reset_data();
    fill_input_data();

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

#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
    free(input_string);
    free(XTEA_encrypt_buffer);
    free(XTEA_decrypt_buffer);
#endif

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}