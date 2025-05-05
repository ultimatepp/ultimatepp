#include <Core/Core.h>
#include <Core/SSL/SSL.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
    StdLogSetup(LOG_COUT | LOG_FILE);
    
    const String plaintext = "Secret message: Doomsday code is 12345.";
    const String password = "password123456!@#";
    
    String encrypted, decrypted;
    
    // Encrypt
    if(encrypted = AES256Encrypt(plaintext, password); !encrypted.IsVoid()) {
        RLOG("Encryption successful.");
        RLOG("Encrypted data (hex): " << HexString(encrypted));
    }
    else {
        RLOG("Encryption failed.");
        return;
    }
    
    // Decrypt
    if(decrypted = AES256Decrypt(encrypted, password); !decrypted.IsVoid()) {
        RLOG("Decryption successful.");
        RLOG("Decrypted text: " << decrypted);
    }
    else {
        RLOG("Decryption failed.");
        return;
    }

    // Check if original matches decrypted
    if(decrypted == plaintext)
        RLOG("Roundtrip successful: Decrypted text matches original.");
    else
        RLOG("Roundtrip failed: Data mismatch.");
}
