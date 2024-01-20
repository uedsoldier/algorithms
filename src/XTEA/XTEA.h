/**
 * @file XTEA.h
 * @author José Roberto Parra Trewartha (you@domain.com)
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @version 0.1
 * @date 2023-01-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef XTEA_H
#define XTEA_H

#pragma region Dependencies
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#pragma endregion

#ifdef __cplusplus
extern "C" {
#endif

#pragma region Useful macros
/**
 * @brief Macro para depuración mediante la función printf(). Se recomienda usar únicamente
 * para fines de desarrollo y pruebas.
 */
#ifndef XTEA_LOG
#define XTEA_LOG 1
#endif

/**
 * @brief Use PKCS7 standard syntax for storing signed and/or encrypted data
 * 
 */
#define XTEA_USE_PKCS7 1

#if defined(XTEA_USE_PKCS7) && XTEA_USE_PKCS7 == 1
#include "../PKCS7/PKCS7.h"      // Modify path to convenience 
#endif

#pragma region Memory macros

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria
 * 
 */
#define XTEA_MAX_BUFFER_SIZE 256

#pragma endregion

#pragma region Consants
/**
 * @brief Constante δ derivada de la razón áurea. Utilizada en las funciones de cifrado y 
 * descifrado de bloques. NO debe modificarse.
*/
#define XTEA_DELTA 0x9E3779B9UL

/**
 * @brief Macro de tamaño de clave fija en bytes. NO debe modificarse.
 * 
 */
#define XTEA_FIXED_KEY_SIZE 16

/**
 * @brief Macro de tamaño de clave fija en words. NO debe modificarse.
 * 
 */
#define XTEA_FIXED_KEY_SIZE_WORDS 4

/**
 * @brief Macro de tamaño de vector de inicialización en bytes. NO debe modificarse.
 * 
 */
#define XTEA_INIT_VECTOR_SIZE 8

/**
 * @brief Macro de tamaño en bytes de bloque XTEA. NO debe modificarse.
 * 
 */
#define XTEA_BLOCK_SIZE 8
#pragma endregion

/**
 * @brief Macro para encontrar el siguiente múltiplo de 8 más cernano a un número. NO debe modificarse.
 * 
 */
#define XTEA_ROUNDUP_TO_NEAREST_MULTIPLE_OF_8(N)    (((N+7)>>3)<<3)

#pragma endregion

#pragma region Custom types
/**
 * @brief Enumeración de códigos de error para funciones XTEA
 * 
 */
typedef enum XTEA_code {
    XTEA_CODE_OK, XTEA_CODE_EMPTY_INPUT_BUFFER, XTEA_CODE_INCORRECT_BUFFER_SIZE, XTEA_CODE_NULL_MALLOC
} XTEA_code_t;

/**
 * @brief Estructura de datos para clave XTEA
 * 
 */
typedef struct xtea_key{
    union {
        uint32_t key_words[XTEA_FIXED_KEY_SIZE_WORDS];
        uint8_t key_bytes[XTEA_FIXED_KEY_SIZE];
    };
} xtea_key_t;

/**
 * @brief Estructura de datos para vector de inicialización
 * 
 */
typedef struct xtea_iv {
    uint8_t iv_array[XTEA_INIT_VECTOR_SIZE];
} xtea_iv_t;

/**
 * @brief Estructura de datos para definición de parámetros XTEA
 * 
 */
typedef struct XTEA{
    /**
     * Suma inicial para descifrar
     * 0x1BBCDC80 para 128 iteraciones
     * 0x8DDE6E40 para 64 iteraciones
     * 0xC6EF3720 para 32 iteraciones
     * 0xE3779B90 para 16 iteraciones
     * 0xF1BBCDC8 para 8 iteraciones
     */
    uint32_t dec_sum;   
    uint8_t iterations;                    // Número de iteraciones para la ejecución del algoritmo. 32 es bastante, 16 son suficientes, y alrededor de 8 serviría para muchas aplicaciones. 
    xtea_key_t key;                         // Llave del algoritmo, consistente en 128 bits(4 enteros de 32 bits).
    xtea_iv_t iv[XTEA_INIT_VECTOR_SIZE];      // Vector de inicialización para modalidad CBC
    size_t encrypted_chunks;
    size_t decrypted_chunks;
    size_t input_len_normalized;
} XTEA_t;

#pragma endregion

#pragma region Function prototypes
void XTEA_init(XTEA_t *xtea, uint16_t rounds, const xtea_key_t *key, const xtea_iv_t *iv);

static void XTEA_set_fixedKey(XTEA_t *xtea);                                            // Función para recorte de clave a 16 bytes

static void XTEA_encrypt_chunk(XTEA_t *xtea, uint32_t *in, uint32_t *out);      // Función de cifrado de trozo de 8 bytes
static void XTEA_decrypt_chunk(XTEA_t *xtea, uint32_t * in,uint32_t * out);     // Función de descrifrado de trozo de 8 bytes

XTEA_code_t XTEA_encrypt(XTEA_t *xtea, void *in, void *out, size_t input_len, bool ecb, uint32_t *output_len);
XTEA_code_t XTEA_decrypt(XTEA_t *xtea, void *in, void *out, size_t input_len, bool ecb, uint32_t *output_len);

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*XTEA_H*/
