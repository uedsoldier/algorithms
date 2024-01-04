/**
 * @file XTEA.c
 * @author José Roberto Parra Trewartha (you@domain.com)
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * Se incluye también el algoritmo XXTEA (Corrected Block TEA)
 * @version 0.1
 * @date 2023-01-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "XTEA.h"

/**
 * @brief 
 * 
 * @param xtea 
 * @param rounds 
 * @param key 
 * @param iv 
 */
void XTEA_init(XTEA_t *xtea, uint16_t rounds, xtea_key_t *key, uint8_t *iv){
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XTEA init\n");
	#endif
	switch (rounds) {
		case 8:
			xtea->iterations = 8;
			break;
		case 16:
			xtea->iterations = 16;
			break;
		case 32:
			xtea->iterations = 32;
			break;
		case 64:
			xtea->iterations = 64;
			break;
		
		default:
			#if defined(XTEA_LOG) && XTEA_LOG == 1
			printf("XTEA rounds must be 8, 16, 32 or 64. Using default value (64).\n");
			#endif
			xtea->iterations = 64;
			break;
	}
	
	xtea->dec_sum = XTEA_DELTA * (xtea->iterations);
	memcpy( xtea->key.key_bytes, key, XTEA_FIXED_KEY_SIZE);
	memcpy( xtea->iv, iv, XTEA_INIT_VECTOR_SIZE);
	XTEA_set_fixedKey(xtea);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint8_t i;
	printf("\t* Iterations: %u\n\t* Dec sum: 0x%08lX\n\t* Key (hex bytes): [",xtea->iterations,xtea->dec_sum);
	for (i = 0; i != XTEA_FIXED_KEY_SIZE; i++){
		printf("%02x (%c)",xtea->key.key_bytes[i],xtea->key.key_bytes[i]);
	}
	printf("]\n\t* Initializating vector (hex bytes): [");
	for (i = 0; i != XTEA_INIT_VECTOR_SIZE; i++){
		printf("%02x (%c)",xtea->iv[i],xtea->iv[i]);
	}
	printf("]\n");
	#endif
	xtea->encrypted_chunks = 0;
	xtea->decrypted_chunks = 0;
}

/**
 * @brief 
 * 
 * @param xtea 
 * @param key 
 * @param iv 
 */
void XXTEA_init(XTEA_t *xxtea, xtea_key_t *key, uint8_t *iv){
#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XXTEA init\n");
	#endif

	
	//xtea->dec_sum = XTEA_DELTA * (xtea->iterations);
	memcpy( xxtea->key.key_bytes, key, XTEA_FIXED_KEY_SIZE);
	memcpy( xxtea->iv, iv, XTEA_INIT_VECTOR_SIZE);
	XTEA_set_fixedKey(xxtea);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint8_t i;
	printf("\t* Iterations: %u\n\t* Dec sum: 0x%08lX\n\t* Key (hex bytes): [",xtea->iterations,xtea->dec_sum);
	for (i = 0; i != XTEA_FIXED_KEY_SIZE; i++){
		printf("%02x (%c)",xtea->key.key_bytes[i],xtea->key.key_bytes[i]);
	}
	printf("]\n\t* Initializating vector (hex bytes): [");
	for (i = 0; i != XTEA_INIT_VECTOR_SIZE; i++){
		printf("%02x (%c)",xtea->iv[i],xtea->iv[i]);
	}
	printf("]\n");
	#endif
}

/**
 * @brief 
 * 
 * @param xtea 
 */
static void XTEA_set_fixedKey(XTEA_t *xtea){
	size_t i;
    uint8_t fixed_key[XTEA_FIXED_KEY_SIZE];
    memcpy(fixed_key, xtea->key.key_bytes, XTEA_FIXED_KEY_SIZE);
    for (i = 0; (i < XTEA_FIXED_KEY_SIZE) && (fixed_key[i] != 0); ++i);
	for (++i; i < XTEA_FIXED_KEY_SIZE; ++i) {
		fixed_key[i] = 0;
		}
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
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=XTEA_BLOCK_SIZE; index++){
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
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=XTEA_BLOCK_SIZE; index++){
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
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=XTEA_BLOCK_SIZE; index++){
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
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=XTEA_BLOCK_SIZE; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
}

