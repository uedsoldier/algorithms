/**
 * @file base64.c
 * @brief Implementation of Base64 encoding and decoding optimized for embedded
 * systems
 */

#include "base64.h"

/**
 * @brief Lookup table for converting Base64 characters to 6-bit binary values
 *
 * Table maps ASCII values to their corresponding 6-bit values:
 * - 'A'-'Z' -> 0-25
 * - 'a'-'z' -> 26-51
 * - '0'-'9' -> 52-61
 * - '+' -> 62
 * - '/' -> 63
 * - '=' -> 0xFE (padding marker)
 * - others -> 0xFF (invalid character marker)
 */
static const uint8_t digit_to_bin[256] = {
    /* ASCII control characters 0x00-0x1F */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x00-0x07 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x08-0x0F */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x10-0x17 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x18-0x1F */

    /* ASCII punctuation and symbols 0x20-0x2F */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* Space ! " # $ % & ' */
    0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, /* ( ) * + , - . / */

    /* ASCII digits 0x30-0x3F */
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, /* 0 1 2 3 4 5 6 7 */
    0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, /* 8 9 : ; < = > ? */

    /* ASCII uppercase letters 0x40-0x5F */
    0xFF,                                           /* @ */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, /* A B C D E F G H */
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, /* I J K L M N O P */
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, /* Q R S T U V W X */
    0x18, 0x19,                                     /* Y Z */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                   /* [ \ ] ^ _ */

    /* ASCII lowercase letters 0x60-0x7F */
    0xFF,                                           /* ` */
    0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, /* a b c d e f g h */
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, /* i j k l m n o p */
    0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, /* q r s t u v w x */
    0x32, 0x33,                                     /* y z */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                   /* { | } ~ DEL */

    /* Extended ASCII 0x80-0xFF (all invalid) */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x80-0x87 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x88-0x8F */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x90-0x97 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0x98-0x9F */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xA0-0xA7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xA8-0xAF */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xB0-0xB7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xB8-0xBF */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xC0-0xC7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xC8-0xCF */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xD0-0xD7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xD8-0xDF */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xE0-0xE7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xE8-0xEF */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* 0xF0-0xF7 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  /* 0xF8-0xFF */
};

