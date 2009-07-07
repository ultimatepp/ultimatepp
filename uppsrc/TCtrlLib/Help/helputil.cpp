#include "TCtrlLibHelp.h"
#pragma hdrstop

#include <plugin/gif/gif.h>
#include <DocTypes/DocTypes.h>
#include <PdfDraw/PdfDraw.h>

NAMESPACE_UPP

#define LLOG(x) // RLOG(x)

String MQTFStrip(const char *p)
{
	return FromUnicode(MQTFStrip(WString(String(p))));
}

WString MQTFStrip(const wchar *p)
{
	WString out;
	Vector<bool> symbol_stack;
	bool is_symbol = false;
	for(; *p; p++)
		if(*p == '[') {
			symbol_stack.Add(is_symbol);
			while(*++p != ' ')
				if(!*p)
					return out;
				else if(*p == 'S')
					is_symbol = true;
		}
		else if(*p == ']') {
			if(!symbol_stack.IsEmpty())
				is_symbol = symbol_stack.Pop();
		}
		else if(*p == '-' && p[1] == '|') {
			out.Cat('\t');
			p++;
		}
		else {
			WString part;
			if(*p == '\1') {
				const wchar *b = ++p;
				while(*p && *p != '\1')
					p++;
				part = WString(b, p);
				if(!*p)
					p--;
			}
			else if((*p == '`' || *p == '\\') && p[1])
				part.Cat(*++p);
			else
				part.Cat(*p);
			if(!is_symbol)
				out.Cat(part);
			else
				for(const wchar *s = part; *s; s++)
					switch(*s) {
					case 163: out << '<' << '='; break;
					case 171: out << '<' << '-' << '>'; break;
					case 172: out << '<' << '-'; break;
					case 173: out << '^'; break;
					case 174: out << '-' << '>'; break;
					case 177: out << '+' << '-'; break;
					case 178: out << '\"'; break;
					case 179: out << '>' << '='; break;
					default:  out.Cat(*s);
					}
		}
	return out;
}

void MQTFCat(RichPara& para, const char *p, const RichPara::CharFormat& basefmt)
{
	Array<RichPara::CharFormat> stack;
	stack.Add(basefmt);
	const char *b = p;
	while(*p)
		if(*p == '[') {
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			RichPara::CharFormat fmt = stack.Top();
			for(p++; *p && *p != ']' && *p != ' '; p++)
				if(*p == '/')
					fmt.Italic(!fmt.IsItalic());
				else if(*p == '*')
					fmt.Bold(!fmt.IsBold());
				else if(*p == '_')
					fmt.Underline(!fmt.IsUnderline());
				else if(*p == '-')
					fmt.Strikeout(!fmt.IsStrikeout());
				else if(*p == 'S' || *p == 'C' || *p == 'R' || *p == 'A')
					fmt.Face(*p == 'S' ? Font::SYMBOL : *p == 'C' ? Font::COURIER
						: *p == 'R' ? Font::ROMAN : Font::ARIAL);
				else if(*p == ';')
					;
				else {
					String str;
					para.Cat((str << "??[" << *p << "??").ToWString(), fmt);
				}
			stack.Add(fmt);
			b = ++p;
		}
		else if(*p == ']') {
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			if(stack.GetCount() > 1)
				stack.Drop();
			else
				para.Cat(WString("??UNDER??"), stack.Top());
			b = ++p;
		}
		else if(*p == '`' || *p == '\\') {
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			if(*(b = ++p))
				p++;
		}
		else if(*p == '-' && p[1] == '|') {
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			para.Cat(WString(wchar('\t'), 1), stack.Top());
			b = (p += 2);
		}
		else if(*p == '\1') {
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			b = p + 1;
			while(*++p && *p != '\1')
				;
			if(p > b)
				para.Cat(WString(b, p), stack.Top());
			if(*p)
				p++;
			b = p;
		}
		else
			p++;
	if(p > b)
		para.Cat(WString(b, p), stack.Top());
}

/*
class HelpDialogFrame : public CtrlFrame
{
public:
	HelpDialogFrame();

	void         Title(const String& t) { title = t; }

	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();

public:
	Button       minimize, maximize, close;
	String       title;
};

HelpDialogFrame::HelpDialogFrame()
{
	minimize.SetImage(CtrlImg::FrameMinimizeBox());
	maximize.SetImage(CtrlImg::FrameMaximizeBox());
	close.SetImage(CtrlImg::FrameCloseBox());
}

void HelpDialogFrame::FrameLayout(Rect& r)
{
	r.Deflate(4);
	close   .SetFrameRect(r.right - 1 * 16 - 2, r.top + 2, 16, 14);
	maximize.SetFrameRect(r.right - 2 * 16 - 2, r.top + 2, 16, 14);
	minimize.SetFrameRect(r.right - 3 * 16 - 2, r.top + 2, 16, 14);
	r.top += 2 * 2 + 14;
}

void HelpDialogFrame::FrameAddSize(Size& sz)
{
	sz.cy += 2 * 2 + 14 + 2 * 4;
	sz.cx += 2 * 4;
}

void HelpDialogFrame::FramePaint(Draw& w, const Rect& r)
{
	DrawFrame(w, r, SWhiteGray, SBlack);
	DrawFrame(w, r.Deflated(1), SWhite, SGray);
	DrawFrame(w, r.Deflated(2), SLtGray, SLtGray);
	DrawFrame(w, r.Deflated(3), SLtGray, SLtGray);
	w.DrawRect(r.left + 4, r.top + 4, r.Width() - 8, 14 + 2 * 2, SBlue);
	w.DrawText(r.left + 6, r.top + 6, title, StdFont().Bold(), SWhite);
}

void HelpDialogFrame::FrameAdd(Ctrl& parent)
{
	parent << minimize << maximize << close;
}

void HelpDialogFrame::FrameRemove()
{
	minimize.Remove();
	maximize.Remove();
	close.Remove();
}
*/

/*
Drawing CtrlToDrawing(Ctrl *ctrl, Point arrow, Image arrow_image)
{
	Size size = ctrl->GetRect().Size();
	Drawing dwg;

	{
		DrawingDraw dd(size);
		dd.SetPixels();
		ctrl->DrawCtrl(dd);
		dwg = dd;
	}

	if(!ctrl->IsChild())
		if(TopWindow *top = dynamic_cast<TopWindow *>(ctrl)) {
			HelpDialogFrame frame;
			frame.Title(top->GetTitle().ToString());
			TopWindow temp;
			temp.SetFrame(frame);
			Size total = temp.AddFrameSize(size);
			temp.SetRect(total);
			temp.Background(PaintRect(DrawingDisplay(), dwg));
			DrawingDraw dd(total);
			dd.SetPixels();
			temp.DrawCtrl(dd);
			dwg = dd;
		}

	return dwg;
}
*/

Image CtrlToStillImage(Ctrl *ctrl, Point arrow, Image arrow_image)
{
	Image img;
#ifdef PLATFORM_WIN32
	HWND hwnd;
	Ctrl *wnd_ctrl = ctrl;
	while(wnd_ctrl && !(hwnd = wnd_ctrl->GetHWND()))
		wnd_ctrl = wnd_ctrl->GetParent();
	if(!hwnd)
		return Null;
	Rect wrc;
	::GetWindowRect(hwnd, wrc);
	Rect rc = wrc;
	if(ctrl != wnd_ctrl) {
		rc = ctrl->GetScreenRect() - wnd_ctrl->GetScreenRect().TopLeft();
		Point lt = rc.TopLeft(), rb = rc.BottomRight();
		::ClientToScreen(hwnd, lt);
		::ClientToScreen(hwnd, rb);
		rc = Rect(lt, rb);
	}
	HDC sdc = GetWindowDC(0);
	ImageDraw idraw(rc.Size());
	BitBlt(idraw, 0, 0, rc.Width(), rc.Height(), sdc, rc.left, rc.top, SRCCOPY);
	ReleaseDC(0, sdc);
	if(!IsNull(arrow)) {
		Point p(0, 0);
		::ClientToScreen(hwnd, p);
		arrow += p - wrc.TopLeft();
		Image im = Nvl(arrow_image, Image::Arrow());
		arrow -= im.GetHotSpot();
		idraw.DrawImage(arrow.x, arrow.y, im);
	}
	img = idraw;
#endif
	return img;
}

