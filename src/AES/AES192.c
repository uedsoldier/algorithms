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
 * @brief Initialize AES-192 context with key and optional IV
 *
 * @param ctx AES192 context structure
 * @param key 24-byte encryption key
 * @param iv 16-byte initialization vector (can be NULL for ECB mode)
 */
void AES192_init_ctx(AES192_ctx_t *ctx, const uint8_t *key, const uint8_t *iv)
{
	memcpy(ctx->key.array, key, AES192_FIXED_KEY_SIZE);
	ctx->decrypted_chunks = 0;
	ctx->encrypted_chunks = 0;
	if (iv == NULL)
		return;
	memcpy(ctx->iv, iv, AES_BLOCK_LEN);
}

/**
 * @brief Initialize AES-192 context for ECB mode (no IV needed)
 *
 * @param ctx AES192 context structure
 * @param key 24-byte encryption key
 */
void AES192_init_ctx_ecb(AES192_ctx_t *ctx, const uint8_t *key)
{
	AES192_init_ctx(ctx, key, NULL);
}

/**
 * @brief Encrypt a single 16-byte block using AES-192
 *
 * @param ctx AES192 context with initialized key
 * @param in Input block (16 bytes)
 * @param out Output block (16 bytes)
 */
static void AES192_encrypt_chunk(AES192_ctx_t *ctx, uint8_t *in, uint8_t *out)
{
	uint8_t round = 0;
	memcpy(out, in, AES_BLOCK_LEN);

	uint8_t expandedKey[AES192_KEY_EXP_SIZE];
	KeyExpansion_AES192(ctx->key.array, expandedKey);
	AddRoundKey(out, ctx->key.array);

	for (size_t i = 0; i < AES192_NUM_ROUNDS - 1; i++)
	{
		SubBytes(out);
		ShiftRows(out);
		MixColumns(out);
		AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * (i + 1)));
	}

	SubBytes(out);
	ShiftRows(out);
	AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES192_NUM_ROUNDS));
}

/**
 * @brief Decrypt a single 16-byte block using AES-192
 *
 * @param ctx AES192 context with initialized key
 * @param in Input block (16 bytes)
 * @param out Output block (16 bytes)
 */
static void AES192_decrypt_chunk(AES192_ctx_t *ctx, uint8_t *in, uint8_t *out)
{
	memcpy(out, in, AES_BLOCK_LEN);

	uint8_t expandedKey[AES192_KEY_EXP_SIZE];

	KeyExpansion_AES192(ctx->key.array, expandedKey);

	AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES192_NUM_ROUNDS));

	ReverseShiftRows(out);
	ReverseSubBytes(out);

	for (size_t i = AES192_NUM_ROUNDS - 1; i >= 1; i--)
	{
		AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * i));
		ReverseMixColumns(out);
		ReverseShiftRows(out);
		ReverseSubBytes(out);
	}
	AddRoundKey(out, expandedKey);
}