/**
 * @brief 
 * 
 * @param xtea 
 * @param in 
 * @param out 
 * @param input_len 
 * @param ecb 
 * @param output_len 
 * @return XTEA_code_t 
 */
XTEA_code_t XTEA_encrypt(XTEA_t *xtea, void *in, void *out, uint16_t input_len, bool ecb, uint32_t *output_len){
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XTEA encrypt (%s mode)\n",ecb? "ECB":"CBC");
	#endif
	// Input buffer length verification
	if(input_len<= 0){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tEmpty input buffer\n");
		#endif
		
		return XTEA_CODE_EMPTY_INPUT_BUFFER;
	}
	// Normalized length calculation
	xtea->encrypted_chunks = 0;
	xtea->input_len_normalized = (uint16_t)((input_len + 7) & (-8));

	// Input buffer allocation, backup and initialization 
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	uint8_t *aux_in = (uint8_t *) malloc(xtea->input_len_normalized);
	uint8_t *_in = (uint8_t *)aux_in;
	if(aux_in == NULL){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tDynamic memory assignment error\n");
		#endif
		return XTEA_CODE_NULL_MALLOC;
	}
	memset(aux_in,0,xtea->input_len_normalized);
	memcpy(_in, in, xtea->input_len_normalized);
	#else
		#if defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
	if( xtea->input_len_normalized > XTEA_MAX_BUFFER_SIZE){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("ERROR: Normalized input length exceeds max buffer size\n");
		#endif
		return XTEA_CODE_INCORRECT_BUFFER_SIZE;
	}
	uint8_t aux_in[XTEA_MAX_BUFFER_SIZE];
	uint8_t *_in = (uint8_t *)aux_in;
	memset(_in, 0 , XTEA_MAX_BUFFER_SIZE);
	memcpy(_in, in, xtea->input_len_normalized);
		#else
	uint8_t *_in = (uint8_t *)in;
		#endif
	#endif
	
	// Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp_iv[XTEA_INIT_VECTOR_SIZE];
	memcpy(temp_iv, xtea->iv, XTEA_INIT_VECTOR_SIZE);
	memset(_out, 0 , xtea->input_len_normalized);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t i;
	printf("\tInput buffer:\t");
	for( i=0 ; i!=xtea->input_len_normalized; i++){
		uint8_t c = _in[i];
		printf("%02x ",c);
	}
	printf("\n");

	printf("\tOutput buffer:\t");
	for( i=0 ; i!=xtea->input_len_normalized; i++){
		uint8_t c = _out[i];
		printf("%02x ",c);
	}
	printf("\n");
	#endif
	
	// Chunk encryption
	while(xtea->input_len_normalized> 0) {
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tChunk %u: ",encrypted_chunks);
		#endif
		if(!ecb){
			for(uint8_t j=0; j != XTEA_INIT_VECTOR_SIZE; j++){
				uint8_t prev = _in[j];
				_in[j] = (uint8_t)(prev ^ temp_iv[j]);
			} 
			XTEA_encrypt_chunk(xtea,(uint32_t *)_in, (uint32_t *)_out);
			memcpy(temp_iv, _out, XTEA_INIT_VECTOR_SIZE);

		} else{
			XTEA_encrypt_chunk(xtea,(uint32_t *)_in, (uint32_t *)_out);
		}

		_in += XTEA_BLOCK_SIZE; _out += XTEA_BLOCK_SIZE; xtea->input_len_normalized-= XTEA_BLOCK_SIZE; xtea->encrypted_chunks++;
		
	}
	*output_len = xtea->encrypted_chunks*XTEA_BLOCK_SIZE;
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	free(aux_in);
	#endif
	return XTEA_CODE_OK;
}

/**
 * @brief 
 * 
 * @param xtea 
 * @param in 
 * @param out 
 * @param input_len 
 * @param ecb 
 * @param output_len 
 * @return XTEA_code_t 
 */