// Base64 encoding table
static const uint8_t bin_to_digit[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
base64_status_t base64_init(base64_converter_t *converter, uint8_t *buffer,
                            size_t buffer_size) {
    if (!converter || !buffer || buffer_size < BASE64_MAX_ENCODED_BUFFER) {
        return BASE64_INVALID_INPUT;
    }

    converter->buffer = buffer;
    converter->buffer_size = buffer_size;
    converter->bytes_processed = 0;

    return BASE64_SUCCESS;
}

/**
 * @brief Calculate required buffer size for Base64 encoded output
 *
 * Internal helper function to determine the buffer size needed for encoding.
 *
 * @param input_size Size of the input data in bytes
 * @return Size needed for encoded output including null terminator, or 0 if
 * invalid
 */
static size_t base64_get_encoded_size(size_t input_size) {
    if (input_size > BASE64_MAX_INPUT_SIZE) {
        return 0;  // Invalid size
    }
    // Formula: Ceiling(input_size / 3) * 4 + 1 for null terminator
    return ((input_size + 2) / 3) * 4 + 1;
}

/**
 * @brief Calculate maximum buffer size needed for Base64 decoded output
 *
 * Internal helper function to determine the buffer size needed for decoding.
 *
 * @param input_size Size of the Base64 encoded input
 * @return Maximum possible decoded size including null terminator, or 0 if
 * invalid
 */
static size_t base64_get_decoded_size(size_t input_size) {
    if (input_size > BASE64_MAX_ENCODED_BUFFER || (input_size % 4) != 0) {
        return 0;  // Invalid size
    }
    // Formula: (input_size / 4) * 3 + 1 for null terminator
    // Note: This is the maximum possible size, actual size might be less due to
    // padding
    return (input_size / 4) * 3 + 1;
}

base64_status_t base64_encode(base64_converter_t *converter, const void *input,
                              size_t input_size, const uint8_t **output,
                              size_t *output_size) {
    // Input validation
    if (!converter || !input || !output_size || !output ||
        input_size > BASE64_MAX_INPUT_SIZE || !converter->buffer) {
        return BASE64_INVALID_INPUT;
    }

    size_t required_size = base64_get_encoded_size(input_size);
    if (converter->buffer_size < required_size) {
        return BASE64_BUFFER_TOO_SMALL;
    }

    const uint8_t *src = (const uint8_t *)input;
    uint8_t *dest = converter->buffer;
    size_t i;

    // Process complete 3-byte blocks
    for (i = 0; i + 2 < input_size; i += 3) {
        // First character: bits 7-2 from byte 1
        dest[0] = bin_to_digit[src[i] >> 2];

        // Second character: bits 1-0 from byte 1 and bits 7-4 from byte 2
        dest[1] = bin_to_digit[((src[i] & 0x03) << 4) | (src[i + 1] >> 4)];

        // Third character: bits 3-0 from byte 2 and bits 7-6 from byte 3
        dest[2] = bin_to_digit[((src[i + 1] & 0x0F) << 2) | (src[i + 2] >> 6)];

        // Fourth character: bits 5-0 from byte 3
        dest[3] = bin_to_digit[src[i + 2] & 0x3F];

        dest += 4;
    }

    // Handle remaining bytes
    if (i < input_size) {
        // First character: bits 7-2 from remaining byte
        dest[0] = bin_to_digit[src[i] >> 2];

        if (i + 1 < input_size) {
            // One byte remaining
            dest[1] = bin_to_digit[((src[i] & 0x03) << 4) | (src[i + 1] >> 4)];
            dest[2] = bin_to_digit[(src[i + 1] & 0x0F) << 2];
            dest[3] = '=';
        } else {
            // Two bytes remaining
            dest[1] = bin_to_digit[(src[i] & 0x03) << 4];
            dest[2] = '=';
            dest[3] = '=';
        }
        dest += 4;
    }

    *dest = '\0';
    *output_size = dest - converter->buffer;
    *output = converter->buffer;

    return BASE64_SUCCESS;
}

base64_status_t base64_decode(base64_converter_t *converter,
                              const uint8_t *input, size_t input_size,
                              uint8_t *output, size_t *output_size) {
    // Input validation
    if (!converter || !input || !output || !output_size) {
        return BASE64_INVALID_INPUT;
    }

    // Get maximum decoded size and validate input
    size_t max_decoded_size = base64_get_decoded_size(input_size);
    if (max_decoded_size == 0) {
        return BASE64_INVALID_LENGTH;
    }

    // Check if output buffer is large enough
    if (converter->buffer_size < max_decoded_size) {
        return BASE64_BUFFER_TOO_SMALL;
    }

    // Count padding characters
    size_t padding = 0;
    if (input_size > 0) {
        if (input[input_size - 1] == '=') padding++;
        if (input[input_size - 2] == '=') padding++;
    }

    // Calculate actual output size
    *output_size = (input_size / 4) * 3 - padding;

    const uint8_t *src = input;
    uint8_t *dest = output;
    size_t chunks = input_size / 4;

    for (size_t i = 0; i < chunks; i++) {
        uint8_t b1, b2, b3, b4;

        // Convert each character to its 6-bit value
        b1 = digit_to_bin[src[0]];
        b2 = digit_to_bin[src[1]];
        b3 = digit_to_bin[src[2]];
        b4 = digit_to_bin[src[3]];

        // Check for invalid characters
        if (b1 == BASE64_NOT_DIGIT || b2 == BASE64_NOT_DIGIT ||
            (b3 == BASE64_NOT_DIGIT && b3 != BASE64_TERMINATOR) ||
            (b4 == BASE64_NOT_DIGIT && b4 != BASE64_TERMINATOR)) {
            return BASE64_INVALID_CHARACTER;
        }

        // Handle padding
        if (src[2] == '=') b3 = 0;
        if (src[3] == '=') b4 = 0;

        // Combine 6-bit values into bytes
        *dest++ = (b1 << 2) | (b2 >> 4);
        if (src[2] != '=') {
            *dest++ = (b2 << 4) | (b3 >> 2);
            if (src[3] != '=') {
                *dest++ = (b3 << 6) | b4;
            }
        }

        src += 4;
    }

    return BASE64_SUCCESS;
}

const char *base64_get_error_string(base64_status_t status) {
    switch (status) {
        case BASE64_SUCCESS:
            return "Success";
        case BASE64_INVALID_INPUT:
            return "Invalid input parameters";
        case BASE64_BUFFER_TOO_SMALL:
            return "Buffer too small";
        case BASE64_INVALID_CHARACTER:
            return "Invalid Base64 character";
        default:
            return "Unknown error";
    }
}