struct BarHelpItemDeep
{
	String         drl;
	String         text;
	Image          image;
	String         help;
	String         description;
};

struct BarHelpItem : BarHelpItemDeep, DeepCopyOption<BarHelpItem>
{
	BarHelpItem() {}
	BarHelpItem(const BarHelpItem& i, int deep)
		: BarHelpItemDeep(i), deep_drl(i.deep_drl, deep), deep_text(i.deep_text, deep), keys(i.keys, deep) {}

	Vector<String> deep_drl;
	Vector<String> deep_text;
	Vector<dword>  keys;

	void           HelpList(String& out) const;
	void           HelpTitle(String& out) const;
	void           HelpText(String& out) const;
};

void BarHelpItem::HelpTitle(String& out) const
{
	for(int i = 0; i < deep_text.GetCount(); i++)
		out << '\1' << deep_text[i] << "\1 `| ";
	out << '\1' << text << '\1';
}

void BarHelpItem::HelpList(String& out) const
{
	RichPara link;
	RichPara::CharFormat format;
	format.Face(Font::ARIAL).Height(84);
	link.Cat(FieldTypeLinkID(), EncodeLink(drl, Null, Null), format);
	RichText text;
	text.Cat(link);
	out << BodyAsQTF(text);
/*	out <<
	out << "[^" << drl << "^ ";
	for(int i = 0; i < deep_text.GetCount(); i++)
		out << '\1' << deep_text[i] << "\1 `| ";
	out << '\1' << text << "\1]";
*/
}

void BarHelpItem::HelpText(String& out) const
{
#ifndef NEWIMAGE
	RichPara::CharFormat cformat;
	cformat.Face(Font::ARIAL).Height(84);
	RichPara para;
	if(!IsNull(image)) {
		Image opaque = image;
		ImageKillMask(opaque, White());
		para.Cat(RichObject("PNG",
			PngEncoder().SaveImage(opaque), PixelsToDots(ScreenInfo(), opaque.GetSize())), cformat);
		para.Cat(WString(" "), cformat);
	}
//	para.Cat(text, cformat);
	if(!keys.IsEmpty()) {
		String out_keys;
		for(int i = 0; i < keys.GetCount(); i++) {
			para.Cat(WString(i ? ", " : "("), cformat);
			cformat.Bold();
			para.Cat(GetKeyDesc(keys[i]).ToWString(), cformat);
			cformat.NoBold();
		}
		para.Cat(WString(")"), cformat);
	}
	RichText rtext;
	rtext.Cat(para);
	para.part.Clear();
	if(!IsNull(help)) {
		para.Cat(help.ToWString(), cformat);
		rtext.Cat(para);
		para.part.Clear();
	}
	if(!IsNull(description)) {
		para.Cat(description.ToWString(), cformat);
		rtext.Cat(para);
		para.part.Clear();
	}
	out.Cat(BodyAsQTF(rtext));
#endif
}

class BarHelpMap
{
public:
	BarHelpMap(const String& ident, const String& module);

	void               HelpText(String& out);

	void               Add(const BarHelpItem& item);

private:
	String             module;
	Array<BarHelpItem> items;
};

BarHelpMap::BarHelpMap(const String& ident, const String& module)
: module(module)
{
	HelpTopicAdd(HelpAppDPP(ident + "$"), module, CNULL, callback(this, &BarHelpMap::HelpText));
}

void BarHelpMap::HelpText(String& out)
{
	for(int i = 0; i < items.GetCount(); i++) {
		if(i) out << "&\r\n";
		items[i].HelpList(out);
	}
}

void BarHelpMap::Add(const BarHelpItem& item)
{
	BarHelpItem& i = items.Add();
	i <<= item;
	HelpTopicAdd(i.drl, module,
		callback(&i, &BarHelpItem::HelpTitle),
		callback(&i, &BarHelpItem::HelpText));
}

class BarScanHelp : public Bar
{
private:
	struct HelpItem : public Bar::Item {
		virtual Item& Text(const char *s)            { info.text = s; return *this; }
		virtual Item& Key(dword _key)                { info.keys.Add(_key); return *this; }
		virtual Item& Image(const UPP::Image& img)   { info.image = img; return *this;  }
		virtual Item& Help(const char *hlp)          { info.help = hlp; return *this; }
		virtual Item& Topic(const char *help)        { info.drl = HelpAppDPP(help); return *this; }
		virtual Item& Description(const char *help)  { info.description = help; return *this; }

		BarHelpItem   info;
	};

	HelpItem        item;
	Callback1<Bar&> submenu;
	String          ident;
	BarHelpMap&     map;

protected:
	virtual Item&   AddItem(Callback cb)              { Flush(); return item; }
	virtual Item&   AddSubMenu(Callback1<Bar&> proc)  { Flush(); submenu = proc; return item; }
	virtual void    AddCtrl(Ctrl *ctrl, int gapsize)  {}
	virtual void    AddCtrl(Ctrl *ctrl, Size sz)      {}

public:
	void            Flush();

	virtual bool    IsMenuBar() const                 { return true; }
	virtual bool    IsToolBar() const                 { return true; }
	virtual bool    IsScanHelp() const                { return true; }

	virtual bool    IsEmpty() const                   { return false; }
	virtual void    Separator()                       {}

	BarScanHelp(BarHelpMap& map, const String& ident) : map(map), ident(ident) {}
};

void BarScanHelp::Flush()
{
	String t = item.info.text = StripHotText(item.info.text);
	if(!IsNull(item.info.drl))
		map.Add(item.info);
	BarHelpItem old_item = item.info;
	item.info = BarHelpItem();
	item.info.deep_drl <<= old_item.deep_drl;
	item.info.deep_text <<= old_item.deep_text;
	Callback1<Bar&> proc = submenu;
	submenu = CNULL;
	if(proc) {
		item.info.deep_drl.Add(old_item.drl);
		item.info.deep_text.Add(old_item.text);
		proc(*this);
		item.info = old_item;
	}
}

void RegisterHelpMenuItems(Callback1<Bar&> barproc, String ident, String module)
{
	static Array<BarHelpMap> maps;
	BarHelpMap& map = maps.Add(new BarHelpMap(ident, module));
	BarScanHelp bsh(map, ident);
	barproc(bsh);
	bsh.Flush();
}

void RegisterHelpFieldTypes()
{
	RichPara::Register(FieldTypeLinkID(), FieldTypeLink());
	RichPara::Register(FieldTypeIndexID(), FieldTypeIndex());
	RichPara::Register(FieldTypeSectionID(), FieldTypeSection());
}

struct FieldTypeLinkCls : public RichPara::FieldType
{
	virtual Array<RichPara::Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
		const RichPara::CharFormat& fmt);
};

Array<RichPara::Part> FieldTypeLinkCls::Evaluate(const String& param,
	VectorMap<String, Value>& vars, const RichPara::CharFormat& linkfmt)
{
	String drl, text, style;
	DecodeLink(param, drl, text, style);
	RichPara para;
	RichPara::CharFormat fmt = linkfmt;
	bool book = ((int)vars.Get("$book", 0) > 0);
	RichPara::CharFormat myfmt = fmt;
	if(IsNull(drl)) {
		myfmt.ink = LtRed;
		myfmt.paper = WhiteGray;
		para.Cat(WString("Cíl odkazu není zadán."), myfmt);
	}
	else if(HelpTopicMap().Find(drl) < 0) {
		myfmt.ink = LtRed;
		myfmt.paper = WhiteGray;
		para.Cat(WString("Nedefinováno: "), myfmt);
		myfmt.Bold();
		para.Cat(drl.ToWString(), myfmt);
	}
	else if(text == "." || text == ".." || text == "=") {
		RichPara::CharFormat bfmt = myfmt;
		bfmt.ink = Green;
		bfmt.paper = myfmt.paper = WhiteGray;
		bfmt.Bold();
		para.Cat(WString(text[0] == '=' ? "inline expand(" : text[1] ? "nested expand(" : "expand("), bfmt);
		para.Cat(drl.ToWString(), myfmt);
		String title = HelpTopicGet(drl).GetTitle();
		if(!IsNull(title)) {
			para.Cat(WString(": "), bfmt);
			MQTFCat(para, title, myfmt);
		}
		para.Cat(WString(")"), bfmt);
	}
	else {
		if(IsNull(text))
			text = HelpTopicGet(drl).GetTitle();
		if(IsNull(text)) {
			myfmt.ink = LtBlue;
			para.Cat(WString("Bez nadpisu: "), myfmt);
			myfmt.Bold();
			para.Cat(drl.ToWString(), myfmt);
		}
		else {
			if(!book)
				myfmt.link = drl;
			MQTFCat(para, text, myfmt);
			if(book && !IsNull(style)) {
				String out;
				for(const char *p = style; *p; p++)
					if(*p == '#')
						out << StdFormat(vars.Get(drl, Null));
					else
						out.Cat(*p);
				myfmt.NoBold();
				MQTFCat(para, out, myfmt);
			}
		}
	}
	return para.part;
}

