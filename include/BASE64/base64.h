/**
 * @file base64.h
 * @brief Header for Base64 encoding and decoding optimized for embedded systems
 */

 #ifndef BASE64_H
 #define BASE64_H
 
 #include <stdint.h>
 #include <stddef.h>
 
 // Define buffer size constants
 #define BASE64_CHUNK_SIZE 3
 #define BASE64_ENCODED_CHUNK_SIZE 4
 #define BASE64_MAX_INPUT_SIZE 1024
 // Calculate the maximum encoded buffer size using the preprocessor
 // Formula: (input_size + 2) / 3 * 4 + 1 (for null terminator)
 #define BASE64_MAX_ENCODED_BUFFER (((BASE64_MAX_INPUT_SIZE + 2) / 3) * 4 + 1)
 
 // Special character markers
 #define BASE64_NOT_DIGIT 0xFF    // Invalid character marker
 #define BASE64_TERMINATOR 0xFE   // Padding character ('=')
 
 /**
  * @brief Status codes for Base64 operations
  */
 typedef enum {
     BASE64_SUCCESS = 0,          /**< Operation completed successfully */
     BASE64_INVALID_INPUT,        /**< Invalid input parameters */
     BASE64_BUFFER_TOO_SMALL,     /**< Provided buffer is too small */
     BASE64_INVALID_CHARACTER,    /**< Invalid Base64 character encountered */
     BASE64_INVALID_LENGTH        /**< Invalid input length (must be multiple of 4 for decoding) */
 } base64_status_t;
 
 /**
  * @brief Base64 converter context structure
  */
 typedef struct {
     uint8_t *buffer;            /**< Working buffer for encoding/decoding */
     size_t buffer_size;         /**< Size of the working buffer */
     size_t bytes_processed;     /**< Number of bytes processed in last operation */
 } base64_converter_t;
 
 /**
  * @brief Initialize a Base64 converter
  *
  * @param converter Pointer to converter structure
  * @param buffer Working buffer for encoding/decoding
  * @param buffer_size Size of the working buffer
  * @return base64_status_t Status code indicating success or failure
  */
 base64_status_t base64_init(base64_converter_t *converter, 
                            uint8_t *buffer, 
                            size_t buffer_size);
 
 /**
  * @brief Calculate the size needed for Base64 encoded output
  *
  * @param input_size Size of the input data in bytes
  * @return Size needed for encoded output, or 0 if input_size is invalid
  */
 size_t base64_get_encoded_size(size_t input_size);
 
 /**
  * @brief Calculate the maximum size needed for Base64 decoded output
  *
  * @param input_size Size of the Base64 encoded input
  * @return Maximum size needed for decoded output, or 0 if input_size is invalid
  */
 size_t base64_get_decoded_size(size_t input_size);
 
 /**
  * @brief Encode binary data to Base64
  *
  * @param converter Initialized converter structure
  * @param input Input data to encode
  * @param input_size Size of input data in bytes
  * @param output Pointer that will be set to the encoded data buffer
  * @param output_size Pointer to variable that will receive the encoded size
  * @return base64_status_t Status code indicating success or failure
  */
 base64_status_t base64_encode(base64_converter_t *converter,
                              const void *input,
                              size_t input_size,
                              const uint8_t **output,
                              size_t *output_size);
 
 /**
  * @brief Decode Base64 data to binary
  *
  * @param converter Initialized converter structure
  * @param input Base64 encoded input data
  * @param input_size Size of input data in bytes (must be multiple of 4)
  * @param output Buffer where decoded data will be written
  * @param output_size Pointer to variable that will receive the decoded size
  * @return base64_status_t Status code indicating success or failure
  */
 base64_status_t base64_decode(base64_converter_t *converter,
                              const uint8_t *input,
                              size_t input_size,
                              uint8_t *output,
                              size_t *output_size);
 
 /**
  * @brief Get a human-readable error message for a status code
  *
  * @param status Status code
  * @return Constant string describing the error
  */
 const char *base64_get_error_string(base64_status_t status);
 
 #endif /* BASE64_H */