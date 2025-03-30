#include "AES128.h"
#include <cstring> // For memcpy

/**
 * @brief AES128 class constructor
 */
AES128::AES128(const uint8_t *key, const uint8_t *iv) {
    if (!key || !iv) {
        error_code_ = AES_CODE_EMPTY_INPUT_BUFFER;
        return;
    }
    AES128_init_ctx(&ctx_, key, iv);
    error_code_ = AES_CODE_OK;
}

/**
 * @brief Encrypt data using AES-128 in ECB mode
 */
AES_errcode_t AES128::encryptECB(const uint8_t *input, size_t input_len,
                                 uint8_t *output, uint32_t *output_len,
                                 bool use_padding) {
    if (!input || !output || !output_len) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    return AES128_ECB_encrypt(&ctx_, input, output, input_len, output_len, use_padding);
}

/**
 * @brief Decrypt data using AES-128 in ECB mode
 */
AES_errcode_t AES128::decryptECB(const uint8_t *input, size_t input_len,
                                 uint8_t *output, uint32_t *output_len,
                                 bool use_padding) {
    if (!input || !output || !output_len) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    return AES128_ECB_decrypt(&ctx_, const_cast<uint8_t *>(input), output, input_len, output_len, use_padding);
}

/**
 * @brief Encrypt data using AES-128 in CBC mode
 */
AES_errcode_t AES128::encryptCBC(const uint8_t *input, size_t input_len,
                                 uint8_t *output, uint32_t *output_len,
                                 bool use_padding) {
    if (!input || !output || !output_len) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    return AES128_CBC_encrypt(&ctx_, input, output, input_len, output_len, use_padding);
}

/**
 * @brief Decrypt data using AES-128 in CBC mode
 */
AES_errcode_t AES128::decryptCBC(const uint8_t *input, size_t input_len,
                                 uint8_t *output, uint32_t *output_len,
                                 bool use_padding) {
    if (!input || !output || !output_len) {
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    return AES128_CBC_decrypt(&ctx_, const_cast<uint8_t *>(input), output, input_len, output_len, use_padding);
}

/**
 * @brief Get the last error code
 */
AES_errcode_t AES128::getErrorCode() const {
    return error_code_;
}
