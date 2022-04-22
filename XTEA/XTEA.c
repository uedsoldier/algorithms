/**
 * @file XTEA.c
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "XTEA.h"

/**
 * @brief 
 * 
 * @param xtea 
 * @param rounds 
 * @param key 
 * @param iv 
 * @return int8_t 
 */
int8_t XTEA_init(XTEA_t *xtea, XTEA_rounds_t rounds, xtea_key_t *key, uint8_t *iv){
	#if XTEA_LOG == 1
	printf("XTEA init\n");
	#endif
	switch (rounds) {
		case XTEA_8:
			xtea->iterations = 8;
			break;
		case XTEA_16:
			xtea->iterations = 16;
			break;
		case XTEA_32:
			xtea->iterations = 32;
			break;
		case XTEA_64:
			xtea->iterations = 64;
			break;
		
		default:
			#if XTEA_LOG == 1
			printf("XTEA rounds must be 8, 16, 32 or 64\n");
			#endif
			return -1;
			break;
	}
	
	xtea->dec_sum = XTEA_DELTA * (xtea->iterations);
	memset( &xtea->key, 0, sizeof( xtea_key_t));
	memcpy( &xtea->key, key, sizeof( xtea_key_t));
	memcpy( &xtea->iv, iv, sizeof(xtea->iv));
	#if XTEA_LOG == 1
	uint8_t i;
	printf("\t* Iterations: %u\n\t* Dec sum: 0x%08lX\n\t* Key (hex bytes): [",xtea->iterations,xtea->dec_sum);
	for (i = 0; i != sizeof(xtea_key_t); i++){
		printf("%02x ",xtea->key.key_bytes[i]);
	}
	printf("]\n\t* Initializating vector (hex bytes): [");
	for (i = 0; i != sizeof(xtea->iv); i++){
		printf("%02x ",xtea->iv[i]);
	}
	printf("]\n");
	#endif
	return 0;
}

/**
 * @brief Cifra trozos de 8 bytes utilizando una clave de 16 bytes (128 bits).
 * @param xtea (XTEA_t *) Apuntador a estructura de datos XTEA
 * @param in (uint32_t *) Arreglo de entrada (a crifrar), consistente en 64 bits (2 enteros de 32 bits)
 * @param out (uint32_t *) Arreglo de salida (cifrado), consistente en 64 bits (2 enteros de 32 bits).
 */
static void XTEA_encrypt_chunk(XTEA_t *xtea, uint32_t *in, uint32_t *out)  {
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out;
	#if XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=8; index++){
		uint8_t c = _in[index];
		printf("%02x ",c);
	}	
	printf("\t");
	#endif
	uint32_t v0, v1, x, y, sum = 0;
	int8_t n = xtea->iterations;
	uint8_t i;
	v0 = in[0];  v1 = in[1];
	while(n != 0) { 
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + xtea->key.key_dwords[i];
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
		y = sum + xtea->key.key_dwords[i];
		x ^= y;
		v1 += x;
		n--;
	}
	out[0] = v0;
	out[1] = v1;
	#if XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=8; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
}

/**
 * @brief Descifra trozos de 8 bytes utilizando una clave de 16 bytes (128 bits).
 * @param xtea (XTEA_t *) Apuntador a estructura de datos XTEA
 * @param in (uint32_t *) Arreglo de entrada (a descrifrar), consistente en 64 bits (2 enteros de 32 bits)
 * @param out (uint32_t *) Arreglo de salida (descifrado), consistente en 64 bits (2 enteros de 32 bits).
 */
