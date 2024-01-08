#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/AES/AES.h"
#include "../minunit.h"

const char IV[AES_BLOCK_LEN] = "0123456789ABCDEF";    // 16-byte init vector  
const char *TEST_STRING = "HolaHolaHola";
const char TEST_KEY[AES_FIXED_KEY_SIZE] = "aesEncryptionKey";  // key

size_t test_string_len, test_string_len_normalized, key_len, comparison;
size_t output_len;

AES_ctx_t AES_ctx;

AES_errcode_t AES_code;

int tests_run = 0;
int total_tests = 1;

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
uint8_t *input_string;
uint8_t *AES_encrypt_buffer;
uint8_t *AES_decrypt_buffer;
#else
uint8_t input_string[AES_MAX_BUFFER_SIZE];
uint8_t AES_encrypt_buffer[AES_MAX_BUFFER_SIZE];
uint8_t AES_decrypt_buffer[AES_MAX_BUFFER_SIZE];
#endif

static void perform_comparison()
{
    comparison = memcmp(TEST_STRING, AES_decrypt_buffer, test_string_len);
}

static void perform_AES_ECB()
{
    AES_init_ctx(&AES_ctx, TEST_KEY);
    printf("Mode: ECB.\n");
    AES_code = AES_ECB_encrypt(&AES_ctx, input_string, AES_encrypt_buffer, test_string_len, &output_len);

    printf("AES output (block size %u bytes): [ ", output_len);
    for (size_t i = 0; i != output_len; i++)
    {
        printf("%02x ", AES_encrypt_buffer[i]);
    }
    printf("]\n");

    AES_code = AES_ECB_decrypt(&AES_ctx, AES_encrypt_buffer, AES_decrypt_buffer, test_string_len, &output_len);
    printf("Back to ASCII (%u bytes):\n<<%s>>\n", strlen(AES_decrypt_buffer), AES_decrypt_buffer);
}

// static void perform_AES_CBC()
// {
//     AES_init_ctx_iv(&AES_ctx, &AES_test_key, IV);
//     printf("Mode: CBC.\n");
//     AES_code = AES_CBC_encrypt(&AES_ctx, input_string, AES_encrypt_buffer, test_string_len, &output_len);

//     printf("AES output (block size %u bytes): [ ", output_len);
//     for (size_t i = 0; i != output_len; i++)
//     {
//         printf("%02x ", AES_encrypt_buffer[i]);
//     }
//     printf("]\n");

//     AES_code = AES_CBC_decrypt(&AES_ctx, AES_encrypt_buffer, AES_decrypt_buffer, test_string_len, &output_len);
//     printf("Back to ASCII (%u bytes):\n<<%s>>\n", strlen(AES_decrypt_buffer), AES_decrypt_buffer);

// }

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
    memset(AES_encrypt_buffer, 0, test_string_len_normalized);
    memset(AES_decrypt_buffer, 0, test_string_len_normalized);
}

static char *test_AES_ECB()
{   
    perform_AES_ECB();
    perform_comparison();
    mu_assert("AES not OK\n", comparison == 0);
    reset_data();
    fill_input_data();
    return 0;
}

// static char *test_AES_CBC()
// {   
//     perform_AES_CBC();
//     perform_comparison();
//     mu_assert("AES not OK\n", comparison == 0);
//     reset_data();
//     fill_input_data();
//     return 0;
// }

void test_init(){
    printf("-------------------------------------------\n");
    printf("Test %u/%u\n", tests_run + 1, total_tests);
    printf("ASCII hex in (%u bytes):\n<<",test_string_len_normalized);
    for (size_t i = 0; i != test_string_len_normalized; i++)
    {
        printf("%02x ",input_string[i]);
    }
    printf(">>\n");
}


static char *all_tests()
{
    // Test 1
    test_init();
    mu_run_test(test_AES_ECB);
    printf("Test passed\n");
    // Test 2
    // test_init();
    // mu_run_test(test_AES_CBC);
    // printf("Test passed\n");
    
    return 0;
}

int main(int argc, char *argv[])
{
    printf("AES testing\n");
    test_string_len = strlen(TEST_STRING);
    printf("Test string length: %u\n", test_string_len);
    test_string_len_normalized = AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(test_string_len);
    // if(test_string_len == test_string_len_normalized){
    //     test_string_len_normalized += AES_BLOCK_LEN;
    // }
    printf("Test string length (normalized to nearest multiple of 16 and incremented if multiple of 16): %u\n", test_string_len_normalized);
    key_len = strlen(TEST_KEY);

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
    input_string = (uint8_t *)malloc(test_string_len_normalized);
    AES_encrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
    AES_decrypt_buffer = (uint8_t *)malloc(test_string_len_normalized);
    if (input_string == NULL || AES_encrypt_buffer == NULL || AES_decrypt_buffer == NULL)
    {
        printf("NULL pointer\n");
        return EXIT_FAILURE;
    }
#endif
    printf("Input string buffer size (bytes): %u\n",test_string_len_normalized);
    printf("Encrypt buffer size (bytes): %u\n",test_string_len_normalized);
    printf("Decrypt buffer size (bytes): %u\n",test_string_len_normalized);

    #if !defined(AES_DYNAMIC_MEMORY) || (AES_DYNAMIC_MEMORY == 0)
    if(test_string_len_normalized > AES_MAX_BUFFER_SIZE){
        printf("Normalized string length (%u) exceeds possible max buffer size (%u)\n",test_string_len_normalized,AES_MAX_BUFFER_SIZE);
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

#if defined(AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
    free(input_string);
    free(AES_encrypt_buffer);
    free(AES_decrypt_buffer);
#endif

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}