#include "uppsrc/Core/SSL/SSL.h"
#include <iostream>
#include <cassert>

using namespace Upp;

int main() {
    std::cout << "Testing AES-256-GCM OpenSSL Compatibility..." << std::endl;
    
    // Test data
    String testData = "Hello, World! This is a test message for AES-256-GCM encryption.";
    String password = "test_password_123";
    
    std::cout << "Original data: " << testData.ToStd() << std::endl;
    std::cout << "Password: " << password.ToStd() << std::endl;
    
    // Test string encryption/decryption
    std::cout << "\n--- Testing String Encryption/Decryption ---" << std::endl;
    
    String encrypted = AES256Encrypt(testData, password);
    if(encrypted.IsVoid()) {
        std::cout << "ERROR: Encryption failed!" << std::endl;
        return 1;
    }
    
    std::cout << "Encryption successful! Encrypted size: " << encrypted.GetLength() << " bytes" << std::endl;
    
    String decrypted = AES256Decrypt(encrypted, password);
    if(decrypted.IsVoid()) {
        std::cout << "ERROR: Decryption failed!" << std::endl;
        return 1;
    }
    
    std::cout << "Decryption successful!" << std::endl;
    std::cout << "Decrypted data: " << decrypted.ToStd() << std::endl;
    
    // Verify data integrity
    if(testData == decrypted) {
        std::cout << "SUCCESS: Data integrity verified!" << std::endl;
    } else {
        std::cout << "ERROR: Data integrity check failed!" << std::endl;
        return 1;
    }
    
    // Test wrong password
    std::cout << "\n--- Testing Wrong Password ---" << std::endl;
    String wrongDecrypted = AES256Decrypt(encrypted, "wrong_password");
    if(wrongDecrypted.IsVoid()) {
        std::cout << "SUCCESS: Wrong password correctly rejected!" << std::endl;
    } else {
        std::cout << "ERROR: Wrong password was accepted!" << std::endl;
        return 1;
    }
    
    // Test Aes256Gcm class directly
    std::cout << "\n--- Testing Aes256Gcm Class ---" << std::endl;
    
    Aes256Gcm aes;
    String classEncrypted, classDecrypted;
    
    if(!aes.Encrypt(testData, password, classEncrypted)) {
        std::cout << "ERROR: Class encryption failed: " << aes.GetErrorDesc().ToStd() << std::endl;
        return 1;
    }
    
    std::cout << "Class encryption successful!" << std::endl;
    
    Aes256Gcm aes2;
    if(!aes2.Decrypt(classEncrypted, password, classDecrypted)) {
        std::cout << "ERROR: Class decryption failed: " << aes2.GetErrorDesc().ToStd() << std::endl;
        return 1;
    }
    
    std::cout << "Class decryption successful!" << std::endl;
    
    if(testData == classDecrypted) {
        std::cout << "SUCCESS: Class-based encryption/decryption verified!" << std::endl;
    } else {
        std::cout << "ERROR: Class-based data integrity check failed!" << std::endl;
        return 1;
    }
    
    // Show OpenSSL version info
    std::cout << "\n--- OpenSSL Version Info ---" << std::endl;
    std::cout << "OPENSSL_VERSION_NUMBER: 0x" << std::hex << OPENSSL_VERSION_NUMBER << std::dec << std::endl;
    
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    std::cout << "Using OpenSSL 3.0+ API (EVP_CIPHER_fetch, OSSL_PARAM)" << std::endl;
#else
    std::cout << "Using OpenSSL 1.1.x API (EVP_aes_256_gcm, EVP_CIPHER_CTX_ctrl)" << std::endl;
#endif
    
    std::cout << "\n=== ALL TESTS PASSED! ===" << std::endl;
    return 0;
}