static void XTEA_decrypt_chunk(XTEA_t *xtea, uint32_t * in,uint32_t * out) {
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out;
	#if XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=8; index++){
		uint8_t c = _in[index];
		printf("%02x ",c);
	}	
	printf("\t");
	#endif
	uint32_t  v0, v1, x, y, sum = xtea->dec_sum;
	int8_t n = xtea->iterations;
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
		y = sum + xtea->key.key_dwords[i];
		x ^= y;
		v1 -= x;
		sum -= XTEA_DELTA;
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + xtea->key.key_dwords[i];
		x ^= y;
		v0 -= x;
		n--;
	}
	out[0] = v0;
	out[1] = v1;
	#if XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=8; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
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
int32_t XTEA_encrypt(XTEA_t *xtea, void *in, void *out, int32_t size, bool ecb){
	int32_t encrypted_chunks = 0;
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp_iv[8];
	memcpy(temp_iv, xtea->iv, sizeof(temp_iv));
	#if XTEA_LOG == 1
	uint32_t i;
	printf("XTEA encrypt - input buffer:\t");
	for( i=0 ; i!=size; i++){
		uint8_t c = _in[i];
		printf("%02x ",c);
	}
	printf("\n");

	printf("XTEA encrypt - output buffer:\t");
	for( i=0 ; i!=size; i++){
		uint8_t c = _out[i];
		printf("%02x ",c);
	}
	printf("\n");
	#endif
	
	while(size > 0) {
		#if XTEA_LOG == 1
		printf("Chunk %u (%s mode): ",encrypted_chunks,ecb? "ECB":"CBC");
		#endif
		if(!ecb){
			#if XTEA_LOG == 1
			printf("\nIN  = IN ^ IV\n");
			#endif


			for(uint8_t j=0; j != sizeof(temp_iv); j++){
				uint8_t prev = _in[j];
				_in[j] = (uint8_t)(prev ^ temp_iv[j]);
				#if XTEA_LOG == 1
				printf("%02x =  %02x ^ %02x\n", _in[j], prev, temp_iv[j]);
				#endif
			} 

			XTEA_encrypt_chunk(xtea,(uint32_t *)_in, (uint32_t *)_out);
			memcpy(temp_iv, _out, sizeof(temp_iv));
			#if XTEA_LOG == 1
			printf(" + New IV: ");
			for(uint8_t j=0; j != sizeof(temp_iv); j++){
				printf("%02x ",temp_iv[j]);
			}
			printf("\n");
			#endif

		} else{
			XTEA_encrypt_chunk(xtea,(uint32_t *)_in, (uint32_t *)_out);
		}
		_in += 8; _out += 8; size -= 8; encrypted_chunks++;
		
	}
	#if XTEA_LOG == 1
	printf("Encrypted chunks: %u\n",encrypted_chunks);
	#endif
	return encrypted_chunks;
}

/**
 * @brief 
 * 
 * @param xtea 
 * @param in 
 * @param out 
 * @param size 
 * @return int32_t 
 */
int32_t XTEA_decrypt(XTEA_t *xtea, void *in, void *out, int32_t size, bool ecb){
	int32_t decrypted_chunks = 0;
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp[8];
	uint8_t temp_iv[8];
	memcpy(temp_iv, xtea->iv, sizeof(temp_iv));
	#if XTEA_LOG == 1
	printf("XTEA decrypt - input buffer: ");
	for(uint32_t i=0 ; i!=size; i++){
		printf("%02x ",_in[i]);
	}
	printf("\n");
	#endif
	
	while(size > 0) {
		memcpy(temp, _in, sizeof(temp));

		#if XTEA_LOG == 1
		printf("Chunk %u (%s mode): ",decrypted_chunks,ecb? "ECB":"CBC");
		#endif
		
		XTEA_decrypt_chunk(xtea, (uint32_t *)_in, (uint32_t *)_out);
		if(!ecb){
			#if XTEA_LOG == 1
			printf("\nOUT = OUT ^ IV\n");
			#endif
			for(uint8_t j=0; j != sizeof(temp); j++){
				uint8_t prev = _out[j];
				_out[j] = (uint8_t)( prev ^ temp_iv[j]);
				#if XTEA_LOG == 1
				printf("%02x =  %02x  ^ %02x\n", _out[j], prev, temp_iv[j]);
				#endif
			}
			#if XTEA_LOG == 1
			printf("\n");
			#endif
			memcpy(temp_iv, temp, sizeof(temp));	
		}
		_in += 8; _out += 8; size -= 8; decrypted_chunks++;
	}

	//
	#if XTEA_LOG == 1
	printf("Decrypted chunks: %u\n",decrypted_chunks);
	#endif
	return decrypted_chunks;
}