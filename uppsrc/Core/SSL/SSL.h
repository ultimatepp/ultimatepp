#include <Core/Core.h>

#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/engine.h>

NAMESPACE_UPP

void SslInitThread();

class SslBuffer
{
public:
	SslBuffer(BUF_MEM *m = NULL) : buf_mem(m) {}
	~SslBuffer()                              { Clear(); }

	bool     IsEmpty() const                  { return !buf_mem; }

	bool     Set(BUF_MEM *b)                  { Clear(); return !!(buf_mem = b); }
	bool     Create()                         { return Set(BUF_MEM_new()); }
	void     Clear()                          { if(buf_mem) { BUF_MEM_free(buf_mem); buf_mem = NULL; } }
	BUF_MEM *Detach()                         { BUF_MEM *b = buf_mem; buf_mem = NULL; return b; }

	bool     Grow(int length);

	String   Get() const;
	bool     Set(const String& d);

	operator BUF_MEM * () const { return buf_mem; }

private:
	BUF_MEM *buf_mem;
};

class SslStream
{
public:
	SslStream(BIO *b = NULL) : bio(b) {}
	~SslStream()                       { Clear(); }

	bool     IsEmpty() const           { return !bio; }

	bool     Set(BIO *b)               { Clear(); return !!(bio = b); }
	bool     Create(BIO_METHOD *meth)  { return Set(BIO_new(meth)); }
	void     Clear()                   { if(bio) { BIO_free(bio); bio = NULL; } }

	bool     OpenBuffer(const char *data, int length);
	bool     CreateBuffer();
	String   GetResult() const;

	operator BIO * () const            { return bio; }

private:
	BIO     *bio;
};

class SslKey
{
public:
	SslKey(EVP_PKEY *k = NULL) : key(k) {}
	~SslKey()                           { Clear(); }

	bool      IsEmpty() const            { return !key; }

	bool      Set(EVP_PKEY *k)           { Clear(); return !!(key = k); }
	void      Clear()                    { if(key) { EVP_PKEY_free(key); key = NULL; } }
	EVP_PKEY *Detach()                   { EVP_PKEY *k = key; key = NULL; return k; }

	operator  EVP_PKEY * () const        { return key; }

	bool      Load(const String& data);

private:
	EVP_PKEY *key;
};

class SslCertificate
{
public:
	SslCertificate(X509 *c = NULL) : cert(c) {}
	~SslCertificate()                        { Clear(); }

	bool     IsEmpty() const                 { return !cert; }

	bool     Set(X509 *c)                    { Clear(); return !!(cert = c); }
	bool     Create()                        { return Set(X509_new()); }
	void     Clear()                         { if(cert) { X509_free(cert); cert = NULL; } }
	X509    *Detach()                        { X509 *c = cert; cert = NULL; return c; }

	bool     Load(const String& data, bool asn1 = false);
	String   Save(bool asn1 = false) const;

	String   GetSubjectName() const;
	String   GetIssuerName() const;
	Date     GetNotBefore() const;
	Date     GetNotAfter() const;
	int      GetVersion() const;
	String   GetSerialNumber() const;

	operator X509 * () const                 { return cert; }

private:
	X509    *cert;
};

class SslContext
{
public:
	SslContext(SSL_CTX *c = NULL);
	~SslContext()                              { Clear(); }

	bool     IsEmpty() const                   { return !ssl_ctx; }

	bool     Set(SSL_CTX *c)                   { Clear(); return !!(ssl_ctx = c); }
	bool     Create(SSL_METHOD *meth)          { return Set(SSL_CTX_new(meth)); }
	void     Clear()                           { if(ssl_ctx) { SSL_CTX_free(ssl_ctx); ssl_ctx = NULL; } }
	SSL_CTX *Detach()                          { SSL_CTX *c = ssl_ctx; ssl_ctx = NULL; return c; }

	operator SSL_CTX * () const                { return ssl_ctx; }

	bool     CipherList(const char *list);
	bool     UseCertificate(String certificate, String private_key, bool cert_asn1 = false);
	void     VerifyPeer(bool verify = true, int depth = 2);

private:
	SSL_CTX *ssl_ctx;
};

String SslGetLastError(int& code);
String SslGetLastError();
String SslToString(X509_NAME *name);
Date   Asn1ToDate(ASN1_STRING *time);
String Asn1ToString(ASN1_STRING *s);

END_UPP_NAMESPACE
