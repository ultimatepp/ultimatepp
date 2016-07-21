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

RichText GetHeaderFooterText(const String& qtf, int page, int pagecount)
{
	VectorMap<String, Value> vars;
	vars.Add("PAGENUMBER", page + 1);
	vars.Add("PAGECOUNT", pagecount);
	RichText txt = ParseQTF(qtf);
	txt.EvaluateFields(vars);
	return txt;
}

void RichContext::NewHeaderFooter(const String& header_qtf_, const String& footer_qtf_)
{
	page.top -= header_cy;
	page.bottom += footer_cy;
	header_qtf = header_qtf_;
	footer_qtf = footer_qtf_;
	header_cy = header_qtf.GetCount() ? GetHeaderFooterText(header_qtf, 999990, 999990).GetHeight(page.GetWidth()) : 0;
	footer_cy = footer_qtf.GetCount() ? GetHeaderFooterText(footer_qtf, 999990, 999990).GetHeight(page.GetWidth()) : 0;
	page.top += header_cy;
	page.bottom -= footer_cy;
}

RichContext RichText::Context(const Rect& page, PageY py, const String& header_qtf, const String& footer_qtf) const
{
	RichContext rc(style, this);
	rc.page = page;
	rc.py = py;
	rc.NewHeaderFooter(header_qtf, footer_qtf);
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
	for(int i = 0; i < GetPartCount() && last_page < to_page; i++) {
		while(last_page < rc.py.page) {
			last_page++;
			if(rc.header_qtf.GetCount())
				GetHeaderFooterText(rc.header_qtf, last_page, pagecount).Paint(pw, PageY(last_page, page.top), page, pi);
			if(rc.footer_qtf.GetCount())
				GetHeaderFooterText(rc.footer_qtf, last_page, pagecount).Paint(pw, PageY(last_page, page.bottom - rc.footer_cy), page, pi);
		}
		RichContext begin;
		Advance(i, rc, begin);
	}
}

void RichTxt::SetHeaderQtf(const char *qtf)
{
	header_qtf = qtf;
	r_type = ALL;
}

void RichTxt::SetFooterQtf(const char *qtf)
{
	footer_qtf = qtf;
	r_type = ALL;
}

END_UPP_NAMESPACE