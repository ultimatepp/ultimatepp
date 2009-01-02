#include "SDraw.h"

NAMESPACE_UPP

void Text(SDraw& sw, double x, double y, const wchar *text, Font fnt, int n, double *dx)
{
	RTIMING("Text");
	FontInfo fi = fnt.Info();
	if(n < 0)
		n = wstrlen(text);
	y += fi.GetAscent();
	while(n) {
		int ch = *text++;
		Character(sw, x, y, ch, fnt);
		if(dx)
			x += *dx++;
		else
			x += fi[ch];
		n--;
	}
}

void Text(SDraw& sw, double x, double y, const WString& s, Font fnt, double *dx)
{
	Text(sw, x, y, s, fnt, s.GetLength(), dx);
}

void Text(SDraw& sw, double x, double y, const String& s, Font fnt, double *dx)
{
	Text(sw, x, y, s.ToWString(), fnt, dx);
}

void Text(SDraw& sw, double x, double y, const char *text, Font fnt, int n, double *dx)
{
	Text(sw, x, y, ToUnicode(text, n < 0 ? strlen(text) : n, CHARSET_DEFAULT), fnt, dx);
}

END_UPP_NAMESPACE
