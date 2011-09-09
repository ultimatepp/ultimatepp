#include <Core/Core.h>

using namespace Upp;

String Sprntf(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	return VFormat(fmt, argptr);
}

CONSOLE_APP_MAIN
{
	DDUMP(String("lubäck").GetCharCount());	
	DLOG("------------------");
	DDUMP(Sprntf("|%s|", "luback"));
	DDUMP(Format("|%s|", "lubäck"));
	DDUMP(Sprntf("|%40s|", "luback"));
	DDUMP(Format("|%40s|", "lubäck"));
	DDUMP(Sprntf("|%40s|", "luback"));
	DDUMP(Format("|%40s|", "lubäck"));
	DDUMP(Sprntf("|%40.40s|", "luback"));
	DDUMP(Format("|%40.40s|", "lubäck"));
	DDUMP(Sprntf("|%-40.40s|", "luback"));
	DDUMP(Format("|%-40.40s|", "lubäck"));
	DDUMP(Sprntf("|%40.5s|", "luback"));
	DDUMP(Format("|%40.5s|", "lubäck"));
	DDUMP(Sprntf("|%-40.5s|", "luback"));
	DDUMP(Format("|%-40.5s|", "lubäck"));
	DLOG("------------------");
	DDUMP(Sprntf("|%5s|", "luback"));
	DDUMP(Format("|%5s|", "lubäck"));
	DDUMP(Sprntf("|%5s|", "luback"));
	DDUMP(Format("|%5s|", "lubäck"));
	DDUMP(Sprntf("|%5.5s|", "luback"));
	DDUMP(Format("|%5.5s|", "lubäck"));
	DDUMP(Sprntf("|%-5.5s|", "luback"));
	DDUMP(Format("|%-5.5s|", "lubäck"));
	DDUMP(Sprntf("|%.5s|", "luback"));
	DDUMP(Format("|%.5s|", "lubäck"));
	DDUMP(Sprntf("|%-.5s|", "luback"));
	DDUMP(Format("|%-.5s|", "lubäck"));
	DLOG("------------------");
	DDUMP(Format("|%-40.40s|", "luback"));
	DDUMP(Format("|%-40.40s|", "lubäck"));
	DDUMP(Format("|%-40.40s|", "luback"));
	DDUMP(Format("|%20<s|", "lubäck"));
	DDUMP(Format("|%20<s|", "luback"));
	DDUMP(Format("|%20=s|", "lubäck"));
	DDUMP(Format("|%20=s|", "luback"));
	DDUMP(Format("|%20>s|", "lubäck"));
	DDUMP(Format("|%20>s|", "luback"));
	DLOG("------------------");
	DDUMP(Format("|%-4.4s|", "luback"));
	DDUMP(Format("|%-4.4s|", "lubäck"));
	DDUMP(Format("|%-4.4s|", "luback"));
	DDUMP(Format("|%5<s|", "lubäck"));
	DDUMP(Format("|%5<s|", "luback"));
	DDUMP(Format("|%5=s|", "lubäck"));
	DDUMP(Format("|%5=s|", "luback"));
	DDUMP(Format("|%5>s|", "lubäck"));
	DDUMP(Format("|%5>s|", "luback"));
	      
	DLOG("------------------");
	DDUMP(Sprintf("%d", 123));
	DDUMP(Format("%d", 123));

	DLOG("------------------");
	SetDefaultCharset(CHARSET_WIN1250);
	DDUMP(String("lubäck").GetCharCount());
}
