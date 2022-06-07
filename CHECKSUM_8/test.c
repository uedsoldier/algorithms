#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "checksum8.h"

char cadena[64];

int main(int argc, char *argv[]){
    if (argc != 2)
    {
        printf("Se debe mandar un argumento ");
        return EXIT_FAILURE;
    }
    memset(cadena,0,sizeof(cadena));
    sprintf(cadena,"%s",argv[1]);
    uint16_t str_len = sizeof(argv[1]);
    printf("Tamano cadena entrada: %u\r\n",str_len);
    printf("Entrada: %s\r\n",argv[1]);
    uint8_t check8_xor = checksum8_XOR(cadena,str_len);
    printf("checksum 8 xor: 0x%02X\r\n",check8_xor);
    uint8_t check8_mod256 = checksum8_modulo256(cadena,str_len);
    printf("checksum 8 modulo 256: 0x%02X\r\n",check8_mod256);
    uint8_t check8_2complement = checksum8_2complement(cadena,str_len);
    printf("checksum 8 2-complement: 0x%02X\r\n",check8_2complement);

    return 0;
}