#include "RichText.h"

namespace Upp {

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

RichContext::RichContext(const RichStyles& styles, const RichText *text)
:	text(text), styles(&styles)
{
	header_cy = footer_cy = 0;
	current_header_cy = current_footer_cy = 0;
}

void RichContext::HeaderFooter(RichText *header_, RichText *footer_)
{
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
}

void RichContext::AdjustPage()
{
	page.top += header_cy - current_header_cy;
	page.bottom -= footer_cy - current_footer_cy;
	current_header_cy = header_cy;
	current_footer_cy = footer_cy;
}

void RichContext::Set(PageY p0, const Rect& first_page, const Rect& next_page, PageY p)
{ // table layout helper, real hdr/ftr is irrelevant, need correct page.top / page.bottom
	current_header_cy = current_footer_cy = 0;
	if(p.page != p0.page) { // we are already on next page
		page = next_page;
		header_cy = footer_cy = 0;
	}
	else {
		page = first_page;
		header_cy = next_page.top - page.top;
		footer_cy = page.bottom - next_page.bottom;
	}
	py = p;
}

void RichContext::Page()
{
	py.page++;
	AdjustPage();
	py.y = page.top;
}

RichContext RichText::Context(const Rect& page, PageY py, RichText *header, RichText *footer) const
{
	RichContext rc(style, this);
	rc.page = page;
	rc.py = py;
	rc.HeaderFooter(header, footer);
	rc.AdjustPage();
	if(rc.py.y < rc.page.top)
		rc.py.y = rc.page.top;
	return rc;
}

void RichText::PaintHeaderFooter(PageDraw& pw, const Rect& page, PageY py, const PaintInfo& pi_,
                                 int from_page, int to_page) const
{
	PaintInfo pi = pi_;
	pi.sell = pi.selh = 0;
	RichContext rc = Context(page, py);
	int last_page = -1;
	int pagecount = GetHeight(page).page + 1;
	int i = 0;
	while(last_page <= to_page) {
		while(last_page < rc.py.page) {
			last_page++;
			if(last_page >= from_page && (rc.header || rc.footer)) {
				if(rc.header_cy)
					GetHeaderFooterText(rc.header, last_page, pagecount).Paint(pw, PageY(last_page, page.top), page, pi);
				if(rc.footer_cy)
					GetHeaderFooterText(rc.footer, last_page, pagecount).Paint(pw, PageY(last_page, page.bottom - rc.footer_cy), page, pi);
			}
		}
		if(++i >= GetPartCount())
			break;
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

}