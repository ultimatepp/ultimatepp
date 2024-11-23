#include "www.h"

String ToHtml(const char *s)
{
	if(*s == '`')
		return String(++s);

	String result;
	while(*s) {
		if(*s == 31)
			result.Cat("&nbsp;");
		else
		if(*s == '<')
			result.Cat("&lt;");
		else
		if(*s == '>')
			result.Cat("&gt;");
		else
		if(*s == '&')
			result.Cat("&amp;");
		else
		if(*s == '\"')
			result.Cat("&quot;");
		else
		if((byte)*s < ' ')
			result.Cat(NFormat("&#%d;", (byte)*s));
		else
			result.Cat(*s);
		s++;
	}
	return result;
}

Htmls& Htmls::Text(const char *s)                  { Cat(ToHtml(s)); return *this; }
Htmls& Htmls::Quote(const char *s)                 { Cat('\"' + ToHtml(s) + '\"'); return *this; }
Htmls& Htmls::Color(class Color color)             { Quote(ColorToHtml(color)); return *this; }
Htmls& Htmls::Percent(double d)                    { return Quote(UPP::Format("%g%%", d)); }

Htmls& Htmls::Color(const char *nm, class Color c) {
	if(UPP::IsNull(c))
		return *this;
	Attr(nm);
	Cat('=');
	return Color(c);
}

Htmls& Htmls::Percent(const char *nm, double d)    { Attr(nm); Cat('='); return Percent(d); }

Htmls& Htmls::Attr(const char *a)                  { Cat(' '); Cat(a); return *this; }
Htmls& Htmls::Attr(const char *a, const char *s)   { Attr(a); Cat('='); return Quote(s); }
Htmls& Htmls::Attr(const char *a, String s)        { return Attr(a, ~s); }
Htmls& Htmls::Attr(const char *a, int i)           { return Attr(a, UPP::Format("%d", i)); }
Htmls& Htmls::Attr(const char *a, double d)        { return Attr(a, UPP::Format("%g", d)); }
Htmls& Htmls::Attr(const char *a, Date d)          { return Attr(a, UPP::Format(d)); }
Htmls& Htmls::Attr(const char *a, Value v)         { return Attr(a, StdFormat(v)); }

Htmls& Htmls::Attp(const char *a, double v)        { return v >= 0 ? Attr(a, v) : Percent(a, -v); }

Htmls& Htmls::Atth(const char *a, const char *h)
{
	Attr(a);
	Cat('=');
	Cat('\"');
	Cat(h);
	Cat('\"');
	return *this;
}

Htmls& Htmls::Tag(const char *tag)                 { Cat('<'); Cat(tag); return *this; }
Htmls& Htmls::Gat()                                { Cat('>'); return *this; }

Htmls& Htmls::Nbsp()                               { Cat("&nbsp;"); return *this; }
Htmls& Htmls::Br()                                 { Cat("<BR>", 4); return *this; }
Htmls& Htmls::Hr()                                 { Cat("<HR>", 4); return *this; }
Htmls& Htmls::Lf()                                 { Cat('\n'); return *this; }

Htmls& Htmls::Cat(const HtmlTag& tag)
{
	Cat(tag.GetTag());
	Cat(tag.GetEndTag());
	return *this;
}

HtmlTag& HtmlTag::Type(const char *s)       { return Attr("TYPE", s); }

HtmlTag& HtmlTag::Name(const char *s)       { return Attr("NAME", s); }
HtmlTag& HtmlTag::Name(String id)           { return Attr("NAME", ~id); }

HtmlTag& HtmlTag::Data(const char *v)       { return Attr("VALUE", v); }
HtmlTag& HtmlTag::Data(String v)            { return Attr("VALUE", v); }
HtmlTag& HtmlTag::Data(double v)            { return Attr("VALUE", v); }
HtmlTag& HtmlTag::Data(Value v)             { return Attr("VALUE", v); }

HtmlTag& HtmlTag::ReadOnly()                { return Attr("READONLY"); }
HtmlTag& HtmlTag::Multiple()                { return Attr("MULTIPLE"); }
HtmlTag& HtmlTag::Selected()                { return Attr("SELECTED"); }

