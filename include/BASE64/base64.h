/**
 * @file base64.h
 * @brief Base64 encoding and decoding implementation optimized for embedded
 * systems
 * @version 1.0
 * @date 2024-01-19
 *
 * This implementation provides Base64 encoding and decoding functionality with
 * fixed buffer sizes and no dynamic memory allocation, making it suitable for
 * embedded systems and memory-constrained environments.
 */

#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Size of a binary data chunk before Base64 encoding
 *
 * Each chunk of 3 bytes of binary data is encoded into 4 Base64 characters.
 * This value MUST NOT be modified as it's fundamental to Base64 encoding.
 */
#define BASE64_CHUNK_SIZE 3

/**
 * @brief Size of an encoded Base64 chunk
 *
 * Each 3 bytes of binary data becomes 4 Base64 characters.
 * This value MUST NOT be modified as it's fundamental to Base64 encoding.
 */
#define BASE64_ENCODED_CHUNK_SIZE 4

/**
 * @brief Maximum allowed input size in bytes
 *
 * Limits the maximum size of input data to prevent buffer overflow
 * in memory-constrained environments.
 */
#define BASE64_MAX_INPUT_SIZE 1024

/**
 * @brief Maximum size needed for Base64 encoded output
 *
 * Calculated as: (input_size + 2) / 3 * 4 + 1
 * The formula accounts for:
 * - Padding adjustment (+2)
 * - 3:4 encoding ratio
 * - Null terminator (+1)
 */
#define BASE64_MAX_ENCODED_BUFFER (((BASE64_MAX_INPUT_SIZE + 2) / 3) * 4 + 1)

/**
 * @brief Marker for invalid Base64 characters
 *
 * Used during decoding to identify invalid input characters.
 */
#define BASE64_NOT_DIGIT 0xFF

/**
 * @brief Marker for Base64 padding character ('=')
 *
 * Used in encoding and decoding to handle padding when input
 * length is not a multiple of 3.
 */
#define BASE64_TERMINATOR 0xFE

/**
 * @brief Status codes for Base64 operations
 *
 * Provides detailed error reporting for encoding and decoding operations.
 */
typedef enum {
    BASE64_SUCCESS = 0,   /**< Operation completed successfully */
    BASE64_INVALID_INPUT, /**< Invalid input parameters (NULL pointers, etc.) */
    BASE64_BUFFER_TOO_SMALL,  /**< Provided buffer cannot hold the result */
    BASE64_INVALID_CHARACTER, /**< Input contains invalid Base64 characters */
    BASE64_INVALID_LENGTH     /**< Input length is not valid for decoding */
} base64_status_t;

/**
 * @brief Base64 converter context structure
 *
 * Maintains state information for encoding and decoding operations.
 * Must be initialized with base64_init() before use.
 */
typedef struct {
    uint8_t *buffer;        /**< Working buffer for encoding/decoding */
    size_t buffer_size;     /**< Size of the working buffer */
    size_t bytes_processed; /**< Number of bytes processed in last operation */
} base64_converter_t;

/**
 * @brief Initialize a Base64 converter instance
 *
 * The provided buffer must be at least BASE64_MAX_ENCODED_BUFFER bytes.
 *
 * @param converter Pointer to converter structure to initialize
 * @param buffer Working buffer for encoding/decoding operations
 * @param buffer_size Size of the working buffer
 * @return base64_status_t Status code indicating success or failure
 */
base64_status_t base64_init(base64_converter_t *converter, uint8_t *buffer,
                            size_t buffer_size);

/**
 * @brief Encode binary data to Base64 format
 *
 * The output buffer is automatically sized based on the input length.
 * Maximum input size is BASE64_MAX_INPUT_SIZE bytes.
 *
 * @param converter Initialized converter instance
 * @param input Binary data to encode
 * @param input_size Size of input data in bytes
 * @param output Pointer that will be set to the encoded data buffer
 * @param output_size Pointer to variable that will receive the encoded size
 * @return base64_status_t Status code indicating success or failure
 */
base64_status_t base64_encode(base64_converter_t *converter, const void *input,
                              size_t input_size, const uint8_t **output,
                              size_t *output_size);

/**
 * @brief Decode Base64 data to binary
 *
 * Input size must be a multiple of 4 bytes.
 * Maximum input size is BASE64_MAX_ENCODED_BUFFER bytes.
 *
 * @param converter Initialized converter instance
 * @param input Base64 encoded input data
 * @param input_size Size of input data in bytes (must be multiple of 4)
 * @param output Buffer where decoded data will be written
 * @param output_size Pointer to variable that will receive the decoded size
 * @return base64_status_t Status code indicating success or failure
 */
base64_status_t base64_decode(base64_converter_t *converter,
                              const uint8_t *input, size_t input_size,
                              uint8_t *output, size_t *output_size);

/**
 * @brief Get human-readable error message for a status code
 *
 * Useful for debugging and error reporting.
 *
 * @param status Status code from an encoding or decoding operation
 * @return Constant string describing the error
 */
const char *base64_get_error_string(base64_status_t status);

#endif /* BASE64_H */
