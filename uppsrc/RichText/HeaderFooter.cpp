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

Rect RichText::GetPageMinusHeaderFooter(const Rect& page) const
{
	Rect p = page;
	VectorMap<String, Value> vars;
	vars.Add("PAGENUMBER", 999999999);
	vars.Add("PAGECOUNT", 999999999);
	if(header) {
		header->EvaluateFields(vars);
		p.top += header->GetHeight(page.GetWidth());
	}
	if(footer) {
		footer->EvaluateFields(vars);
		p.bottom -= footer->GetHeight(page.GetWidth());
	}
	return p.GetHeight() > page.GetHeight() / 2 ? p : page;
}

void RichText::PaintHeaderFooter(PageDraw& pw, const Rect& page, const PaintInfo& pi_,
                                 int from_page, int to_page) const
{
	Rect rpage = GetPageMinusHeaderFooter(page);
	if(rpage == page)
		return;
	PaintInfo pi = pi_;
	pi.sell = pi.selh = 0;
	VectorMap<String, Value> vars;
	vars.Add("PAGECOUNT", GetHeight(page).page + 1);
	for(int i = from_page; i <= to_page; i++) {
		vars.GetAdd("PAGENUMBER") = i + 1;
		if(header) {
			header->EvaluateFields(vars);
			header->Paint(pw, PageY(i, page.top), page, pi);
		}
		if(footer) {
			footer->EvaluateFields(vars);
			Rect pr = page;
			pr.top = rpage.bottom;
			footer->Paint(pw, PageY(i, pr.top), page, pi);
		}
	}
}

void RichText::PickHeader(RichText rval_ txt)
{
	RichText b = pick(txt);
	b.ClearHeader();
	header.Create() = pick(b);
}

void RichText::PickFooter(RichText rval_ txt)
{
	RichText b = pick(txt);
	b.ClearFooter();
	footer.Create() = pick(b);
}

void RichText::SetHeaderQtf(const char *qtf)
{
	header.Clear();
	if(qtf)
		PickHeader(ParseQTF(qtf));
}

void RichText::SetFooterQtf(const char *qtf)
{
	if(qtf)
		PickFooter(ParseQTF(qtf));
}

String RichText::GetHeaderQtf(byte charset, dword options) const
{
	return header ? AsQTF(*header, charset, options) : String();
}

String RichText::GetFooterQtf(byte charset, dword options) const
{
	return footer ? AsQTF(*footer, charset, options) : String();
}

END_UPP_NAMESPACE