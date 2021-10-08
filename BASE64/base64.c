/**
 * @file base64.c
 * @brief Librería de implementación de codificación base 64
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "base64.h"

/**
 * @brief Función para obtener el primer dígito (base 64) de un bloque de 4
 * @param a (char) Primer byte (de tres) del bloque de datos fuente
 *@return (char) Dígito en base 64
*/
static char get0( char a ) {
    char index = (a >> 2);
    return bin2digit[ index ];
}

/** 
 * @brief Función para obtener el segundo dígito (base 64) de un bloque de 4
 * @param a (char) Primer byte (de tres) del bloque de datos fuente
 * @param b (char) Segundo byte (de tres) del bloque de datos fuente
 * @return (char) Dígito en base 64
*/
static char get1( char a, char b ) {
    char indexA = ( a & 3 ) << 4;
    char indexB = b >> 4;
    char index  = indexA | indexB;
    return bin2digit[ index ];
}

/**
 * @brief Función para obtener el tercer dígito (base 64) de un bloque de 4
 * @param b (char) Segundo byte (de tres) del bloque de datos fuente
 * @param c (char) Tercer byte (de tres) del bloque de datos fuente
 * @return (char) Dígito en base 64
*/
static char get2( char b, char c ) {
    char indexB = ( b & 15 ) << 2;
    char indexC = c >> 6;
    char index  = indexB | indexC;
    return bin2digit[ index ];
}

/** 
 * @brief Función para obtener el cuarto dígito (base 64) de un bloque de 4
 * @param c (char) Tercer byte (de tres) del bloque de datos fuente
 * @return (char) Dígito en base 64
*/
static char get3( char c ) {
    char index = c & 0x3F;
    return bin2digit[ index ];
}

/** 
 * @brief Función para convertir un bloque binario de memoria en una cadena de caracteres base64 terminada en 0 (caracter nulo).
 * @param dest (char*) Apuntador de memoria donde se almacenará la cadena de caracteres base64.
 * @param src (const void *) Apuntador a bloque binario de memoria a codificar.
 * @param size (size_t) Tamaño en bytes del bloque binario de memoria a codificar.
 * @return (char*) Apuntador al caracter nulo de la cadena base64.
*/
char *bin_to_base64(char* dest, const void *src, size_t size){
	typedef struct {
        char a; char b; char c;
    } block_t;
    char block_size = sizeof( block_t );
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
void *base64_to_bin(char* dest, const char *src, size_t size){
	char const* s = (char*)src;
    char* p = dest;
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