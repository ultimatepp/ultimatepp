#include "SSL.h"

namespace Upp {

String GetP7Signature_imp(const void *data, int length, const String& cert_pem, const String& pkey_pem)
{
	SslCertificate x509;
	SslKey pkey;
	if(!x509.Load(cert_pem))
		return Null;
	if(!pkey.Load(pkey_pem))
		return Null;

	SslStream in;
	in.OpenBuffer((const char *)data, length);

    PKCS7 *p7 = PKCS7_sign(x509, pkey, NULL, in, PKCS7_DETACHED|PKCS7_BINARY|PKCS7_STREAM);

	if (!p7)
		return Null;

	SslStream out;
	out.CreateBuffer();

	String r;
    if (SMIME_write_PKCS7(out, p7, in, PKCS7_DETACHED|PKCS7_BINARY|PKCS7_STREAM)) {
		SslStream out1;
		out1.CreateBuffer();
		i2d_PKCS7_bio(out1, p7);
		r = out1.GetResult();
    }
  
    PKCS7_free(p7);
    
	return r;
}

extern String (*GetP7Signature__)(const void *data, int length, const String& cert_pem, const String& pkey_pem);

INITIALIZER(P7S)
{
	GetP7Signature__ = GetP7Signature_imp;
}

}
