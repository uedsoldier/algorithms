#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/XTEA/XTEA.h"
#include "../minunit.h"

uint8_t IV[XTEA_INIT_VECTOR_SIZE] = "vector**";
const uint8_t *TEST_STRING = "Test XTEA string for validation";
const uint8_t *TEST_KEY = "XTEAkey_0123456";

uint16_t test_string_len, test_string_len_normalized, key_len, comparison;
uint32_t output_len;

xtea_key_t xtea_test_key;
XTEA_t xtea_test;

int tests_run = 0;
int total_tests = 8;

uint8_t xtea_rounds;
bool ecb;

#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *XTEA_encrypt_buffer;
uint8_t *XTEA_decrypt_buffer;
#else
uint8_t input_string[256];
uint8_t XTEA_encrypt_buffer[256];
uint8_t XTEA_decrypt_buffer[256];
#endif

static void perform_comparison()
{
#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
    comparison = memcmp(input_string, XTEA_decrypt_buffer, test_string_len);
#elif defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
    comparison = memcmp(input_string, XTEA_decrypt_buffer, test_string_len);
#elif defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 0)
    comparison = memcmp(TEST_STRING, XTEA_decrypt_buffer, test_string_len);
#endif
}

static void perform_xtea(uint16_t _rounds, bool _ecb)
{
    XTEA_init(&xtea_test, _rounds, &xtea_test_key, IV);
    printf("Mode: %s. Rounds: %u\n",_ecb? "ECB":"CBC", _rounds);
    XTEA_encrypt(&xtea_test, input_string, XTEA_encrypt_buffer, test_string_len, _ecb, &output_len);

    printf("XTEA output (block size %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", XTEA_encrypt_buffer[i]);
    }
    printf("]\n");

    XTEA_decrypt(&xtea_test, XTEA_encrypt_buffer, XTEA_decrypt_buffer, test_string_len, _ecb, &output_len);
    printf("Back to ASCII (%u bytes):\n<<%s>>\n", strlen(XTEA_decrypt_buffer), XTEA_decrypt_buffer);
}

void fill_input_data()
{
    sprintf(input_string, "%s", TEST_STRING);
}

void reset_data()
{
    memset(input_string, 0, sizeof(test_string_len_normalized));
    memset(XTEA_encrypt_buffer, 0, sizeof(test_string_len_normalized));
    memset(XTEA_decrypt_buffer, 0, sizeof(test_string_len_normalized));
}

static char *test_xtea_cbc_8rounds()
{
    fill_input_data();
    xtea_rounds = 8;
    ecb = false;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_cbc_16rounds()
{
    fill_input_data();
    xtea_rounds = 16;
    ecb = false;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_cbc_32rounds()
{
    fill_input_data();
    xtea_rounds = 32;
    ecb = false;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_cbc_64rounds()
{

    fill_input_data();
    xtea_rounds = 64;
    ecb = false;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_ebc_8rounds()
{
    fill_input_data();
    xtea_rounds = 8;
    ecb = true;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    XTEA_init(&xtea_test, xtea_rounds, &xtea_test_key, IV);
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_ebc_16rounds()
{
    fill_input_data();
    xtea_rounds = 16;
    ecb = true;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    XTEA_init(&xtea_test, xtea_rounds, &xtea_test_key, IV);
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_ebc_32rounds()
{
    fill_input_data();
    xtea_rounds = 32;
    ecb = true;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    XTEA_init(&xtea_test, xtea_rounds, &xtea_test_key, IV);
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *test_xtea_ebc_64rounds()
{
    fill_input_data();
    xtea_rounds = 64;
    ecb = true;
    perform_xtea(xtea_rounds, ecb);
    perform_comparison();
    XTEA_init(&xtea_test, xtea_rounds, &xtea_test_key, IV);
    mu_assert("XTEA not OK\r\n", comparison == 0);
    return 0;
}

static char *all_tests()
{
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_ebc_8rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_ebc_16rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_ebc_32rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_ebc_64rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_cbc_8rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_cbc_16rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_cbc_32rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    printf("ASCII in (%u bytes):\n<<%s>>\n", test_string_len, input_string);
    printf("Normalized input (bytes): %u\n", test_string_len_normalized);
    mu_run_test(test_xtea_cbc_64rounds);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    return 0;
}

int main(int argc, char *argv[])
{

#if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
    *input_string = (uint8_t *)malloc(test_string_len_normalized);
    *XTEA_encrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
    *XTEA_decrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
#endif
    if (input_string == NULL || XTEA_encrypt_buffer == NULL || XTEA_decrypt_buffer == NULL)
    {
        printf("NULL pointer\n");
        return EXIT_FAILURE;
    }

    reset_data();

    test_string_len = strlen(TEST_STRING);
    test_string_len_normalized = ((test_string_len + 7) & (-8));
    key_len = strlen(TEST_KEY);
    memcpy(&xtea_test_key, TEST_KEY, key_len);

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