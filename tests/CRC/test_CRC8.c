#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CRC/crc.h"

const char *TEST_NAME = "CRC8 test";
const char *TEST_STRING="Test string for CRC validation";
uint16_t test_str_len;
uint8_t impl_cnt = 0;

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

int main(int argc, char *argv[])
{
    bool crc_equal;
    printf("%s\n",TEST_NAME);
    test_str_len = strlen(TEST_STRING);
    printf("ASCII input: <<%s>> (length: %u bytes)\n", TEST_STRING, test_str_len);

    for (crc_t i = CRC8_CCITT; i <= CRC8_8H2F; i++)
    {
        uint8_t result = CRC8(TEST_STRING, test_str_len, i);
        crc_equal = (result == test_CRC8_values[impl_cnt]);
        printf("Algorithm: %-22s\n", get_crc8_implementation(impl_cnt));
        printf(" * Result: 0x%02X  Expected: 0x%02X\n", result,test_CRC8_values[impl_cnt]);
        if( !crc_equal){
            break;
        }
        impl_cnt++;
    }
    
    printf("%s %s\n",TEST_NAME,crc_equal? "OK":"not OK");
    return crc_equal? EXIT_SUCCESS:EXIT_FAILURE;
}
