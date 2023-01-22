#include "RSA.h"

namespace Upp {

RSAPrivateKey::~RSAPrivateKey()
{
	if (priv)
		RSA_free(priv);
}

bool RSAPrivateKey::SetKey(const String& pem_string)
{
	if (pem_string.IsEmpty())
		return false;
	BIO *bio = BIO_new_mem_buf((void *)~pem_string, pem_string.GetCount());
	if (bio) {
		priv = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
		if (!priv) {
			LOG(SslGetLastError());
		}
		BIO_free(bio);
	}
	return priv != nullptr;
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

RSAPublicKey::~RSAPublicKey()
{
	if (pub)
		RSA_free(pub);
}

bool RSAPublicKey::SetKey(const String& pem_string)
{
	if (pem_string.IsEmpty())
		return false;
	BIO *bio = BIO_new_mem_buf((void *)~pem_string, pem_string.GetCount());
	if (bio) {
		pub = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
		if (!pub) {
			LOG(SslGetLastError());
		}
		BIO_free(bio);
	}
	return pub != nullptr;
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
