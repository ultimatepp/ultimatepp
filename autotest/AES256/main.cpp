#include <Core/Core.h>
#include <Core/SSL/SSL.h>

using namespace Upp;


CONSOLE_APP_MAIN
{
    StdLogSetup(LOG_FILE | LOG_COUT);
    
    const String password = "password@!^+1234";
    const String original = "data: username=admin; password=12345!";
    
    // Encrypt must succeed and NOT match original
    String encrypted = AES256Encrypt(original, password);
    ASSERT(!encrypted.IsVoid());
    ASSERT(encrypted != original);

     // Decrypt must succeed and match original
    String decrypted = AES256Decrypt(encrypted, password);
    ASSERT(decrypted == original);

    // Wrong password should fail
    ASSERT(AES256Decrypt(encrypted, "wrong_password").IsVoid());

    // Empty string encryption/decryption
    encrypted = AES256Encrypt("", password);
    ASSERT(encrypted.IsVoid());
    decrypted = AES256Decrypt(encrypted, password);
    ASSERT(decrypted.IsVoid());

    // Multiple encryptions of same blob yield different cipherblobs (non-deterministic output)
    String encrypted2 = AES256Encrypt(original, password);
    String encrypted3 = AES256Encrypt(original, password);
    ASSERT(encrypted2 != encrypted3);

    // Decrypt both variants
    ASSERT(AES256Decrypt(encrypted2, password) == original);
    ASSERT(AES256Decrypt(encrypted3, password) == original);

    // Allow empty passwords
    encrypted = AES256Encrypt(original, Null);
    decrypted = AES256Decrypt(encrypted, Null);
    ASSERT(original == decrypted);

	// Tampering
    StringBuffer tampered = encrypted;
    tampered[tampered.GetLength() - 1]^= 0x01;
    ASSERT(AES256Decrypt(tampered, password).IsVoid());

    // Large input (to ensure stream-based encryption works)
    String large_string('A', 1024 * 1024 * 4);
    encrypted = AES256Encrypt(large_string, password);
    decrypted = AES256Decrypt(encrypted, password);
    ASSERT(!encrypted.IsVoid());
    ASSERT(!decrypted.IsVoid());
    ASSERT(large_string == decrypted);

    // Large input + tampering
    tampered = encrypted;
    tampered[tampered.GetLength() - 1] ^= 0x01;
    ASSERT(AES256Decrypt(tampered, password).IsVoid());

	{
	    const String filename = GetTempFileName();
		{
			// Write encrypted blob to a file
			MemReadStream ms(large_string, large_string.GetLength());
			FileOut fo(filename);
			ASSERT(AES256Encrypt(ms, password, fo));
		}
		{
			// Read encrypted blob from a file
			StringStream ss;
			FileIn fi(filename);
			ASSERT(AES256Decrypt(fi, password, ss));
			ASSERT(ss.GetResult() == large_string);
		}
		
		if(FileExists(filename))
			DeleteFile(filename);
	}

	LOG("================ OK");
}