RichPara::FieldType& FieldTypeLink()
{
	static FieldTypeLinkCls ftl;
	return ftl;
}

Id FieldTypeLinkID() { static Id ftl("LINK"); return ftl; }

struct FieldTypeSectionCls : public RichPara::FieldType
{
	virtual Array<RichPara::Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
		const RichPara::CharFormat& fmt);
};

Array<RichPara::Part> FieldTypeSectionCls::Evaluate(const String& param,
	VectorMap<String, Value>& vars, const RichPara::CharFormat& basefmt)
{
	RichTextSection section = DecodeSection(param);
	RichPara para;
	RichPara::CharFormat fmt = basefmt;
	fmt.paper = WhiteGray;
	RichPara::CharFormat ctlfmt = fmt, datafmt = fmt;
	ctlfmt.ink = Green;
	ctlfmt.Bold();
	datafmt.Bold();
	para.Cat(WString(section.nested ? "nested section(" : "section("), ctlfmt);
	bool first = true;
//	if(!IsNull(section.pagesize))
//	{
//		para.Cat(String("pagesize "), fmt);
//		para.Cat(NFormat("%d x %d", section.pagesize.cx, section.pagesize.cy), datafmt);
//		first = false;
//	}
	if(!IsNull(section.columns) || !IsNull(section.column_space)) {
		if(!first) para.Cat(WString("; "), ctlfmt);
		para.Cat(WString("columns<"), fmt);
		bool done = false;
		if(!IsNull(section.columns)) {
			para.Cat(FormatInt(section.columns).ToWString(), datafmt);
			done = true;
		}
		if(!IsNull(section.column_space)) {
			if(done) para.Cat(WString(", "), ctlfmt);
			para.Cat(WString("spacing "), fmt);
			para.Cat(FormatInt(section.column_space).ToWString(), datafmt);
		}
		para.Cat(WString(">"), fmt);
		first = false;
	}
	if(!IsNull(section.firstpage)) {
		if(!first) para.Cat(WString("; "), ctlfmt);
		para.Cat(WString("firstpage "), fmt);
		para.Cat(FormatInt(section.firstpage).ToWString(), datafmt);
		first = false;
	}
	if(!IsNull(section.margin.left) || !IsNull(section.margin.top)
	|| !IsNull(section.margin.right) || !IsNull(section.margin.bottom)) {
		if(!first) para.Cat(WString("; "), ctlfmt);
		para.Cat(WString("margin<"), fmt);
		bool done = false;
		if(!IsNull(section.margin.left)) {
			para.Cat(WString("left "), fmt);
			para.Cat(FormatInt(section.margin.left).ToWString(), datafmt);
			done = true;
		}
		if(!IsNull(section.margin.top)) {
			if(done) para.Cat(WString(", "), ctlfmt);
			para.Cat(WString("top "), fmt);
			para.Cat(FormatInt(section.margin.top).ToWString(), datafmt);
			done = true;
		}
		if(!IsNull(section.margin.right)) {
			if(done) para.Cat(WString(", "), ctlfmt);
			para.Cat(WString("right "), fmt);
			para.Cat(FormatInt(section.margin.right).ToWString(), datafmt);
			done = true;
		}
		if(!IsNull(section.margin.bottom)) {
			if(done) para.Cat(WString(", "), ctlfmt);
			para.Cat(WString("bottom "), fmt);
			para.Cat(FormatInt(section.margin.bottom).ToWString(), datafmt);
			done = true;
		}
		para.Cat(WString(">"), fmt);
		first = false;
	}
	static const char *hftypes[] = { "left", "center", "right" };
	if(!IsNull(section.header_space) || !section.header[0].IsVoid()
	|| !section.header[1].IsVoid() || !section.header[2].IsVoid()) {
		if(!first) para.Cat(WString("; "), ctlfmt);
		para.Cat(WString("header<"), fmt);
		bool done = false;
		if(!IsNull(section.header_space)) {
			para.Cat(FormatInt(section.header_space).ToWString(), datafmt);
			done = true;
		}
		for(int i = 0; i < 3; i++)
			if(!section.header[i].IsVoid()) {
				if(done) para.Cat(WString(", "), ctlfmt);
				para.Cat((String().Cat() << hftypes[i] << ' ').ToWString(), fmt);
				para.Cat(section.header[i].ToWString(), datafmt);
				done = true;
			}
		para.Cat(WString(">"), fmt);
	}
	if(!IsNull(section.footer_space) || !section.footer[0].IsVoid()
	|| !section.footer[1].IsVoid() || !section.footer[2].IsVoid()) {
		if(!first) para.Cat(WString("; "), ctlfmt);
		para.Cat(WString("footer<"), fmt);
		bool done = false;
		if(!IsNull(section.footer_space)) {
			para.Cat(FormatInt(section.footer_space).ToWString(), datafmt);
			done = true;
		}
		for(int i = 0; i < 3; i++)
			if(!section.footer[i].IsVoid()) {
				if(done) para.Cat(WString(", "), ctlfmt);
				para.Cat((String().Cat() << hftypes[i] << ' ').ToWString(), fmt);
				para.Cat(section.footer[i].ToWString(), datafmt);
				done = true;
			}
		para.Cat(WString(">"), fmt);
	}
	para.Cat(WString(")"), ctlfmt);
	return para.part;
}

RichPara::FieldType& FieldTypeSection()
{
	static FieldTypeSectionCls ftl;
	return ftl;
}

Id FieldTypeSectionID() { static Id ftl("SECTION"); return ftl; }

RichTextSection::RichTextSection()
//: pagesize(Null)
{
	nested = false;
	columns = Null;
	firstpage = Null;
	margin.left = margin.top = margin.right = margin.bottom = Null;
	header_space = footer_space = column_space = Null;
	header[0] = header[1] = header[2] =
	footer[0] = footer[1] = footer[2] = String::GetVoid();
}

Rect RichTextSection::GetPageRect(Size pagesize) const
{
	return Rect(pagesize).Deflated(margin);
}

Rect RichTextSection::GetTextRect(Size pagesize, int colindex) const
{
	Rect textarea = GetPageRect(pagesize).Deflated(0, header_space, 0, footer_space);
	if(columns <= 1)
		return textarea;
	int colwd = (textarea.Width() - column_space * (columns - 1)) / columns;
	int offset = iscale(textarea.Width() - colwd, colindex, columns - 1);
	textarea.left += offset;
	textarea.right = textarea.left + colwd;
	return textarea;
}

Size RichTextSection::GetTextSize(Size pagesize) const
{
	return GetTextRect(pagesize, 0).Size();
}

void RichTextSection::MakeDefault()
{
//	pagesize = pg;
	columns = 1;
	firstpage = 1;
	margin = Rect(0, 0, 0, 0);
	header_space = 200;
	footer_space = 200;
	column_space = 200;
//	footer[1] = "- # -";
}

void RichTextSection::Override(RichTextSection& old) const
{
//	old.pagesize      = Nvl(pagesize, old.pagesize);
	old.columns       = Nvl(columns, old.columns);
	old.firstpage     = firstpage;
	old.charformat    = charformat;
	old.header_space  = Nvl(header_space, old.header_space);
	old.footer_space  = Nvl(footer_space, old.footer_space);
	old.column_space  = Nvl(column_space, old.column_space);
	old.margin.left   = Nvl(margin.left, old.margin.left);
	old.margin.top    = Nvl(margin.top, old.margin.top);
	old.margin.right  = Nvl(margin.right, old.margin.right);
	old.margin.bottom = Nvl(margin.bottom, old.margin.bottom);
	for(int i = 0; i < 3; i++) {
		if(!header[i].IsVoid()) old.header[i] = header[i];
		if(!footer[i].IsVoid()) old.footer[i] = footer[i];
	}
}

