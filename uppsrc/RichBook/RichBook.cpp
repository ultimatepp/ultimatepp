#include "RichBook.h"

#include <DocTypes/DocTypes.h>
#include <PdfDraw/PdfDraw.h>

NAMESPACE_UPP

struct FieldTypeVarCls : public RichPara::FieldType
{
	virtual Array<RichPara::Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
		const RichPara::CharFormat& fmt);
};

Array<RichPara::Part> FieldTypeVarCls::Evaluate(const String& dest,
	VectorMap<String, Value>& vars, const RichPara::CharFormat& linkfmt)
{
	RichPara para;
	RichPara::CharFormat fmt = linkfmt;
	int f = vars.Find(dest);
	if(f < 0) {
		fmt.ink = LtRed();
		fmt.paper = WhiteGray();
		para.Cat(dest, fmt);
	}
	else
		para.Cat(StdFormat(vars[f]), fmt);
	return para.part;
}

RichPara::FieldType& GLOBAL_V(FieldTypeVarCls, FieldTypeVar);

Id FieldTypeVarID()
{
	static Id ftv("VAR");
	return ftv;
}

INITBLOCK {
	RichPara::Register(FieldTypeVarID(), FieldTypeVar());
}

/*
RichPara::FieldType& FieldTypeIndexEntry()
{
}

Id FieldTypeIndexEntryID()
{
}

String EncodeIndexEntry(const RichIndexEntry& idx)
{
}

RichIndexEntry DecodeIndexEntry(String encoded_indexentry)
{
}
*/

RichBookSection::RichBookSection()
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

Rect RichBookSection::GetPageRect(Size pagesize) const
{
	return Rect(pagesize).Deflated(margin);
}

Rect RichBookSection::GetTextRect(Size pagesize, int colindex) const
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

Size RichBookSection::GetTextSize(Size pagesize) const
{
	return GetTextRect(pagesize, 0).Size();
}