HtmlTag& HtmlTag::Align(Alignment align)
{
	switch(align) {
		case ALIGN_CENTER: return Attr("ALIGN", "CENTER");
		case ALIGN_RIGHT:  return Attr("ALIGN", "RIGHT");
		default:           return Attr("ALIGN", "LEFT");
	}
}

HtmlTag& HtmlTag::Left()                    { return Align(ALIGN_LEFT); }
HtmlTag& HtmlTag::Right()                   { return Align(ALIGN_RIGHT); }
HtmlTag& HtmlTag::Center()                  { return Align(ALIGN_CENTER); }

HtmlTag& HtmlTag::VAlign(Alignment align)
{
	switch(align) {
		case ALIGN_CENTER: return Attr("VALIGN", "MIDDLE");
		case ALIGN_BOTTOM: return Attr("VALIGN", "BOTTOM");
		default:           return Attr("VALIGN", "TOP");
	}
}

HtmlTag& HtmlTag::Top()                     { return VAlign(ALIGN_TOP); }
HtmlTag& HtmlTag::VCenter()                 { return VAlign(ALIGN_CENTER); }
HtmlTag& HtmlTag::Bottom()                  { return VAlign(ALIGN_BOTTOM); }

HtmlTag& HtmlTag::HotKey(char c)            { return c ? Attr("ACCESSKEY", String(c, 1)) : *this; }
HtmlTag& HtmlTag::Size(int c)               { return Attr("SIZE", c); }
HtmlTag& HtmlTag::Size(int cx, int cy)      { return Attr("SIZE", Format("%d, %d", cx, cy)); }

HtmlTag& HtmlTag::Color(class Color color)      { return Color("COLOR", color); }
HtmlTag& HtmlTag::BorderColor(class Color col)  { return Color("BORDERCOLOR", col); }
HtmlTag& HtmlTag::BgColor(class Color color)    { return Color("BGCOLOR", color); }

HtmlTag& HtmlTag::Face(const char *s)       { return Attr("FACE", s); }
HtmlTag& HtmlTag::Border(int i)             { return Attr("BORDER", i); }
HtmlTag& HtmlTag::CellSpacing(int i)        { return Attr("CELLSPACING", i); }
HtmlTag& HtmlTag::CellPadding(int i)        { return Attr("CELLPADDING", i); }

HtmlTag& HtmlTag::Width(double w)           { return Attp("WIDTH", w); }
HtmlTag& HtmlTag::Height(double h)          { return Attp("HEIGHT", h); }

HtmlTag& HtmlTag::RowSpan(int i)            { return Attr("ROWSPAN", i); }
HtmlTag& HtmlTag::ColSpan(int i)            { return Attr("COLSPAN", i); }

HtmlTag& HtmlTag::Class(const char *s)      { return Attr("CLASS", s); }
HtmlTag& HtmlTag::DataFld(const char *s)    { return Attr("DATAFLD", s); }
HtmlTag& HtmlTag::DataSrc(const char *s)    { return Attr("DATASRC", s); }
HtmlTag& HtmlTag::Href(const char *s)       { return Atth("HREF", s); }
HtmlTag& HtmlTag::HtmlId(const char *s)     { return Attr("ID", s); }
HtmlTag& HtmlTag::Lang(const char *s)       { return Attr("LANG", s); }
HtmlTag& HtmlTag::Language(const char *s)   { return Attr("LANGUAGE", s); }
HtmlTag& HtmlTag::JavaScript()              { return Language("JAVASCRIPT"); }
HtmlTag& HtmlTag::VbScript()                { return Language("VBSCRIPT"); }
HtmlTag& HtmlTag::Methods(const char *s)    { return Attr("METHODS", s); }
HtmlTag& HtmlTag::Rel(const char *s)        { return Attr("REL", s); }
HtmlTag& HtmlTag::Rev(const char *s)        { return Attr("REV", s); }
HtmlTag& HtmlTag::Style(const char *s)      { return Attr("STYLE", s); }
HtmlTag& HtmlTag::TabIndex(int i)           { return Attr("TABINDEX", i); }
HtmlTag& HtmlTag::Target(const char *s)     { return Attr("TARGET", s); }
HtmlTag& HtmlTag::TargetBlank()             { return Target("_blank"); }
HtmlTag& HtmlTag::TargetParent()            { return Target("_parent"); }
HtmlTag& HtmlTag::TargetSelf()              { return Target("_self"); }
HtmlTag& HtmlTag::TargetTop()               { return Target("_top"); }
HtmlTag& HtmlTag::Title(const char *s)      { return Attr("TITLE", s); }
HtmlTag& HtmlTag::Urn(const char *s)        { return Attr("URN", s); }
HtmlTag& HtmlTag::Alt(const char *s)        { return Attr("ALT", s); }
HtmlTag& HtmlTag::Coords(const char *s)     { return Attr("COORDS", s); }
HtmlTag& HtmlTag::NoHref()                  { return Attr("NOHREF"); }
HtmlTag& HtmlTag::Shape(const char *s)      { return Attr("SHAPE", s); }
HtmlTag& HtmlTag::ShapeCirc()               { return Shape("CIRC"); }
HtmlTag& HtmlTag::ShapePoly()               { return Shape("POLY"); }
HtmlTag& HtmlTag::ShapeRect()               { return Shape("RECT"); }
HtmlTag& HtmlTag::Balance(double v)         { return Attr("BALANCE", v); }
HtmlTag& HtmlTag::Loop(int i)               { return Attr("LOOP", i); }
HtmlTag& HtmlTag::Volume(double v)          { return Attr("VOLUME", v); }
HtmlTag& HtmlTag::Src(const char *s)        { return Attr("SRC", s); }
HtmlTag& HtmlTag::Alink(class Color c)      { return Color("ALINK", c); }
HtmlTag& HtmlTag::Vlink(class Color c)      { return Color("VLINK", c); }
HtmlTag& HtmlTag::Link(class Color c)       { return Color("LINK", c); }
HtmlTag& HtmlTag::Background(const char *s) { return Attr("BACKGROUND", s); }
HtmlTag& HtmlTag::OnClick(const char *s)    { return Atth("onClick", s); }

