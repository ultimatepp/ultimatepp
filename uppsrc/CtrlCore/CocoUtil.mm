#include "CocoMM.h"

#ifdef PLATFORM_COCOA

namespace Upp {

WString ToWString(CFStringRef s)
{
	if(!s) return Null;
	CFIndex l = CFStringGetLength(s);
	if(!l) return Null;
	WStringBuffer b(l);
    CFStringGetCharacters(s, CFRangeMake(0, l), (UniChar *)~b);
    return b;
}

String ToString(CFStringRef s)
{
	return ToWString(s).ToString();
}

};

#endif
