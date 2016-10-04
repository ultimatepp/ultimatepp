#ifndef _src_support_Core_h_
#define _src_support_Core_h_

const char* GetSystemDecimalSeparator()
{
#ifdef PLATFORM_WIN32
	return ~GetWinRegString("sDecimal", "Control Panel\\International", HKEY_CURRENT_USER).Mid(0, 1);
#else
	return '.';
#endif
}

double MyScanDouble(const char *p, const char **endptr = NULL);
double MyScanDouble(const wchar *p, const wchar **endptr = NULL);
String MyFormatDouble(double d, int digits=10, int flags=0, int pad_exp=0);
String MyFormatDoubleFix(double d, int digits, int flags);
String MyFormatDoubleExp(double d, int digits, int flags, int fill_exp);

#endif