XTEA_code_t XTEA_decrypt(XTEA_t *xtea, void *in, void *out, uint16_t input_len, bool ecb, uint32_t *output_len){
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XTEA decrypt (%s mode)\n",ecb? "ECB":"CBC");
	#endif
	// Input buffer length verification
	if(input_len<= 0){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tEmpty input buffer\n");
		#endif
		return XTEA_CODE_EMPTY_INPUT_BUFFER;
	}
	// Normalized length calculation
	xtea->decrypted_chunks = 0;
	xtea->input_len_normalized = (uint16_t)((input_len + 7) & (-8));

	// Input buffer allocation, backup and initialization 
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	uint8_t *aux_in = (uint8_t *) malloc(xtea->input_len_normalized);
	uint8_t *_in = (uint8_t *)aux_in;
	if(aux_in == NULL){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tDynamic memory assignment error\n");
		#endif
		return XTEA_CODE_NULL_MALLOC;
	}
	memset(aux_in,0,xtea->input_len_normalized);
	memcpy(_in, in, xtea->input_len_normalized);
	#else
	#if defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
	if( xtea->input_len_normalized > XTEA_MAX_BUFFER_SIZE){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("ERROR: Normalized input length exceeds max buffer size\n");
		#endif
		return XTEA_CODE_INCORRECT_BUFFER_SIZE;
	}
	uint8_t aux_in[XTEA_MAX_BUFFER_SIZE];
	uint8_t *_in = (uint8_t *)aux_in;
	memset(_in, 0 , XTEA_MAX_BUFFER_SIZE);
	memcpy(_in, in, xtea->input_len_normalized);
		#else
	uint8_t *_in = (uint8_t *)in;
	#endif
	#endif
	
	// Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp[XTEA_INIT_VECTOR_SIZE];
	uint8_t temp_iv[XTEA_INIT_VECTOR_SIZE];
	memcpy(temp_iv, xtea->iv, XTEA_INIT_VECTOR_SIZE);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("\tInput buffer: ");
	for(uint32_t i=0 ; i!=xtea->input_len_normalized; i++){
		printf("%02x ",_in[i]);
	}
	printf("\n");
	#endif
	
	// Chunk decryption
	while(xtea->input_len_normalized> 0) {
		memcpy(temp, _in, XTEA_INIT_VECTOR_SIZE);

		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tChunk %u: ",decrypted_chunks);
		#endif
		
		XTEA_decrypt_chunk(xtea, (uint32_t *)_in, (uint32_t *)_out);
		if(!ecb){
			for(uint8_t j=0; j != XTEA_INIT_VECTOR_SIZE; j++){
				uint8_t prev = _out[j];
				_out[j] = (uint8_t)( prev ^ temp_iv[j]);
			}
			memcpy(temp_iv, temp, XTEA_INIT_VECTOR_SIZE);	
		}
		_in += XTEA_BLOCK_SIZE; _out += XTEA_BLOCK_SIZE; xtea->input_len_normalized-= XTEA_BLOCK_SIZE; xtea->decrypted_chunks++;
	}
	*output_len = xtea->decrypted_chunks*XTEA_BLOCK_SIZE;
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	free(aux_in);
	#endif
	return XTEA_CODE_OK;
}

/**
 * @brief 
 * 
 * @param xxtea 
 * @param in 
 * @param out 
 * @param input_len
 * @return XTEA_code_t 
 */
