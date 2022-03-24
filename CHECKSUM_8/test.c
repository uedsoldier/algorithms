#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "checksum8.h"

const char texto[] = "Gol de la fiera";

char cadena[32];
char buffer1[64];
char buffer2[64];


int main(){
    memset(cadena,0,sizeof(cadena));
    sprintf(cadena,"%s",texto);
    memset(buffer1,0,sizeof(buffer1));
    uint16_t str_len = sizeof(texto);
    printf("Tamano cadena entrada: %u\r\n",str_len);
    printf("Entrada: %s\r\n",texto);
    uint8_t check8_xor = checksum8_XOR(cadena,str_len);
    printf("checksum 8 xor: 0x%02X\r\n",check8_xor);
    uint8_t check8_mod256 = checksum8_modulo256(cadena,str_len);
    printf("checksum 8 modulo 256: 0x%02X\r\n",check8_mod256);
    uint8_t check8_2complement = checksum8_2complement(cadena,str_len);
    printf("checksum 82 complement: 0x%02X\r\n",check8_2complement);

    



    return 0;
}