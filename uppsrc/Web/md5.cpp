#include "Web.h"

NAMESPACE_UPP

String MD5Digest(const char *text, int length)
{
	unsigned char digest[16];
	MD5(digest, text, length);
	return String(digest, 16);
}

END_UPP_NAMESPACE
