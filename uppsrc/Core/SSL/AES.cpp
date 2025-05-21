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
	constexpr const int AES_GCM_HEADER_SIZE       = AES_GCM_PREFIX_LEN + AES_GCM_SALT_SIZE + AES_GCM_IV_SIZE;
	constexpr const int AES_GCM_ENVELOPE_SIZE     = AES_GCM_HEADER_SIZE + AES_GCM_TAG_SIZE;

const EVP_CIPHER* GetAes256GcmCipher()
{
	struct sCipher {
		EVP_CIPHER *ptr;
		sCipher()  { ptr = EVP_CIPHER_fetch(nullptr, "AES-256-GCM", nullptr); }
		~sCipher() { if(ptr) EVP_CIPHER_free(ptr); }
	};
	return Single<sCipher>().ptr;
}
	
}

Aes256Gcm::Aes256Gcm()
: iteration(AES_GCM_DEFAULT_ITERATION)
, chunksize(1024)
{
	SslInitThread();
	ctx = EVP_CIPHER_CTX_new();
	cipher = GetAes256GcmCipher();
}

Aes256Gcm::~Aes256Gcm()
{
	if(ctx)
		EVP_CIPHER_CTX_free(ctx);
}

void Aes256Gcm::SetError(const String& txt)
{
	err = txt;
	LLOG("Aes256Gcm: " << txt);
}

bool Aes256Gcm::Init(Stream& in)
{
	err.Clear();

	if(!ctx) {
		SetError("Failed to create context");
		return false;
	}
	
	if(!EVP_CIPHER_CTX_reset(ctx)) {
	    SetError("Failed to reset context");
		return false;
	}
	
	if(!cipher) {
		SetError("Failed to fetch AES-256-GCM cipher");
		return false;
	}
	
	// Require size information
	if(in.GetSize() <= 0) {
		SetError("Invalid stream size or no data to encrypt/decrypt");
		return false;
	}
	
	return true;
}

bool Aes256Gcm::GenerateSaltAndIV(String& salt, String& iv)
{
	salt = SecureRandom(AES_GCM_SALT_SIZE);
	iv    = GetAESGCMNonce();
	return !salt.IsVoid() && !iv.IsVoid();
}

bool Aes256Gcm::DeriveKey(const String& password, const String& salt, byte *key, int keylen)
{
	return PKCS5_PBKDF2_HMAC(~password, password.GetLength(), (const byte*) ~salt, salt.GetLength(), iteration, EVP_sha256(), keylen, key);
}

int Aes256Gcm::WriteHeader(Stream& out, const String& salt, const String& iv)
{
	out.Put(AES_GCM_FORMAT_PREFIX, AES_GCM_PREFIX_LEN);
	out.Put(salt);
	out.Put(iv);
	return AES_GCM_HEADER_SIZE;
}

bool Aes256Gcm::EncryptStream(Stream& in, Stream& out, int64& processed)
{
	Buffer<byte> buffer(chunksize);
	int buflen = 0;
	
	while(!in.IsEof()) {
		String chunk = in.Get(chunksize);
		if(chunk.IsEmpty())
			break;
	
		if(!EVP_EncryptUpdate(ctx, buffer, &buflen, (const byte*) ~chunk, chunk.GetLength()))
			return false;
	
		if(buflen > 0)
			out.Put(buffer, buflen);
	
		processed += chunk.GetLength();
		if(WhenProgress(processed, in.GetSize() + AES_GCM_ENVELOPE_SIZE))
			return false;
	}
	return true;
}

bool Aes256Gcm::FinalizeEncryption(Stream& out, int64& processed)
{
	byte tag[AES_GCM_TAG_SIZE];
	Buffer<byte> buffer(chunksize);
	int buflen = 0;

	if(!EVP_EncryptFinal_ex(ctx, buffer, &buflen))
		return false;

	if(buflen > 0) {
		out.Put(buffer, buflen);
		processed += buflen;
	}

	OSSL_PARAM params[] = {
		OSSL_PARAM_construct_octet_string("tag", tag, sizeof(tag)),
		OSSL_PARAM_construct_end()
	};
	
	if(!EVP_CIPHER_CTX_get_params(ctx, params))
		return false;

	out.Put(tag, sizeof(tag));
	processed += sizeof(tag);
	return true;
}


bool Aes256Gcm::Encrypt(Stream& in, const String& password, Stream& out)
{
	if(!Init(in))
		return false;

	byte key[AES_GCM_KEY_SIZE];
	String salt, iv;

	try {
		if(!GenerateSaltAndIV(salt, iv))
			throw Exc("Salt/IV generation failed");

		if(!DeriveKey(password, salt, key, sizeof(key)))
			throw Exc("Key derivation failed");

		if(!EVP_EncryptInit_ex2(ctx, cipher, key, (const byte*)~iv, nullptr))
			throw Exc("Cipher initialization failed");

		int64 processed = WriteHeader(out, salt, iv);

		if(WhenProgress(processed, in.GetSize() + AES_GCM_ENVELOPE_SIZE))
			throw Exc("Encryption aborted");

		if(!EncryptStream(in, out, processed))
			throw Exc("Encryption failed");

		if(!FinalizeEncryption(out, processed))
			throw Exc("Finalization failed");

		if(WhenProgress(processed, in.GetSize() + AES_GCM_ENVELOPE_SIZE))
			throw Exc("Encryption aborted");

		// Leave no trace
		OPENSSL_cleanse(key, sizeof(key));
		return true;
	}
	catch(const Exc& e) {
		SetError(e);
	}
	catch(...) {
		SetError("Unknown exception");
	}

	// Leave no trace
	OPENSSL_cleanse(key, sizeof(key));
	return false;
}