Htmls HtmlTag::GetTag() const {
	return tag.IsEmpty() ? (String&)tag : tag + '>';
}

Htmls HtmlTag::GetEndTag() const {
	return end;
}

void  HtmlTag::Combine(const HtmlTag& tag2)
{
	if(tag2.IsEmpty()) return;
	if(tag.GetLength()) {
		tag = tag + '>' + tag2.tag;
		end = tag2.end + end;
	}
	else {
		tag = tag2.tag;
		end = tag2.end;
	}
}

Htmls HtmlTag::ApplyTo(String s) const
{
	Htmls h;
	h.Cat(GetTag());
	h.Cat(s);
	h.Cat(GetEndTag());
	return h;
}

Htmls HtmlTag::ApplyTo(const char *s) const
{
	Htmls h;
	h.Cat(GetTag());
	h.Cat(s);
	h.Cat(GetEndTag());
	return h;
}

HtmlTag& HtmlTag::SingleTag(const char *s)
{
	tag = "<";
	tag.Cat(s);
	return *this;
}

HtmlTag& HtmlTag::PairTag(const char *s)
{
	SingleTag(s);
	end = "</";
	end.Cat(s);
	end.Cat('>');
	end.Cat('\n');
	return *this;
}

HtmlTag::HtmlTag(const char *tag) {
	PairTag(tag);
}

HtmlTag HtmlSingleTag(const char *tag) {
	return HtmlTag().SingleTag(tag);
}

HtmlTag HtmlInput(const char *type, const char *name)
{
	HtmlTag tag = HtmlSingleTag("INPUT").Type(type);
	if(name && *name)
		tag.Name(name);
	return tag;
}

HtmlTag HtmlEdit(String name)
{
	return HtmlInput("TEXT", name);
}

HtmlTag HtmlEdit(String name, int size, int maxlength, const char *defaultValue, bool password)
{
	HtmlTag tag = HtmlInput(password ? "PASSWORD" : "TEXT", name)
		.Size(size).Attr("MAXLENGTH", maxlength);
	if(defaultValue && *defaultValue)
		tag.Data(defaultValue);
	return tag;
}

HtmlTag HtmlHidden(String name, Value val) {
	return HtmlInput("HIDDEN").Name(name).Data(val);
}

HtmlTag HtmlHidden(const char *name, Value val) {
	return HtmlInput("HIDDEN").Name(name).Data(val);
}

HtmlTag HtmlHidden(Value val) {
	return HtmlHidden("STATE", val);
}

HtmlTag HtmlSubmit(const char *s) {
	return HtmlInput("SUBMIT").Data(s);
}

