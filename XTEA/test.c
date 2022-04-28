#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "XTEA.h"

char cadena[128];
char buffer1[128];
char buffer2[128];
// uint8_t iv[] = {0x22,0xBB,0xCC,0xDD,0xEE,0xFF,0x55,0x33};
uint8_t iv[] = {'U','x','G','Z','V','F','r','T'};


xtea_key_t xtea_test_key;
XTEA_t xtea_test;


int main(int argc, char *argv[]){
    if (argc != 5)
    {
        printf("Se deben mandar 4 argumentos: Cadena a cifrar, cadena de llave, bandera cbc y numero de rondas XTEA...");
        return EXIT_FAILURE;
    }
    
    memset(cadena,0,sizeof(cadena));
    memset(buffer1,0,sizeof(buffer1));
    memset(buffer2,0,sizeof(buffer2));
    memset(&xtea_test_key,0,sizeof(xtea_key_t));
    sprintf(cadena,"%s",argv[1]);
    uint16_t str_len = strlen(argv[1]);
    uint16_t key_len =  strlen(argv[2]);
    bool ecb = (strcmp(argv[3],"1"))==0? true:false;
    uint32_t rounds = strtol(argv[4],NULL,10);
    memcpy(&xtea_test_key, argv[2], key_len);
    XTEA_init( &xtea_test, rounds, &xtea_test_key,iv);

    printf("ASCII in: %s\nASCII input length: %u\n",argv[1],str_len);
    printf("ECB mode: %s\n",ecb? "true":"false");
    printf("Rounds: %lu\n",rounds);


    uint32_t enc_chunks = XTEA_encrypt(&xtea_test, cadena,buffer1,str_len,ecb);
    
    printf("XTEA output: ");
    for (size_t i = 0; i != str_len; i++){
        printf("%02x ",(uint8_t)buffer1[i]);
    }
    printf("\n");
    
    XTEA_decrypt(&xtea_test, buffer1,buffer2,str_len,ecb);
    printf("De vuelta a ASCII: %s",buffer2);
    return 0;
}