#include "SDraw.h"

NAMESPACE_UPP

SDraw& SDraw::Text(double x, double y, const wchar *text, Font fnt, int n, double *dx)
{
	RTIMING("Text");
	FontInfo fi = fnt.Info();
	if(n < 0)
		n = wstrlen(text);
	y += fi.GetAscent();
	while(n) {
		int ch = *text++;
		Character(x, y, ch, fnt);
		if(dx)
			x += *dx++;
		else
			x += fi[ch];
		n--;
	}
	return *this;
}

SDraw& SDraw::Text(double x, double y, const WString& s, Font fnt, double *dx)
{
	Text(x, y, s, fnt, s.GetLength(), dx);
	return *this;
}

SDraw& SDraw::Text(double x, double y, const String& s, Font fnt, double *dx)
{
	Text(x, y, s.ToWString(), fnt, dx);
	return *this;
}

SDraw& SDraw::Text(double x, double y, const char *text, Font fnt, int n, double *dx)
{
	Text(x, y, ToUnicode(text, n < 0 ? strlen(text) : n, CHARSET_DEFAULT), fnt, dx);
	return *this;
}

END_UPP_NAMESPACE