XTEA_code_t XXTEA_encrypt(XTEA_t *xxtea, void *in, void *out, uint16_t input_len, bool ecb, uint32_t *output_len){
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XXTEA encrypt (%s mode)\n",ecb? "ECB":"CBC");
	#endif
	// Input buffer length verification
	if(input_len<= 0){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tEmpty input buffer\n");
		#endif
		
		return XTEA_CODE_EMPTY_INPUT_BUFFER;
	}
	// Normalized length calculation
	xxtea->encrypted_chunks = 0;
	xxtea->input_len_normalized = (uint16_t)((input_len + 7) & (-8L));

	// Input buffer allocation, backup and initialization 
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	uint8_t *aux_in = (uint8_t *) malloc(xxtea->input_len_normalized);
	uint8_t *_in = (uint8_t *)aux_in;
	if(aux_in == NULL){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tDynamic memory assignment error\n");
		#endif
		return XTEA_CODE_NULL_MALLOC;
	}
	memset(aux_in,0,xxtea->input_len_normalized);
	memcpy(_in, in, xxtea->input_len_normalized);
	#else
		#if defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
	if( xxtea->input_len_normalized > XTEA_MAX_BUFFER_SIZE){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("ERROR: Normalized input length exceeds max buffer size\n");
		#endif
		return XTEA_CODE_INCORRECT_BUFFER_SIZE;
	}
	uint8_t aux_in[XTEA_MAX_BUFFER_SIZE];
	uint8_t *_in = (uint8_t *)aux_in;
	memset(_in, 0 , XTEA_MAX_BUFFER_SIZE);
	memcpy(_in, in, xxtea->input_len_normalized);
		#else
	uint8_t *_in = (uint8_t *)in;
		#endif
	#endif
	
	// Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp_iv[XTEA_INIT_VECTOR_SIZE];
	memcpy(temp_iv, xxtea->iv, XTEA_INIT_VECTOR_SIZE);
	memset(_out, 0 , xxtea->input_len_normalized);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t i;
	printf("\tInput buffer:\t");
	for( i=0 ; i!=xxtea->input_len_normalized; i++){
		uint8_t c = _in[i];
		printf("%02x ",c);
	}
	printf("\n");
	printf("\tOutput buffer:\t");
	for( i=0 ; i!=xxtea->input_len_normalized; i++){
		uint8_t c = _out[i];
		printf("%02x ",c);
	}
	printf("\n");
	#endif
	
	// Chunk encryption
	while(xxtea->input_len_normalized> 0) {
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tChunk %u: ",encrypted_chunks);
		#endif
		if(!ecb){
			for(uint8_t j=0; j != XTEA_INIT_VECTOR_SIZE; j++){
				uint8_t prev = _in[j];
				_in[j] = (uint8_t)(prev ^ temp_iv[j]);
			} 
			XXTEA_encrypt_chunk(xxtea,(uint32_t *)_in, (uint32_t *)_out);
			memcpy(temp_iv, _out, XTEA_INIT_VECTOR_SIZE);

		} else{
			XXTEA_encrypt_chunk(xxtea,(uint32_t *)_in, (uint32_t *)_out);
		}

		_in += XTEA_BLOCK_SIZE; _out += XTEA_BLOCK_SIZE; xxtea->input_len_normalized-= XTEA_BLOCK_SIZE; xxtea->encrypted_chunks++;
		
	}
	*output_len = xxtea->encrypted_chunks*XTEA_BLOCK_SIZE;
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	free(aux_in);
	#endif
	return XTEA_CODE_OK;
}

/**
 * @brief 
 * 
 * @param xxtea 
 * @param in 
 * @param out 
 * @param input_len
 * @return XTEA_code_t 
 */
