#include <iostream>
#include <cstring>
#include <vector>
#include <openssl/opensslv.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#include <openssl/kdf.h>
#endif

void print_hex(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", data[i]);
    }
}

// Simple test to verify our OpenSSL compatibility approach works
int main() {
    std::cout << "Testing OpenSSL AES-256-GCM compatibility..." << std::endl;
    std::cout << "OpenSSL version: " << OPENSSL_VERSION_TEXT << std::endl;
    std::cout << "OpenSSL version number: 0x" << std::hex << OPENSSL_VERSION_NUMBER << std::dec << std::endl;
    std::cout << std::endl;
    
    // Test 1: EVP_CIPHER access (the main compatibility issue)
    std::cout << "Test 1: Cipher access..." << std::endl;
    const EVP_CIPHER *cipher;
    
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    std::cout << "  Using OpenSSL 3.0+ API (EVP_CIPHER_fetch)" << std::endl;
    cipher = EVP_CIPHER_fetch(nullptr, "AES-256-GCM", nullptr);
    if (cipher == nullptr) {
        std::cout << "  ❌ ERROR: EVP_CIPHER_fetch failed" << std::endl;
        return 1;
    }
    std::cout << "  ✓ EVP_CIPHER_fetch succeeded" << std::endl;
#else
    std::cout << "  Using OpenSSL 1.1.1 API (EVP_aes_256_gcm)" << std::endl;
    cipher = EVP_aes_256_gcm();
    if (cipher == nullptr) {
        std::cout << "  ❌ ERROR: EVP_aes_256_gcm failed" << std::endl;
        return 1;
    }
    std::cout << "  ✓ EVP_aes_256_gcm succeeded" << std::endl;
    // No need to free static cipher pointer
#endif
    std::cout << std::endl;

    // Test 2: PBKDF2 (should work on both versions)
    std::cout << "Test 2: PBKDF2 key derivation..." << std::endl;
    unsigned char derived_key[32];
    unsigned char salt[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                              0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    const char* password = "test_password";
    
    int result = PKCS5_PBKDF2_HMAC(password, strlen(password), salt, 16, 1000, EVP_sha256(), 32, derived_key);
    if (!result) {
        std::cout << "  ❌ ERROR: PKCS5_PBKDF2_HMAC failed" << std::endl;
        return 1;
    }
    std::cout << "  ✓ PKCS5_PBKDF2_HMAC succeeded" << std::endl;
    std::cout << "  Key: ";
    print_hex(derived_key, 16);
    std::cout << "..." << std::endl << std::endl;
    // Test 3: Complete AES-256-GCM round-trip test
    std::cout << "Test 3: Complete AES-256-GCM encryption/decryption..." << std::endl;
    
    // Test data
    const char* plaintext = "Hello, OpenSSL compatibility test! This is a longer message to test chunked encryption.";
    size_t plaintext_len = strlen(plaintext);
    std::cout << "  Original: " << plaintext << std::endl;
    
    unsigned char iv[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
    
    // Encryption
    EVP_CIPHER_CTX *encrypt_ctx = EVP_CIPHER_CTX_new();
    if (!encrypt_ctx) {
        std::cout << "  ❌ ERROR: EVP_CIPHER_CTX_new failed" << std::endl;
        return 1;
    }
    
    // Initialize encryption
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    std::cout << "  Using EVP_EncryptInit_ex2..." << std::endl;
    result = EVP_EncryptInit_ex2(encrypt_ctx, cipher, derived_key, iv, nullptr);
#else
    std::cout << "  Using EVP_EncryptInit_ex..." << std::endl;
    result = EVP_EncryptInit_ex(encrypt_ctx, cipher, nullptr, derived_key, iv);
#endif
    
    if (!result) {
        std::cout << "  ❌ ERROR: EVP_EncryptInit failed" << std::endl;
        EVP_CIPHER_CTX_free(encrypt_ctx);
        return 1;
    }
    std::cout << "  ✓ Encryption initialization succeeded" << std::endl;
    
    // Encrypt
    unsigned char ciphertext[1024];
    int len, ciphertext_len;
    
    if (!EVP_EncryptUpdate(encrypt_ctx, ciphertext, &len, (const unsigned char*)plaintext, plaintext_len)) {
        std::cout << "  ❌ ERROR: EVP_EncryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(encrypt_ctx);
        return 1;
    }
    ciphertext_len = len;
    
    if (!EVP_EncryptFinal_ex(encrypt_ctx, ciphertext + len, &len)) {
        std::cout << "  ❌ ERROR: EVP_EncryptFinal_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(encrypt_ctx);
        return 1;
    }
    ciphertext_len += len;
    
    // Get authentication tag
    unsigned char tag[16];
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    OSSL_PARAM params[] = {
        OSSL_PARAM_construct_octet_string("tag", tag, 16),
        OSSL_PARAM_construct_end()
    };
    if (!EVP_CIPHER_CTX_get_params(encrypt_ctx, params)) {
        std::cout << "  ❌ ERROR: Getting tag failed (OSSL_PARAM)" << std::endl;
        EVP_CIPHER_CTX_free(encrypt_ctx);
        return 1;
    }
#else
    if (!EVP_CIPHER_CTX_ctrl(encrypt_ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) {
        std::cout << "  ❌ ERROR: Getting tag failed (EVP_CTRL)" << std::endl;
        EVP_CIPHER_CTX_free(encrypt_ctx);
        return 1;
    }
#endif
    
    std::cout << "  ✓ Encryption completed, ciphertext length: " << ciphertext_len << std::endl;
    std::cout << "  Tag: ";
    print_hex(tag, 16);
    std::cout << std::endl;
    
    EVP_CIPHER_CTX_free(encrypt_ctx);
    
    // Decryption
    std::cout << "  Starting decryption..." << std::endl;
    EVP_CIPHER_CTX *decrypt_ctx = EVP_CIPHER_CTX_new();
    if (!decrypt_ctx) {
        std::cout << "  ❌ ERROR: EVP_CIPHER_CTX_new for decryption failed" << std::endl;
        return 1;
    }
    
    // Initialize decryption
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    result = EVP_DecryptInit_ex2(decrypt_ctx, cipher, derived_key, iv, nullptr);
#else
    result = EVP_DecryptInit_ex(decrypt_ctx, cipher, nullptr, derived_key, iv);
#endif
    
    if (!result) {
        std::cout << "  ❌ ERROR: EVP_DecryptInit failed" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
    
    // Decrypt
    unsigned char decryptedtext[1024];
    int decryptedtext_len;
    
    if (!EVP_DecryptUpdate(decrypt_ctx, decryptedtext, &len, ciphertext, ciphertext_len)) {
        std::cout << "  ❌ ERROR: EVP_DecryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
    decryptedtext_len = len;
    
    // Set the expected tag value
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    OSSL_PARAM tag_params[] = {
        OSSL_PARAM_construct_octet_string("tag", tag, 16),
        OSSL_PARAM_construct_end()
    };
    if (!EVP_CIPHER_CTX_set_params(decrypt_ctx, tag_params)) {
        std::cout << "  ❌ ERROR: Setting tag failed (OSSL_PARAM)" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
#else
    if (!EVP_CIPHER_CTX_ctrl(decrypt_ctx, EVP_CTRL_GCM_SET_TAG, 16, tag)) {
        std::cout << "  ❌ ERROR: Setting tag failed (EVP_CTRL)" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
#endif
    
    // Finalize decryption and verify authentication tag
    if (!EVP_DecryptFinal_ex(decrypt_ctx, decryptedtext + len, &len)) {
        std::cout << "  ❌ ERROR: EVP_DecryptFinal_ex failed (authentication failed)" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
    decryptedtext_len += len;
    
    // Null terminate
    decryptedtext[decryptedtext_len] = '\0';
    
    std::cout << "  ✓ Decryption completed, length: " << decryptedtext_len << std::endl;
    std::cout << "  Decrypted: " << (char*)decryptedtext << std::endl;
    
    // Verify the round-trip
    if (plaintext_len != decryptedtext_len || memcmp(plaintext, decryptedtext, plaintext_len) != 0) {
        std::cout << "  ❌ ERROR: Round-trip failed! Decrypted text doesn't match original" << std::endl;
        EVP_CIPHER_CTX_free(decrypt_ctx);
        return 1;
    }
    
    std::cout << "  ✓ Round-trip verification successful!" << std::endl;
    
    EVP_CIPHER_CTX_free(decrypt_ctx);
    
    // Cleanup cipher if we fetched it
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    EVP_CIPHER_free(const_cast<EVP_CIPHER*>(cipher));
#endif
    
    std::cout << std::endl << "🎉 All OpenSSL AES-256-GCM compatibility tests passed!" << std::endl;
    std::cout << "The Ultimate++ AES compatibility fix should work correctly on this system." << std::endl;
    
    return 0;
}
