/**
 * @file XTEA.c
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
*/

#include <stdbool.h>
#include <stdint.h>
#include "funciones_auxiliares.h"
#include "XTEA.h"

/**
 * @brief Cifra 8 bytes utilizando una clave de 16 bytes (128 bits).
 * @param in (uint32_t *) Arreglo de entrada (a crifrar), consistente en 64 bits (2 enteros de 32 bits)
 * @param out (uint32_t *) Arreglo de salida (cifrado), consistente en 64 bits (2 enteros de 32 bits).
 * @param key (uint32_t *) Llave del algoritmo, consistente en 128 bits(4 entero de 32 bits). Debe coincidir con la clave de descifrado.
 * @return (void) 
*/
void XTEA_encipher(uint32_t *in, uint32_t *out, uint32_t *key )
{
	uint32_t v0, v1, x, y, sum = 0;
	int8_t n = XTEA_ITERATIONS;
	uint8_t  i;

	v0 = in[0];  v1 = in[1];

	while(n != 0)
	{ 
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = make8(sum, 0);
		i &= 0x03;
		y = sum + key[i];
		x ^= y;
		v0 += x;

		sum += XTEA_DELTA;

		x = v0 << 4;
		y = v0 >> 5;
		x ^= y;
		x += v0;
		i = make8(sum, 1);
		i >>= 3;
		i &= 0x03;
		y = sum + key[i];
		x ^= y;
		v1 += x;

		n--;
	}
	out[0] = v0;
	out[1] = v1;
}

/**
 * @brief Descifra 8 bytes utilizando una clave de 16 bytes (128 bits).
 * @param in (uint32_t *) Arreglo de entrada (a descrifrar), consistente en 64 bits (2 enteros de 32 bits)
 * @param out (uint32_t *) Arreglo de salida (descifrado), consistente en 64 bits (2 enteros de 32 bits)
 * @param key (uint32_t *) Llave del algoritmo, consistente en 128 bits(4 entero de 32 bits). Debe coincidir con la clave de cifrado.
 * @return (void) 
*/
void XTEA_decipher(uint32_t * in,uint32_t * out,uint32_t * key)
{
	uint32_t  v0, v1, x, y, sum = XTEA_DEC_SUM;
	int8_t n = XTEA_ITERATIONS;
	uint8_t   i;

	v0 = in[0];  v1 = in[1];

	while(n > 0) 
	{
		x = v0 << 4;
		y = v0 >> 5;
		x ^= y;
		x += v0;
		i = (uint8_t)((sum&0xFF00)>>8);//make8(sum, 1);
		i >>= 3;
		i &= 0x03;
		y = sum + key[i];
		x ^= y;
		v1 -= x;

		sum -= XTEA_DELTA;

		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum&0xFF);//make8(sum, 0);
		i &= 0x03;
		y = sum + key[i];
		x ^= y;
		v0 -= x;

		n--;
	}
	out[0] = v0;
	out[1] = v1;
}


