#include "RichText.h"

NAMESPACE_UPP

Draw& SimplePageDraw::Info()
{
	return w;
}

Draw& SimplePageDraw::Page(int)
{
	return w;
}

void RichText::ApplyZoom(Zoom z)
{
	RichStyles ostyle(style, 1);
	for(int i = 0; i < style.GetCount(); i++)
		style[i].format *= z;
	RichTxt::ApplyZoom(z, ostyle, style);
	RefreshAll();
}

Zoom GetRichTextStdScreenZoom()
{
	return Zoom(Ctrl::HorzLayoutZoom(96), 600);
}

struct QTFDisplayCls : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;
};

Size QTFDisplayCls::GetStdSize(const Value& q) const
{
	Size sz;
	RichText txt = ParseQTF((String)q);
	txt.ApplyZoom(GetRichTextStdScreenZoom());
	sz.cx = txt.GetWidth();
	sz.cy = txt.GetHeight(Zoom(1, 1), sz.cx);
	return sz;
}

Size QTFDisplayCls::RatioSize(const Value& q, int cx, int cy) const
{
	if(cy == 0 && cx > 0) {
		RichText txt = ParseQTF((String)q);
		return Size(cx, txt.GetHeight(Zoom(1, 1), cx));
	}
	return GetStdSize(q);
}

void QTFDisplayCls::Paint(Draw& draw, const Rect& r, const Value& v, Color ink, Color paper, dword style) const
{
	String s;
	s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	rtext.Paint(Zoom(1, 1), draw, 0, 0, r.Width());
	draw.End();
}

const Display& QTFDisplay()
{
	return Single<QTFDisplayCls>();
}

struct QTFDisplayCCls : QTFDisplayCls {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
};

void QTFDisplayCCls::Paint(Draw& draw, const Rect& r, const Value& v, Color ink, Color paper, dword style) const
{
	String s;
	s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	int cy = rtext.GetHeight(Zoom(1, 1), r.Width());
	rtext.Paint(Zoom(1, 1), draw, 0, max(0, (r.Height() - cy) / 2), r.Width());
	draw.End();
}

const Display& QTFDisplayVCenter()
{
	return Single<QTFDisplayCCls>();
}

RichText AsRichText(const wchar *s, const RichPara::Format& f)
{
	RichText clip;
	RichPara p;
	p.format = f;
	p.part.Add().format = f;
	RichStyle cs;
	cs.format = f;
	cs.format.sscript = 0;
	cs.format.link.Clear();
	cs.format.indexentry.Clear();
	cs.format.language = LNG_ENGLISH;
	cs.format.label.Clear();
	clip.SetStyle(f.styleid, cs);
	WString& part = p.part.Top().text;
	while(*s) {
		if(*s == '\n') {
			clip.Cat(p);
			part.Clear();
		}
		if(*s >= 32 || *s == '\t')
			part.Cat(*s);
		s++;
	}
	clip.Cat(p);
	return clip;
}

END_UPP_NAMESPACE
