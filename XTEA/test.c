#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "XTEA.h"

char cadena[128];
char buffer1[128];
char buffer2[128];


xtea_key_t xtea_test_key;


int main(int argc, char *argv[]){
    if (argc != 3)
    {
        printf("Se deben mandar dos argumentos: Cadena a cifrar y cadena de llave...");
        return EXIT_FAILURE;
    }
    memset(cadena,0,sizeof(cadena));
    memset(buffer1,0,sizeof(buffer1));
    memset(buffer2,0,sizeof(buffer2));
    sprintf(cadena,"%s",argv[1]);
    uint16_t str_len = strlen(argv[1]);
    uint16_t key_len =  strlen(argv[2]);
    memcpy(xtea_test_key.key_bytes, argv[2],key_len);
    
    printf("ASCII in: %s. ASCII input length: %u\r\n",argv[1],str_len);
    printf("XTEA key: %s. Key size: %u\r\n",xtea_test_key.key_bytes, key_len);
    XTEA_encrypt(cadena,buffer1,str_len,&xtea_test_key);
    
    // XTEA_crypt_chunk((uint32_t *)cadena,(uint32_t *)buffer1, &xtea_test_key);
    printf("XTEA: %s\r\n",buffer1);

    XTEA_decrypt(buffer1,buffer2,str_len,&xtea_test_key);
    // XTEA_decrypt_chunk((uint32_t *)buffer1,(uint32_t *)buffer2, &xtea_test_key);
    printf("De vuelta a ASCII: %s",buffer2);
    return 0;
}