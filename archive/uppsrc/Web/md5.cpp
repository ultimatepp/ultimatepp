#include "Web.h"

namespace Upp {

String MD5Digest(const char *text, int length)
{
	unsigned char digest[16];
	MD5(digest, text, length);
	return String(digest, 16);
}

}