HtmlTag HtmlSelect(String name) {
	return HtmlTag("SELECT").Name(name);
}

Htmls HtmlOption(Value val, const char *text, bool selected)
{
	HtmlTag option("OPTION");
	option.Data(val);
	if(selected)
		option.Selected();
	return option / text;
}

HtmlTag HtmlTable() {
	return HtmlTag("TABLE");
}

HtmlTag HtmlRow() {
	return HtmlTag("TR");
}

HtmlTag HtmlCell() {
	return HtmlTag("TD");
}

HtmlTag HtmlLink(const char *link)
{
	return HtmlTag("A").Href(link);
}

Htmls HtmlBlock(Htmls html, double width, double left, double top, double right, double bottom,
				Alignment align)
{
	Htmls h;
	if(top)
		h = HtmlRow() / HtmlCell().Height(top) / "";
	if(left || right || align != ALIGN_LEFT)
		h += HtmlRow() / (
				HtmlCell().Width(left) +
				~(HtmlCell().Align(align).Width(width) / html) +
				~HtmlCell().Width(right)
			 );
	if(bottom)
		h += HtmlRow() / HtmlCell().Height(bottom) / "";
	return h.GetLength() ? HtmlTable().Border(0).Width(-100).CellSpacing(0).CellPadding(0) / h
		                 : html;
}

HtmlTag HtmlHeader::Create()
{
	String h =
		"HTML lang=\"en-us\">\n"
	    "<HEAD>\n"
	    "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">\n"
	    "<META NAME=\"Generator\" CONTENT=\"U++ HTML Package\">\n"
	    ;
	if(!description.IsEmpty())
		h << "<META NAME=\"Description\" CONTENT=\"" + description + "\">\n";
	if(!title.IsEmpty())
		h << "<TITLE>" + title + "</TITLE>\n";
	if(!css.IsEmpty())
		h << "<STYLE TYPE=\"text/css\"><!--\n"
		  << css << "\r\n-->\r\n</STYLE>\r\n";
	if(!other.IsEmpty())
		h << other;
	h << "</HEAD";
	return HtmlSingleTag(h) / HtmlTag("BODY");
}

Htmls operator+(const HtmlTag& tag1, const HtmlTag& tag2)
{
	Htmls h;
	h.Cat(tag1);
	h.Cat(tag2);
	return h;
}

Htmls operator+(const char *s, const HtmlTag& tag) {
	Htmls h;
	h.Cat(s);
	h.Cat(tag);
	return h;
}

Htmls operator+(const HtmlTag& tag, const char *s)
{
	Htmls h;
	h.Cat(tag);
	h.Cat(s);
	return h;
}

HtmlTag operator/(const HtmlTag& t1, const HtmlTag& t2)
{
	HtmlTag t = t1;
	t.Combine(t2);
	return t;
}

HtmlTag& operator/=(HtmlTag& tag, const HtmlTag& s)
{
	tag.Combine(s);
	return tag;
}

HtmlTag operator % (const HtmlTag& t1, const HtmlTag& t2)
{
	if(t1.Tag().IsEmpty())
		return t2;
	else if(t2.Tag().IsEmpty())
		return t1;
	else
	{
		HtmlTag t;
		t.Tag() = t1.Tag() + ">\n" + t2.Tag();
		t.End() = t2.End() + "\n" + t1.End();
		return t;
	}
}

Htmls operator % (const HtmlTag& tag, const char *s)
{
	Htmls h;
	h.Cat(tag.GetTag());
	h.Lf();
	h.Cat(s);
	h.Lf();
	h.Cat(tag.GetEndTag());
//	h.Lf();
	return h;
}

HtmlTag HtmlFontColor(Color c)   { return !IsNull(c) ? HtmlTag("FONT").Color(c) : HtmlTag(); }
HtmlTag HtmlFontSize(int s)      { return HtmlTag("FONT").Attr("SIZE", s); }

HtmlTag HtmlLink(const char *link, const char *target)
{
	HtmlTag tag("A");
	tag.Href(link);
	if(target && *target)
		tag.Target(target);
	return tag;
}

HtmlTag HtmlImg(String src, String alt)
{
	return HtmlSingleTag("IMG").Src(src).Alt(alt);
}