XTEA_code_t XXTEA_decrypt(XTEA_t *xxtea, void *in, void *out, uint16_t input_len, bool ecb, uint32_t *output_len){
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("XXTEA decrypt (%s mode)\n",ecb? "ECB":"CBC");
	#endif
	// Input buffer length verification
	if(input_len<= 0){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tEmpty input buffer\n");
		#endif
		return XTEA_CODE_EMPTY_INPUT_BUFFER;
	}
	// Normalized length calculation
	xxtea->decrypted_chunks = 0;
	xxtea->input_len_normalized = (uint16_t)((input_len + 7) & (-8L));

	// Input buffer allocation, backup and initialization 
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	uint8_t *aux_in = (uint8_t *) malloc(xxtea->input_len_normalized);
	uint8_t *_in = (uint8_t *)aux_in;
	if(aux_in == NULL){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tDynamic memory assignment error\n");
		#endif
		return XTEA_CODE_NULL_MALLOC;
	}
	memset(aux_in,0,xxtea->input_len_normalized);
	memcpy(_in, in, xxtea->input_len_normalized);
	#else
	#if defined(XTEA_USE_BUFFERS) && (XTEA_USE_BUFFERS == 1)
	if( xxtea->input_len_normalized > XTEA_MAX_BUFFER_SIZE){
		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("ERROR: Normalized input length exceeds max buffer size\n");
		#endif
		return XTEA_CODE_INCORRECT_BUFFER_SIZE;
	}
	uint8_t aux_in[XTEA_MAX_BUFFER_SIZE];
	uint8_t *_in = (uint8_t *)aux_in;
	memset(_in, 0 , XTEA_MAX_BUFFER_SIZE);
	memcpy(_in, in, xxtea->input_len_normalized);
		#else
	uint8_t *_in = (uint8_t *)in;
	#endif
	#endif
	
	// Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	uint8_t temp[XTEA_INIT_VECTOR_SIZE];
	uint8_t temp_iv[XTEA_INIT_VECTOR_SIZE];
	memcpy(temp_iv, xxtea->iv, XTEA_INIT_VECTOR_SIZE);
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("\tInput buffer: ");
	for(uint32_t i=0 ; i!=xxtea->input_len_normalized; i++){
		printf("%02x ",_in[i]);
	}
	printf("\n");
	#endif
	
	// Chunk decryption
	while(xxtea->input_len_normalized> 0) {
		memcpy(temp, _in, XTEA_INIT_VECTOR_SIZE);

		#if defined(XTEA_LOG) && XTEA_LOG == 1
		printf("\tChunk %u: ",decrypted_chunks);
		#endif
		
		XXTEA_decrypt_chunk(xxtea, (uint32_t *)_in, (uint32_t *)_out);
		if(!ecb){
			for(uint8_t j=0; j != XTEA_INIT_VECTOR_SIZE; j++){
				uint8_t prev = _out[j];
				_out[j] = (uint8_t)( prev ^ temp_iv[j]);
			}
			memcpy(temp_iv, temp, XTEA_INIT_VECTOR_SIZE);	
		}
		_in += XTEA_BLOCK_SIZE; _out += XTEA_BLOCK_SIZE; xxtea->input_len_normalized-= XTEA_BLOCK_SIZE; xxtea->decrypted_chunks++;
	}
	*output_len = xxtea->decrypted_chunks*XTEA_BLOCK_SIZE;
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	free(aux_in);
	#endif
	return XTEA_CODE_OK;		
}

/**
 * @brief 
 * 
 * @param xxtea 
 * @param in 
 * @param out 
 */
static void XXTEA_encrypt_chunk(XTEA_t *xxtea, uint32_t *in, uint32_t *out){
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out;
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=XTEA_BLOCK_SIZE; index++){
		uint8_t c = _in[index];
		printf("%02x ",c);
	}	
	printf("\t");
	#endif
	uint32_t v0, v1, x, y, sum = 0;
	int8_t n = xxtea->iterations;
	uint8_t i;
	v0 = in[0];  v1 = in[1];
	while(n != 0) { 
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + xxtea->key.key_dwords[i];
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
		y = sum + xxtea->key.key_dwords[i];
		x ^= y;
		v1 += x;
		n--;
	}
	out[0] = v0;
	out[1] = v1;
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=XTEA_BLOCK_SIZE; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
}

/**
 * @brief 
 * 
 * @param xxtea 
 * @param in 
 * @param out 
 */
static void XXTEA_decrypt_chunk(XTEA_t *xxtea, uint32_t * in,uint32_t * out){
	uint8_t *_in = (uint8_t *)in;
	uint8_t *_out = (uint8_t *)out;
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=XTEA_BLOCK_SIZE; index++){
		uint8_t c = _in[index];
		printf("%02x ",c);
	}	
	printf("\t");
	#endif
	uint32_t  v0, v1, x, y, sum = xxtea->dec_sum;
	int8_t n = xxtea->iterations;
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
		y = sum + xxtea->key.key_dwords[i];
		x ^= y;
		v1 -= x;
		sum -= XTEA_DELTA;
		x = v1 << 4;
		y = v1 >> 5;
		x ^= y;
		x += v1;
		i = (uint8_t)(sum & 0xFF);
		i &= 0x03;
		y = sum + xxtea->key.key_dwords[i];
		x ^= y;
		v0 -= x;
		n--;
	}
	out[0] = v0;
	out[1] = v1;
	#if defined(XTEA_LOG) && XTEA_LOG == 1
	printf("OUT: ");
	for( index =0 ; index !=XTEA_BLOCK_SIZE; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
}