static inline void EncodeInt(String& out, const char *tag, int value)
{
	if(!IsNull(value))
		out << tag << ' ' << value << "; ";
}

String EncodeSection(const RichTextSection& sec)
{
	String out;
//	if(!IsNull(sec.pagesize))
//		out << "pagesize " << sec.pagesize.cx << " " << sec.pagesize.cy << "; ";
	if(sec.nested) out << "nested; ";
	EncodeInt(out, "columns",       sec.columns);
	EncodeInt(out, "column_space",  sec.column_space);
	EncodeInt(out, "firstpage",     sec.firstpage);
	EncodeInt(out, "margin_left",   sec.margin.left);
	EncodeInt(out, "margin_top",    sec.margin.top);
	EncodeInt(out, "margin_right",  sec.margin.right);
	EncodeInt(out, "margin_bottom", sec.margin.bottom);
	EncodeInt(out, "header_space",  sec.header_space);
	EncodeInt(out, "footer_space",  sec.footer_space);
	for(int i = 0; i < 3; i++) {
		if(!sec.header[i].IsVoid())
			out << "header[" << i << "] " << AsCString(sec.header[i]) << "; ";
		if(!sec.footer[i].IsVoid())
			out << "footer[" << i << "] " << AsCString(sec.footer[i]) << "; ";
	}
	return out;
}

RichTextSection DecodeSection(String sect)
{
	RichTextSection out;
	CParser parser(sect, "", 1);
	try {
		for(; !parser.IsEof(); parser.PassChar(';'))
//			if(parser.Id("pagesize"))
//			{
//				out.pagesize.cx = parser.ReadInt();
//				out.pagesize.cy = parser.ReadInt();
//			}
//			else
			/**/ if(parser.Id("nested"))        out.nested        = true;
			else if(parser.Id("columns"))       out.columns       = parser.ReadInt();
			else if(parser.Id("firstpage"))     out.firstpage     = parser.ReadInt();
			else if(parser.Id("header_space"))  out.header_space  = parser.ReadInt();
			else if(parser.Id("footer_space"))  out.footer_space  = parser.ReadInt();
			else if(parser.Id("column_space"))  out.column_space  = parser.ReadInt();
			else if(parser.Id("margin_left"))   out.margin.left   = parser.ReadInt();
			else if(parser.Id("margin_top"))    out.margin.top    = parser.ReadInt();
			else if(parser.Id("margin_right"))  out.margin.right  = parser.ReadInt();
			else if(parser.Id("margin_bottom")) out.margin.bottom = parser.ReadInt();
			else if(parser.Id("header")) {
				parser.PassChar('[');
				int index = parser.ReadInt();
				if(index < 0 || index >= 3)
					parser.ThrowError(NFormat("invalid header index: %d", index));
				parser.PassChar(']');
				out.header[index] = parser.ReadString();
			}
			else if(parser.Id("footer")) {
				parser.PassChar('[');
				int index = parser.ReadInt();
				if(index < 0 || index >= 3)
					parser.ThrowError(NFormat("invalid footer index: %d", index));
				parser.PassChar(']');
				out.footer[index] = parser.ReadString();
			}
			else
				parser.ThrowError("invalid text section parameter");
	}
	catch(Exc e) {
		RLOG("DecodeSection: " << e);
	}
	return out;
}

struct FieldTypeIndexCls : public RichPara::FieldType
{
	virtual Array<RichPara::Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
		const RichPara::CharFormat& fmt);
};

Array<RichPara::Part> FieldTypeIndexCls::Evaluate(const String& param,
	VectorMap<String, Value>& vars, const RichPara::CharFormat& linkfmt)
{
	RichTextIndex idx = DecodeIndex(param);
	RichPara para;
	RichPara::CharFormat ctlfmt = linkfmt, fmt = linkfmt;
	ctlfmt.ink = Green;
	ctlfmt.Bold();
	if((int)vars.Get("$book", 0) <= 0) { // hypertext
		ctlfmt.paper = fmt.paper = WhiteGray;
		para.Cat(WString("index("), ctlfmt);
		for(int i = 0; i < idx.reflist.GetCount(); i++) {
			if(i)
				para.Cat(WString("; "), ctlfmt);
			String space, nesting, topic;
			const char *drl = idx.reflist[i];
			if(*drl == '!')
				drl++;
			HelpParseDPP(drl, space, nesting, topic);
			para.Cat(topic.ToWString(), fmt);
		}
		para.Cat(WString(")"), ctlfmt);
	}
	else {
		Index<String> pageref;
		int i;
		for(i = 0; i < idx.reflist.GetCount(); i++) {
			String ref = idx.reflist[i];
			if(*ref == '!')
				ref.Remove(0);
			int f = vars.Find(ref);
			if(f < 0)
				f = vars.Find(HelpAppDPP(ref));
			String s = StdFormat(f >= 0 ? vars[f] : Value());
			if(!IsNull(s))
				pageref.FindAdd(s);
		}
		Vector<String> pagelist = pageref.PickKeys();
		Sort(pagelist, GetLanguageInfo());
		for(i = 0; i < pagelist.GetCount(); i++)
			if(!i || pagelist[i] != pagelist[i - 1]) {
				if(i)
					para.Cat(WString(", "), fmt);
				para.Cat(pagelist[i].ToWString(), fmt);
			}
	}
	return para.part;
}

RichPara::FieldType& FieldTypeIndex()
{
	static FieldTypeIndexCls ftl;
	return ftl;
}

Id FieldTypeIndexID() { static Id ftl("INDEX"); return ftl; }

String EncodeIndex(const RichTextIndex& idx)
{
	return Join(idx.reflist, ";");
}

RichTextIndex DecodeIndex(String packed)
{
	RichTextIndex out;
	out.reflist = Split(packed, ';');
	return out;
}

String EncodeLink(String target, String text, String style)
{
	return target + "\t" + text + "\t" + style;
}

void DecodeLink(String link, String& target, String& text, String& style)
{
	const char *p = link, *b = p;
	while(*p && *p != '\t')
		p++;
	target = String(b, p);
	text = style = Null;
	if(*p == '\t') {
		b = ++p;
		while(*p && *p != '\t')
			p++;
		text = String(b, p);
		if(*p == '\t')
			style = p + 1;
	}
}

String AsCStringWrap(const char *s, int linemax, const char *prefix)
{
	if(!s || !*s)
		return "\"\"";
	String out, best, next;
	for(; *s; s++)
		if((byte)*s >= ' ') {
			if(*s == '\"' || *s == '\\' || *s == '\'')
				next.Cat('\\');
			next.Cat(*s);
			if(*s == ' ') {
				if(!best.IsEmpty() && best.GetLength() + next.GetLength() > linemax) {
					if(!out.IsEmpty())
						out << '\n' << prefix;
					out << '\"' << best << '\"';
					best = Null;
				}
				best.Cat(next);
				next = Null;
			}
		}
		else if(*s == '\n') {
			next.Cat("\\n");
			if(!out.IsEmpty())
				out << '\n' << prefix;
			out << '\"' << best << next << '\"';
			best = next = Null;
		}
		else
			switch(*s) {
			case '\a': next << "\\a"; break;
			case '\b': next << "\\b"; break;
			case '\t': next << "\\t"; break;
			case '\v': next << "\\v"; break;
			case '\f': next << "\\f"; break;
			case '\r': next << "\\r"; break;
			default:   next << '\\' << FormatIntOct((byte)*s, 3); break;
			}
	if(!best.IsEmpty() || !next.IsEmpty()) {
		if(!out.IsEmpty())
			out << '\n' << prefix;
		out << '\"' << best << next << '\"';
	}
	return out;
}


RichObject CreateImageObject(Image image, Size dot_size, Size out_size)
{
#ifndef NEWIMAGE
	RichObject out("PNG", PngEncoder().DotSize(dot_size).SaveImage(image));
	out.SetSize(out_size);
#else
	RichObject out("PNG", Null);
#endif
	return out;
}

struct RichObjectTypeEmptyCls : public RichObjectType
{
	RichObjectTypeEmptyCls() {}

