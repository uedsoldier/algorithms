/**
 * @file base64.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "base64.h"

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
 * @brief Función para convertir un bloque binario de memoria en una cadena de caracteres base64 terminada en 0 (caracter nulo).
 * @param dest (uint8_t*) Apuntador de memoria donde se almacenará la cadena de caracteres base64.
 * @param src (const void *) Apuntador a bloque binario de memoria a codificar.
 * @param size (size_t) Tamaño en bytes del bloque binario de memoria a codificar.
 * @return (uint8_t*) Apuntador al caracter nulo de la cadena base64.
*/
uint8_t *bin_to_base64(uint8_t* dest, const void *src, size_t size){
	typedef struct {
        uint8_t a; uint8_t b; uint8_t c;
    } block_t;
    uint8_t block_size = sizeof( block_t );
    block_t *block = (block_t*)src;
	for( ; size >= block_size; size -= block_size, ++block ) {
        *dest++ = get0( block->a );
        *dest++ = get1( block->a, block->b );
        *dest++ = get2( block->b, block->c );
        *dest++ = get3( block->c );
    }

	if(size){
		*dest++ = get0( block->a );
		if ( !--size ) {
			*dest++ = get1( block->a, 0 );
			*dest++ = '=';
			*dest++ = '=';
		} else{
			*dest++ = get1( block->a, block->b );
			*dest++ = get2( block->b, 0 );
			*dest++ = '=';
		}
	} 						// final:
	*dest = '\0';
	return dest;
}

/**
 * 
 * @param dest
 * @param src
 * @param size
 * @return 
 */
void *base64_to_bin(uint8_t* dest, const uint8_t *src, size_t size){
	uint8_t const* s = (uint8_t*)src;
    uint8_t* p = dest;
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