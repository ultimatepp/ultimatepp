#ifndef _Core_SSL_RSA_h_
#define _Core_SSL_RSA_h_

#include <Core/SSL/SSL.h>

namespace Upp {

struct RSAPrivateKey : Moveable<RSAPrivateKey>
{
	RSA* priv = nullptr;
	
	RSAPrivateKey()			{}
	RSAPrivateKey(const String& pem_string)
							{ SetKey(pem_string); }
	~RSAPrivateKey();
	
	operator RSA*() const	{ return priv; }
	operator bool() const	{ return priv != nullptr; }
	
	bool SetKey(const String& pem_string);
	String GetKey() const;
	bool Generate(int bits, BN_ULONG e = RSA_F4);
	RSA* GetPublicKey() const;

	String SignRS256(const String& text) const;
};

struct RSAPublicKey : Moveable<RSAPublicKey>
{
	RSA *pub = nullptr;
	
	RSAPublicKey()		{}
	RSAPublicKey(const String& pem_string)
							{ SetKey(pem_string); }
	RSAPublicKey(RSA* pubkey)
							{ pub = pubkey; }
	~RSAPublicKey();
	
	operator RSA*() const	{ return pub; }
	operator bool() const	{ return pub != nullptr; }
	
	bool SetKey(const String& pem);
	String GetKey() const;
	
	bool VerifyRS256(const String& text, const String& signature) const;
};

} // Upp

#endif