class RichSectionPageDraw : public PageDraw
{
public:
	RichSectionPageDraw(Size drawingsize, Gate2<int, int> progress, int& progress_pos);

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

RichSectionPageDraw::RichSectionPageDraw(Size dsz, Gate2<int, int> progress, int& progress_pos)
: progress(progress)
, progress_pos(progress_pos)
{
	currentpage = -1;
	info.Create(drawingsize = dsz);
}

Draw& RichSectionPageDraw::Page(int page)
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

Draw& RichSectionPageDraw::Info()
{
	return Page(0);
}

Vector<Drawing> RichSectionPageDraw::Close()
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

Vector<Drawing> RichPrintSection::Print(Size pagesize, Gate2<int, int> progress, int& progress_pos) const
{
	ASSERT(columns > 0);
	Size drawingsize = GetTextSize(pagesize);
	RichSectionPageDraw spdraw(drawingsize, progress, progress_pos);
	PaintInfo paintinfo;
	paintinfo.top = PageY(0, 0);
	paintinfo.bottom = PageY(32767, 0);
	paintinfo.indexentry = Null;
	text.Paint(spdraw, drawingsize, paintinfo);
	Vector<Drawing> in = spdraw.Close();
	Vector<Drawing> out;
	for(int i = 0; i < in.GetCount(); i += columns) {
		int pageno = firstpage + out.GetCount();
		DrawingDraw ddraw(pagesize);
		Rect pagerect = GetPageRect(pagesize);
		for(int c = 0; c < columns && c + i < in.GetCount(); c++)
			ddraw.DrawDrawing(GetTextRect(pagesize, c), in[i + c]);
		for(int hf = 0; hf < 3; hf++) {
			if(!IsNull(header[hf])) {
				String s = ExpandHFText(header[hf], pageno);
				Size sz = UPP::GetTextSize(s, charformat);
				int x;
				switch(hf) {
				case 0: x = pagerect.left; break;
				case 1: x = (pagerect.left + pagerect.right - sz.cx) >> 1; break;
				case 2: x = pagerect.right - sz.cx; break;
				}
				ddraw.DrawText(x, pagerect.top, s, charformat);
			}
			if(!IsNull(footer[hf])) {
				String s = ExpandHFText(footer[hf], pageno);
				Size sz = UPP::GetTextSize(s, charformat);
				int x;
				switch(hf) {
				case 0: x = pagerect.left; break;
				case 1: x = (pagerect.left + pagerect.right - sz.cx) >> 1; break;
				case 2: x = pagerect.right - sz.cx; break;
				}
				ddraw.DrawText(x, pagerect.bottom - sz.cy, s, charformat);
			}
		}
		out.Add() = ddraw;
	}
	return out;
}

String GetLangSuffix(int language = GetCurrentLanguage())
{
	String lt = LNGAsText(language);
	String out;
	out << '$' << (char)ToLower(lt[0]) << (char)ToLower(lt[1]) << '-' << (char)ToLower(lt[3]) << (char)ToLower(lt[4]);
	return out;
}

void MakeStdToc(RichText& out, const RichToc& toc, int print_width)
{
	VectorMap< String, ArrayMap<String, RichPara> > subparts;
	String suffix = GetLangSuffix();
	for(int i = 0; i < toc.entries.GetCount(); i++) {
		const RichTocEntry& e = toc.entries[i];
		RichPara::Format pfmt = out.GetStyle(out.GetStyleId("Toc" + FormatInt(e.level))).format;
		RichPara para;
		para.format = pfmt;
		WString name = e.text;
		String tname = e.topic, subpart;
		int th = tname.Find('#');
		if(th >= 0) {
			subpart = tname.Mid(th);
			tname.Trim(th);
		}
		tname.Cat(suffix);
		Topic tp = GetTopic(tname);
		if(!IsNull(name))
			para.Cat(name, pfmt);
		else {
			if(!IsNull(subpart)) {
				int sp = subparts.Find(tname);
				if(sp < 0) {
					sp = subparts.GetCount();
					ArrayMap<String, RichPara>& spmap = subparts.Add(tname);
					RichText rt = ParseQTF(tp.text);
					for(int p = 0; p < rt.GetPartCount(); p++)
						if(rt.IsPara(p)) {
							RichPara rp = rt.Get(p);
//							if(*rp.format.label == '#')
							spmap.AddPick("#" + rp.format.label, rp);
						}
				}
				para <<= subparts[sp].Get(subpart, RichPara());
				Array<RichObject> objs;
				String packed = para.Pack(para.format, objs);
				para.Unpack(packed, objs, pfmt);
			}
			else if(!IsNull(tp.title))
				para.Cat(FromUtf8(tp.title), pfmt);
		}
		if(para.part.IsEmpty()) {
			pfmt.ink = LtRed();
			pfmt.paper = WhiteGray();
			para.Cat((WString)(tname + subpart), pfmt);
		}
		para.Cat("\t", pfmt);
		para.Cat(FieldTypeVarID(), String().Cat() << tname << subpart << "$page", pfmt);
		out.Cat(para);
	}
	RLOG("MakeStdToc:\n" << AsQTF(out));
}

void MakeOnlineToc(RichText& out, const RichToc& toc)
{
	String suffix = GetLangSuffix();
	VectorMap< String, ArrayMap<String, RichPara> > subparts;
	for(int i = 0; i < toc.entries.GetCount(); i++) {
		const RichTocEntry& e = toc.entries[i];
		RichPara::Format pfmt = out.GetStyle(out.GetStyleId("Topics" + FormatInt(e.level))).format;
		RichPara para;
		para.format = pfmt;
		WString name = e.text;
		String tname = e.topic, subpart;
		int th = tname.Find('#');
		if(th >= 0) {
			subpart = tname.Mid(th);
			tname.Trim(th);
		}
		tname.Cat(suffix);
		Topic tp = GetTopic(tname);
		pfmt.link = tname;
		if(!IsNull(name))
			para.Cat(name, pfmt);
		else {
			if(!IsNull(subpart)) {
				int sp = subparts.Find(tname);
				if(sp < 0) {
					sp = subparts.GetCount();
					ArrayMap<String, RichPara>& spmap = subparts.Add(tname);
					RichText rt = ParseQTF(tp.text);
					for(int p = 0; p < rt.GetPartCount(); p++)
						if(rt.IsPara(p)) {
							RichPara rp = rt.Get(p);
//							if(*rp.format.label == '#')
							spmap.AddPick("#" + rp.format.label, rp);
						}
				}
				para <<= subparts[sp].Get(subpart, RichPara());
				Array<RichObject> objs;
				String packed = para.Pack(para.format, objs);
				para.Unpack(packed, objs, pfmt);
			}
			else if(!IsNull(tp.title))
				para.Cat(FromUtf8(tp.title), pfmt);
		}
		if(para.part.IsEmpty()) {
			pfmt.ink = LtRed();
			pfmt.paper = WhiteGray();
			para.Cat((WString)(tname + subpart), pfmt);
		}
		out.Cat(para);
	}
	RLOG("MakeOnlineToc:\n" << AsQTF(out));
}

void MakeTocTopics(RichText& out, const RichToc& toc)
{
	String suffix = GetLangSuffix();
	VectorMap< String, VectorMap<String, int> > topic_tocrefs;
	for(int i = 0; i < toc.entries.GetCount(); i++) {
		String tname = toc.entries[i].topic;
		int th = tname.Find('#');
		if(th >= 0)
			topic_tocrefs.GetAdd(tname.Left(th)).GetAdd(tname.Mid(th), i);
	}
	Index<String> done;
	for(int i = 0; i < toc.entries.GetCount(); i++) {
		const RichTocEntry& e = toc.entries[i];
		WString name = e.text;
		String tname = e.topic;
		int th = tname.Find('#');
		if(th >= 0)
			tname.Trim(th);
		int tr = topic_tocrefs.Find(tname);
		tname << suffix;
		if(done.Find(tname) >= 0)
			continue;
		done.Add(tname);
		Topic tp = GetTopic(tname);
		RichPara title_para;
		title_para.format = out.GetStyle(out.GetStyleId("Heading" + FormatInt(e.level))).format;
		if(IsNull(name) && IsNull(name = FromUtf8(tp.title))) {
			name = (WString)tname;
			title_para.format.ink = LtRed();
			title_para.format.paper = WhiteGray();
		}
		title_para.format.label = tname;
		title_para.Cat(name, title_para.format);
		out.Cat(title_para);
		RichText data = ParseQTF(tp.text);
		int pcount = out.GetPartCount();
		out.CatPick(data);
		for(int t = pcount; t < out.GetPartCount(); t++)
			if(out.IsPara(t)) {
				RichPara p = out.Get(t);
				String lbl = "#" + p.format.label;
				int tx;
				if(tr >= 0 && (tx = topic_tocrefs[tr].Find(lbl)) >= 0) {
					const RichTocEntry& ex = toc.entries[topic_tocrefs[tr][tx]];
					Array<RichObject> objs;
					String pack = p.Pack(p.format, objs);
					p.format = out.GetStyle(out.GetStyleId("Heading" + FormatInt(ex.level))).format;
					p.Unpack(pack, objs, p.format);
					p.format.label = tname + lbl;
					out.Set(t, p, out.GetStyles());
				}
			}
	}
}

//RichText MakeStdIndex(

RichBook::RichBook(Size pg)
: page_size(pg)
{
	RichStyle toc1, toc2, toc3, toc4;
	RichPara::Tab& t1tab = toc1.format.tab.Add();
	t1tab.pos = pg.cx - 300;
	t1tab.align = ALIGN_RIGHT;
	t1tab.fillchar = 1;
	toc2 = toc3 = toc4 = toc1;
	t1tab.fillchar = 0;

	toc1.name = "Toc1";
	(Font&)toc1.format = Arial(90).Bold();
	toc1.format.indent = 100;
	toc1.format.before = 100;
	toc1.format.number[0] = RichPara::NUMBER_1;

	toc2.name = "Toc2";
	(Font&)toc2.format = Arial(84).Bold();
	toc2.format.indent = 200;
	toc2.format.lm = 200;
	toc2.format.number[0] = toc2.format.number[1] = RichPara::NUMBER_1;

	toc3.name = "Toc3";
	(Font&)toc3.format = Arial(84);
	toc3.format.indent = 300;
	toc3.format.lm = 300;
	toc3.format.number[0] = toc3.format.number[1] = toc3.format.number[2] = RichPara::NUMBER_1;

	toc4.name = "Toc4";
	toc4.format.number[0] = toc4.format.number[1] = toc4.format.number[2] = toc4.format.number[3] = RichPara::NUMBER_1;

	RichStyle head1, head2, head3, head4;
	head1.name = "Heading1";
	(Font&)head1.format = Arial(144).Bold();
	head1.format.newpage = true;
	head1.format.before = 300;
	head1.format.after = 100;
	head1.format.indent = 150;
	head1.format.number[0] = RichPara::NUMBER_1;

	head2.name = "Heading2";
	(Font&)head2.format = Arial(120).Bold();
	head2.format.before = 200;
	head2.format.after = 100;
	head2.format.indent = 200;
	head2.format.number[0] = head2.format.number[1] = RichPara::NUMBER_1;

	head3.name = "Heading3";
	(Font&)head3.format = Arial(100).Bold();
	head3.format.before = 100;
	head3.format.after = 50;
	head3.format.indent = 250;
	head3.format.number[0] = head3.format.number[1] = head3.format.number[2] = RichPara::NUMBER_1;

	head4 = head3;
	head4.name = "Heading4";
	head4.format.number[0] = head4.format.number[1] = head4.format.number[2] = head4.format.number[3] = RichPara::NUMBER_1;

	default_styles.Add(Uuid::Create(), toc1);
	default_styles.Add(Uuid::Create(), toc2);
	default_styles.Add(Uuid::Create(), toc3);
	default_styles.Add(Uuid::Create(), toc4);

	default_styles.Add(Uuid::Create(), head1);
	default_styles.Add(Uuid::Create(), head2);
	default_styles.Add(Uuid::Create(), head3);
	default_styles.Add(Uuid::Create(), head4);
}

RichPrintSection& RichBook::AddSection()
{
	RichText init;
	init.OverrideStyles(default_styles);
	return sections.Add(new RichPrintSection(default_section, init));
}

Vector<Drawing> RichBook::Print(Size pagesize, Gate2<int, int> progress)
{
	Vector<Drawing> out;
	int pos = 0;

	VectorMap<String, Value> vars;
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
			RichPrintSection& psec = sections[s];
			if(psec.text.GetPartCount() > 0 && psec.text.IsPara(0)) {
				RichPara p0 = psec.text.Get(0);
				if(p0.format.newpage) {
					p0.format.newpage = false;
					psec.text.Set(0, p0, psec.text.GetStyles());
				}
			}
			Size textsize = psec.GetTextSize(pagesize);
			PageY begin;
			pageno = Nvl(psec.firstpage, pageno);
			psec.text.EvaluateFields(vars);
			int numpages = psec.text.GetHeight(textsize).page / psec.columns + 1;
			Vector<RichValPos> posinfo = psec.text.GetValPos(textsize, RichText::LABELS);
			for(int i = 0; i < posinfo.GetCount(); i++) {
				String key = posinfo[i].data.ToString() + "$page";
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
	for(int i = 0; i < sections.GetCount(); i++)
		out.AppendPick(sections[i].Print(pagesize, progress, pos));
#ifdef _DEBUG
	RichText symtext;
	Vector<int> order = GetSortOrder(vars.GetKeys());
	for(int o = 0; o < order.GetCount(); o++) {
		RichPara para;
		para.Cat(NFormat("<%s> %vt", vars.GetKey(order[o]), vars[order[o]]), para.format);
		symtext.Cat(para);
	}
	RichPrintSection symsec(default_section, symtext);
	out.AppendPick(symsec.Print(pagesize, progress, pos));
#endif
	return out;
}

void CreateHelpBook(RichBook& book)
{
	Progress progress(t_("Typesetting page %d..."));
	DocReport report;
	Vector<Drawing> pages = book.Print(report.GetPageSize(), progress);
	progress.SetText(t_("Printing page %d..."));
	for(int i = 0; i < pages.GetCount(); i++) {
		if(progress.SetCanceled(i, pages.GetCount()))
			throw AbortExc();
		if(i)
			report.NextPage();
		report.DrawDrawing(pages[i].GetSize(), pages[i]);
	}
	report.Perform();
}

String CreateHelpPDF(RichBook& book)
{
	PdfDraw pdf(210 * 6000 / 254, 297 * 6000 / 254);
	Rect margin = Rect(pdf.GetPagePixels()).Deflated(236);
	Progress progress(t_("Typesetting..."));
	Vector<Drawing> pages = book.Print(margin.Size(), progress);
	progress.SetText(t_("Exporting PDF..."));
	progress.SetTotal(pages.GetCount());
	for(int i = 0; i < pages.GetCount(); i++) {
		if(progress.StepCanceled())
			throw AbortExc();
		pdf.StartPage();
		pdf.DrawDrawing(margin, pages[i]);
		pdf.EndPage();
	}
	return pdf.Finish();
}

String CreateHelpRTF(RichBook& book, byte charset)
{
	RichText richtext;
	for(int i = 0; i < book.sections.GetCount(); i++)
		richtext.CatPick(book.sections[i].text);
	return EncodeRTF(richtext, charset);
}

RichBookHtml::RichBookHtml()
{
}

void RichBookHtml::AddFile(String url, String data)
{
}

void RichBookHtml::AddImage(String url, String data)
{
}

String RichBookHtml::GetURLFile(String url) const
{
	String out;
	return out;
}

void CreateHelpHtml(RichBook& book, RichBookHtml& html)
{
}

GLOBAL_VAR(Callback1<RichBook&>, DefaultBook)

void CreateDefaultBook()
{
	RichBook book;
	DefaultBook()(book);
	CreateHelpBook(book);
}

void CreateDefaultPDF()
{
	RichBook book;
	DefaultBook()(book);
	try {
		String data = CreateHelpPDF(book);
		FileSelector fsel;
		fsel.Type(t_("PDF documents (*.pdf)"), "*.pdf")
			.DefaultExt("pdf")
			.AllFilesType();
		static String recent;
		fsel <<= recent;
		if(fsel.ExecuteSaveAs(t_("PDF export"))) {
			recent = ~fsel;
			if(!SaveFile(recent, data))
				throw Exc(NFormat("Error saving file '%s' (%d B).", recent, data.GetLength()));
		}
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void CreateDefaultRTF()
{
	RichBook book;
	DefaultBook()(book);
	try {
		String data = CreateHelpRTF(book, GetDefaultCharset());
		FileSelector fsel;
		fsel.Type(t_("Rich text documents (*.rtf)"), "*.rtf")
			.DefaultExt("rtf")
			.AllFilesType();
		static String recent;
		fsel <<= recent;
		if(fsel.ExecuteSaveAs(t_("Save manual as"))) {
			recent = ~fsel;
			if(!SaveFile(recent, data))
				throw Exc(NFormat("Error saving file '%s' (%d B).", recent, data.GetLength()));
		}
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void CreateDefaultHTML()
{
	RichBook book;
	DefaultBook()(book);
	try {
		RichBookHtml html;
		CreateHelpHtml(book, html);
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void ManualMenu()
{
	MenuBar menu;
	bool isdb = DefaultBook();
	menu.Add(isdb, t_("Print manual"), callback(&CreateDefaultBook));
	menu.Add(isdb, t_("PDF export"), callback(&CreateDefaultPDF));
	menu.Add(isdb, t_("RTF export"), callback(&CreateDefaultRTF));
	menu.Add(isdb, t_("HTML export"), callback(&CreateDefaultHTML));
	menu.Execute();
}

void HelpBookMenu(Bar& bar)
{
	bar.Add(t_("Save manual"), CtrlImg::save(), callback(&ManualMenu));
}

END_UPP_NAMESPACE