/**
 * @brief Encrypt data using AES-192 in ECB mode
 *
 * @param ctx AES192 context with initialized key
 * @param in Input data buffer
 * @param out Output data buffer (must be at least as large as the padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_encrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
								 size_t input_len, size_t *output_len, bool usePKCS7)
{
	// Input buffer length verification
	if (input_len <= 0)
	{
		return AES_CODE_EMPTY_INPUT_BUFFER;
	}

	// Normalized length calculation
	ctx->encrypted_chunks = 0;
	ctx->input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));

	// If input is already a multiple of block size and padding is required, add another block
	if (input_len == ctx->input_len_normalized && usePKCS7)
	{
		ctx->input_len_normalized += AES_BLOCK_LEN;
	}

	if (ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	// Create a temporary buffer for the padded input
	uint8_t temp_buffer[AES192_MAX_BUFFER_SIZE];
	memset(temp_buffer, 0, ctx->input_len_normalized);

	// Apply padding based on the usePKCS7 flag
	if (usePKCS7)
	{
		PKCS7_padding_t padder;
		PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
		memcpy(temp_buffer, padder.data_with_padding, ctx->input_len_normalized);
	}
	else
	{
		// Zero padding - just copy the input and leave the rest as zeros
		memcpy(temp_buffer, in, input_len);
	}

	// Encrypt each block
	uint8_t *temp_in = temp_buffer;
	uint8_t *temp_out = out;
	size_t remaining = ctx->input_len_normalized;

	while (remaining > 0)
	{
		AES192_encrypt_chunk(ctx, temp_in, temp_out);

		temp_in += AES_BLOCK_LEN;
		temp_out += AES_BLOCK_LEN;
		remaining -= AES_BLOCK_LEN;
		ctx->encrypted_chunks++;
	}

	*output_len = ctx->encrypted_chunks * AES_BLOCK_LEN;
	return AES_CODE_OK;
}

/**
 * @brief Decrypt data using AES-192 in ECB mode
 *
 * @param ctx AES192 context with initialized key
 * @param in Input data buffer (encrypted)
 * @param out Output data buffer (decrypted)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_decrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
								 size_t input_len, size_t *output_len, bool usePKCS7)
{
	// Input buffer length verification
	if (input_len <= 0)
	{
		return AES_CODE_EMPTY_INPUT_BUFFER;
	}

	// Normalized length calculation - input must be a multiple of block size
	if (input_len % AES_BLOCK_LEN != 0)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	ctx->decrypted_chunks = 0;
	ctx->input_len_normalized = input_len;

	if (ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	// Create a temporary buffer for the decrypted output
	uint8_t temp_buffer[AES192_MAX_BUFFER_SIZE];
	memset(temp_buffer, 0, ctx->input_len_normalized);

	// Decrypt each block
	const uint8_t *temp_in = in;
	uint8_t *temp_out = temp_buffer;
	size_t remaining = ctx->input_len_normalized;

	while (remaining > 0)
	{
		AES192_decrypt_chunk(ctx, (uint8_t *)temp_in, temp_out);

		temp_in += AES_BLOCK_LEN;
		temp_out += AES_BLOCK_LEN;
		remaining -= AES_BLOCK_LEN;
		ctx->decrypted_chunks++;
	}

	// Handle padding based on the usePKCS7 flag
	if (usePKCS7)
	{
		PKCS7_unpadding_t unpadder;
		PKCS7_remove_padding(&unpadder, temp_buffer, ctx->input_len_normalized);
		memcpy(out, unpadder.data_without_padding, unpadder.data_len_without_padding);
		*output_len = unpadder.data_len_without_padding;
	}
	else
	{
		// For zero padding, just copy all the data (including trailing zeros)
		memcpy(out, temp_buffer, ctx->input_len_normalized);
		*output_len = ctx->input_len_normalized;
	}

	return AES_CODE_OK;
}

/**
 * @brief Encrypt data using AES-192 in CBC mode
 *
 * @param ctx AES192 context with initialized key and IV
 * @param in Input data buffer
 * @param out Output data buffer (must be at least as large as the padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_encrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
								 size_t input_len, size_t *output_len, bool usePKCS7)
{
	// Input buffer length verification
	if (input_len <= 0)
	{
		return AES_CODE_EMPTY_INPUT_BUFFER;
	}

	// Normalized length calculation
	ctx->encrypted_chunks = 0;
	ctx->input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));

	// If input is already a multiple of block size and padding is required, add another block
	if (input_len == ctx->input_len_normalized && usePKCS7)
	{
		ctx->input_len_normalized += AES_BLOCK_LEN;
	}

	if (ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	// Create a temporary buffer for the padded input
	uint8_t temp_buffer[AES192_MAX_BUFFER_SIZE];
	memset(temp_buffer, 0, ctx->input_len_normalized);

	// Apply padding based on the usePKCS7 flag
	if (usePKCS7)
	{
		PKCS7_padding_t padder;
		PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
		memcpy(temp_buffer, padder.data_with_padding, ctx->input_len_normalized);
	}
	else
	{
		// Zero padding - just copy the input and leave the rest as zeros
		memcpy(temp_buffer, in, input_len);
	}

	// Encrypt each block
	uint8_t *temp_in = temp_buffer;
	uint8_t *temp_out = out;
	size_t remaining = ctx->input_len_normalized;
	uint8_t *xor_vector = ctx->iv;
	uint8_t block_buffer[AES_BLOCK_LEN];

	while (remaining > 0)
	{
		// XOR with previous ciphertext or IV
		for (int i = 0; i < AES_BLOCK_LEN; i++)
		{
			block_buffer[i] = temp_in[i] ^ xor_vector[i];
		}

		// Encrypt the XORed block
		AES192_encrypt_chunk(ctx, block_buffer, temp_out);

		// Update XOR vector for next block
		xor_vector = temp_out;

		temp_in += AES_BLOCK_LEN;
		temp_out += AES_BLOCK_LEN;
		remaining -= AES_BLOCK_LEN;
		ctx->encrypted_chunks++;
	}

	*output_len = ctx->encrypted_chunks * AES_BLOCK_LEN;
	return AES_CODE_OK;
}

/**
 * @brief Decrypt data using AES-192 in CBC mode
 *
 * @param ctx AES192 context with initialized key and IV
 * @param in Input data buffer (encrypted)
 * @param out Output data buffer (decrypted)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_decrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
								 size_t input_len, size_t *output_len, bool usePKCS7)
{
	// Input buffer length verification
	if (input_len <= 0)
	{
		return AES_CODE_EMPTY_INPUT_BUFFER;
	}

	// Normalized length calculation - input must be a multiple of block size
	if (input_len % AES_BLOCK_LEN != 0)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	ctx->decrypted_chunks = 0;
	ctx->input_len_normalized = input_len;

	if (ctx->input_len_normalized > AES192_MAX_BUFFER_SIZE)
	{
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}

	// Create a temporary buffer for the decrypted output
	uint8_t temp_buffer[AES192_MAX_BUFFER_SIZE];
	memset(temp_buffer, 0, ctx->input_len_normalized);

	// Decrypt each block
	const uint8_t *temp_in = in;
	uint8_t *temp_out = temp_buffer;
	size_t remaining = ctx->input_len_normalized;
	uint8_t *xor_vector = ctx->iv;
	uint8_t current_block[AES_BLOCK_LEN];
	uint8_t next_xor_vector[AES_BLOCK_LEN];

	while (remaining > 0)
	{
		// Save current ciphertext block for next iteration's XOR
		memcpy(next_xor_vector, temp_in, AES_BLOCK_LEN);

		// Decrypt the current block
		AES192_decrypt_chunk(ctx, (uint8_t *)temp_in, current_block);

		// XOR with previous ciphertext or IV
		for (int i = 0; i < AES_BLOCK_LEN; i++)
		{
			temp_out[i] = current_block[i] ^ xor_vector[i];
		}

		// Update XOR vector for next block
		memcpy(xor_vector, next_xor_vector, AES_BLOCK_LEN);

		temp_in += AES_BLOCK_LEN;
		temp_out += AES_BLOCK_LEN;
		remaining -= AES_BLOCK_LEN;
		ctx->decrypted_chunks++;
	}

	// Handle padding based on the usePKCS7 flag
	if (usePKCS7)
	{
		PKCS7_unpadding_t unpadder;
		PKCS7_remove_padding(&unpadder, temp_buffer, ctx->input_len_normalized);
		memcpy(out, unpadder.data_without_padding, unpadder.data_len_without_padding);
		*output_len = unpadder.data_len_without_padding;
	}
	else
	{
		// For zero padding, just copy all the data (including trailing zeros)
		memcpy(out, temp_buffer, ctx->input_len_normalized);
		*output_len = ctx->input_len_normalized;
	}

	return AES_CODE_OK;
}

/**
 * @brief Expand the AES-192 key for encryption/decryption
 *
 * @param inputKey The 24-byte input key
 * @param expandedKeys Buffer to store the expanded key (must be AES192_KEY_EXP_SIZE bytes)
 */
