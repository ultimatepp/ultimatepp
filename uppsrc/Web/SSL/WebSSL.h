#ifndef __Web_SSL__
#define __Web_SSL__

#ifndef flagNOSSL

#include <Web/Web.h>

#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/err.h>

NAMESPACE_UPP

void  *SSLAlloc(size_t size);
void   SSLFree(void *ptr);
void  *SSLRealloc(void *ptr, size_t size);

String SSLGetLastError();
String SSLToString(X509_NAME *name);
String ASN1ToString(ASN1_STRING *time);
Date   ASN1ToDate(ASN1_STRING *time);

//void   SSLInit();

class SSLBuffer
{
public:
	SSLBuffer(BUF_MEM *m = NULL) : buf_mem(m) {}
	~SSLBuffer()                              { Clear(); }

	bool     IsEmpty() const                  { return !buf_mem; }

	bool     Set(BUF_MEM *b)                  { Clear(); return !!(buf_mem = b); }
	bool     Create()                         { return Set(BUF_MEM_new()); }
	void     Clear()                          { if(buf_mem) { BUF_MEM_free(buf_mem); buf_mem = NULL; } }
	BUF_MEM *Detach()                         { BUF_MEM *b = buf_mem; buf_mem = NULL; return b; }

	bool     Grow(int length);

	String   Get() const;
	bool     Set(String d);

	operator BUF_MEM * () const { return buf_mem; }

private:
	BUF_MEM *buf_mem;
};

class SSLStream
{
public:
	SSLStream(BIO *b = NULL) : bio(b) {}
	~SSLStream()                       { Clear(); }

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

class SSLKey
{
public:
	SSLKey(EVP_PKEY *k = NULL) : key(k) {}
	~SSLKey()                           { Clear(); }

	bool      IsEmpty() const            { return !key; }

	bool      Set(EVP_PKEY *k)           { Clear(); return !!(key = k); }
	void      Clear()                    { if(key) { EVP_PKEY_free(key); key = NULL; } }
	EVP_PKEY *Detach()                   { EVP_PKEY *k = key; key = NULL; return k; }

	operator  EVP_PKEY * () const        { return key; }

	bool      Load(String data);

private:
	EVP_PKEY *key;
};

class SSLCertificate
{
public:
	SSLCertificate(X509 *c = NULL) : cert(c) {}
	~SSLCertificate()                        { Clear(); }

	bool     IsEmpty() const                 { return !cert; }

	bool     Set(X509 *c)                    { Clear(); return !!(cert = c); }
	bool     Create()                        { return Set(X509_new()); }
	void     Clear()                         { if(cert) { X509_free(cert); cert = NULL; } }
	X509    *Detach()                        { X509 *c = cert; cert = NULL; return c; }

	bool     Load(String data, bool asn1 = false);
	String   Save(bool asn1 = false) const;

	String   GetSubjectName() const;
	String   GetIssuerName() const;
	Date     GetNotBefore() const;
	Date     GetNotAfter() const;
	int      GetVersion() const;
	String   GetSerialNumber() const;
//	String   GetSignatureType() const;
//	String   GetPublicKey() const;
//	String   GetAuthority() const;
//	String   GetHashType() const;
//	String   GetHash() const;

	operator X509 * () const                 { return cert; }

private:
	X509    *cert;
};

class SSLContext
{
public:
	SSLContext(SSL_CTX *c = NULL);
	~SSLContext()                              { Clear(); }

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

bool SSLServerSocket(Socket& socket, SSLContext& ssl_context, int port, bool nodelay = true, int listen_count = 5, bool is_blocking = true);
bool SSLClientSocket(Socket& socket, SSLContext& ssl_context, const char *host, int port,
                     bool nodelay = true, dword *my_addr = NULL, int timeout = DEFAULT_CONNECT_TIMEOUT,
                     bool is_blocking = true);


bool SSLClientSocketUnsecured(Socket& socket, SSLContext& ssl_context, const char *host, int port,
                              bool nodelay = true, dword *my_addr = NULL, int timeout = DEFAULT_CONNECT_TIMEOUT,
                              bool is_blocking = true);
bool SSLSecureSocket(Socket& socket);

static const int SOCKKIND_SSL = 1; // GetKind() for SSL socket

inline String SSLInfoCipher()           { return "SSL_CIPHER"; }             // String
inline String SSLInfoCertAvail()        { return "SSL_CERT_AVAIL"; }         // bool
inline String SSLInfoCertVerified()     { return "SSL_CERT_VERIFIED"; }      // bool
inline String SSLInfoCertSubjectName()  { return "SSL_CERT_SUBJECT_NAME"; }  // String
inline String SSLInfoCertIssuerName()   { return "SSL_CERT_ISSUER_NAME"; }   // String
inline String SSLInfoCertNotBefore()    { return "SSL_CERT_NOT_BEFORE"; }    // Date
inline String SSLInfoCertNotAfter()     { return "SSL_CERT_NOT_AFTER"; }     // Date
inline String SSLInfoCertVersion()      { return "SSL_CERT_VERSION"; }       // int
inline String SSLInfoCertSerialNumber() { return "SSL_CERT_SERIAL_NUMBER"; } // String

class HttpsClient : public HttpClient {
public:
	HttpsClient();
	
	void            Secure(bool s) { secure = s; }
	
	virtual bool    CreateClientSocket();
	virtual bool    IsSecure();

public:
	One<SSLContext> ssl_context;
	
	bool            secure;
	
	enum { DEFAULT_HTTPS_PORT = 443 };

private:
	typedef HttpsClient CLASSNAME;

	bool            ProxyConnect();
};

END_UPP_NAMESPACE

#endif//flagNOSSL

#endif//__Web_SSL__