bool Aes256Gcm::ReadHeader(Stream& in, String& salt, String& iv)
{
	if(String header = in.Get(AES_GCM_PREFIX_LEN); header.GetLength() < AES_GCM_PREFIX_LEN || !header.StartsWith(AES_GCM_FORMAT_PREFIX)) {
		SetError("Invalid format");
		return false;
	}
	
	if(salt = in.Get(AES_GCM_SALT_SIZE); salt.GetLength() < AES_GCM_SALT_SIZE) {
		SetError("Failed to read salt");
		return false;
	}
	
	if(iv = in.Get(AES_GCM_IV_SIZE); iv.GetLength() < AES_GCM_IV_SIZE) {
		SetError("Failed to read initialization vector");
		return false;
	}

	if(int64 ctlen = in.GetSize() - AES_GCM_ENVELOPE_SIZE; ctlen <= 0) {
		SetError("Encrypted input is too short");
		return false;
	}

	return true;
}

bool Aes256Gcm::DecryptStream(Stream& in, Stream& out, int64& processed)
{
	Buffer<byte> buffer(chunksize);
	int buflen = 0;
	processed = AES_GCM_ENVELOPE_SIZE;
	int64 remaining = in.GetSize() - AES_GCM_ENVELOPE_SIZE;

	if(WhenProgress(processed, in.GetSize()))
		return false;

	while(remaining > 0) {
		int64 csz = min(static_cast<int64>(chunksize), remaining);
		String chunk = in.Get(static_cast<int>(csz)); // Chunk size is guaranteed to be smaller than INT_MAX
		if(chunk.IsEmpty())
			break;
		
		// Decrypt the chunk
		if(!EVP_DecryptUpdate(ctx, buffer, &buflen, (const byte*) ~chunk, chunk.GetLength()))
			return false;
		
		if(buflen > 0)
			out.Put(buffer, buflen);

		processed += chunk.GetLength();
		remaining -= chunk.GetLength();

		if(WhenProgress(processed, in.GetSize()))
			return false;
	}

	return true;
}

bool Aes256Gcm::ReadTag(Stream& in, String& tag)
{
	if(tag = in.Get(AES_GCM_TAG_SIZE); tag.GetLength() < AES_GCM_TAG_SIZE)
		return false;
	return in.IsEof(); // There should be no trailing data
}

bool Aes256Gcm::SetGcmTag(const String& tag)
{
	OSSL_PARAM params[] = {
		OSSL_PARAM_construct_octet_string("tag", (void*) ~tag, AES_GCM_TAG_SIZE),
		OSSL_PARAM_construct_end()
	};
	
	return EVP_CIPHER_CTX_set_params(ctx, params);
}

bool Aes256Gcm::FinalizeDecryption(Stream& out)
{
	Buffer<byte> buffer(chunksize);
	int buflen = 0;

	if(!EVP_DecryptFinal_ex(ctx, buffer, &buflen))
		return false;

	if(buflen > 0)
		out.Put(buffer, buflen);
	return true;
}

bool Aes256Gcm::Decrypt(Stream& in, const String& password, Stream& out)
{
	if(!Init(in))
		return false;

	byte key[AES_GCM_KEY_SIZE];

	try {
		String salt, iv;
		int64 processed = 0;

		if(!ReadHeader(in, salt, iv))
			return false;

		if(!DeriveKey(password, salt, key, sizeof(key)))
			throw Exc("Key derivation failed");

		if(!EVP_DecryptInit_ex2(ctx, cipher, key, iv, nullptr))
			throw Exc("Initialization failed");

		if(!DecryptStream(in, out, processed))
			throw Exc("Decryption failed");
		
		String tag;
		if(!ReadTag(in, tag))
			throw Exc("Unable to retrieve authentication tag");

		if(!SetGcmTag(tag))
			throw Exc("Failed to set authentication tag");

		if(!FinalizeDecryption(out))
			throw Exc("Authentication failed");

		WhenProgress(in.GetSize(), in.GetSize());

		// Leave no trace
		OPENSSL_cleanse(&key, sizeof(key));
		return true;
	}
	catch(const Exc& e) {
		SetError(e);
	}
	catch(...) {
		SetError("Unknown exception");
	}
	
	// Leave no trace
	OPENSSL_cleanse(&key, sizeof(key));
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