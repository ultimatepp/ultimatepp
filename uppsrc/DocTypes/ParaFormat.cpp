#include "DocTypes.h"

namespace Upp {

FontInfo ParaTypo::Part::Set(Font _font, Color _color) {
	font = _font;
	color = _color;
	FontInfo pf = font.Info();
	external = pf.GetExternal();
	overhang = pf.GetOverhang();
	ascent = pf.GetAscent();
	descent = pf.GetDescent();
	voidptr = NULL;
	return pf;
}

int  ParaTypo::Align() {
	switch(align) {
	case ALIGN_RIGHT:
		return pgcx - cx;
	case ALIGN_CENTER:
		return (pgcx - cx) / 2;
	case ALIGN_JUSTIFY:
		if(last) break;
		int nsp = nspc;
		const char *clim = text + len;
		const char *s;
		for(s = text; *s == ' ' && s != clim; s++)
			nsp--;
		if(nsp == 0) break;
		int q = ((pgcx - cx) << 12) / nsp;
		int *w = s - text + width;
		Part **i = s - text + info;
		int prec = 0;
		while(s < clim) {
			if(*s == ' ') {
				*w += (prec + q) >> 12;
				prec = (prec + q) & 4095;
				*i = NULL;
			}
			w++;
			s++;
			i++;
		}
	}
	return 0;
}

bool ParaTypo::PutAligned(int lm) {
	return false;
}

bool ParaTypo::Put() {
	return PutAligned(Align());
}

bool ParaTypo::PutRaw() {
	ASSERT(len >= 0);
	ASSERT(nspc <= len);
	ascent = minascent;
	descent = mindescent;
	external = minexternal;
	if(len) {
		Part *last = *(info + len - 1);
		for(Part *p = *info; p <= last; p++) {
			if(p->ascent > ascent)
				ascent = p->ascent;
			if(p->descent > descent)
				descent = p->descent;
			if(p->external > external)
				external = p->external;
		}
	}
	return Put();
}

bool ParaTypo::Format(Alignment _align, int _total, char *_text, int *_width,
                      ParaTypo::Part **_info, int _pgcx) {
	align = _align;
	total = _total;
	pgcx = _pgcx;
	textbeg = _text;
	char *s = _text;
	int *w = _width;
	Part **i = _info;
	const char *space = NULL;
	const char *slim = _text + total;
	Begin(_text, _width, _info);
	int px = 0;
	last = false;
	while(s < slim) {
		cx += *w;
		if(*s == ' ') {
			space = s;
			nspc++;
		}
		if(*s == 31)
			*s = ' ';
		if(cx > pgcx && text < s) {
			int q;
			if(space && text < space) {
				len = space - text;
				q = space - _text;
				cx -= _width[q];
				nspc--;
				q++;
			}
			else {
				len = s - text;
				q = s - _text;
			}
			int dw = 0;
			for(int *ww = _width + q; ww <= w; ww++)
				dw += *ww;
			cx -= dw;
			if(PutRaw()) return true;
			Begin(_text + q, _width + q, _info + q);
			cx = dw;
			space = NULL;
		}
		w++;
		s++;
		i++;
	}
	len = s - text;
	last = true;
	return PutRaw();
}

}