	virtual String GetTypeName(const Value&) const { return "EMPTY"; }
	virtual Size   GetPhysicalSize(const Value& data) const { return Size(0, 0); }
	virtual Size   GetPixelSize(const Value& data) const { return Size(1, 1); }
	virtual void   Paint(const Value& data, Draw& w, Size sz) const {}
};

RichObjectType *RichObjectTypeEmpty() { return &Single<RichObjectTypeEmptyCls>(); }

#if 0
struct RichObjectTypeGIFCls : public RichObjectType
{
	RichObjectTypeGIFCls() {}

	virtual String GetTypeName(const Value&) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
//	virtual Value  Read(const String& s) const;
//	virtual String Write(const Value& v) const;
//	virtual Value  ReadClipboard() const;
//	virtual void   Menu(Bar& bar, RichObjectExchange& ex) const;
//	virtual void   DefaultAction(RichObjectExchange& ex) const;
};

RichObjectType *RichObjectTypeGIF() { return &Single<RichObjectTypeGIFCls>(); }

String RichObjectTypeGIFCls::GetTypeName(const Value&) const
{
	return "GIF";
}

Size RichObjectTypeGIFCls::GetPhysicalSize(const Value& data) const
{
	ImageInfo ii = GifEncoder().InfoImage(data);
	if(ii.dots.cx > 0 && ii.dots.cy > 0)
		return ii.dots;
	return 6 * ii.size;
}

Size RichObjectTypeGIFCls::GetPixelSize(const Value& data) const
{
	return GifEncoder().InfoImage(data).size;
}

/*
Value RichObjectTypeGIFCls::Read(const String& s) const
{
}

String RichObjectTypeGIFCls::Write(const Value& v) const
{
}
*/

void RichObjectTypeGIFCls::Paint(const Value& data, Draw& w, Size sz) const
{
	if(IsNull(data)) {
		w.DrawRect(Rect(sz), SGray);
		return;
	}
	PixelArray array = GifEncoder().LoadArray(data).pixel;
	Size aasize(min(sz.cx, array.GetWidth() * 4), min(sz.cy, array.GetHeight() * 4));
	PixelArray aa(aasize, -3);
	PixelCopyAntiAlias(aa, aa.GetRect(), array, array.GetRect());
	if(w.IsDrawing())
		w.DrawImage(Rect(sz), PixelArrayToImage(aa));
	else
		aa.Paint(w, Rect(sz));
}
#endif

/*
void CatRichText(RichText& text, const RichText& text2)
{
	text.OverrideStyles(text2.GetStyles(), false);
	for(int i = 0; i < text2.GetParaCount(); i++)
		text.Cat(text2.Get(i));
//	int sn = text.GetStyleCount();
//	for(i = 0; i < text2.GetStyleCount(); i++)
//		text.SetParaStyle(sn + i, text2.GetParaStyle(i));
//	for(i = 0; i < text2.GetParaCount(); i++)
//		text.Cat(text2.Get(i), text2.GetParaStyle(i) + sn);
}
*/

void AppendQTF(RichText& text, String qtf)
{
	text.CatPick(ParseQTF(qtf));
//	CatRichText(text, ParseQTF(qtf));
}

class RichTextDisplayCls : public Display
{
public:
	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const;

	struct RTDPageDraw : public PageDraw {
		virtual Draw& Page(int _page)          { return _page ? (Draw&)ScreenInfo() : w; }
		virtual Draw& Info()                   { return w; }

		Draw&              w;

		RTDPageDraw(Draw& w) : w(w) {}
	};
};

void RichTextDisplayCls::Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const
{
	String s;
	s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") " << StdFormat(v);
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(Zoom(140, 1024));
	draw.DrawRect(rc, paper);
	draw.Clipoff(rc);
	rtext.Paint(Zoom(1, 1), draw, 0, 0, rc.Width());
	draw.End();
/*
//	Size page = text.GetPage();
	Size sz = rc.Size();
	PaintInfo paintinfo;
	paintinfo.sell = paintinfo.selh = 0;
	paintinfo.zoom = Zoom(DOC_SCREEN_ZOOM, 1024);
	int doty = sz.cy / paintinfo.zoom;
	paintinfo.bottom = PageY(0, doty);
	RTDPageDraw pd(draw);
	draw.Clipoff(rc);
	draw.DrawRect(sz, paper);
	rtext.Paint(pd, Rect(0, 0, 1000000, doty), PageY(0, 0), paintinfo);
	draw.End();
*/
}

const Display& RichTextDisplay()
{
	static RichTextDisplayCls rtd;
	return rtd;
}

void RichObjectDisplay::Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const
{
	Size outsize = rc.Size();
	if(outsize.cx <= 0 || outsize.cy <= 0)
		return;
	draw.DrawRect(rc, paper);
	if(type) {
		Size rawsize = type->GetPhysicalSize(v, context);
		if(MulVector(rawsize, outsize) >= 0)
			outsize.cy = iscale(outsize.cx, rawsize.cy, rawsize.cx);
		else
			outsize.cx = iscale(outsize.cy, rawsize.cx, rawsize.cy);
		Rect rrc(Point((rc.TopLeft() + rc.BottomRight() - outsize) >> 1), outsize);
		draw.Clipoff(rrc);
		type->Paint(v, draw, outsize, context);
		draw.End();
	}
	else {
		Point pos = (rc.CenterPoint() - CtrlImg::question().GetSize()) >> 1;
		draw.DrawImage(pos.x, pos.y, CtrlImg::question());
	}
}

Size RichObjectDisplay::GetStdSize(const Value& v) const
{
	return type ? type->GetPhysicalSize(v, context) : CtrlImg::question().GetSize();
}

/*
struct RichObjectTypeCalcHelpCls : public RichObjectType
{
	RichObjectTypeCalcHelpCls() { RichObject::Register(this); }

	virtual String GetTypeName() const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
//	virtual Value  Read(const String& s) const;
//	virtual String Write(const Value& v) const;
//	virtual Value  ReadClipboard() const;
//	virtual void   Menu(Bar& bar, RichObjectExchange& ex) const;
//	virtual void   DefaultAction(RichObjectExchange& ex) const;

private:
	static Document  GetDoc(const String& data);
};

RichObjectType *RichObjectTypeCalcHelp()
{
	static RichObjectTypeCalcHelpCls __rotch;
	return &__rotch;
}

//	RichObject::Register(RichObjectTypeDialogHelp());
//	RichObject::Register(RichObjectTypeCalcHelp());
)

String RichObjectTypeCalcHelpCls::GetTypeName() const
{
	return "RichObjectTypeCalcHelp";
}

Document RichObjectTypeCalcHelpCls::GetDoc(const String& data)
{
	VectorMap<String, String> attr = DecodeAttr(data);
	String qtf;
	for(int i = 0; i < attr.GetCount(); i++)
		qtf << "\1" << attr.GetKey(i) << "\1 = [* \1" << attr[i] << "\1\n";
	return Document(qtf);
}

Size RichObjectTypeCalcHelpCls::GetPhysicalSize(const Value& data) const
{
	Document doc = GetDoc(data);
	int wd = doc.GetWidth(DOC_SCREEN_ZOOM, ScreenInfo());
	int ht = doc.GetHeight(DOC_SCREEN_ZOOM, ScreenInfo(), wd);
	return Size(wd, ht);
}

void RichObjectTypeCalcHelpCls::Paint(const Value& data, Draw& w, Size sz) const
{
	Document doc = GetDoc(data);
	int wd = doc.GetWidth(DOC_SCREEN_ZOOM, ScreenInfo());
	int ht = doc.GetHeight(DOC_SCREEN_ZOOM, ScreenInfo(), wd);
	DrawingDraw ddraw(wd, ht);
	ddraw.SetPixels(true);
	doc.Paint(DOC_SCREEN_ZOOM, ddraw, 0, 0, wd);
	w.DrawDrawing(Rect(sz), ddraw.GetResult());
}
*/

static Vector<WString> SplitIndexEntry(const wchar *p, char brk)
{
	Vector<WString> out;
	const wchar *b = p;
	while(*p)
		if((*p == '`' || *p == '\\')) {
			if(*++p)
				p++;
		}
		else if(*p == '[') {
			while(*++p && *p != ' ' && *p != ']')
				;
		}
		else if(*p == '\1') {
			while(*p && *p++ != '\1')
				;
		}
		else if(*p == brk) {
			if(p > b)
				out.Add(WString(b, p));
			while(*p == brk || *p && *p <= L' ')
				p++;
			b = p;
		}
		else
			p++;
	if(p > b)
		out.Add(WString(b, p));
	return out;
}

