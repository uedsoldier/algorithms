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
#define XTEA_LOG 1
#endif

/**
 * @brief  Número de interaciones para la ejecución del algoritmo. 32 es bastante, 16 son suficientes, y alrededor de 8 serviría para muchas aplicaciones.
*/
#define XTEA_ITERATIONS 16


/**
 * @brief Constante δ derivada de la razón áurea
*/
#define XTEA_DELTA 0x9E3779B9UL

/**
 * @brief Suma inicial para desencriptado
 * 0x8DDE6E40 para 64
 * 0xC6EF3720 para 32
 * 0xE3779B90 para 16
 * 0xF1BBCDC8 para 8
 * 
*/
static uint32_t XTEA_DEC_SUM = XTEA_DELTA * XTEA_ITERATIONS;

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


/**
 * Prototipos de funciones
*/
static void XTEA_encrypt_chunk(uint32_t *in, uint32_t *out, xtea_key_t *key );  // Función de cifrado de trozo de 8 bytes
static void XTEA_decrypt_chunk(uint32_t * in,uint32_t * out, xtea_key_t *key);  // Función de descrifrado de trozo de 8 bytes

int32_t XTEA_encrypt(void *in, void *out, int32_t size, xtea_key_t *key);
int32_t XTEA_decrypt(void *in, void *out, int32_t size, xtea_key_t *key);


#endif /*XTEA_H*/
