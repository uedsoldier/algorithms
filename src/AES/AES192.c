/**
 * @file AES192.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "AES192.h"

/**
 * @brief 
 * 
 * @param ctx 
 * @param key 
 * @param iv 
 */
void AES192_init_ctx(AES192_ctx_t *ctx, const uint8_t *key, const uint8_t *iv)
{
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("AES-192 init context. Make sure key is %u bytes long\n",AES192_FIXED_KEY_SIZE);
    #endif
    memcpy(ctx->key.array,key,AES192_FIXED_KEY_SIZE);
    ctx->decrypted_chunks = 0;
    ctx->encrypted_chunks = 0;
    #if defined(AES192_LOG) && AES192_LOG == 1
	
    print_buffer(ctx->key.array,AES192_FIXED_KEY_SIZE,"AES-192 KEY");
	#endif
	if(iv == NULL) return;
    memcpy(ctx->iv, iv, AES_BLOCK_LEN);
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("* Initializating vector (hex bytes): [");
	for (uint8_t i = 0; i != AES_BLOCK_LEN; i++){
		printf("%02x (%c)",iv[i],iv[i]);
	}
	printf("]\n");
    #endif
}

/**
 * @brief Function that encrypts the PlainText.
 * 
 * @param ctx 
 * @param in_state 
 * @param out_state 
 */
static void AES192_encrypt_chunk(AES192_ctx_t *ctx, uint8_t *in,uint8_t *out)
{
	#if defined(AES192_LOG) && AES192_LOG == 1
    print_buffer(in,AES_BLOCK_LEN,"in");
	#endif
    uint8_t round = 0;
    memcpy(out,in,AES_BLOCK_LEN);

    uint8_t expandedKey[AES192_KEY_EXP_SIZE]; 
	KeyExpansion_AES192(ctx->key.array, expandedKey);
	AddRoundKey(out, ctx->key.array);

	for (size_t i = 0; i < AES192_NUM_ROUNDS - 1; i++) {
		SubBytes(out);
		ShiftRows(out);
		MixColumns(out);
		AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * (i + 1)));
	}

	SubBytes(out);
	ShiftRows(out);
	AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES192_NUM_ROUNDS));

    #if defined(AES192_LOG) && AES192_LOG == 1
    print_buffer(out,AES_BLOCK_LEN,"out");
	#endif
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param in 
 * @param out 
 */
static void AES192_decrypt_chunk(AES192_ctx_t *ctx, uint8_t *in,uint8_t *out)
{
	#if defined(AES192_LOG) && AES192_LOG == 1
    print_buffer(in,AES_BLOCK_LEN,"in");
	#endif
    memcpy(out,in,AES_BLOCK_LEN);

    uint8_t expandedKey[AES192_KEY_EXP_SIZE];

	KeyExpansion_AES192(ctx->key.array, expandedKey);

	AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES192_NUM_ROUNDS ));

	ReverseShiftRows(out);
	ReverseSubBytes(out);

	for (size_t i = AES192_NUM_ROUNDS -1; i >= 1; i--) {
		AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * i));
		ReverseMixColumns(out);
		ReverseShiftRows(out);
		ReverseSubBytes(out);
	}
	AddRoundKey(out, expandedKey);

    #if defined(AES192_LOG) && AES192_LOG == 1
    print_buffer(out,AES_BLOCK_LEN,"out");
	#endif
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param in 
 * @param out 
 * @param input_len 
 * @param output_len 
 * @return AES_errcode_t 
 */