struct sIndexEntryIterator : public RichText::Iterator
{
};

void HelpTopicGetIndex(Index<WString>& entries, Vector<String>& drls, Gate2<int, int> progress)
{
	Index<WString> lent;
	Vector<String> ltop;
	int i;
	for(i = 0; i < HelpAutoIndexMap().GetCount(); i++) {
		HelpAutoIndexMap()[i](lent, ltop);
		ASSERT(lent.GetCount() == ltop.GetCount());
	}

	const ArrayMap<String, HelpTopicInfo>& map = HelpTopicMap();
	String styles = HelpTopicGet(HelpDPPStyle()).GetText() + "\r\n";
	for(i = 0; i < map.GetCount(); i++) {
		String drl = map.GetKey(i);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		if(nesting == "sys")
			continue;
		const HelpTopicInfo& info = map[i];
		RichText text = ParseQTF(styles + info.GetText());
		Vector<RichValPos> valpov = text.GetValPos(Size(10000, INT_MAX), RichText::INDEXENTRIES);
		Index<WString> tent;
		for(int p = 0; p < valpov.GetCount(); p++) {
			WString ixe = valpov[p].data;
			if(!IsNull(ixe) && !(ixe[0] == '@' && ixe[1] == '@')) {
				Vector<WString> ient = SplitIndexEntry(ixe, ';');
				for(int e = 0; e < ient.GetCount(); e++) {
					const wchar *p = ient[e];
					String top;
					if(*p == '!') {
						p++;
						top.Cat('!');
					}
					top.Cat(topic);
					WString nent(p);
					if(tent.Find(nent) < 0) {
						tent.Add(nent);
						int f = lent.Find(nent);
						if(f < 0) {
							tent.Add(nent);
							lent.Add(nent);
							ltop.Add(top);
						}
						else
							ltop[f] << "; " << top;
					}
				}
			}
		}
/*
		for(int p = 0; p < text.GetParaCount(); p++) {
			const RichPara& rp = text.GetCached(p);
			for(int pp = 0; pp < rp.part.GetCount(); pp++) {
				WString ixe = rp.part[pp].format.indexentry;
				if(!IsNull(ixe) && !(ixe[0] == '@' && ixe[1] == '@')) {
					Vector<WString> ient = SplitIndexEntry(ixe, ';');
					for(int e = 0; e < ient.GetCount(); e++) {
						const wchar *p = ient[e];
						String top;
						if(*p == '!') {
							p++;
							top.Cat('!');
						}
						top.Cat(topic);
						WString nent(p);
						if(tent.Find(nent) < 0) {
							tent.Add(nent);
							int f = lent.Find(nent);
							if(f < 0) {
								tent.Add(nent);
								lent.Add(nent);
								ltop.Add(top);
							}
							else
								ltop[f] << "; " << top;
						}
					}
				}
			}
		}
*/
	}
	Vector<WString> lstrip;
	lstrip.SetCount(lent.GetCount());
	for(i = 0; i < lent.GetCount(); i++)
		lstrip[i] = MQTFStrip(lent[i]);
	Vector<int> order = GetSortOrder(lstrip, GetLanguageInfo());
	for(i = 0; i < order.GetCount(); i++) {
		entries.Add(lent[order[i]]);
		drls.Add(ltop[order[i]]);
	}
}

Array<HelpIndexFormattedEntry> HelpTopicFormatIndex()
{
	Index<WString> index;
	Vector<String> topics;

	HelpTopicGetIndex(index, topics, Progress("Pøipravuji rejstøík"));

	// format index entries
	Array<HelpIndexFormattedEntry> entries;
	Vector<WString> prefix;
	WString last_first;

	for(int i = 0; i < index.GetCount(); i++) {
		Vector<WString> parts = SplitIndexEntry(index[i], ',');
		if(parts.IsEmpty())
			continue;

		// generate letter breaker
		WString next = GetLanguageInfo().GetIndexLetter(MQTFStrip(parts[0]));
		if(next != last_first) { // add letter entry
			HelpIndexFormattedEntry& entry = entries.Add();
			entry.nesting = -1;
			entry.text = next;
		}
		last_first = next;

		HelpIndexFormattedEntry& entry = entries.Add();
		entry.nesting = 0;
		for(int px = 0; px < parts.GetCount(); px++) {
			if(IsNull(entry.text) && prefix.GetCount() > px && prefix[px] == parts[px])
				entry.nesting++;
			else {
				prefix.DoIndex(px) = parts[px];
				if(!IsNull(entry.text))
					entry.text << ',' << ' ';
				entry.text.Cat(parts[px]);
			}
		}
		prefix.SetCount(parts.GetCount());
		entry.full_text = index[i];
		entry.topics = topics[i];
	}

	return entries;
}

bool CtrlShowHelp(Ctrl *ctrl)
{
	String topic;
	for(; IsNull(topic) && ctrl; ctrl = ctrl->GetParent())
		topic = ctrl->GetHelpTopic();
	if(IsNull(topic))
		return false;
	RunDlgHelpTopic(topic);
	return true;
}

static bool sViewerKeyHook(Ctrl *ctrl, dword key, int repcnt)
{
	if(key == K_F1 && CtrlShowHelp(ctrl))
		return true;
	return false;
}

void InstallHelpViewerKeyHook()
{
	Ctrl::InstallKeyHook(&sViewerKeyHook);
}

static bool sEditorKeyHook(Ctrl *ctrl, dword key, int repcnt)
{
	if(sViewerKeyHook(ctrl, key, repcnt))
		return true;
	if(key == K_SHIFT_F12 || key == K_CTRL_F12 || key == K_F12) {
		if(ctrl == NULL) {
/*
			Ctrl *f = Ctrl::GetFocusCtrl();
			String fd = "NULL";
			if(f) {
				StringStream out;
				f->Dump(out);
				fd = out;
			}
			Exclamation("Current control = NULL, focus = " + fd);
*/
			BeepExclamation();
			return true;
		}
		Ctrl *wctrl = ctrl->GetTopCtrl();
		LLOG("sEditorKeyHook, key = " << GetKeyDesc(key) << " for " << Name(wctrl));
		if(key == K_SHIFT_F12) {
			Point mouse = UPP::GetMousePos();
			Point wmouse = mouse - wctrl->GetScreenView().TopLeft();
			Ctrl *cpt = wctrl->ChildFromPoint(wmouse);
			if(!cpt)
				cpt = wctrl;
			Point delta = mouse - cpt->GetScreenView().TopLeft();
			Image img = cpt->MouseEvent(Ctrl::CURSORIMAGE, delta, 0, GetMouseFlags());
			Point imgmouse = mouse - wctrl->GetScreenRect().TopLeft();
//			RunDlgHelpEditor(wctrl, CtrlToStillImage(wctrl, imgmouse, img));
			RunDlgShot(wctrl, CtrlToStillImage(wctrl, imgmouse, img));
		}
		else // if(key == K_CTRL_F12)
			RunDlgShot(wctrl, CtrlToStillImage(wctrl));
//			RunDlgHelpEditor(wctrl, CtrlToStillImage(wctrl));
//		else
//			RunDlgHelpEditor(wctrl);
		return true;
	}
	return false;
}

void InstallHelpEditorKeyHook()
{
	Ctrl::InstallKeyHook(&sEditorKeyHook);
}

