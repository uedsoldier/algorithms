#include "AES128.h"
#include <iostream>
#include <iomanip>
#include <cstring> // For std::memcmp

static const char *TEST_NAME = "AES-128 CBC Tester (C++ Class)";

struct TestCase {
    const char *input_string;
    const uint8_t key[AES128_FIXED_KEY_SIZE];
    const uint8_t init_vector[AES_BLOCK_LEN];
    const char *description;
    bool usePKCS7;
};

// Test cases
static const TestCase test_cases[] = {
    {"This is a test string for AES-128 CBC mode!",
     {'A', 'E', 'S', '1', '2', '8', '_', 'S', 'E', 'C', 'R', 'E', 'T', 'K', 'E', 'Y'},
     {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6'},
     "Standard test with text that requires padding",
     true},
    {"ExactBlock16BytesX",
     {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
     {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},
     "Exact block size test (16 bytes)",
     true},
    {"Short text",
     {'S', 'E', 'C', 'R', 'E', 'T', 'K', 'E', 'Y', '1', '2', '3', '4', '5', '6', '7'},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     "Short text with zero IV",
     true},
    {"This is a multi-block input that will require more than one block of AES encryption to fully process.",
     {'T', 'e', 's', 't', 'K', 'e', 'y', '1', '2', '3', '4', '5', '6', '7', '8', '9'},
     {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
     "Multi-block test with padding",
     true},
    {"ExactBlock16BytesXExactBlock16BytesY",
     {'S', 'e', 'c', 'u', 'r', 'i', 't', 'y', 'K', 'e', 'y', '1', '2', '3', '4', '5'},
     {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f'},
     "Multiple exact blocks test (32 bytes)",
     true},
};

// Helper function to print hex representation of data
void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << TEST_NAME << std::endl;

    bool all_tests_passed = true;

    // Run tests
    for (size_t i = 0; i < sizeof(test_cases) / sizeof(TestCase); i++) {
        const TestCase &test = test_cases[i];

        std::cout << "\n--- Test " << (i + 1) << ": " << test.description << " ---" << std::endl;

        size_t input_len = std::strlen(test.input_string);
        uint8_t encrypted[AES128_MAX_BUFFER_SIZE] = {0};
        uint8_t decrypted[AES128_MAX_BUFFER_SIZE] = {0};
        uint32_t encrypted_len = 0;
        uint32_t decrypted_len = 0;

        std::cout << "Input string (" << input_len << " bytes): " << test.input_string << std::endl;
        std::cout << "Key: ";
        print_hex(test.key, AES128_FIXED_KEY_SIZE);
        std::cout << "IV: ";
        print_hex(test.init_vector, AES_BLOCK_LEN);
        std::cout << "PKCS7 Padding: " << (test.usePKCS7 ? "Enabled" : "Disabled") << std::endl;

        // Create AES128 instance
        AES128 aes(test.key, test.init_vector);

        // Check initialization
        if (aes.getErrorCode() != AES_CODE_OK) {
            std::cerr << "Failed to initialize AES128 context." << std::endl;
            all_tests_passed = false;
            continue;
        }

        // Encrypt
        AES_errcode_t encrypt_result = aes.encryptCBC(
            reinterpret_cast<const uint8_t *>(test.input_string), input_len,
            encrypted, &encrypted_len, test.usePKCS7);

        if (encrypt_result != AES_CODE_OK) {
            std::cerr << "Encryption failed with code: " << encrypt_result << std::endl;
            all_tests_passed = false;
            continue;
        }

        std::cout << "Encrypted (" << encrypted_len << " bytes): ";
        print_hex(encrypted, encrypted_len);

        // Decrypt
        AES_errcode_t decrypt_result = aes.decryptCBC(
            encrypted, encrypted_len, decrypted, &decrypted_len, test.usePKCS7);

        if (decrypt_result != AES_CODE_OK) {
            std::cerr << "Decryption failed with code: " << decrypt_result << std::endl;
            all_tests_passed = false;
            continue;
        }

        std::cout << "Decrypted (" << decrypted_len << " bytes): ";
        print_hex(decrypted, decrypted_len);
        std::cout << "Decrypted text: " << decrypted << std::endl;

        // Verify result
        bool test_passed = (decrypted_len == input_len) &&
                           (std::memcmp(test.input_string, decrypted, input_len) == 0);

        std::cout << "Test " << (i + 1) << " result: " << (test_passed ? "PASSED" : "FAILED") << std::endl;

        if (!test_passed) {
            all_tests_passed = false;
        }
    }

    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Total tests: " << (sizeof(test_cases) / sizeof(TestCase)) << std::endl;
    std::cout << "Final result: " << (all_tests_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
