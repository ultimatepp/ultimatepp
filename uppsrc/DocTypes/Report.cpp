#include "DocTypes.h"

namespace Upp {

DocReport::DocReport(const char *_name) {
	name = _name;
	pgsz.cx = 3968;
	pgsz.cy = 6074;
	size = pgsz;
	pgnformat = NULL;
	pageno = 1;
	dortf = true;
	WhenNewPage();
	StartPage();
	valrects = false;
	lm = 0;
}

DocReport::~DocReport() {}

void DocReport::Clear() {
	pgnformat = NULL;
	pageno = 1;
	page.Clear();
	header.Clear();
	footer.Clear();
	qtfheader.Clear();
	qtffooter.Clear();
	WhenNewPage();
	StartPage();
	rtf.Clear();
	vr.Clear();
}

void DocReport::SetCount(int n)
{
	page.SetCount(n);
	StartPage();
}


DocReport& DocReport::SetPageSize(int cx, int cy) {
	pgsz = Size(cx, cy);
	StartPage();
	return *this;
}

DocReport& DocReport::SetPageSize(Size sz) {
	pgsz = sz;
	StartPage();
	return *this;
}

DocReport& DocReport::SetPageNo(int _pageno) {
	pageno = _pageno;
	return *this;
}

DocReport& DocReport::SetPageno(int x, int y, const char *format, Font font, int fp) {
	pgnx = x;
	pgny = y;
	pgnformat = format;
	pgnfont = font;
	pageno = fp;
	WhenNewPage();
	StartPage();
	return *this;
}

DocReport& DocReport::SetHeader(const Drawing& h) {
	header = h;
	StartPage();
	return *this;
}

DocReport& DocReport::SetFooter(const Drawing& f) {
	footer = f;
	StartPage();
	return *this;
}

DocReport& DocReport::SetHeader(const char *qtf) {
	qtfheader = qtf;
	StartPage();
	return *this;
}

DocReport& DocReport::SetFooter(const char *qtf) {
	qtffooter = qtf;
	StartPage();
	return *this;
}

String  DocReport::HFormat(const char *s) {
	String result;
	while(*s) {
		if(s[0] == '$' && s[1] == '$') {
			if(s[2] == 'P')
				result.Cat(Format("%d", pageno));
			if(s[2] == 'D')
				result.Cat(Format(GetSysDate()));
			s += 3;
		}
		result.Cat(*s++);
	}
	return result;
}

void DocReport::StartPage() {
	Create(pgsz.cx, pgsz.cy);
	size = pgsz;
	ylim = size.cy;
	bool artf = dortf;
	dortf = false;
	if(qtffooter.IsEmpty()) {
		if(footer) {
			Size sz = footer.GetSize();
			DrawDrawing(0, pgsz.cy - sz.cy, sz.cx, sz.cy, footer);
			size.cy -= sz.cy;
		}
	}
	else {
		ypos = 0;
		Qtf(HFormat(qtffooter));
		Drawing h = GetResult();
		Create(pgsz.cx, pgsz.cy);
		DrawDrawing(0, pgsz.cy - ypos, pgsz.cx, pgsz.cy, h);
		size.cy -= ypos;
	}
	ylim = size.cy;
	ypos = 0;
	if(qtfheader.IsEmpty()) {
		if(header) {
			Size sz = header.GetSize();
			DrawDrawing(0, 0, sz.cx, sz.cy, header);
			ypos = max(0, sz.cy);
		}
	}
	else
		Qtf(HFormat(qtfheader));
	size.cy -= ypos;
	if(pgnformat)
		DrawText(pgnx, pgny >= 0 ? pgny : pgsz.cy - pgny,
		         Format(pgnformat, pageno), pgnfont);
	dortf = artf;
}

void DocReport::EndPage() {
	page.Add(GetResult());
	pageno++;
}

void DocReport::NextPage() {
	EndPage();
	WhenNewPage();
	StartPage();
}

void DocReport::Page() {
	if(dortf) rtf.Page();
	NextPage();
}

void DocReport::SkipBefore(int cy) {
	ypos += cy;
	if(ypos > GetYLim()) {
		Page();
		if(ypos + cy < GetYLim())
			ypos += cy;
	}
}

void DocReport::SkipAfter(int cy) {
	ypos += cy;
	if(ypos > GetYLim())
		Page();
}

void DocReport::AddVR(const Vector<ValueRect>& v) {
	for(int i = 0; i < v.GetCount(); i++) {
		RValueRect& r = vr.Add();
		(ValueRect&)r = v[i];
		r.toppage = r.bottompage = page.GetCount();
		while(r.rect.top >= GetYLim()) {
			r.rect.top -= GetYLim();
			r.toppage++;
		}
		while(r.rect.bottom >= GetYLim()) {
			r.rect.bottom -= GetYLim();
			r.bottompage++;
		}
	}
}

void DocReport::Put(const Paragraph& p) {
	if(dortf) rtf.Put(p);
	SkipBefore(p.GetBefore());
	if(valrects) {
		int y = ypos;
		Vector<ValueRect> v;
		p.GetValueRects(1024, *this, lm, y, pgsz.cx - lm, v);
		AddVR(v);
	}
	Paragraph::PaintInfo pi;
	while(p.Paint(*this, lm, ypos, pgsz.cx - lm, GetYLim(), pi))
		Page();
	SetYPos(pi.ypos);
	SkipAfter(p.GetAfter());
}

void DocReport::Put(const Table& tab) {
	if(dortf) rtf.Put(tab);
	SkipBefore(tab.GetBefore());
	if(valrects) {
		int y = ypos;
		Vector<ValueRect> v;
		tab.GetValueRects(1024, *this, lm, y, pgsz.cx - lm, v);
		AddVR(v);
	}
	Table::PaintInfo pi;
	while(tab.Paint(*this, lm, ypos, pgsz.cx - lm, GetYLim(), pi))
		Page();
	SetYPos(pi.ypos);
	SkipAfter(tab.GetAfter());
}

void  DocReport::KillLastPage() {
	if(page.GetCount())
		page.Drop();
	pageno--;
}

void PutQTF(DocReport& r, const char *qtf)
{
	RichText text;
	text = ParseQTF(qtf);

	Size sz = r.GetPageSize();
	Size pgsz = r.GetSize();
	PageY end = text.GetHeight(pgsz);
	int lastpage = end.page;
	int x = (sz.cx - pgsz.cx) / 2;
	int y = (sz.cy - pgsz.cy) / 2;
	for(int i = 0; i <= lastpage; i++) {
		if(i)
			r.Page();
		PrintPageDraw pw(r);
		pw.SetPage(i);
		PaintInfo paintinfo;
		paintinfo.top = PageY(i, 0);
		paintinfo.bottom = PageY(i + 1, 0);
		text.Paint(pw, Rect(Point(x, y), pgsz), paintinfo);
	}
	r.SetYPos(end.y);
}

}
