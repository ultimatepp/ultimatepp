#include "RSA.h"

// implemented according to openssl 1.1 manual:
// https://www.openssl.org/docs/man1.1.1/man3/RSA_sign.html

namespace Upp {

// helpers

struct Scoped_BIGNUM
{
	BIGNUM* bn;
	Scoped_BIGNUM() : bn(BN_new()) {}
	~Scoped_BIGNUM() { if (bn) BN_free(bn); }
	operator BIGNUM*() const { return bn; }
	operator bool() const { return bn != nullptr; }
};

struct Scoped_BIO
{
	BIO* bio;
	Scoped_BIO() : bio(BIO_new(BIO_s_mem())) {}
	Scoped_BIO(const String& string)
		: bio(BIO_new_mem_buf((void *)~string, string.GetCount())) {};
	~Scoped_BIO() { if (bio) BIO_free(bio); }
	operator BIO*() const { return bio; }
	operator bool() const { return bio != nullptr; }
};

// private key

RSAPrivateKey::~RSAPrivateKey()
{
	if (priv)
		RSA_free(priv);
}

bool RSAPrivateKey::SetKey(const String& pem_string)
{
	if (pem_string.IsEmpty())
		return false;
	Scoped_BIO bio(pem_string);
	if (bio) {
		priv = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
		if (!priv) {
			LOG(SslGetLastError());
		}
	}
	return priv != nullptr;
}

bool RSAPrivateKey::Generate(int bits, BN_ULONG e)
{
	if (priv) {
		RSA_free(priv);
		priv = nullptr;
	}
	Scoped_BIGNUM bne;
	if (!bne)
		return false;
	if (!BN_set_word(bne, e)) {
		return false;
	}
	priv = RSA_new();
	if (!priv) {
		LOG(SslGetLastError());
		return false;
	}
	if (!RSA_generate_key_ex(priv, bits, bne, NULL)) { // bit unsure about the need for seeding RAND here
		LOG(SslGetLastError());
		RSA_free(priv);
		priv = nullptr;
		return false;
	}
	return true;
}

String RSAPrivateKey::GetKey() const
{
	if (!priv)
		return String::GetVoid();
	String key = String::GetVoid();
	Scoped_BIO bio;
	if (bio) {
		if (PEM_write_bio_RSAPrivateKey(bio, priv, NULL, NULL, 0, NULL, NULL)) {
			char* ptr = nullptr;
			auto len = BIO_get_mem_data(bio, &ptr);
			if (len > 0) {
				ASSERT(ptr != nullptr);
				key = String(ptr, len);
			}
		}
	}
	return key;
}

RSA* RSAPrivateKey::GetPublicKey() const
{
	RSA* pub = RSAPublicKey_dup(priv);
	if (!pub) {
		LOG(SslGetLastError());
	}
	return pub;
}

String RSAPrivateKey::SignRS256(const String& message) const
{
	if (!priv)
		return String::GetVoid();

	byte hash[SHA256_DIGEST_LENGTH];
	SHA256(hash, message);

	Buffer<byte> sig(RSA_size(priv));
	unsigned int sigLen;

	if (RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, ~sig, &sigLen, priv))
		return String(~sig, sigLen);
	LOG(SslGetLastError());
	return String::GetVoid();
}

// public key

RSAPublicKey::~RSAPublicKey()
{
	if (pub)
		RSA_free(pub);
}

bool RSAPublicKey::SetKey(const String& pem_string)
{
	if (pem_string.IsEmpty())
		return false;
	Scoped_BIO bio(pem_string);
	if (bio) {
		pub = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
		if (!pub) {
			LOG(SslGetLastError());
		}
	}
	return pub != nullptr;
}

String RSAPublicKey::GetKey() const
{
	if (!pub)
		return String::GetVoid();
	String key = String::GetVoid();
	Scoped_BIO bio;
	if (bio) {
		if (PEM_write_bio_RSAPublicKey(bio, pub)) {
			char* ptr = nullptr;
			auto len = BIO_get_mem_data(bio, &ptr);
			if (len > 0) {
				ASSERT(ptr != nullptr);
				key = String(ptr, len);
			}
		}
	}
	return key;
}

bool RSAPublicKey::VerifyRS256(const String& message, const String& signature) const
{
	if (signature.IsEmpty())
		return false;

	byte hash[SHA256_DIGEST_LENGTH];
	SHA256(hash, message);

	// Verify the message
	if (RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, (const byte*)~signature, (unsigned)signature.GetCount(), pub))
		return true;
	LOG(SslGetLastError());
	return false;
}

} // Upp