AES_errcode_t AES192_ECB_encrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len){
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("AES-192 ECB encrypt\n");
    #endif
    // Input buffer length verification
    if(input_len<= 0){
		return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    // Normalized length calculation
    ctx->encrypted_chunks = 0;
    ctx->input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    if(input_len == ctx->input_len_normalized){
        ctx->input_len_normalized += AES_BLOCK_LEN;
    }

	if( ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE){
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

    // Input buffer allocation, backup and initialization 
	uint8_t *_in = (uint8_t *)in;

	#if defined(AES192_USE_PKCS7) && AES192_USE_PKCS7 == 1
	PKCS7_padding_t padder;
	PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
	memcpy(_in, padder.data_with_padding, ctx->input_len_normalized);
	#else
	memcpy(_in, in, ctx->input_len_normalized);
	#endif

    // Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	memset(_out, 0 , ctx->input_len_normalized);

    // Chunk encryption
	while( ctx->input_len_normalized != 0) {
		#if defined(AES192_LOG) && AES192_LOG == 1
		printf("Chunk %u (bytes %u to %u):\n",ctx->encrypted_chunks+1,ctx->encrypted_chunks*AES_BLOCK_LEN,(ctx->encrypted_chunks+1)*AES_BLOCK_LEN-1);
		#endif

		AES192_encrypt_chunk(ctx,_in,_out);

		_in += AES_BLOCK_LEN; _out += AES_BLOCK_LEN; ctx->input_len_normalized-= AES_BLOCK_LEN; ctx->encrypted_chunks++;
		
	}
	*output_len = ctx->encrypted_chunks*AES_BLOCK_LEN;

	return AES_CODE_OK;
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param in 
 * @param out 
 * @param input_len 
 * @param output_len 
 * @return AES_errcode_t 
 */
AES_errcode_t AES192_ECB_decrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len){
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("AES-192 ECB decrypt\n");
    #endif
    // Input buffer length verification
    if(input_len<= 0){
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    // Normalized length calculation
    ctx->decrypted_chunks = 0;
    size_t input_len_normalized = ctx->input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    if(input_len == input_len_normalized){
        ctx->input_len_normalized = input_len_normalized += AES_BLOCK_LEN;
    }
	if( ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE){
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

    // Input buffer allocation, backup and initialization
	uint8_t *_in = (uint8_t *)in;

	memcpy(_in, in, ctx->input_len_normalized);

    // Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	memset(_out, 0 , input_len_normalized);

    // Chunk decryption
	while( input_len_normalized != 0) {
		#if defined(AES192_LOG) && AES192_LOG == 1
		printf("Chunk %u (bytes %u to %u):\n",ctx->decrypted_chunks+1,ctx->decrypted_chunks*AES_BLOCK_LEN,(ctx->decrypted_chunks+1)*AES_BLOCK_LEN-1);
		#endif

		AES192_decrypt_chunk(ctx,_in,_out);

		_in += AES_BLOCK_LEN; _out += AES_BLOCK_LEN; input_len_normalized-= AES_BLOCK_LEN; ctx->decrypted_chunks++;
		
	}
	*output_len = ctx->decrypted_chunks*AES_BLOCK_LEN;
	
	#if defined(AES192_USE_PKCS7) && AES192_USE_PKCS7 == 1
	PKCS7_unpadding_t unpadder;
	PKCS7_remove_padding(&unpadder, out, ctx->input_len_normalized);
	memcpy(out, unpadder.data_without_padding, ctx->input_len_normalized);
	#endif

	return AES_CODE_OK;
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param in 
 * @param out 
 * @param input_len 
 * @param output_len 
 * @return AES_errcode_t 
 */
AES_errcode_t AES192_CBC_encrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len){
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("AES-192 CBC encrypt\n");
    #endif
    // Input buffer length verification
    if(input_len<= 0){
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    // Normalized length calculation
    ctx->encrypted_chunks = 0;
    size_t input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    ctx->input_len_normalized = input_len_normalized;
    if(input_len == input_len_normalized){
        ctx->input_len_normalized = input_len_normalized += AES_BLOCK_LEN;
    }

    // Input buffer allocation, backup and initialization 
	uint8_t *_in = (uint8_t *)in;

	#if defined(AES192_USE_PKCS7) && AES192_USE_PKCS7 == 1
	PKCS7_padding_t padder;
	PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
	memcpy(_in, padder.data_with_padding, input_len_normalized);
	#else
	memcpy(_in, in, input_len_normalized);
	#endif

    // Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	memset(_out, 0 , input_len_normalized);
	uint8_t *xor_vector = ctx->iv;

    // Chunk encryption
	while( ctx->input_len_normalized > 0) {
		#if defined(AES192_LOG) && AES192_LOG == 1
		printf("Chunk %u (bytes %u to %u):\n",ctx->encrypted_chunks+1,ctx->encrypted_chunks*AES_BLOCK_LEN,(ctx->encrypted_chunks+1)*AES_BLOCK_LEN-1);
		#endif

			AddRoundKey(_in,xor_vector);
			AES192_encrypt_chunk(ctx,_in,_out);
			xor_vector = _out;

		_in += AES_BLOCK_LEN; _out += AES_BLOCK_LEN; ctx->input_len_normalized-= AES_BLOCK_LEN; ctx->encrypted_chunks++;
		
	}
	*output_len = ctx->encrypted_chunks*AES_BLOCK_LEN;

	return AES_CODE_OK;
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param in 
 * @param out 
 * @param input_len 
 * @param output_len 
 * @return AES_errcode_t 
 */
AES_errcode_t AES192_CBC_decrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len){
    #if defined(AES192_LOG) && AES192_LOG == 1
    printf("AES-192 CBC decrypt\n");
    #endif
    // Input buffer length verification
    if(input_len<= 0){
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    // Normalized length calculation
    ctx->decrypted_chunks = 0;
    size_t input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    ctx->input_len_normalized = input_len_normalized;
    if(input_len == input_len_normalized){
        ctx->input_len_normalized = input_len_normalized += AES_BLOCK_LEN;
    }

    // Input buffer allocation, backup and initialization
	uint8_t *_in = (uint8_t *)in;

	memcpy(_in, in, ctx->input_len_normalized);

    // Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	memset(_out, 0 , input_len_normalized);
	uint8_t *xor_vector = ctx->iv;
	uint8_t placeholder[AES_BLOCK_LEN];


    // Chunk decryption
	while( ctx->input_len_normalized > 0) {
		#if defined(AES192_LOG) && AES192_LOG == 1
		printf("Chunk %u (bytes %u to %u):\n",ctx->decrypted_chunks+1,ctx->decrypted_chunks*AES_BLOCK_LEN,(ctx->decrypted_chunks+1)*AES_BLOCK_LEN-1);
		#endif
		memcpy(placeholder,_in,AES_BLOCK_LEN);
		AES192_decrypt_chunk(ctx,_in,_out);
		AddRoundKey(_out, xor_vector);
		memcpy(xor_vector,_in,AES_BLOCK_LEN);

		_in += AES_BLOCK_LEN; _out += AES_BLOCK_LEN; ctx->input_len_normalized-= AES_BLOCK_LEN; ctx->decrypted_chunks++;
		
	}
	*output_len = ctx->decrypted_chunks*AES_BLOCK_LEN;
	
	#if defined(AES192_USE_PKCS7) && AES192_USE_PKCS7 == 1
	PKCS7_unpadding_t unpadder;
	PKCS7_remove_padding(&unpadder, out, input_len_normalized);
	memcpy(out, unpadder.data_without_padding, input_len_normalized);
	#endif

	return AES_CODE_OK;
}

/**
 * @brief 
 * 
 * @param inputKey 
 * @param expandedKeys 
 */
static void KeyExpansion_AES192(uint8_t *inputKey, uint8_t *expandedKeys){
    size_t i;
    for (i = 0; i != AES192_FIXED_KEY_SIZE; i++) {
		expandedKeys[i] = inputKey[i];
	}

	size_t bytesGenerated = AES192_FIXED_KEY_SIZE;
	size_t rcon_location = 1;
	uint8_t key_block[4];


	while (bytesGenerated < AES192_KEY_EXP_SIZE) {
		for (i = 0; i != 4; i++) { //while is less than the number of 32 bit words in 176 bit expanded keys
			key_block[i] = expandedKeys[i + bytesGenerated - 4];
		}
		if (bytesGenerated % AES192_FIXED_KEY_SIZE == 0) {

			uint8_t temp_val = key_block[0];
			key_block[0] = key_block[1];
			key_block[1] = key_block[2];
			key_block[2] = key_block[3];
			key_block[3] = temp_val;

			key_block[0] = s_box[key_block[0]];
			key_block[1] = s_box[key_block[1]];
			key_block[2] = s_box[key_block[2]];
			key_block[3] = s_box[key_block[3]];

			key_block[0] ^= rcon[rcon_location];
			rcon_location++;
		}

		#if defined(AES256) && AES256 == 1
		if(bytesGenerated % AES192_FIXED_KEY_SIZE == AES_BLOCK_LEN){
			for (i = 0; i != 4; i++) {
			key_block[i] = s_box[key_block[i]];
		}
		}
		
		#endif

		for (i = 0; i != 4; i++) {
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - AES192_FIXED_KEY_SIZE] ^ key_block[i];
			bytesGenerated++;
		}
	}
}

