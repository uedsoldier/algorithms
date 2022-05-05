#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "XTEA.h"

uint8_t *iv = "init vector";


xtea_key_t xtea_test_key;
XTEA_t xtea_test;



int main(int argc, char *argv[]){
    if (argc != 5) {
        printf("Se deben mandar 4 argumentos: Cadena a cifrar, cadena de llave, bandera cbc y numero de rondas XTEA...");
        return EXIT_FAILURE;
    }
    float time_spent = 0.0F;
    clock_t begin = clock();
    uint16_t input_len = strlen(argv[1]);
    uint16_t input_len_normalized = ((input_len + 7) & (-8));
    uint16_t key_len =  strlen(argv[2]);
    uint32_t output_len;
    char *cadena = (char *) malloc(input_len_normalized);
    char *buffer1 = (char *) malloc(input_len_normalized);
    char *buffer2 = (char *) malloc(input_len_normalized);

    if( cadena == NULL || buffer1 == NULL || buffer2 == NULL ){
        printf("NULL pointer\n");
        return EXIT_FAILURE;
    }

    memset(cadena,0,sizeof(input_len_normalized));
    memset(buffer1,0,sizeof(input_len_normalized));
    memset(buffer2,0,sizeof(input_len_normalized));
    sprintf(cadena,"%s",argv[1]);
    
    bool ecb = (strcmp(argv[3],"1"))==0? true:false;
    uint32_t rounds = strtol(argv[4],NULL,10);
    memcpy(&xtea_test_key, argv[2], key_len);
    XTEA_init( &xtea_test, rounds, &xtea_test_key,iv);

    printf("ASCII in (%u bytes):\n%s\n",input_len,cadena);
    printf("Normalized input (bytes): %u\n",input_len_normalized);


    XTEA_encrypt(&xtea_test, cadena,buffer1,input_len,ecb,&output_len);
    
    printf("XTEA output (block size %u bytes): [ ",output_len);
    for (size_t i = 0; i != output_len; i++){
        printf("%02x ",(uint8_t)buffer1[i]);
    }
    printf("]\n");
    
    XTEA_decrypt(&xtea_test, buffer1, buffer2, input_len, ecb, &output_len);
    printf("De vuelta a ASCII (%u bytes): %s\n",strlen(buffer2),buffer2);

    uint8_t retVal;
    #if defined(XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
    uint8_t comparison = memcmp(cadena,buffer2,input_len);
    #elif defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
    uint8_t comparison = memcmp(cadena,buffer2,input_len);
    #elif defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 0)
    uint8_t comparison = memcmp(argv[1],buffer2,input_len);
    #endif

    
    if(comparison == 0) {
        printf("XTEA OK\n");
        retVal = EXIT_SUCCESS;
    } else{
        printf("XTEA not OK\n");
        retVal = EXIT_FAILURE;
    }

    clock_t end = clock();

    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time_spent = (float)(end - begin);
    printf("Elapsed time: %0.3f [ms]", time_spent);
    free(cadena);
    free(buffer1);
    free(buffer2);

    return retVal;

}