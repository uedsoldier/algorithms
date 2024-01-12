/**
 * @file SHA1.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SHA1_H
#define SHA1_H

#pragma region Dependencies
#include <stdint.h>
#include <stdlib.h>
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Useful macros

/**
 * @brief Macro para depuración mediante la función printf(). Se recomienda usar únicamente
 * para fines de desarrollo y pruebas.
 */
#ifndef SHA1_LOG
#define SHA1_LOG 1
#endif

#if defined(SHA1_LOG) && SHA1_LOG == 1
static const char *SHA1_TAG = "SHA1";
#endif


#pragma region Operation modes

#pragma endregion

#pragma region Memory macros
/**
 * @brief Macro para utilización de asignación dinámica de memoria en las funciones SHA1.
 * Usar esta característica permite ahorrar memoria RAM, pero no todos los dispositivos ni todos
 * los compiladores soportan tales funcionalidades (malloc(), calloc(), realloc(), etc.)
 */
#ifndef SHA1_DYNAMIC_MEMORY
#define SHA1_DYNAMIC_MEMORY 0
#endif

/**
 * @brief Macro para utilizacion de buffers fijos auxiliares para entrada de datos, de tal forma
 * que el buffer de entrada de datos no se modifica. Para utilizarse, la macro SHA1_DYNAMIC_MEMORY
 * debe estar indefinida o con valor igual a 0.
 *
 */
#if !defined(SHA1_DYNAMIC_MEMORY) || (SHA1_DYNAMIC_MEMORY == 0)
#ifndef SHA1_USE_BUFFERS
#define SHA1_USE_BUFFERS 1
#endif
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#ifdef SHA1_USE_BUFFERS
#define SHA1_MAX_BUFFER_SIZE 64
#endif
#pragma endregion

#pragma region General constants
/**
 * @brief 
 * 
 */
#define SHA1_DIGEST_SIZE 20


#pragma endregion



#pragma region Custom types

    /**
     * @brief Enumeración de códigos de error para funciones SHA1
     *
     */
    typedef enum SHA1_errcode
    {
        SHA1_CODE_OK,
        SHA1_CODE_EMPTY_INPUT_BUFFER,
        SHA1_CODE_INCORRECT_BUFFER_SIZE,
        SHA1_CODE_NULL_MALLOC
    } SHA1_errcode_t;

    /**
     * @brief
     *
     */
    typedef struct SHA1_ctx
    {
        

    } SHA1_ctx_t;

#pragma endregion

#pragma region variables

#pragma endregion

#pragma region Function prototypes

    #if defined(SHA1_LOG) && SHA1_LOG == 1
    void print_buffer(uint8_t *buffer, size_t len, const char *msg);
    #endif

    void SHA1_init_ctx(SHA1_ctx_t *ctx);
    static void SHA1_nextBlock(SHA1_ctx_t *ctx, const void *block);
    static void SHA1_lastBlock(SHA1_ctx_t *ctx, const void *block);
    static void SHA1_ctx2hash(SHA1_ctx_t *ctx, const void *block);
    void SHA1_hash(SHA1_ctx_t *ctx, void *in, void *out, size_t len);

#pragma endregion




#pragma region Auxiliary functions


#pragma endregion



#ifdef __cplusplus
}
#endif

#endif /*SHA1_H*/