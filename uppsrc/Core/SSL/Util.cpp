#include "SSL.h"

namespace Upp {

String SslBuffer::Get() const
{
	if(IsEmpty())
		return String::GetVoid();
	return String(buf_mem->data, (int)buf_mem->length);
}

bool SslBuffer::Grow(int length)
{
	return !IsEmpty() && BUF_MEM_grow(buf_mem, length);
}

bool SslBuffer::Set(const String& d)
{
	if(!buf_mem && !Create())
		return false;
	int len = d.GetLength();
	if((int)buf_mem->max < len && !Grow(len))
		return false;
	ASSERT((int)buf_mem->max >= len);
	buf_mem->length = len;
	memcpy(buf_mem, d, len);
	return true;
}

bool SslStream::OpenBuffer(const char *data, int length)
{
	return Set(BIO_new_mem_buf(const_cast<char *>(data), length));
}

bool SslStream::CreateBuffer()
{
	Clear();
	SslBuffer buf;
	if(!buf.Create() || !Create(BIO_s_mem()))
		return false;
	BIO_set_mem_buf(bio, buf.Detach(), BIO_CLOSE);
	return true;
}

String SslStream::GetResult() const
{
	if(IsEmpty())
		return String::GetVoid();
	BUF_MEM *bm = NULL;
	BIO_get_mem_ptr(bio, &bm);
	if(!bm)
		return String::GetVoid();
	return String(bm->data, (int)bm->length);
}

bool SslKey::Load(const String& data)
{
	Clear();
	SslStream strm;
	if(!strm.OpenBuffer(data.Begin(), data.GetLength()))
		return false;
	return Set(PEM_read_bio_PrivateKey(strm, NULL, NULL, NULL));
}

bool SslCertificate::Load(const String& data, bool asn1)
{
	Clear();
	SslStream in, pem, *sio = &in;
	if(!in.OpenBuffer(data, data.GetLength()))
		return false;
	if(!asn1)
	{
		if(!pem.Create(BIO_f_base64()))
			return false;
		BIO_push(pem, in);
		sio = &pem;
	}
	return Set(d2i_X509_bio(*sio, NULL));
}

String SslCertificate::Save(bool asn1) const
{
	if(IsEmpty())
		return String::GetVoid();
	SslStream out, pem, *sio = &out;
	if(!out.CreateBuffer())
		return String::GetVoid();
	if(!asn1)
	{
		if(!pem.Create(BIO_f_base64()))
			return String::GetVoid();
		BIO_push(pem, out);
		sio = &pem;
	}
	i2d_X509_bio(*sio, cert);
	return out.GetResult();
}

String SslCertificate::GetSubjectName() const
{
	ASSERT(!IsEmpty());
	return SslToString(X509_get_subject_name(cert));
}

String SslCertificate::GetIssuerName() const
{
	ASSERT(!IsEmpty());
	return SslToString(X509_get_issuer_name(cert));
}

Date SslCertificate::GetNotBefore() const
{
	ASSERT(!IsEmpty());
	return Asn1ToDate(X509_get_notBefore(cert));
}

Date SslCertificate::GetNotAfter() const
{
	ASSERT(!IsEmpty());
	return Asn1ToDate(X509_get_notAfter(cert));
}

int SslCertificate::GetVersion() const
{
	ASSERT(!IsEmpty());
	return X509_get_version(cert);
}

String SslCertificate::GetSerialNumber() const
{
	ASSERT(!IsEmpty());
	return Asn1ToString(X509_get_serialNumber(cert));
}

SslContext::SslContext(SSL_CTX *c)
: ssl_ctx(c)
{
	SslInitThread();
}

bool SslContext::CipherList(const char *list)
{
	ASSERT(ssl_ctx);
	return SSL_CTX_set_cipher_list(ssl_ctx, list);
}

bool SslContext::UseCertificate(String certdata, String pkeydata, bool cert_asn1)
{
	ASSERT(ssl_ctx);
	if(IsNull(certdata) || IsNull(pkeydata))
		return false;
	SslCertificate cert;
	SslKey pkey;
	if(!cert.Load(certdata, cert_asn1) || !pkey.Load(pkeydata))
		return false;
	if(!SSL_CTX_use_certificate(ssl_ctx, cert) || !SSL_CTX_use_PrivateKey(ssl_ctx, pkey))
		return false;
	if(!SSL_CTX_check_private_key(ssl_ctx))
		return false;
	return true;
}

void SslContext::VerifyPeer(bool verify, int depth)
{
	ASSERT(ssl_ctx);
	SSL_CTX_set_verify(ssl_ctx, verify ? SSL_VERIFY_PEER : SSL_VERIFY_NONE, NULL);
	SSL_CTX_set_verify_depth(ssl_ctx, depth);
}

bool SslContext::UseCAcert(String ca_cert, bool cert_asn1)
{
    ASSERT(ssl_ctx);
    if(IsNull(ca_cert))
        return false;
    SslCertificate ca;
    if(!ca.Load(ca_cert, cert_asn1))
        return false;
    
    X509_STORE *castore = SSL_CTX_get_cert_store(ssl_ctx);
    if(!castore)
        return false;
    
    return X509_STORE_add_cert(castore, ca);
}

String SslGetLastError(int& code)
{
	char errbuf[150];
	ERR_error_string(code = ERR_get_error(), errbuf);
	return errbuf;
}

String SslGetLastError()
{
	int dummy;
	return SslGetLastError(dummy);
}

String SslToString(X509_NAME *name)
{
	char buffer[500];
	return X509_NAME_oneline(name, buffer, sizeof(buffer));
}

Date Asn1ToDate(ASN1_STRING *time)
{
	if(!time) return Null;
	int digit = 0;
	while(digit < time->length && IsDigit(time->data[digit]))
		digit++;
	if(digit < 6)
		return Null;
	int year2 = time->data[0] * 10 + time->data[1] - 11 * '0';
	int month = time->data[2] * 10 + time->data[3] - 11 * '0';
	int day   = time->data[4] * 10 + time->data[5] - 11 * '0';
	return Date(year2 + (year2 < 90 ? 2000 : 1900), month, day);
}

String Asn1ToString(ASN1_STRING *s)
{
	return String(s->data, s->length);
}

}
