#include "SSL.h"

// Encrypts a string using AES-256-GCM with PBKDF2 key derivation
// Format of encrypted data: "GCMv1__" + salt(16) + iv(12) + ciphertext + tag(16)

#define LLOG(x)  // DLOG(x)

namespace Upp {

namespace {
	constexpr const char* AES_GCM_FORMAT_PREFIX   = "GCMv1__";
	constexpr const int AES_GCM_PREFIX_LEN        = 7;
	constexpr const int AES_GCM_SALT_SIZE         = 16;
	constexpr const int AES_GCM_KEY_SIZE          = 32;
	constexpr const int AES_GCM_IV_SIZE           = 12;                // GCM standard IV size
	constexpr const int AES_GCM_TAG_SIZE          = 16;
    constexpr const int AES_GCM_BLOCK_SIZE        = 16;
	constexpr const int AES_GCM_ENVELOPE_SIZE     = AES_GCM_PREFIX_LEN + AES_GCM_SALT_SIZE + AES_GCM_IV_SIZE + AES_GCM_TAG_SIZE;
}

Aes256Gcm::Aes256Gcm()
: ctx(nullptr)
, cipher(nullptr)
, chunksize(4096)
, iteration(AES_GCM_DEFAULT_ITERATION)
{
	SslInitThread();
	
	ctx = EVP_CIPHER_CTX_new();
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	cipher = EVP_CIPHER_fetch(nullptr, "AES-256-GCM", nullptr);
#else
	cipher = const_cast<EVP_CIPHER*>(EVP_aes_256_gcm());
#endif
}

Aes256Gcm::~Aes256Gcm()
{
	if(ctx)
		EVP_CIPHER_CTX_free(ctx);
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if(cipher)
		EVP_CIPHER_free(cipher);
#endif
}

void Aes256Gcm::SetError(const String& txt)
{
	err = txt;
	ERR_clear_error();
	LLOG("Aes256Gcm: " << txt);
}

bool Aes256Gcm::Encrypt(Stream& in, const String& password, Stream& out)
{
	err.Clear();
	
	try {
		if(!ctx)
			throw Exc("Failed to create context");
	
		if(!cipher)
			throw Exc("Failed to fetch AES-256-GCM cipher");
		
		EVP_CIPHER_CTX_reset(ctx);
	
		// Require size information
		if(in.GetSize() <= 0)
			throw Exc("Invalid stream size or no data to encrypt");

	    SecureBuffer<byte> key(AES_GCM_KEY_SIZE);
	    byte salt[AES_GCM_SALT_SIZE], iv[AES_GCM_IV_SIZE], tag[AES_GCM_TAG_SIZE];
	    int64 done = 0;

		// Generate random salt
		if(!RAND_bytes(salt, sizeof(salt)))
			throw Exc("Salt generation failed");
		
		// Generate random initialization vector
		if(!RAND_bytes(iv, sizeof(iv)))
			throw Exc("IV generation failed");

		// Derive key from password (can be Null)
		if(!PKCS5_PBKDF2_HMAC(~password, password.GetLength(), salt, sizeof(salt), iteration, EVP_sha256(), key.GetSize(), ~key))
			throw Exc("PBKDF2: Key derivation failed");

		// Initialize cipher
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
		if(!EVP_EncryptInit_ex2(ctx, cipher, ~key, iv, nullptr))
			throw Exc("Cipher initialization failed");
#else
		if(!EVP_EncryptInit_ex(ctx, cipher, nullptr, ~key, iv))
			throw Exc("Cipher initialization failed");
#endif

		// Put header
		out.Put(AES_GCM_FORMAT_PREFIX, AES_GCM_PREFIX_LEN);
		out.Put(salt, sizeof(salt));
		out.Put(iv, sizeof(iv));
		
		// Update counter with header size
		done = AES_GCM_PREFIX_LEN + sizeof(salt) + sizeof(iv);
		const int64 total = in.GetSize() + AES_GCM_ENVELOPE_SIZE;
		
		if(WhenProgress(done, total))
			throw Exc("Encryption aborted");

        SecureBuffer<byte> temp(chunksize);
        SecureBuffer<byte> buffer(chunksize + AES_GCM_BLOCK_SIZE);
        int buflen = 0;

        for(;;) {
            int n = in.Get(~temp, chunksize);
            if(n <= 0)
				break;

            if(!EVP_EncryptUpdate(ctx, ~buffer, &buflen, ~temp, n))
                throw Exc("Encryption failed step update");

            if(buflen > 0)
                out.Put(~buffer, buflen);

            done += n;
            if(WhenProgress(done, total))
                throw Exc("Encryption aborted");
        }

        if(!EVP_EncryptFinal_ex(ctx, ~buffer, &buflen))
            throw Exc("Finalization padding step failed");

        if(buflen > 0)
            out.Put(~buffer, buflen);
		
		// Get GCM tag
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
		OSSL_PARAM params[] = {
			OSSL_PARAM_construct_octet_string("tag", tag, sizeof(tag)),
			OSSL_PARAM_construct_end()
		};
		if(!EVP_CIPHER_CTX_get_params(ctx, params))
			throw Exc("Failed to get tag");
#else
		if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag))
			throw Exc("Failed to get tag");
#endif

