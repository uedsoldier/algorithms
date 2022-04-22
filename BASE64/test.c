#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "base64.h"


char cadena[64];
char buffer1[128];
char buffer2[128];


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
    printf("Entrada ASCII: %s\r\n",argv[1]);
    bin_to_base64(buffer1,cadena,str_len);
    printf("BASE64: %s\r\n",buffer1);

    base64_to_bin(buffer2,buffer1,strlen(buffer1));
    printf("De vuelta a ASCII: %s",buffer2);
    return 0;
}