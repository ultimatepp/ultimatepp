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
	
	operator bool() const	{ return priv != nullptr; }
	
	bool SetKey(const String& pem_string);
	String SignRS256(const String& text) const;
};

struct RSAPublicKey : Moveable<RSAPublicKey>
{
	RSA *pub = nullptr;
	
	RSAPublicKey()			{}
	RSAPublicKey(const String& pem_string)
							{ SetKey(pem_string); }
	~RSAPublicKey();
	
	operator bool() const	{ return pub != nullptr; }
	
	bool SetKey(const String& pem);
	bool VerifyRS256(const String& text, const String& signature) const;
};

} // Upp

#endif
