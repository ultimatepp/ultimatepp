#ifndef _wincert_wincert_h
#define _wincert_wincert_h

#include <Web/Web.h>
#include <WinCrypt.h>

using namespace Upp;

struct CertInfo {
	String hash;
	String common_name;
	String organization;
	String organizational_unit;
	String email;
	String issuer;
	Time   valid_from;
	Time   valid_until;
};

String CertGetProperty(PCCERT_CONTEXT cert_context, DWORD propertyID);
String CertGetName(PCCERT_CONTEXT cert_context, DWORD dwType, DWORD flags = 0);
String CertGetNameAttr(PCCERT_CONTEXT cert_context, const char *attrOID);
CertInfo CertGetInfo(PCCERT_CONTEXT cert_context);

Array<CertInfo> CertEnum(HCERTSTORE store);
PCCERT_CONTEXT CertFind(HCERTSTORE store, const String& hash);

int CertSignLength(PCCERT_CONTEXT cert_context, Vector<const BYTE *>& sections, Vector<DWORD>& sizes);
String CertSign(PCCERT_CONTEXT cert_context, Vector<const BYTE *>& sections, Vector<DWORD>& sizes);
String CertSign(PCCERT_CONTEXT context, const String& message);

String CertSignPDF(PCCERT_CONTEXT cert_context, const String& pdf_data);

#endif
