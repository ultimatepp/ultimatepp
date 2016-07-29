#include "RichText.h"

NAMESPACE_UPP

struct RichNumberField : RichPara::FieldType {
	virtual Array<RichPara::Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
	                                       const RichPara::CharFormat& fmt) {
		String h = AsString(vars.Get(param, Null));
		Array<RichPara::Part> ps;
		RichPara::Part& p = ps.Add();
		p.format = fmt;
		if(!IsNull(vars.Get("__DISPLAY_VALUE_FIELDS", Null)))
			p.format.paper = WhiteGray();
		p.text = h.ToWString();
		return ps;
	}
};

INITBLOCK {
	RichPara::Register<RichNumberField>("VALUE");
}

RichText& GetHeaderFooterText(RichText *text, int page, int pagecount)
{
	VectorMap<String, Value> vars;
	vars.Add("PAGENUMBER", page + 1);
	vars.Add("PAGECOUNT", pagecount);
	text->EvaluateFields(vars);
	return *text;
}

void RichContext::NewHeaderFooter(RichText *header_, RichText *footer_)
{
	page.top -= header_cy;
	page.bottom += footer_cy;
	header = header_;
	footer = footer_;
	int cx = page.GetWidth();
	header_cy = header ? GetHeaderFooterText(header, 999990, 999990).GetHeight(cx) : 0;
	footer_cy = footer ? GetHeaderFooterText(footer, 999990, 999990).GetHeight(cx) : 0;
	int maxcy = page.GetHeight();
	if(maxcy < INT_MAX / 2)
		maxcy = maxcy * 4 / 5;
	if(header_cy + footer_cy > maxcy)
		header_cy = footer_cy = 0;
	page.top += header_cy;
	page.bottom -= footer_cy;
}

RichContext RichText::Context(const Rect& page, PageY py, RichText *header, RichText *footer) const
{
	RichContext rc(style, this);
	rc.page = page;
	rc.py = py;
	rc.NewHeaderFooter(header, footer);
	if(rc.py.y < rc.page.top)
		rc.py.y = rc.page.top;
	return rc;
}

void RichText::PaintHeaderFooter(PageDraw& pw, const Rect& page, const PaintInfo& pi_,
                                 int from_page, int to_page) const
{
	PaintInfo pi = pi_;
	pi.sell = pi.selh = 0;
	RichContext rc = Context(page);
	int last_page = -1;
	int pagecount = GetHeight(page).page + 1;
	for(int i = 0; i < GetPartCount() && last_page <= to_page; i++) {
		while(last_page < rc.py.page) {
			last_page++;
			if(rc.header || rc.footer) {
				if(rc.header_cy)
					GetHeaderFooterText(rc.header, last_page, pagecount).Paint(pw, PageY(last_page, page.top), page, pi);
				if(rc.footer_cy)
					GetHeaderFooterText(rc.footer, last_page, pagecount).Paint(pw, PageY(last_page, page.bottom - rc.footer_cy), page, pi);
			}
		}
		RichContext begin;
		Advance(i, rc, begin);
	}
}

void SetQTF(One<RichText>& txt, const String& qtf)
{
	txt.Clear();
	if(qtf.GetCount())
		txt.Create() = ParseQTF(qtf);
}

void RichTxt::SetHeaderQtf(const char *qtf)
{
	header_qtf = qtf;
	SetQTF(header, header_qtf);
	r_type = ALL;
}

void RichTxt::SetFooterQtf(const char *qtf)
{
	footer_qtf = qtf;
	SetQTF(footer, footer_qtf);
	r_type = ALL;
}

END_UPP_NAMESPACE