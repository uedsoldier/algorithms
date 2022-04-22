/**
 * @file XTEA.c
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "XTEA.h"

/**
 * @brief Cifra 8 bytes utilizando una clave de 16 bytes (128 bits).
 * @param in (uint32_t *) Arreglo de entrada (a crifrar), consistente en 64 bits (2 enteros de 32 bits)
 * @param out (uint32_t *) Arreglo de salida (cifrado), consistente en 64 bits (2 enteros de 32 bits).
 * @param key (uint32_t *) Llave del algoritmo, consistente en 128 bits(4 entero de 32 bits). Debe coincidir con la clave de descifrado.
 * @return (void) 
*/
static void XTEA_encrypt_chunk(uint32_t *in, uint32_t *out, xtea_key_t *key ) {
	#ifdef XTEA_LOG
	printf("Input chunk: ");
	for(uint32_t i=0 ; i!=2; i++){
		printf("%08lx ",in[i]);
	}	
	printf("\n");
	#endif
	uint32_t v0, v1, x, y, sum = 0;
	int8_t n = XTEA_ITERATIONS;
	uint8_t  i;
	v0 = in[0];  v1 = in[1];
	while(n != 0) { 
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + key->key_dwords[i];
		x ^= y;
		v0 += x;
		sum += XTEA_DELTA;
		x = v0 << 4;
		y = v0 >> 5;
		x ^= y;
		x += v0;
		i = (uint8_t)(( sum & 0xFF00 )>>8);
		i >>= 3;
		i &= 0x03;
		y = sum + key->key_dwords[i];
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
static void XTEA_decrypt_chunk(uint32_t * in,uint32_t * out, xtea_key_t * key) {
	uint32_t  v0, v1, x, y, sum = XTEA_DEC_SUM;
	int8_t n = XTEA_ITERATIONS;
	uint8_t i;
	v0 = in[0];  v1 = in[1];
	while(n > 0) {
		x = v0 << 4;
		y = v0 >> 5;
		x ^= y;
		x += v0;
		i = (uint8_t)((sum&0xFF00)>>8);
		i >>= 3;
		i &= 0x03;
		y = sum + key->key_dwords[i];
		x ^= y;
		v1 -= x;
		sum -= XTEA_DELTA;
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + key->key_dwords[i];
		x ^= y;
		v0 -= x;
		n--;
	}
	out[0] = v0;
	out[1] = v1;
}

/**
 * @brief 
 * 
 * @param in 
 * @param out 
 * @param size 
 * @param xtea_key 
 * @return size_t 
 */
int32_t XTEA_encrypt(void *in, void *out, int32_t size, xtea_key_t *key){
	int32_t encrypted_chunks = 0;
	uint8_t *_in = in;
	uint8_t *_out = out; 
	#ifdef XTEA_LOG
	printf("XTEA encrypt\nByte number: %u\n",size);
	printf("Input buffer: ");
	for(uint32_t i=0 ; i!=size; i++){
		printf("%02x",_in[i]);
	}
	printf("\n");
	#endif
	
	do {
		size -= 8;
		encrypted_chunks++;
		XTEA_encrypt_chunk((uint32_t *)_in, (uint32_t *)_out, key);
		_in += 8; _out += 8;
	} while(size > 0);

	//
	#ifdef XTEA_LOG
	printf("Encrypted chunks: %u\n",encrypted_chunks);
	#endif
	return encrypted_chunks;
}

int32_t XTEA_decrypt(void *in, void *out, int32_t size, xtea_key_t *key){
	int32_t decrypted_chunks = 0;
	uint8_t *_in = in;
	uint8_t *_out = out; 
	#ifdef XTEA_LOG
	printf("XTEA decrypt\nByte number: %u\n",size);
	printf("Input buffer: ");
	for(uint32_t i=0 ; i!=size; i++){
		printf("%02x",_in[i]);
	}
	printf("\n");
	#endif
	
	do {
		size -= 8;
		decrypted_chunks++;
		XTEA_decrypt_chunk((uint32_t *)_in, (uint32_t *)_out, key);
		_in += 8; _out += 8;
	} while(size > 0);

	//
	#ifdef XTEA_LOG
	printf("Decrypted chunks: %u\n",decrypted_chunks);
	#endif
	return decrypted_chunks;
}