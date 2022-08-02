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
    uint16_t str_len = strlen(argv[1]);
    printf("Tamano cadena entrada: %u\r\n",str_len);
    printf("Entrada: %s\r\n",argv[1]);

    for(uint8_t i=0; i!=CHECKSUM8_COUNT; i++){
        printf("Checksum 8 %s: 0x%02X\r\n",checksum8_implementations[i],checksum8(cadena,str_len,i));
    }


    return 0;
}