RichText HelpTopicExpand(String style_qtf, String title_qtf, String topic_qtf,
	String title_style_name, bool nested, int depth, const RichText::FormatInfo& formatinfo,
	String inline_link_style, Gate2<int, int> progress, int& progress_pos)
{
	RichText out_text, in_text, styles = ParseQTF(style_qtf);
	in_text.SetStyles(styles.GetStyles());

	AppendQTF(in_text, style_qtf + "\r\n" + topic_qtf);
	in_text.OverrideStyles(styles.GetStyles());
	out_text.SetStyles(in_text.GetStyles());
	RichPara empty_para;

	if(!IsNull(title_qtf) && !IsNull(title_style_name)) {
		Uuid styleid = styles.GetStyleId(title_style_name);
		const RichStyle& style = styles.GetStyle(styleid);
		RichPara title_para;
		title_para.format = style.format;
		formatinfo.ApplyTo(title_para.format);
		RichPara::CharFormat charformat = style.format;
	//	format.paper = Color(224, 224, 224);
		MQTFCat(title_para, title_qtf, charformat);
		out_text.Cat(title_para);
//		out_text.Cat(empty_para);
	}

	Id linkfld = FieldTypeLinkID();
	Id secfld = FieldTypeSectionID();
	for(int i = 0; i < in_text.GetPartCount(); i++) {
		if(progress(++progress_pos, 0))
			throw AbortExc();
		if(in_text.IsTable(i)) {
			out_text.CatPick(RichTable(in_text.GetTable(i), 0));
//			out_text.Cat(empty_para);
			continue;
		}
		RichPara para = in_text.Get(i);
		for(int p = 0; p < para.GetCount(); p++)
			if(para[p].field == linkfld) {
				String drl, linktext, linkstyle;
				DecodeLink(para[p].fieldparam, drl, linktext, linkstyle);
				int linkexpand = (linktext == "." ? 1 : linktext == ".." ? 2 : linktext == "=" ? 3 : 0);
				if(linkexpand || nested) {
					RichPara next;
					next <<= para;
					next.Mid(p + 1);
					if(p) {
						para.Trim(p);
						out_text.Cat(para);
					}
					RichText subtext;
					if(depth >= 10) {
						RichPara error;
						RichPara::CharFormat fmt = para.format;
						fmt.ink = LtRed;
						error.Cat(WString("Byla pøekroèena maximální hloubka vnoøení 'expand'!"), fmt);
						subtext.Set(0, error, subtext.GetStyles());
					}
					else {
						Uuid titleid = para.format.styleid;
						const RichStyle& style = in_text.GetStyle(titleid);
						String titlestyle = style.name;
						RichText::FormatInfo finfo;
						finfo.Set(para.format);
						finfo.Combine(style.format);
						finfo.charvalid ^= -1;
						finfo.paravalid ^= -1;
						if(titleid == RichStyle::GetDefaultId())
							titlestyle = title_style_name;
						else {
							int dot = titlestyle.Find('.');
							if(dot >= 0)
								titlestyle.Remove(0, dot + 1);
						}
						subtext = HelpTopicExpand(style_qtf,
							HelpTopicGet(drl).GetTitle(), HelpTopicGet(drl).GetText(),
							titlestyle, linkexpand == 2 || linkexpand == 3 && nested, depth + 1, finfo,
							linkexpand == 3 ? linkstyle : String(Null), progress, progress_pos);
						if(!subtext.IsEmpty() && linkexpand != 2) {
							subtext.SetStyle(titleid, style);
							RichPara para0 = subtext.Get(0);
							RichPara parab;
							parab.format = para0.format;
							parab.Cat(RichObject(RichObjectTypeEmpty(), Null), parab.format);
							parab[0].format.indexentry = ("@@" + drl).ToWString();
							parab.Append(para0);
							subtext.Set(0, parab, subtext.GetStyles());
							if(!nested)
								for(int p = 0; p < subtext.GetPartCount(); p++)
									if(subtext.GetParaStyle(p) == RichStyle::GetDefaultId())
										subtext.SetParaStyle(p, titleid);
						}
					}
					out_text.CatPick(subtext);
					para = next;
					p = -1;
				}
				else if(IsNull(linkstyle) && !IsNull(inline_link_style)) {
					para[p].fieldparam = EncodeLink(drl, linktext, inline_link_style);
				}
			}
			else if(p == 0 && para[p].field == secfld && !nested) {
				RichTextSection sect = DecodeSection(para[p].fieldparam);
				if(sect.nested)
					para = RichPara();
				break;
			}
		if(!para.IsEmpty()) {
			out_text.Cat(para);
		}
	}

	return out_text;
}

RichText HelpTopicExpand(String drl, String title_style_name, const RichText::FormatInfo& formatinfo, Gate2<int, int> progress)
{
	String style_qtf = HelpTopicGet(HelpDPPStyle()).GetText();
	String title_qtf = HelpTopicGet(drl).GetTitle();
	String topic_qtf = HelpTopicGet(drl).GetText();
	int progress_pos = 0;
	return HelpTopicExpand(style_qtf, title_qtf, topic_qtf, title_style_name, false, 0, formatinfo, Null, progress, progress_pos);
}

GLOBAL_VAR(const RichText::FormatInfo, NoFormat)

/*
struct SectionPageDraw : PageDraw {
	int         page;
	DrawingDraw w;
	Vector<Drawing> pagemap;
	NilDraw     nw;

	Draw& Page(int _page)     { return page == _page ? w : (Draw&)nw; }
	Draw& Info()              { return w; }
	void  SetPage(int _page)  { page = _page; }

	PrintPageDraw(Draw& w) : w(w) {}
};

void Print(PrintDraw& w, const RichText& text,
           int firstpage, int lastpage, int copies, bool collate)
{
	firstpage = max(0, firstpage);
	lastpage = min(lastpage, text.GetHeight(text.GetPage()).page);
	PrintPageDraw pw(w);
	Rect page = text.GetPage();
	Size sz = w.GetPageMMs();
	Size pgsz = page.Size();
	int x = (6000 * sz.cx / 254 - pgsz.cx) / 2;
	int y = (6000 * sz.cy / 254 - pgsz.cy) / 2;
	for(int c = 0; c < (collate ? copies : 1); c++)
		for(int i = firstpage; i <= lastpage; i++)
			for(int c = 0; c < (collate ? 1 : copies); c++) {
				w.StartPage();
				w.Offset(x, y);
				pw.SetPage(i);
				PaintInfo paintinfo;
				paintinfo.top = PageY(i, 0);
				paintinfo.bottom = PageY(i + 1, 0);
				text.Paint(pw, page, PageY(), paintinfo);
				w.End();
				w.EndPage();
			}
}
*/

class SectionPageDraw : public PageDraw
{
public:
	SectionPageDraw(Size drawingsize, Gate2<int, int> progress, int& progress_pos);

	virtual Draw&   Page(int page);
	virtual Draw&   Info();

	Vector<Drawing> Close();

private:
	Gate2<int, int> progress;
	int&            progress_pos;
	Size            drawingsize;
	Vector<Drawing> pages;
	DrawingDraw     ddraw;
	DrawingDraw     info;
	int             currentpage;
};

SectionPageDraw::SectionPageDraw(Size dsz, Gate2<int, int> progress, int& progress_pos)
: progress(progress)
, progress_pos(progress_pos)
{
	currentpage = -1;
	info.Create(drawingsize = dsz);
}

Draw& SectionPageDraw::Page(int page)
{
	if(page != currentpage) {
		ASSERT(ddraw.GetCloffLevel() == 0);
		progress(progress_pos + page, -1);
		if(currentpage >= 0)
			pages.At(currentpage) = ddraw;
		ddraw.Create(drawingsize);
		ddraw.DrawDrawing(drawingsize, pages.At(currentpage = page));
	}
	return ddraw;
}

Draw& SectionPageDraw::Info()
{
	return Page(0);
}

Vector<Drawing> SectionPageDraw::Close()
{
	if(currentpage >= 0) {
		ASSERT(ddraw.GetCloffLevel() == 0);
		pages.At(currentpage) = ddraw;
	}
	return pages;
}

static String ExpandHFText(const char *text, int pageno)
{
	String out;
	for(char c; c = *text++;)
		if(c == '#')
			switch(*text++) {
			case 'r': out.Cat(FormatIntRoman(pageno, false)); break;
			case 'R': out.Cat(FormatIntRoman(pageno, true)); break;
			case 'a': out.Cat(FormatIntAlpha(pageno, false)); break;
			case 'A': out.Cat(FormatIntAlpha(pageno, true)); break;
			default:  text--; out.Cat(FormatInt(pageno)); break;
			}
		else
			out.Cat(c);
	return out;
}

