/**
 * @file XTEA.h
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#ifndef XTEA_H
#define XTEA_H

// Dependencias
#include <stdbool.h>
#include <stdint.h>
#include "../../utilities/utils.h"

/**
 * @brief 
 * 
 */
#ifndef XTEA_LOG
#define XTEA_LOG 0
#endif

/**
 * @brief Constante δ derivada de la razón áurea
*/
#define XTEA_DELTA 0x9E3779B9UL

/**
 * @brief 
 * 
 */
typedef struct xtea_key{
    union {
        uint32_t key_dwords[4];
        uint8_t key_bytes[16];
    };
} xtea_key_t;

typedef struct XTEA{
    uint32_t dec_sum;
    /**
     * Suma inicial para desencriptado
     * 0x1BBCDC80 para 128 iteraciones
     * 0x8DDE6E40 para 64 iteraciones
     * 0xC6EF3720 para 32 iteraciones
     * 0xE3779B90 para 16 iteraciones
     * 0xF1BBCDC8 para 8 iteraciones
     */   
    uint32_t iterations;    // Número de iteraciones para la ejecución del algoritmo. 32 es bastante, 16 son suficientes, y alrededor de 8 serviría para muchas aplicaciones. 
    xtea_key_t key;         // Llave del algoritmo, consistente en 128 bits(4 enteros de 32 bits).
    uint8_t iv[8];          // Vector de inicialización para modalidad CBC
} XTEA_t;

typedef enum {
    XTEA_8, XTEA_16, XTEA_32, XTEA_64
} XTEA_rounds_t;

/**
 * Prototipos de funciones
*/
int8_t XTEA_init(XTEA_t *xtea, XTEA_rounds_t rounds, xtea_key_t *key, uint8_t *iv);
static void XTEA_encrypt_chunk(XTEA_t *xtea, uint32_t *in, uint32_t *out);  // Función de cifrado de trozo de 8 bytes
static void XTEA_decrypt_chunk(XTEA_t *xtea, uint32_t * in,uint32_t * out);  // Función de descrifrado de trozo de 8 bytes

int32_t XTEA_encrypt(XTEA_t *xtea, void *in, void *out, int32_t size, bool ecb);
int32_t XTEA_decrypt(XTEA_t *xtea, void *in, void *out, int32_t size, bool ecb);


#endif /*XTEA_H*/
