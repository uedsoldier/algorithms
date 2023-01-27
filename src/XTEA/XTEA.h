/**
 * @file XTEA.h
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
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

#pragma region Useful macros
/**
 * @brief Macro para depuración mediante la función printf(). Se recomienda usar únicamente
 * para fines de desarrollo y pruebas.
 */
#ifndef XTEA_LOG
#define XTEA_LOG 0
#endif

/**
 * @brief Macro para utilización de asignación dinámica de memoria en las funciones XTEA.
 * Usar esta característica permite ahorrar memoria RAM, pero no todos los dispositivos ni todos
 * los compiladores soportan tales funcionalidades (malloc(), calloc(), realloc(), etc.)
 */
#ifndef XTEA_DYNAMIC_MEMORY
#define XTEA_DYNAMIC_MEMORY 1
#endif

/**
 * @brief Macro para utilizacion de buffers fijos auxiliares para entrada de datos, de tal forma
 * que el buffer de entrada de datos no se modifica. Para utilizarse, la macro XTEA_DYNAMIC_MEMORY
 * debe estar indefinida o con valor igual a 0.
 * 
 */
#if !defined(XTEA_DYNAMIC_MEMORY) || (XTEA_DYNAMIC_MEMORY == 0)
#ifndef XTEA_USE_BUFFERS
#define XTEA_USE_BUFFERS 1
#endif
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria
 * 
 */
#ifdef XTEA_USE_BUFFERS
#define XTEA_MAX_BUFFER_SIZE 128
#endif

/**
 * @brief Constante δ derivada de la razón áurea. Utilizada en las funciones de cifrado y 
 * descifrado de bloques.
*/
#define XTEA_DELTA 0x9E3779B9UL

/**
 * @brief Macro de tamaño de clave fija en bytes
 * 
 */
#define XTEA_FIXED_KEY_SIZE 16

/**
 * @brief Macro de tamaño de vector de inicialización en bytes
 * 
 */
#define XTEA_INIT_VECTOR_SIZE 8

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
        uint32_t key_dwords[4];
        uint8_t key_bytes[16];
    };
} xtea_key_t;

/**
 * @brief Estructura de datos para definición de parámetros XTEA
 * 
 */
typedef struct XTEA{
    uint32_t dec_sum;
    /**
     * Suma inicial para descifrar
     * 0x1BBCDC80 para 128 iteraciones
     * 0x8DDE6E40 para 64 iteraciones
     * 0xC6EF3720 para 32 iteraciones
     * 0xE3779B90 para 16 iteraciones
     * 0xF1BBCDC8 para 8 iteraciones
     */   
    uint32_t iterations;                    // Número de iteraciones para la ejecución del algoritmo. 32 es bastante, 16 son suficientes, y alrededor de 8 serviría para muchas aplicaciones. 
    xtea_key_t key;                         // Llave del algoritmo, consistente en 128 bits(4 enteros de 32 bits).
    uint8_t iv[XTEA_INIT_VECTOR_SIZE];      // Vector de inicialización para modalidad CBC
} XTEA_t;

/**
 * @brief Estructura de datos para definición de parámetros XXTEA
 * 
 */
typedef struct {
    uint32_t dec_sum;
    /**
     * Suma inicial para descifrar
     * 0x1BBCDC80 para 128 iteraciones
     * 0x8DDE6E40 para 64 iteraciones
     * 0xC6EF3720 para 32 iteraciones
     * 0xE3779B90 para 16 iteraciones
     * 0xF1BBCDC8 para 8 iteraciones
     */   
    xtea_key_t key;         // Llave del algoritmo, consistente en 128 bits(4 enteros de 32 bits).
} XXTEA_t;
#pragma endregion

#pragma region Function prototypes
void XTEA_init(XTEA_t *xtea, uint16_t rounds, xtea_key_t *key, uint8_t *iv);
static void XTEA_set_fixedKey(XTEA_t *xtea);                                            // Función para recorte de clave a 16 bytes
static void XTEA_encrypt_chunk(XTEA_t *xtea, uint32_t *in, uint32_t *out);      // Función de cifrado de trozo de 8 bytes
static void XTEA_decrypt_chunk(XTEA_t *xtea, uint32_t * in,uint32_t * out);     // Función de descrifrado de trozo de 8 bytes
XTEA_code_t XTEA_encrypt(XTEA_t *xtea, void *in, void *out, int32_t input_len, bool ecb, uint32_t *output_len);
XTEA_code_t XTEA_decrypt(XTEA_t *xtea, void *in, void *out, int32_t input_len, bool ecb, uint32_t *output_len);
XTEA_code_t XXTEA_encrypt(XXTEA_t *xxtea, void *in, void *out, int32_t input_len);
XTEA_code_t XXTEA_decrypt(XXTEA_t *xxtea, void *in, void *out, int32_t input_len);
#pragma endregion

#endif /*XTEA_H*/