		// Put tag
		out.Put(tag, sizeof(tag));
		done += sizeof(tag);
		
		// Final progress (100%)
		if(WhenProgress(done, total))
			throw Exc("Encryption aborted");

		return true;
	}
	catch(const Exc& e) {
		if(ctx) EVP_CIPHER_CTX_reset(ctx);
		SetError(e);
	}
	catch(...) {
		if(ctx) EVP_CIPHER_CTX_reset(ctx);
		SetError("Unknown exception");
	}

	return false;
}

bool Aes256Gcm::Decrypt(Stream& in, const String& password, Stream& out)
{
	err.Clear();
	
	try {
		if(!ctx)
			throw Exc("Failed to create context");
		
		if(!cipher)
			throw Exc("Failed to fetch AES-256-GCM cipher");
		
		EVP_CIPHER_CTX_reset(ctx);
		
		// Require size information
		if(in.GetSize() <= 0)
			throw Exc("Invalid stream size or no data to decrypt");
			
		// Read and validate header
		if(String header = in.Get(AES_GCM_PREFIX_LEN); !header.StartsWith(AES_GCM_FORMAT_PREFIX))
			throw Exc("Invalid format");
		
		// Read salt
		String salt = in.Get(AES_GCM_SALT_SIZE);
		if(salt.GetLength() < AES_GCM_SALT_SIZE)
			throw Exc("Failed to read salt");
		
		// Read initialization vector
		String iv = in.Get(AES_GCM_IV_SIZE);
		if(iv.GetLength() < AES_GCM_IV_SIZE)
			throw Exc("Failed to read initialization vector");
	
		const int64 total = in.GetSize();
		
		// Get the size of the ciphertext content
		const int64 ciphertextlen = total - AES_GCM_ENVELOPE_SIZE;
		if(ciphertextlen <= 0)
			throw Exc("Encrypted input is too short");

		SecureBuffer<byte> key(AES_GCM_KEY_SIZE);
		
		// Derive key from password (can be Null)
		if(!PKCS5_PBKDF2_HMAC(~password, password.GetLength(), salt, AES_GCM_SALT_SIZE, iteration, EVP_sha256(), key.GetSize(), ~key))
			throw Exc("PBKDF2: Key derivation failed");
		
		// Init decryption
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
		if(!EVP_DecryptInit_ex2(ctx, cipher, ~key, iv, nullptr))
			throw Exc("Initialization failed");
#else
		if(!EVP_DecryptInit_ex(ctx, cipher, nullptr, ~key, iv))
			throw Exc("Initialization failed");
#endif

        SecureBuffer<byte> temp(chunksize);
        SecureBuffer<byte> buffer(chunksize + AES_GCM_BLOCK_SIZE);
        int buflen = 0;

        int64 remaining = ciphertextlen;
        int64 done = AES_GCM_PREFIX_LEN + AES_GCM_SALT_SIZE + AES_GCM_IV_SIZE;
		
		if(WhenProgress(done, total))
			throw Exc("Decryption aborted");
		
       // Loop purely over ciphertext payload
        while(remaining > 0) {
            int64 csz = min(static_cast<int64>(chunksize), remaining);
            int n = in.Get(~temp, static_cast<int>(csz));
            if(n <= 0)
                throw Exc("Unexpected end of stream payload");

            if(!EVP_DecryptUpdate(ctx, ~buffer, &buflen, ~temp, n))
                throw Exc("Decryption transformation processing step failed");

            if(buflen > 0)
                out.Put(~buffer, buflen);

            done += n;
            remaining -= n;

            if(WhenProgress(done, total))
                throw Exc("Decryption aborted");
        }

		String tag = in.Get(AES_GCM_TAG_SIZE);
		if(tag.GetLength() < AES_GCM_TAG_SIZE)
			throw Exc("Unable to retrieve authentication tag");
		
		if(in.GetLeft() > 0)
			throw Exc("Trailing data found after authentication tag");
		
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
		OSSL_PARAM params[] = {
			OSSL_PARAM_construct_octet_string("tag", (void*) ~tag, AES_GCM_TAG_SIZE),
			OSSL_PARAM_construct_end()
		};
		
		if(!EVP_CIPHER_CTX_set_params(ctx, params))
			throw Exc("Failed to set tag");
#else
		if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM_TAG_SIZE, (void*) ~tag))
			throw Exc("Failed to set tag");
