/**
 * @file base64.c
 * @brief Refactored Base64 implementation using a struct for better organization
 * @version 0.2
 * @date 2023-01-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "base64.h"

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
} block_t;

typedef struct {
    const uint8_t *src;
    uint8_t *dest;
    size_t size;
} base64_context_t;

/**
 * @brief 
 * Function to obtain the first base64 digit from a block of 4
 * @param a First byte (of 3) from the source data block
 * @return uint8_t Base64 digit
 */
static uint8_t get0( uint8_t a ) {
    uint8_t index = (a >> 2);
    return bin2digit[ index ];
}

/**
 * @brief 
 * Function to obtain the second base64 digit from a block of 4
 * @param a First byte (of 3) from the source data block
 * @param b Second byte (of 3) from the source data block
 * @return uint8_t Base64 digit
 */
static uint8_t get1( uint8_t a, uint8_t b ) {
    uint8_t indexA = ( a & 3 ) << 4;
    uint8_t indexB = b >> 4;
    uint8_t index  = indexA | indexB;
    return bin2digit[ index ];
}

/**
 * @brief 
 * Function to obtain the third base64 digit from a block of 4
 * @param b Second byte (of 3) from the source data block
 * @param c Third byte (of 3) from the source data block
 * @return uint8_t  Base64 digit
 */
static uint8_t get2( uint8_t b, uint8_t c ) {
    uint8_t indexB = ( b & 15 ) << 2;
    uint8_t indexC = c >> 6;
    uint8_t index  = indexB | indexC;
    return bin2digit[ index ];
}

/**
 * @brief 
 * Function to obtain the fourth base64 digit from a block of 4
 * @param c Third byte (of 3) from the source data block
 * @return uint8_t Base64 digit
 */
static uint8_t get3( uint8_t c ) {
    uint8_t index = c & 0x3F;
    return bin2digit[ index ];
}

/** 
 * @brief Function to convert a binary memory block into a null-terminated base64 character string.
 * @param dest (uint8_t*) Pointer to memory where the base64 character string will be stored.
 * @param src (const void *) Pointer to binary memory block to encode.
 * @param size (size_t) Size in bytes of the binary memory block to encode.
 * @return (uint8_t*) Pointer to the null character of the base64 string.
*/
uint8_t *bin_to_base64(uint8_t* dest, const void *src, size_t size){
    base64_context_t ctx = { .src = src, .dest = dest, .size = size };
    block_t *block = (block_t*)ctx.src;
    uint8_t block_size = sizeof(block_t);
    
    for( ; ctx.size >= block_size; ctx.size -= block_size, ++block ) {
        *ctx.dest++ = get0( block->a );
        *ctx.dest++ = get1( block->a, block->b );
        *ctx.dest++ = get2( block->b, block->c );
        *ctx.dest++ = get3( block->c );
    }

    if(ctx.size){
        *ctx.dest++ = get0( block->a );
        if ( !--ctx.size ) {
            *ctx.dest++ = get1( block->a, 0 );
            *ctx.dest++ = '=';
            *ctx.dest++ = '=';
        } else{
            *ctx.dest++ = get1( block->a, block->b );
            *ctx.dest++ = get2( block->b, 0 );
            *ctx.dest++ = '=';
        }
    }
    *ctx.dest = '\0';
    return ctx.dest;
}

/**
 * @brief Function to convert a base64 string to binary data
 * @param dest Pointer to memory where the binary data will be stored
 * @param src Pointer to the base64 string to decode
 * @param size Size of the source string (not used in current implementation)
 * @return Pointer to the end of the decoded data
 */
void *base64_to_bin(uint8_t* dest, const uint8_t *src, size_t size){
    base64_context_t ctx = { .src = src, .dest = dest, .size = size };
    uint8_t const* s = ctx.src;
    uint8_t* p = ctx.dest;
    
    for(;;) {
        int const a = digit2bin[ *s ];
        if ( a == notabase64 ) return p;
        if ( a == terminator ) return p;

        int const b = digit2bin[ *++s ];
        if ( b == notabase64 ) return 0;
        if ( b == terminator ) return 0;

        *p++ = ( a << 2 ) | ( b >> 4 );

        int const c = digit2bin[ *++s ];
        if ( c == notabase64 ) return 0;

        int const d = digit2bin[ *++s ];
        if ( d == notabase64 ) return 0;
        if ( c == terminator ) {
            if ( d != terminator ) return 0;
            return p;
        }

        *p++ = ( b << 4 ) | ( c >> 2 );

        if ( d == terminator ) return p;

        *p++ = ( c << 6 ) | ( d >> 0 );
        ++s;
    }
    return p;
}
