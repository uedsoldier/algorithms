#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CRC/crc.h"

const char *TEST_NAME = "CRC32 test";
const char *TEST_STRING="Test string for CRC validation";
uint16_t test_str_len;
uint8_t impl_cnt = 0;

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

int main(int argc, char *argv[])
{
    bool crc_equal;
    printf("%s\n",TEST_NAME);
    test_str_len = strlen(TEST_STRING);
    printf("ASCII input: <<%s>> (length: %u bytes)\n", TEST_STRING, test_str_len);

    for (crc_t i = CRC32_D; i <= CRC32_XFER; i++)
    {
        uint32_t result = CRC32(TEST_STRING, test_str_len, i);
        crc_equal = (result == test_CRC32_values[impl_cnt]);
        printf("Algorithm: %-22s\n", get_crc16_implementation(impl_cnt));
        printf(" * Result: 0x%02X  Expected: 0x%02X\n", result,test_CRC32_values[impl_cnt]);
        if( !crc_equal){
            break;
        }
        impl_cnt++;
    }
    
    printf("%s %s\n",TEST_NAME,crc_equal? "OK":"not OK");
    return crc_equal? EXIT_SUCCESS:EXIT_FAILURE;
}