#endif

		// Finalize decryption
		if(!EVP_DecryptFinal_ex(ctx, ~buffer, &buflen))
			throw Exc("Integrity check / Authentication failed");
		
		// Write final block (if any)
		if(buflen > 0)
			out.Put(~buffer, buflen);
		
		// Final progress (100%)
		done += AES_GCM_TAG_SIZE;
		if(WhenProgress(done, total))
			throw Exc("Decryption aborted");

		return true;
	}
	catch(const Exc& e) {
		if(ctx) EVP_CIPHER_CTX_reset(ctx);
		SetError(e);
	}
	catch(...) {
		if(ctx) EVP_CIPHER_CTX_reset(ctx);
		SetError("Unknown exception");
	}

	return false;
}

bool Aes256Gcm::EncDec(bool enc, const String& in, const String& pwd, String& out)
{
	StringStream sin(in), sout;
	if(bool b = enc ? Encrypt(sin, pwd, sout) : Decrypt(sin, pwd, sout); b) {
		out = sout.GetResult();
		return true;
	}
	return false;
}

String AES256Encrypt(const String& in, const String& password, Gate<int64, int64> WhenProgress)
{
	// Use sane defaults
	String out;
	Aes256Gcm aes;
	aes.WhenProgress = WhenProgress;
	return aes.Encrypt(in, password, out) ? out : String::GetVoid();
}

String AES256Decrypt(const String& in, const String& password, Gate<int64, int64> WhenProgress)
{
	// Use sane defaults
	String out;
	Aes256Gcm aes;
	aes.WhenProgress = WhenProgress;
	return aes.Decrypt(in, password, out) ? out : String::GetVoid();
}

bool AES256Encrypt(Stream& in, const String& password, Stream& out, Gate<int64, int64> WhenProgress)
{
	// Use sane defaults
	Aes256Gcm aes;
	aes.WhenProgress = WhenProgress;
	return aes.Encrypt(in, password, out);
}

bool AES256Decrypt(Stream& in, const String& password, Stream& out, Gate<int64, int64> WhenProgress)
{
	// Use sane defaults
	Aes256Gcm aes;
	aes.WhenProgress = WhenProgress;
	return aes.Decrypt(in, password, out);
}
	
	
}
