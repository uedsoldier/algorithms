#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CRC/crc.h"
#include "../minunit.h"

int tests_run = 0;
int total_tests = 3;


const uint8_t *TEST_STRING="Test string for CRC validation";
uint16_t test_str_len;
uint8_t cnt = 0;

const uint8_t test_CRC8_values[] = {
    0xDC, //CRC8_CCITT         
    0xF8, //CRC8_CDMA2000      
    0xCD, //CRC8_DARC          
    0x34, //CRC8_DVB_S2        
    0x18, //CRC8_EBU           
    0x34, //CRC8_I_CODE        
    0x89, //CRC8_ITU           
    0xD0, //CRC8_MAXIM         
    0x07, //CRC8_ROHC          
    0xF1, //CRC8_WCDMA         
    0x93, //CRC8_SAE_J1850     
    0x8A, //CRC8_SAE_J1850_ZERO
    0x62, //CRC8_8H2F          
};

const uint16_t test_CRC16_values[] = {
    0xC8D8,// CRC16_XMODEM      
    0x3994,// CRC16_AUG_CCITT   
    0xE29D,// CRC16_CCITT_FALSE 
    0x1D62,// CRC16_GENIBUS     
    0xD7E5,// CRC16_CCITT_KERMIT
    0xDB01,// CRC16_TMS37157    
    0x5EBD,// CRC16_RIELLO      
    0x9F9E,// CRC16_A           
    0x75B1,// CRC16_MCRF4XX     
    0x8A4E,// CRC16_X25         
    0xEFD8,// CRC16_ARC         
    0x5CA8,// CRC16_BUYPASS     
    0xDC81,// CRC16_DDS110      
    0x1027,// CRC16_MAXIM       
    0x5FD9,// CRC16_USB         
    0xA026,// CRC16_MODBUS      
    0x37F5,// CRC16_DECT_X      
    0x37F4,// CRC16_DECT_R      
    0xB57E,// CRC16_DNP         
    0x2A3A,// CRC16_EN13757     
    0x24DA,// CRC16_T10_DIF     
    0xC8D7,// CRC16_TELEDISK    
    0xDC8F,// CRC16_CDMA2000             
};

const uint32_t test_CRC32_values[] = {
    0x6ABDD5BA,// CRC32_D     
    0x26374F09,// CRC32_Q     
    0x44E584B8,// CRC32_C     
    0xFA2E39F4,// CRC32_ISO   
    0xFF368778,// CRC32_BZIP2 
    0x00C97887,// CRC32_MPEG_2
    0xADB55887,// CRC32_POSIX 
    0x05D1C60B,// CRC32_JAMCRC
    0xA2761C90,// CRC32_XFER               
};

static char *test_CRC8()
{
    for (crc_t i = CRC8_CCITT; i != CRC16_XMODEM; i++)
    {
        uint8_t result = CRC8(TEST_STRING, test_str_len, i);
        printf("Algorithm: %-22s\n", get_crc8_implementation(cnt));
        printf(" * Result: 0x%02X   Expected: 0x%02X\n", result,test_CRC8_values[cnt]);
        mu_assert("Error: incorrect CRC8", result == test_CRC8_values[cnt++]);
    }
    cnt = 0; 
    return 0;
}

static char *test_CRC16()
{
    for (uint8_t i = CRC16_XMODEM; i != CRC32_D; i++)
    {
        uint16_t result = CRC16(TEST_STRING, test_str_len, i);
        printf("Algorithm: %-22s\n", get_crc16_implementation(cnt));
        printf(" * Result: 0x%04X   Expected: 0x%04X\n", result,test_CRC16_values[cnt]);
        mu_assert("Error: incorrect CRC16", result == test_CRC16_values[cnt++]);
    }
    cnt = 0;
    return 0; 
}

static char *test_CRC32()
{
    for (uint8_t i = CRC32_D; i <= CRC32_XFER; i++)
    {
        uint32_t result = CRC32(TEST_STRING, test_str_len, i);
        printf("Algorithm: %-22s\n", get_crc32_implementation(cnt));
        printf(" * Result: 0x%08X   Expected: 0x%08X\n", result,test_CRC32_values[cnt]);
        mu_assert("Error: incorrect CRC32", result == test_CRC32_values[cnt++]);
    }
    cnt = 0;
    return 0; 
}


static char *all_tests()
{
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    mu_run_test(test_CRC8);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    mu_run_test(test_CRC16);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    printf("Test %u/%u\r\n", tests_run + 1, total_tests);
    mu_run_test(test_CRC32);
    printf("Test passed\r\n");
    printf("-------------------------------------------\r\n");
    return 0;
}

int main(int argc, char *argv[])
{

    test_str_len = strlen(TEST_STRING);
    printf("ASCII input: %s\r\n", TEST_STRING);
    printf("Input string length: %u\r\n", test_str_len);

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

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}