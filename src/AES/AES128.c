/**
 * @file AES128.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AES128.h"

static void KeyExpansion_AES128(uint8_t *inputKey, uint8_t *expandedKeys);

void AES128_init_ctx(AES128_ctx_t *ctx, const uint8_t *key, const uint8_t *iv) {
    memcpy(ctx->key.array, key, AES128_FIXED_KEY_SIZE);
    ctx->decrypted_chunks = 0;
    ctx->encrypted_chunks = 0;

    if (iv == NULL) return;
    memcpy(ctx->iv, iv, AES_BLOCK_LEN);
}

static void AES128_encrypt_chunk(AES128_ctx_t *ctx, uint8_t *in, uint8_t *out) {
    memcpy(out, in, AES_BLOCK_LEN);

    uint8_t expandedKey[AES128_KEY_EXP_SIZE];
    KeyExpansion_AES128(ctx->key.array, expandedKey);
    AddRoundKey(out, ctx->key.array);

    for (size_t i = 0; i < AES128_NUM_ROUNDS - 1; i++) {
        SubBytes(out);
        ShiftRows(out);
        MixColumns(out);
        AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * (i + 1)));
    }

    SubBytes(out);
    ShiftRows(out);
    AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES128_NUM_ROUNDS));
}

static void AES128_decrypt_chunk(AES128_ctx_t *ctx, uint8_t *in, uint8_t *out) {
    memcpy(out, in, AES_BLOCK_LEN);

    uint8_t expandedKey[AES128_KEY_EXP_SIZE];

    KeyExpansion_AES128(ctx->key.array, expandedKey);

    AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * AES128_NUM_ROUNDS));

    ReverseShiftRows(out);
    ReverseSubBytes(out);

    for (size_t i = AES128_NUM_ROUNDS - 1; i >= 1; i--) {
        AddRoundKey(out, expandedKey + (AES_BLOCK_LEN * i));
        ReverseMixColumns(out);
        ReverseShiftRows(out);
        ReverseSubBytes(out);
    }
    AddRoundKey(out, expandedKey);
}

AES_errcode_t AES128_ECB_encrypt(AES128_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding) {
    // Input buffer length verification
    if (input_len <= 0) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }

    // Normalized length calculation
    ctx->encrypted_chunks = 0;
    ctx->input_len_normalized =
        (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    if (input_len == ctx->input_len_normalized && use_padding) {
        ctx->input_len_normalized += AES_BLOCK_LEN;
    }

    if (ctx->input_len_normalized > AES128_MAX_BUFFER_SIZE) {
        return AES_CODE_INCORRECT_BUFFER_SIZE;
    }

    // Create a temporary buffer for processing
    uint8_t temp_buffer[AES128_MAX_BUFFER_SIZE];
    memset(temp_buffer, 0, AES128_MAX_BUFFER_SIZE);

    // Prepare input data with or without padding
    if (use_padding) {
        PKCS7_padding_t padder;
        PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
        memcpy(temp_buffer, padder.data_with_padding,
               ctx->input_len_normalized);
    } else {
        memcpy(temp_buffer, in, input_len);
        // Zero-pad the rest if not using PKCS7
        if (input_len < ctx->input_len_normalized) {
            memset(temp_buffer + input_len, 0,
                   ctx->input_len_normalized - input_len);
        }
    }

    // Initialization vector and output buffer allocation
    uint8_t *_out = (uint8_t *)out;
    memset(_out, 0, ctx->input_len_normalized);

    // Pointer to the current position in the temp buffer
    uint8_t *_in = temp_buffer;
    size_t remaining = ctx->input_len_normalized;

    // Chunk encryption
    while (remaining > 0) {
        AES128_encrypt_chunk(ctx, _in, _out);
        _in += AES_BLOCK_LEN;
        _out += AES_BLOCK_LEN;
        remaining -= AES_BLOCK_LEN;
        ctx->encrypted_chunks++;
    }

    *output_len = ctx->encrypted_chunks * AES_BLOCK_LEN;
    return AES_CODE_OK;
}

AES_errcode_t AES128_ECB_decrypt(AES128_ctx_t *ctx, void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding) {
    // Input buffer length verification
    if (input_len <= 0) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }

    // Normalized length calculation
    ctx->decrypted_chunks = 0;
    size_t input_len_normalized = ctx->input_len_normalized =
        (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));

    if (ctx->input_len_normalized > AES128_MAX_BUFFER_SIZE) {
        return AES_CODE_INCORRECT_BUFFER_SIZE;
    }

    // Create a temporary buffer for processing
    uint8_t temp_buffer[AES128_MAX_BUFFER_SIZE];
    memcpy(temp_buffer, in, input_len);

    // Initialization vector and output buffer allocation
    uint8_t *_out = (uint8_t *)out;
    memset(_out, 0, input_len_normalized);

    // Pointer to the current position in the temp buffer
    uint8_t *_in = temp_buffer;
    size_t remaining = input_len_normalized;

    // Chunk decryption
    while (remaining > 0) {
        AES128_decrypt_chunk(ctx, _in, _out);
        _in += AES_BLOCK_LEN;
        _out += AES_BLOCK_LEN;
        remaining -= AES_BLOCK_LEN;
        ctx->decrypted_chunks++;
    }

    *output_len = ctx->decrypted_chunks * AES_BLOCK_LEN;

    if (use_padding) {
        PKCS7_unpadding_t unpadder;
        PKCS7_remove_padding(&unpadder, out, input_len_normalized);

        // Create a temporary buffer for the unpadded data
        uint8_t unpadded_buffer[AES128_MAX_BUFFER_SIZE];
        memcpy(unpadded_buffer, unpadder.data_without_padding,
               unpadder.data_len_without_padding);

        // Copy the unpadded data back to the output buffer
        memset(out, 0, input_len_normalized);  // Clear the output buffer first
        memcpy(out, unpadded_buffer, unpadder.data_len_without_padding);

        *output_len = unpadder.data_len_without_padding;
    }

    return AES_CODE_OK;
}

AES_errcode_t AES128_CBC_encrypt(AES128_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding) {
    // Input buffer length verification
    if (input_len <= 0) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }

    // Normalized length calculation
    ctx->encrypted_chunks = 0;
    size_t input_len_normalized =
        (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    ctx->input_len_normalized = input_len_normalized;
    if (input_len == input_len_normalized && use_padding) {
        ctx->input_len_normalized = input_len_normalized += AES_BLOCK_LEN;
    }

    if (input_len_normalized > AES128_MAX_BUFFER_SIZE) {
        return AES_CODE_INCORRECT_BUFFER_SIZE;
    }

    // Create a temporary buffer for processing
    uint8_t temp_buffer[AES128_MAX_BUFFER_SIZE];
    memset(temp_buffer, 0, AES128_MAX_BUFFER_SIZE);

    // Prepare input data with or without padding
    if (use_padding) {
        PKCS7_padding_t padder;
        PKCS7_add_padding(&padder, in, input_len, AES_BLOCK_LEN);
        memcpy(temp_buffer, padder.data_with_padding, input_len_normalized);
    } else {
        memcpy(temp_buffer, in, input_len);
        // Zero-pad the rest if not using PKCS7
        if (input_len < input_len_normalized) {
            memset(temp_buffer + input_len, 0,
                   input_len_normalized - input_len);
        }
    }

    // Initialization vector and output buffer allocation
    uint8_t *_out = (uint8_t *)out;
    memset(_out, 0, input_len_normalized);
    uint8_t *xor_vector = ctx->iv;

    // Pointer to the current position in the temp buffer
    uint8_t *_in = temp_buffer;
    size_t remaining = input_len_normalized;

    // Chunk encryption
    while (remaining > 0) {
        AddRoundKey(_in, xor_vector);
        AES128_encrypt_chunk(ctx, _in, _out);
        xor_vector = _out;

        _in += AES_BLOCK_LEN;
        _out += AES_BLOCK_LEN;
        remaining -= AES_BLOCK_LEN;
        ctx->encrypted_chunks++;
    }

    *output_len = ctx->encrypted_chunks * AES_BLOCK_LEN;
    return AES_CODE_OK;
}

AES_errcode_t AES128_CBC_decrypt(AES128_ctx_t *ctx, void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding) {
    // Input buffer length verification
    if (input_len <= 0) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }

    // Normalized length calculation
    ctx->decrypted_chunks = 0;
    size_t input_len_normalized =
        (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));
    ctx->input_len_normalized = input_len_normalized;

    if (input_len_normalized > AES128_MAX_BUFFER_SIZE) {
        return AES_CODE_INCORRECT_BUFFER_SIZE;
    }

    // Create a temporary buffer for processing
    uint8_t temp_buffer[AES128_MAX_BUFFER_SIZE];
    memcpy(temp_buffer, in, input_len);

    // Initialization vector and output buffer allocation
    uint8_t *_out = (uint8_t *)out;
    memset(_out, 0, input_len_normalized);
    uint8_t *xor_vector = ctx->iv;
    uint8_t placeholder[AES_BLOCK_LEN];

    // Pointer to the current position in the temp buffer
    uint8_t *_in = temp_buffer;
    size_t remaining = input_len_normalized;

    // Chunk decryption
    while (remaining > 0) {
        memcpy(placeholder, _in, AES_BLOCK_LEN);
        AES128_decrypt_chunk(ctx, _in, _out);
        AddRoundKey(_out, xor_vector);
        memcpy(xor_vector, placeholder, AES_BLOCK_LEN);

        _in += AES_BLOCK_LEN;
        _out += AES_BLOCK_LEN;
        remaining -= AES_BLOCK_LEN;
        ctx->decrypted_chunks++;
    }

    *output_len = ctx->decrypted_chunks * AES_BLOCK_LEN;

    if (use_padding) {
        PKCS7_unpadding_t unpadder;
        PKCS7_remove_padding(&unpadder, out, input_len_normalized);

        // Create a temporary buffer for the unpadded data
        uint8_t unpadded_buffer[AES128_MAX_BUFFER_SIZE];
        memcpy(unpadded_buffer, unpadder.data_without_padding,
               unpadder.data_len_without_padding);

        // Copy the unpadded data back to the output buffer
        memset(out, 0, input_len_normalized);  // Clear the output buffer first
        memcpy(out, unpadded_buffer, unpadder.data_len_without_padding);

        *output_len = unpadder.data_len_without_padding;
    }

    return AES_CODE_OK;
}

/**
 * @brief Key expansion for AES-128
 *
 * @param inputKey Input key (16 bytes)
 * @param expandedKeys Output expanded key (176 bytes)
 */
static void KeyExpansion_AES128(uint8_t *inputKey, uint8_t *expandedKeys) {
    size_t i;
    for (i = 0; i != AES128_FIXED_KEY_SIZE; i++) {
        expandedKeys[i] = inputKey[i];
    }

    size_t bytesGenerated = AES128_FIXED_KEY_SIZE;
    size_t rcon_location = 1;
    uint8_t key_block[4];

    while (bytesGenerated < AES128_KEY_EXP_SIZE) {
        for (i = 0; i != 4; i++) {
            key_block[i] = expandedKeys[i + bytesGenerated - 4];
        }

        if (bytesGenerated % AES128_FIXED_KEY_SIZE == 0) {
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

        for (i = 0; i != 4; i++) {
            expandedKeys[bytesGenerated] =
                expandedKeys[bytesGenerated - AES128_FIXED_KEY_SIZE] ^
                key_block[i];
            bytesGenerated++;
        }
    }
}
