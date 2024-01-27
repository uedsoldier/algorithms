/**
 * @file SHA256.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SHA256_H
#define SHA256_H

#pragma region Dependencies
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Useful macros
#pragma endregion

#pragma region Memory macros

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#define SHA256_MAX_BUFFER_SIZE_BYTES 64
#pragma endregion

#pragma region General constants
/**
 * @brief 
 * 
 */
#define SHA256_DIGEST_SIZE_BYTES 32

/**
 * @brief 
 * 
 */
#define SHA256_DIGEST_SIZE_WORDS (SHA256_DIGEST_SIZE_BYTES/4)

/**
 * @brief 
 * 
 */
#define SHA256_BLOCK_SIZE_BITS 512

/**
 * @brief 
 * 
 */
#define SHA256_BLOCK_SIZE_BYTES (SHA256_BLOCK_SIZE_BITS/8)

/**
 * @brief 
 * 
 */
#define SHA256_MASK   (SHA256_BLOCK_SIZE_BYTES - 1)

/**
 * @brief 
 * 
 */
static const uint32_t k[64] = {
	0x428a2f98UL,0x71374491UL,0xb5c0fbcfUL,0xe9b5dba5UL,0x3956c25bUL,0x59f111f1UL,0x923f82a4UL,0xab1c5ed5UL,
	0xd807aa98UL,0x12835b01UL,0x243185beUL,0x550c7dc3UL,0x72be5d74UL,0x80deb1feUL,0x9bdc06a7UL,0xc19bf174UL,
	0xe49b69c1UL,0xefbe4786UL,0x0fc19dc6UL,0x240ca1ccUL,0x2de92c6fUL,0x4a7484aaUL,0x5cb0a9dcUL,0x76f988daUL,
	0x983e5152UL,0xa831c66dUL,0xb00327c8UL,0xbf597fc7UL,0xc6e00bf3UL,0xd5a79147UL,0x06ca6351UL,0x14292967UL,
	0x27b70a85UL,0x2e1b2138UL,0x4d2c6dfcUL,0x53380d13UL,0x650a7354UL,0x766a0abbUL,0x81c2c92eUL,0x92722c85UL,
	0xa2bfe8a1UL,0xa81a664bUL,0xc24b8b70UL,0xc76c51a3UL,0xd192e819UL,0xd6990624UL,0xf40e3585UL,0x106aa070UL,
	0x19a4c116UL,0x1e376c08UL,0x2748774cUL,0x34b0bcb5UL,0x391c0cb3UL,0x4ed8aa4aUL,0x5b9cca4fUL,0x682e6ff3UL,
	0x748f82eeUL,0x78a5636fUL,0x84c87814UL,0x8cc70208UL,0x90befffaUL,0xa4506cebUL,0xbef9a3f7UL,0xc67178f2UL
};


#pragma endregion

#pragma region Custom types

/**
 * @brief
 *
 */
typedef struct SHA256_ctx
{

    uint8_t data[64];
    size_t dataLen;
    uint64_t bitLen;
    uint32_t state[8];
} SHA256_ctx_t;

#pragma endregion

#pragma region Function prototypes
    static void SHA256_init(SHA256_ctx_t *ctx);
    static void SHA256_transform(SHA256_ctx_t *ctx, const uint8_t *data);
    static void SHA256_hash(SHA256_ctx_t *ctx, const uint8_t *data, size_t len);
    static void SHA256_end(SHA256_ctx_t *ctx, uint8_t *hash);
    void SHA256(const void *in, const void *out, size_t len);

#pragma endregion

#pragma region Auxiliary macros

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

#pragma endregion





#ifdef __cplusplus
}
#endif

#endif /*SHA256_H*/