Vector<Drawing> RichTextPrintSection(const PrintSection& section, Size pagesize, Gate2<int, int> progress, int& progress_pos)
{
	ASSERT(section.columns > 0);
	Size drawingsize = section.GetTextSize(pagesize);
	SectionPageDraw spdraw(drawingsize, progress, progress_pos);
	PaintInfo paintinfo;
	paintinfo.top = PageY(0, 0);
	paintinfo.bottom = PageY(32767, 0);
	paintinfo.indexentry = Null;
	section.text.Paint(spdraw, drawingsize, paintinfo);
	Vector<Drawing> in = spdraw.Close();
	Vector<Drawing> out;
	for(int i = 0; i < in.GetCount(); i += section.columns) {
		int pageno = section.firstpage + out.GetCount();
		DrawingDraw ddraw(pagesize);
		Rect pagerect = section.GetPageRect(pagesize);
		for(int c = 0; c < section.columns && c + i < in.GetCount(); c++)
			ddraw.DrawDrawing(section.GetTextRect(pagesize, c), in[i + c]);
		for(int hf = 0; hf < 3; hf++) {
			if(!IsNull(section.header[hf])) {
				String s = ExpandHFText(section.header[hf], pageno);
				Size sz = GetTextSize(s, section.charformat);
				int x;
				switch(hf) {
				case 0: x = pagerect.left; break;
				case 1: x = (pagerect.left + pagerect.right - sz.cx) >> 1; break;
				case 2: x = pagerect.right - sz.cx; break;
				}
				ddraw.DrawText(x, pagerect.top, s, section.charformat);
			}
			if(!IsNull(section.footer[hf])) {
				String s = ExpandHFText(section.footer[hf], pageno);
				Size sz = GetTextSize(s, section.charformat);
				int x;
				switch(hf) {
				case 0: x = pagerect.left; break;
				case 1: x = (pagerect.left + pagerect.right - sz.cx) >> 1; break;
				case 2: x = pagerect.right - sz.cx; break;
				}
				ddraw.DrawText(x, pagerect.bottom - sz.cy, s, section.charformat);
			}
		}
		out.Add() = ddraw;
	}
	return out;
}

Array<PrintSection> RichTextSplitSections(const RichText& text, Gate2<int, int> progress)
{
	Array<PrintSection> out;
	PrintSection psec;
	psec.MakeDefault();
	psec.text.SetStyles(text.GetStyles());
	Id secfld = FieldTypeSectionID();
	for(int i = 0; i < text.GetPartCount(); i++) {
		if(progress(i, text.GetPartCount()))
			throw AbortExc();
		if(text.IsTable(i))
			psec.text.CatPick(RichTable(text.GetTable(i), 0));
		else {
			RichPara para = text.Get(i);
			if(para.GetCount() > 0 && para[0].field == secfld) {
				if(!psec.text.IsEmpty())
					out.Add() = psec;
				psec.text.Clear();
				psec.text.SetStyles(text.GetStyles());
				DecodeSection(para[0].fieldparam).Override(psec);
			}
			else {
				if(psec.text.GetPartCount() == 0)
					para.format.newpage = false;
				psec.text.Cat(para);
			}
		}
	}
	if(!psec.text.IsEmpty())
		out.Add() = psec;
	return out;
}

Vector<Drawing> SectionPrintBook(Array<PrintSection>& sections, Size pagesize, VectorMap<String, Value>& vars, Gate2<int, int> progress)
{
	vars.GetAdd("$book") = 1;
	bool stable = false;
	enum { MAX_PASSES = 10 };
	int pass;
	for(pass = 1; !stable && pass <= MAX_PASSES; pass++) {
		if(progress(pass, 0))
			throw AbortExc();
		stable = true;
		int pageno = 1;
		Index<String> chkdup;
		for(int s = 0; s < sections.GetCount(); s++) {
			PrintSection& psec = sections[s];
			Size textsize = psec.GetTextSize(pagesize);
			PageY begin;
			pageno = Nvl(psec.firstpage, pageno);
			psec.text.EvaluateFields(vars);
			int numpages = psec.text.GetHeight(textsize).page / psec.columns + 1;
			Vector<RichValPos> posinfo = psec.text.GetValPos(textsize, RichText::INDEXENTRIES);
			for(int i = 0; i < posinfo.GetCount(); i++) {
				String key = posinfo[i].data.ToString();
				if(key[0] != '@' || key[1] != '@')
					continue;
				key.Remove(0, 2);
				if(chkdup.Find(key) >= 0) {
					if(pass == 1)
						RLOG("Duplicate: " << key);
					continue;
				}
				chkdup.Add(key);
				int oldpage = vars.Get(key, Value());
				int newpage = pageno + posinfo[i].py.page;
				if(newpage != oldpage) {
					if(pass > 2)
						RLOG("Unstable: " << key << ": " << oldpage << " -> " << newpage);
					vars.GetAdd(key) = newpage;
					stable = false;
				}
			}
			pageno += numpages;
		}
	}

	Vector<Drawing> out;
	int pageno = 1;
	int progress_pos = pass;
	for(int s = 0; s < sections.GetCount(); s++) {
		PrintSection& psec = sections[s];
		psec.firstpage = Nvl(psec.firstpage, pageno);
		Vector<Drawing> part = RichTextPrintSection(psec, pagesize, progress, progress_pos);
		pageno = psec.firstpage + part.GetCount();
		out.AppendPick(part);
	}
	return out;
}

void CreateHelpBook()
{
	try {
		VectorMap<String, Value> vars;
		DocReport report;
		Progress progress;
		progress.SetText("Sestavuji pøíruèku (odstavec %d)");
		RichText text = HelpTopicExpand(HelpHostDPP("book"), Null, NoFormat(), progress);
		progress.SetText("Pøipravuji tiskové oddíly (odstavec %d)");
		Array<PrintSection> sections = RichTextSplitSections(text, progress);
		progress.SetText("Formátuji pøíruèku (strana %d)");
		Vector<Drawing> pages = SectionPrintBook(sections, report.GetPageSize(), vars, progress);
		progress.SetText("Tisknu pøíruèku (strana %d)");
		for(int i = 0; i < pages.GetCount(); i++) {
			if(progress.SetCanceled(i, pages.GetCount()))
				throw AbortExc();
			if(i)
				report.NextPage();
			report.DrawDrawing(pages[i].GetSize(), pages[i]);
		}
		report.Perform();
	}
	catch(Exc e) {
		ShowExc(e);
	}
/*
	One<BookHelpStyle> book_style = CreateBookHelpStyle();
	help_style = ~book_style;
	book_style -> RunBook();
	help_style = 0;
*/
}

#ifdef PLATFORM_WIN32
String CreateHelpPDF()
{
	try {
		VectorMap<String, Value> vars;
		PdfDraw pdf(210 * 6000 / 254, 297 * 6000 / 254);
#ifdef SYSTEMDRAW
		Rect margin = Rect(pdf.GetPageSize()).Deflated(236);
#else
		Rect margin = Rect(pdf.GetPagePixels()).Deflated(236);
#endif
		Progress progress;
		progress.SetText("Sestavuji pøíruèku (odstavec %d)");
		RichText text = HelpTopicExpand(HelpHostDPP("book"), Null, NoFormat(), progress);
		progress.SetText("Pøipravuji tiskové oddíly (odstavec %d)");
		Array<PrintSection> sections = RichTextSplitSections(text, progress);
		progress.SetText("Formátuji pøíruèku (strana %d)");
		Vector<Drawing> pages = SectionPrintBook(sections, margin.Size(), vars, progress);
		progress.SetText("Tisknu pøíruèku (strana %d)");
//		progress.Set(0, pages.GetCount());
		for(int i = 0; i < pages.GetCount(); i++) {
//			LOG("CreateHelpPDF: GDI space = " << GdiGetFreeSpace());
			if(progress.SetCanceled(i, pages.GetCount()))
				throw AbortExc();
			pdf.StartPage();
//			ScreenInfo().DrawDrawing(margin, pages[i]);
			pdf.DrawDrawing(margin, pages[i]);
			pdf.EndPage();
		}
		return pdf.Finish();
	}
	catch(Exc e) {
		ShowExc(e);
		return Null;
	}
}
#endif

String CreateHelpRTF()
{
	try {
		VectorMap<String, Value> vars;
		Progress progress;
		progress.SetText("Sestavuji pøíruèku (odstavec %d)");
		RichText text = HelpTopicExpand(HelpHostDPP("book"), Null, NoFormat(), progress);
		return EncodeRTF(text, CHARSET_WIN1250);
	}
	catch(Exc e) {
		ShowExc(e);
		return Null;
	}
}


END_UPP_NAMESPACE
