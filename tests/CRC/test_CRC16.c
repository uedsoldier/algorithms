#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/CRC/crc.h"

const char *TEST_NAME = "CRC16 test";
const char *TEST_STRING="Test string for CRC validation";
uint16_t test_str_len;
uint8_t impl_cnt = 0;

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

int main(int argc, char *argv[])
{
    bool crc_equal;
    printf("%s\n",TEST_NAME);
    test_str_len = strlen(TEST_STRING);
    printf("ASCII input: <<%s>> (length: %u bytes)\n", TEST_STRING, test_str_len);

    for (crc_t i = CRC16_XMODEM; i <= CRC16_CDMA2000; i++)
    {
        uint16_t result = CRC16(TEST_STRING, test_str_len, i);
        crc_equal = (result == test_CRC16_values[impl_cnt]);
        printf("Algorithm: %-22s\n", get_crc16_implementation(impl_cnt));
        printf(" * Result: 0x%02X  Expected: 0x%02X\n", result,test_CRC16_values[impl_cnt]);
        if( !crc_equal){
            break;
        }
        impl_cnt++;
    }
    
    printf("%s %s\n",TEST_NAME,crc_equal? "OK":"not OK");
    return crc_equal? EXIT_SUCCESS:EXIT_FAILURE;
}
