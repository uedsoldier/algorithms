#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "crc.h"
#include <time.h>
#include "../../utilities/utils.h"
char cadena[16];
uint8_t cnt = 0;

#define TEST_CRC8 0
#define TEST_CRC16 0
#define TEST_CRC32 1

#define DEBUG 1

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Se debe mandar un argumento ");
        return EXIT_FAILURE;
    }
    float time_spent = 0.0F;
    clock_t begin = clock();
    memset(cadena, 0, sizeof(cadena));
    uint16_t str_len = strlen(argv[1]);
    memcpy(cadena, argv[1], str_len);

    printf("Tamano cadena entrada: %u\r\n", str_len);
    printf("Entrada: %s\r\n", cadena);
#if TEST_CRC8 == 1
    for (crc_t i = CRC8_CCITT; i != CRC16_XMODEM; i++)
    {
        printf("Algorithm: %s:\t", get_crc8_implementation(cnt++));
        printf("0x%02X\n", CRC8(cadena, str_len, i));
    }
#endif
#if TEST_CRC16 == 1
    cnt = 0;
    for (uint8_t i = CRC16_XMODEM; i != CRC32_D; i++)
    {
        printf("Algorithm: %s:\t", get_crc16_implementation(cnt++));
        printf("0x%04X\n", CRC16(cadena, str_len, i));
    }
#endif
#if TEST_CRC32 == 1
    cnt = 0;
    for (uint8_t i = CRC32_D; i <= CRC32_XFER; i++)
    {
        printf("Algorithm: %s:\t", get_crc32_implementation(cnt++));
        printf("0x%08X\n", CRC32(cadena, str_len, i));
    }
#endif
    clock_t end = clock();

    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time_spent = (float)(end - begin);
    printf("Elapsed time: %0.3f [ms]", time_spent);
    return EXIT_SUCCESS;
}