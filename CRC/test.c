#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "crc.h"

const char texto[] = "0";

char cadena[16];
uint8_t cnt = 0;

// #define TEST_CRC8
// #define TEST_CRC16
#define TEST_CRC32

int main(){
    memset(cadena,0,sizeof(cadena));
    uint16_t str_len = sizeof(texto);
    memcpy(cadena,texto,str_len);
    
    printf("Tamano cadena entrada: %u\r\n",str_len);
    printf("Entrada: %s\r\n",cadena);
    #ifdef TEST_CRC8
    for(crc_t i=CRC8_CCITT; i != CRC16_XMODEM; i++){
        printf("%s:\n",get_crc8_implementation(cnt++));
        printf("\tResult: 0x%02X\n",CRC8(cadena,str_len-1,i));
    }
    #endif
    #ifdef TEST_CRC16
    cnt = 0;
    for(uint8_t i=CRC16_XMODEM; i != CRC32_D; i++){
        printf("%s:\n",get_crc16_implementation(cnt++));
        printf("\tResult: 0x%04X\n",CRC16(cadena,str_len-1,i));
    }
    #endif
    #ifdef TEST_CRC32
    cnt = 0;
    for(uint8_t i=CRC32_D; i <= CRC32_XFER; i++){
        printf("%s:\n",get_crc32_implementation(cnt++));
        printf("\tResult: 0x%08X\n",CRC32(cadena,str_len-1,i));
    }
    return 0;
    #endif
}