static void KeyExpansion_AES192(uint8_t *inputKey, uint8_t *expandedKeys)
{
	size_t i;
	for (i = 0; i != AES192_FIXED_KEY_SIZE; i++)
	{
		expandedKeys[i] = inputKey[i];
	}

	size_t bytesGenerated = AES192_FIXED_KEY_SIZE;
	size_t rcon_location = 1;
	uint8_t key_block[4];

	while (bytesGenerated < AES192_KEY_EXP_SIZE)
	{
		for (i = 0; i != 4; i++)
		{
			key_block[i] = expandedKeys[i + bytesGenerated - 4];
		}
		if (bytesGenerated % AES192_FIXED_KEY_SIZE == 0)
		{
			// Rotate word
			uint8_t temp_val = key_block[0];
			key_block[0] = key_block[1];
			key_block[1] = key_block[2];
			key_block[2] = key_block[3];
			key_block[3] = temp_val;

			// Apply S-box
			key_block[0] = s_box[key_block[0]];
			key_block[1] = s_box[key_block[1]];
			key_block[2] = s_box[key_block[2]];
			key_block[3] = s_box[key_block[3]];

			// XOR with round constant
			key_block[0] ^= rcon[rcon_location];
			rcon_location++;
		}

#if defined(AES256) && AES256 == 1
		if (bytesGenerated % AES192_FIXED_KEY_SIZE == AES_BLOCK_LEN)
		{
			for (i = 0; i != 4; i++)
			{
				key_block[i] = s_box[key_block[i]];
			}
		}
#endif

		// Generate new key material
		for (i = 0; i != 4; i++)
		{
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - AES192_FIXED_KEY_SIZE] ^ key_block[i];
			bytesGenerated++;
		}
	}
}