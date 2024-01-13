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
#define SHA1_MAX_BUFFER_SIZE_BYTES 64
#endif
#pragma endregion

#pragma region General constants
/**
 * @brief 
 * 
 */
#define SHA1_DIGEST_SIZE_BYTES 20

/**
 * @brief 
 * 
 */
#define SHA1_DIGEST_SIZE_WORDS (SHA1_DIGEST_SIZE_BYTES/4)

/**
 * @brief 
 * 
 */
#define SHA1_BLOCK_SIZE_BITS 512

/**
 * @brief 
 * 
 */
#define SHA1_BLOCK_SIZE_BYTES (SHA1_BLOCK_SIZE_BITS/8)

/**
 * @brief 
 * 
 */
#define SHA1_MASK   (SHA1_BLOCK_SIZE_BYTES - 1)

#pragma endregion

#pragma region Custom types

/**
 * @brief
 *
 */
typedef struct SHA1_ctx
{
    uint32_t count[2];
    uint32_t hash[SHA1_DIGEST_SIZE_WORDS];
    uint32_t wbuf[16];
} SHA1_ctx_t;

#pragma endregion

#pragma region Function prototypes
    static void sha1_init(SHA1_ctx_t *ctx);
    static void sha1_compile(SHA1_ctx_t *ctx);
    static void sha1_hash(SHA1_ctx_t *ctx, const uint8_t *data, size_t len);
    static void sha1_end(SHA1_ctx_t *ctx, uint8_t *hval);
    void sha1(const void *in, const void *out, size_t len);

#pragma endregion

#pragma region Auxiliary macros

#define rotl32(x,n)   (((x) << n) | ((x) >> (32 - n)))
#define rotr32(x,n)   (((x) >> n) | ((x) << (32 - n)))

#define bswap_32(x) ((rotr32((x), 24) & 0X00FF00FF) | (rotr32((x), 8) & 0XFF00FF00))

#define bsw_32(p,n) \
    { int _i = (n); while(_i--) ((uint32_t*)p)[_i] = bswap_32(((uint32_t*)p)[_i]); }

#define ch(x,y,z)       ((z) ^ ((x) & ((y) ^ (z))))
#define parity(x,y,z)   ((x) ^ (y) ^ (z))
#define maj(x,y,z)      (((x) & (y)) | ((z) & ((x) ^ (y))))

#define q(v,n)  v[n]

#define one_cycle(v,a,b,c,d,e,f,k,h)            \
    q(v,e) += rotr32(q(v,a),27) +               \
        f(q(v,b),q(v,c),q(v,d)) + k + h;  \
    q(v,b)  = rotr32(q(v,b), 2)

#define five_cycle(v,f,k,i)                 \
    one_cycle(v, 0,1,2,3,4, f,k,hf(i  ));   \
    one_cycle(v, 4,0,1,2,3, f,k,hf(i+1));   \
    one_cycle(v, 3,4,0,1,2, f,k,hf(i+2));   \
    one_cycle(v, 2,3,4,0,1, f,k,hf(i+3));   \
    one_cycle(v, 1,2,3,4,0, f,k,hf(i+4))

#pragma endregion

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*SHA1_H*/