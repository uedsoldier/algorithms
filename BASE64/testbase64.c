#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "base64.h"

/**
 * @brief Función para obtener el primer dígito (base 64) de un bloque de 4
 * @param a (uint8_t) Primer byte (de tres) del bloque de datos fuente
 *@return (uint8_t) Dígito en base 64
*/
static uint8_t get0( uint8_t a ) {
    uint8_t index = (a >> 2);
    return bin2digit[ index ];
}

/** 
 * @brief Función para obtener el segundo dígito (base 64) de un bloque de 4
 * @param a (uint8_t) Primer byte (de tres) del bloque de datos fuente
 * @param b (uint8_t) Segundo byte (de tres) del bloque de datos fuente
 * @return (uint8_t) Dígito en base 64
*/
static uint8_t get1( uint8_t a, uint8_t b ) {
    uint8_t indexA = ( a & 3 ) << 4;
    uint8_t indexB = b >> 4;
    uint8_t index  = indexA | indexB;
    return bin2digit[ index ];
}

/**
 * @brief Función para obtener el tercer dígito (base 64) de un bloque de 4
 * @param b (uint8_t) Segundo byte (de tres) del bloque de datos fuente
 * @param c (uint8_t) Tercer byte (de tres) del bloque de datos fuente
 * @return (uint8_t) Dígito en base 64
*/
static uint8_t get2( uint8_t b, uint8_t c ) {
    uint8_t indexB = ( b & 15 ) << 2;
    uint8_t indexC = c >> 6;
    uint8_t index  = indexB | indexC;
    return bin2digit[ index ];
}

/** 
 * @brief Función para obtener el cuarto dígito (base 64) de un bloque de 4
 * @param c (uint8_t) Tercer byte (de tres) del bloque de datos fuente
 * @return (uint8_t) Dígito en base 64
*/
static uint8_t get3( uint8_t c ) {
    uint8_t index = c & 0x3F;
    return bin2digit[ index ];
}

/** 
 * @brief Función para convertir un bloque binario de memoria en una cadena de caracteres base64 terminada en 0 (caracter nulo).
 * @param dest (char*) Apuntador de memoria donde se almacenará la cadena de caracteres base64.
 * @param src (const void *) Apuntador a bloque binario de memoria a codificar.
 * @param size (size_t) Tamaño en bytes del bloque binario de memoria a codificar
 * @return (char*) Apuntador  A pointer to the null character of the base64 null-terminated string. PENDIENTE POR EVALUAR!!!!!!///////////////////////////////////////////////////////////
*/
char *bin_to_base64(char* dest, const void *src, size_t size){
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

//
void *base64_to_bin(char* dest, const char *src, size_t size){
	uint8_t const* s = (uint8_t*)src;
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


const char texto[] = "CuW6BW8bJ7PX";

char cadena[32];
char buffer1[64];
char buffer2[64];


int main(){
    memset(cadena,0,sizeof(cadena));
    sprintf(cadena,"%s",texto);
    memset(buffer1,0,sizeof(buffer1));
    uint16_t str_len = sizeof(texto);
    printf("Tamano cadena entrada: %u\r\n",str_len);
    printf("Entrada: %s\r\n",texto);
    bin_to_base64(buffer1,texto,str_len);
    printf("BASE64: %s\r\n",buffer1);

    base64_to_bin(buffer2,buffer1,strlen(buffer1));
    printf("BINARIO: %s",buffer2);



    return 0;
}