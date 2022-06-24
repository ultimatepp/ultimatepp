#include "clang.h"

String CleanupSignature(const String& signature)
{
	DTIMING("CleanupSignature");
	StringBuffer result;
	const char *s = signature;
	while(*s)
		if(s[0] == ' ' && s[1] == '*' && s[2] == ' ') {
			result.Cat(' ');
			result.Cat('*');
			s += 3;
		}
		else
		if(s[0] == ' ' && s[1] == '&' && s[2] == ' ') {
			result.Cat('&');
			result.Cat(' ');
			s += 3;
		}
		else
		if(s[0] == ' ' && s[1] == '&') {
			result.Cat('&');
			if(iscib(s[2]))
				result.Cat(' ');
			s += 2;
		}
		else
		if(*s == ' ') {
			while(*s == ' ')
				s++;
			result.Cat(' ');
		}
		else
			result.Cat(*s++);
	return result;
}
