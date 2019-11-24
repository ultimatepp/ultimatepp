#include <CtrlLib/CtrlLib.h>

#include "PopUpText.h"
	
namespace Upp {
		
static Size GetEditSize(const String &_str, const Font &font) {
	WString str(_str);
	Size ret(0, 0);
	int retx = 0, nlines = 1;
	for (int i = 0; i < str.GetCount(); ++i) {
		int c = str[i];
		if (c == '\n') {
			nlines++;
			ret.cx = max(ret.cx, retx);
			retx = 0;
		} else
			retx += font.GetWidth(c);
	}
	ret.cx = max(ret.cx, retx);
	ret.cy = nlines*font.GetHeight() + font.GetDescent();
	return ret;
}

PopUpText &PopUpText::SetText(String _text) {
	text = _text;		
	sz = GetEditSize(text, font);
	return *this;
}

void PopUpInfo::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, color);
	PaintLabel(w, 0, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
}

PopUpInfo::PopUpInfo(): color(SColorInfo()) {
	Transparent(false);
	NoWantFocus();
	IgnoreMouse();
	SetAlign(ALIGN_CENTER);
	SetFrame(BlackFrame());
	opened = false;
}

}