Htmls HtmlTextArea(String name, Size size, const char *defaultValue)
{
	return HtmlTag("TEXTAREA").Name(name).Attr("ROWS", size.cy).Attr("COLS", size.cx)
		.Attr("WRAP", "virtual")
		/ ToHtml(defaultValue ? defaultValue : "");
}

HtmlTag HtmlButton(String name, const char *text)
{
	return HtmlInput("SUBMIT", name).Data(text);
}

HtmlTag HtmlWarn(Color color)
{
	return HtmlItalic() / HtmlFontColor(color);
}

HtmlTag HtmlPackedTable()
{
	return HtmlTable().Border(0).CellSpacing(0).CellPadding(0);
}

HtmlTag HtmlForm(String action, bool multipart, String method)
{
	HtmlTag tag = HtmlTag("FORM").Attr("ACTION", action).Attr("METHOD", method);
	if(multipart)
		tag.Attr("ENCTYPE", "multipart/form-data");
	return tag;
}

Htmls HtmlUpload(String name)
{
	return HtmlInput("FILE", name);
}

Htmls HtmlCheck(String name, bool on)
{
	HtmlTag tag = HtmlInput("CHECKBOX", name).Data("1");
	if(on)
		tag.Attr("CHECKED");
	return tag;
}

HtmlTag HtmlTCell()
{
	return HtmlCell().Top();
}

HtmlTag HtmlRowCell()
{
	return HtmlRow() % HtmlCell();
}

HtmlTag HtmlRowTCell()
{
	return HtmlRow() % HtmlTCell();
}

Htmls HtmlMenu(bool active, const char *text, const char *url, int wd)
{
	HtmlTag table = HtmlTable().CellSpacing(0).Border(2)
		.BorderColor(Black).BgColor(active ? Yellow : WhiteGray);
	if(wd)
		table.Width(wd);
	return table % HtmlRow() % HtmlCell().Center()
		/ HtmlLink(url) / HtmlFontColor(Black) / (active ? HtmlBold() / text : Htmls(text));
}

HtmlTag HtmlDialog(int width)
{
	return HtmlTable().Width(width).CellSpacing(0).CellPadding(1).Border(1).BgColor(WhiteGray)
		% HtmlRow() % HtmlCell()
		% HtmlTable().Width(-100).CellSpacing(4).CellPadding(0).Border(0).BgColor(WhiteGray);
}

Htmls HtmlHead(String title, const char *charset)
{
	Htmls head;
	head << HtmlTag("TITLE") / title << '\n'
		<< HtmlSingleTag("META")
			.Attr("HTTP-EQUIV", "Content-Type")
			.Attr("CONTENT", String("text/html; charset=") + charset);
	return HtmlTag("HEAD") % head + "\n\n";
}

Htmls HtmlPage(String title, const Htmls& body, Color bgcolor, const char *charset)
{
	return HtmlTag("HTML") % (HtmlHead(title, charset) + HtmlTag("BODY").BgColor(bgcolor) % body);
}

Htmls HtmlTitlePage(String title, const Htmls& body, Color bgcolor, const char *charset)
{
	return HtmlPage(title, HtmlTag("H1") / title + "\n\n" + body, bgcolor);
}

HtmlTag HtmlLine()
{
	return HtmlRow() / HtmlCell();
}

HtmlTag HtmlPara()
{
	return HtmlTag("p");
}

HtmlTag HtmlCourier(int size)
{
	return HtmlTag("span")
	       .Attr("style", Sprintf("font-size: %dpt; font-family: monospace;", size));
}

HtmlTag HtmlArial(int size)
{
	return HtmlTag("span")
	       .Attr("style", Sprintf("font-size: %dpt; font-family: sans-serif;", size));
}

HtmlTag HtmlRoman(int size)
{
	return HtmlTag("span")
	       .Attr("style", Sprintf("font-size: %dpt; font-family: serif;", size));
}

HtmlTag HtmlCourier()
{
	return HtmlTag("span").Attr("style", "font-family: monospace;");
}

HtmlTag HtmlArial()
{
	return HtmlTag("span").Attr("style", "font-family: sans-serif;");
}

HtmlTag HtmlRoman()
{
	return HtmlTag("span").Attr("style", "font-family: serif;");
}

HtmlTag HtmlFntSize(int size)
{
	return HtmlTag("span").Attr("style", Sprintf("font-size: %dpt", size));
}
