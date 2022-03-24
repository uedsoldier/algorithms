#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "base64.h"

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
    bin_to_base64(buffer1,texto,str_len);
    printf("BASE64: %s\r\n",buffer1);

    base64_to_bin(buffer2,buffer1,strlen(buffer1));
    printf("BINARIO: %s",buffer2);



    return 0;
}