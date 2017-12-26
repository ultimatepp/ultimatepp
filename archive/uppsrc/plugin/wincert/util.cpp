#include "wincert.h"

String CertGetProperty(PCCERT_CONTEXT cert_context, DWORD propertyID)
{
	dword len = 0;
	if(!CertGetCertificateContextProperty(cert_context, propertyID, NULL, &len))
		return String::GetVoid();
	Buffer<byte> prop(len);
	if(!CertGetCertificateContextProperty(cert_context, propertyID, prop, &len))
		return String::GetVoid();
	return String(~prop, len);
}

String CertGetName(PCCERT_CONTEXT cert_context, DWORD dwType, DWORD flags)
{
	dword len = CertGetNameString(cert_context, dwType, flags, NULL, NULL, NULL);
	if(!len)
		return String::GetVoid();
	Buffer<char> name(len);
	if(!(len = CertGetNameString(cert_context, dwType, flags, NULL, name, len)))
		return String::GetVoid();
	return String(name, len - 1);
}

String CertGetNameAttr(PCCERT_CONTEXT cert_context, const char *attrOID)
{
	dword len = CertGetNameString(cert_context, CERT_NAME_ATTR_TYPE, 0, (void *)attrOID, NULL, NULL);
	if(!len)
		return String::GetVoid();
	Buffer<char> name(len);
	if(!(len = CertGetNameString(cert_context, CERT_NAME_ATTR_TYPE, 0, (void *)attrOID, name, len)))
		return String::GetVoid();
	return String(name, len - 1);
}

/*
String CertGetHash(PCCERT_CONTEXT cert_context)
{
	dword hash_len = 0;
	CertGetCertificateContextProperty(cert_context, CERT_HASH_PROP_ID, NULL, &hash_len);
	Buffer<byte> hash(hash_len);
	CertGetCertificateContextProperty(cert_context, CERT_HASH_PROP_ID, hash, &hash_len);
	String cert_hash(~hash, hash_len);
	return cert_hash;
}
*/

/*
String CertGetName(PCCERT_CONTEXT cert_context)
{
	dword type;
	dword len = CertGetNameString(cert_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, &type, NULL, NULL);
	Buffer<char> name(len);
	CertGetNameString(cert_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, &type, name, len);
	String cert_name(name, len ? len - 1 : 0);
	return cert_name;
}
*/

CertInfo CertGetInfo(PCCERT_CONTEXT cert_context)
{
	CertInfo out;
	out.hash = CertGetProperty(cert_context, CERT_HASH_PROP_ID);
	out.common_name = CertGetNameAttr(cert_context, szOID_COMMON_NAME);
	out.organization = CertGetNameAttr(cert_context, szOID_ORGANIZATION_NAME);
	out.organizational_unit = CertGetNameAttr(cert_context, szOID_ORGANIZATIONAL_UNIT_NAME);
	out.email = CertGetNameAttr(cert_context, szOID_RSA_emailAddr);
	out.issuer = CertGetName(cert_context, CERT_NAME_FRIENDLY_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
	out.valid_from = Time(cert_context->pCertInfo->NotBefore);
	out.valid_until = Time(cert_context->pCertInfo->NotAfter);
//	out.valid_until = Cert
	return out;
}

static void _sCertSignSet(PCCERT_CONTEXT cert_context, CRYPT_SIGN_MESSAGE_PARA& sign)
{
	Zero(sign);
	sign.cbSize = sizeof(sign);
	sign.dwMsgEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
	sign.pSigningCert = cert_context;
	sign.HashAlgorithm.pszObjId = szOID_RSA_SHA1RSA;
	sign.HashAlgorithm.Parameters.cbData = NULL;
	sign.cMsgCert = 1;
	sign.rgpMsgCert = &sign.pSigningCert;
	sign.cAuthAttr = 0;
	sign.dwInnerContentType = 0;
	sign.cMsgCrl = 0;
	sign.cUnauthAttr = 0;
	sign.dwFlags = 0;
	sign.pvHashAuxInfo = NULL;
	sign.rgAuthAttr = NULL;
}

int CertSignLength(PCCERT_CONTEXT cert_context, Vector<const BYTE *>& sections, Vector<DWORD>& sizes)
{
	ASSERT(sections.GetCount() == sizes.GetCount());

	CRYPT_SIGN_MESSAGE_PARA sign;

	_sCertSignSet(cert_context, sign);

	const BYTE **psect = sections.Begin();
	DWORD *psize = sizes.Begin();

	DWORD out_size = 0;
	if(!CryptSignMessage(&sign, TRUE, sections.GetCount(), psect, psize, NULL, &out_size))
		return 0;
	return out_size;
}

String CertSign(PCCERT_CONTEXT cert_context, Vector<const BYTE *>& sections, Vector<DWORD>& sizes)
{
	ASSERT(sections.GetCount() == sizes.GetCount());

	CRYPT_SIGN_MESSAGE_PARA sign;

	_sCertSignSet(cert_context, sign);

	const BYTE **psect = sections.Begin();
	DWORD *psize = sizes.Begin();

	DWORD out_size = 0;
	if(!CryptSignMessage(&sign, TRUE, sections.GetCount(), psect, psize, NULL, &out_size))
		return String::GetVoid();
	Buffer<byte> out_data(out_size);
	if(!CryptSignMessage(&sign, TRUE, sections.GetCount(), psect, psize, out_data, &out_size))
		return String::GetVoid();

	return String(~out_data, out_size);
}

String CertSign(PCCERT_CONTEXT context, const String& message)
{
	Vector<const BYTE *> sections;
	Vector<DWORD> sizes;
	sections.SetCount(1, (const BYTE *)message.Begin());
	sizes.SetCount(1, message.GetLength());
	return CertSign(context, sections, sizes);
}

Array<CertInfo> CertEnum(HCERTSTORE store)
{
	Array<CertInfo> out_list;

	RLOG("store = " << FormatIntHex(store));

	PCCERT_CONTEXT cert_context = NULL;
	while(cert_context = CertEnumCertificatesInStore(store, cert_context)) {
		out_list.Add() = CertGetInfo(cert_context);
	}

	return out_list;
}

PCCERT_CONTEXT CertFind(HCERTSTORE store, const String& hash)
{
	CRYPT_HASH_BLOB hash_blob;
	hash_blob.pbData = (BYTE *)hash.Begin();
	hash_blob.cbData = hash.GetLength();

	return CertFindCertificateInStore(store, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		0, CERT_FIND_HASH, &hash_blob, NULL);
}
