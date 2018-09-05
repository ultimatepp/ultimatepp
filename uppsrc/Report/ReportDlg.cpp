#include "Report.h"

namespace Upp {

#define LLOG(x) // LOG(x)

#define  TFILE <Report/Report.t>
#include <Core/t.h>

void Print(Report& r, PrinterJob& pd)
{
	Draw& w = pd;
	Size sz = w.GetPageSize();
	Point mg = r.GetMargins();
	Size pgsz = r.GetPage(0).GetSize();
	int	x = Nvl(mg.x, (sz.cx - pgsz.cx) / 2);
	int y = Nvl(mg.y, (sz.cy - pgsz.cy) / 2);
	for(int i = 0; i < pd.GetPageCount(); i++) {
		Drawing iw = r.GetPage(pd[i]);
		Size sz = iw.GetSize();
		w.StartPage();
		w.DrawDrawing(x, y, sz.cx, sz.cy, iw);
		w.EndPage();
	}
}

bool Print0(Report& r, int i, const char *_name, bool dodlg) {
	if(r.GetPrinterJob()) {
		Print(r, *r.GetPrinterJob());
		return true;
	}
	PrinterJob pd(_name);
	pd.CurrentPage(i);
	pd.MinMaxPage(0, r.GetCount() - 1);
	Size pgsz = r.GetPage(0).GetSize();
	pd.Landscape(pgsz.cx > pgsz.cy);
	if(dodlg && !pd.Execute())
		return false;
	Print(r, pd);
	return true;
}

bool Print(Report& r, int i, const char *_name)
{
	return Print0(r, i, _name, true);
}

bool DefaultPrint(Report& r, int i, const char *_name)
{
	return Print0(r, i, _name, false);
}

void ReportView::MouseWheel(Point, int zdelta, dword) {
	sb.Wheel(zdelta);
}

ReportView::ReportView() {
	report = NULL;
	vsize = 0;
	pagesize.cx = pagesize.cy = 0;
	sb.WhenScroll = callback(this, &ReportView::Sb);
	sb.SetLine(16);
	AddFrame(sb);
	SetFrame(FieldFrame());
	numbers = false;
	pages = PG1;
}

void ReportView::Set(Report& p) {
	Init();
	report = &p;
	Refresh();
	Layout();
}

Size ReportView::GetReportSize()
{
	return report->GetPage(0).GetSize();
}

bool ReportView::Key(dword key, int) {
	return sb.VertKey(key);
}

void ReportView::LeftDown(Point p, dword) {
	if(!report || pagesize.cy == 0 || pagesize.cx == 0) return;
	int pg = (sb + p.y) / pagesize.cy * pvn + min(pvn - 1, p.x / pagesize.cx);
	WhenGoPage();
	Layout();
	sb = pg * pagesize.cy;
}

void ReportView::Sb() {
	Refresh();
	WhenAction();
}

void ReportView::Init() {
	for(int i = 0; i < 64; i++) {
		page[i].Clear();
		pagei[i] = -1;
	}
}

void ReportView::Layout() {
	Size sz = GetSize();
	if(report && report->GetCount()) {
		ASSERT(pages >= PG1 && pages <= PG16);
		pvn = 1 << pages;
		static int h[] = { 1, 3, 15, 63 };
		pm = h[pages];
		Size sr = report->GetPage(0).GetSize();
		pagesize.cx = sz.cx / pvn;
		pagesize.cy = sr.cy * pagesize.cx / sr.cx;
		vsize = (report->GetCount() + pvn - 1) / pvn * pagesize.cy;
		sb.SetPage(sz.cy);
		sb.SetTotal(vsize);
		Init();
		Refresh();
	}
	else {
		vsize = 0;
		sb.Set(0, 0, 0);
		return;
	}
}

Image ReportView::GetPage(int i) {
	ASSERT(report);
	int ii = i & pm;
	if(pagei[ii] != i) {
		pagei[ii] = i;
		Size sz = Size(max(pagesize.cx - 2, 1), max(pagesize.cy - 2, 1));
		if(HasPainter()) {
			ImageBuffer ib(sz);
			Fill(~ib, White(), ib.GetLength());
			PaintImageBuffer(ib, report->GetPage(i));
			page[ii] = ib;
		}
		else {
			ImageDraw iw(sz);
			iw.DrawRect(sz, White);
			iw.DrawDrawing(0, 0, sz.cx, sz.cy, report->GetPage(i));
			page[ii] = iw;
		}
	}
	return page[ii];
}

void ReportView::ScrollInto(int toppage, int top, int bottompage, int bottom) {
	sb.ScrollInto(toppage * pagesize.cy + top * pagesize.cy / GetReportSize().cy,
	              (bottompage - toppage) * pagesize.cy +
	                (bottom - top) * pagesize.cy / GetReportSize().cy);
}

void ReportView::Paint(Draw& w) {
	Size sz = GetSize();
	if(sz.cx <= 0 || sz.cy <= 0) return;
	if(!vsize || !report) {
		w.DrawRect(0, 0, sz.cx, sz.cy, SGray);
		return;
	}
	int i = sb / pagesize.cy;
	int y = i * pagesize.cy - sb;
	i *= pvn;
	while(y < sz.cy) {
		int x = 0;
		for(int j = pvn; j--;) {
			if(i < report->GetCount()) {
				w.DrawImage(x + 1, y + 1, GetPage(i));
				if(j == 0) {
					w.DrawRect(x + pagesize.cx - 1, y, sz.cx - x - pagesize.cx, pagesize.cy, White);
					DrawFrame(w, x, y, sz.cx - x, pagesize.cy, White, LtGray);
				}
				else
					DrawFrame(w, x, y, pagesize.cx, pagesize.cy, White, LtGray);
				if(numbers) {
					String n = Format("%d", i + 1);
					Size tsz = GetTextSize(n, StdFont());
					tsz += Size(8, 4);
					int tx = x + pagesize.cx - tsz.cx;
					DrawFrame(w, tx, y, tsz.cx, tsz.cy, Black, Black);
					w.DrawRect(tx + 1, y + 1, tsz.cx - 2, tsz.cy - 2, Yellow);
					w.DrawText(tx + 4, y + 2, n);
				}
			}
			else {
				w.DrawRect(x, y, sz.cx - x, pagesize.cy, Gray);
				break;
			}
			x += pagesize.cx;
			i++;
		}
		y += pagesize.cy;
	}
}

ReportWindow::ReportWindow()
{
	CtrlLayoutCancel(*this, "");
	sw <<= ReportView::PG1;
	sw <<= THISBACK(Pages);
	numbers <<= THISBACK(Numbers);
	pg.WhenGoPage = THISBACK(GoPage);
	lbl.SetFont(ArialZ(20).Italic());
	lbl.SetAlign(ALIGN_CENTER);
	pg.WhenAction = THISBACK(ShowPage);
	Sizeable();
	MaximizeBox();
	Icon(CtrlImg::smallreporticon());
	SetButton(0, t_("&Print"), 999995);
	pdf.Show(GetDrawingToPdfFn());
	pdf <<= THISBACK(Pdf);
}

String Pdf(Report& report, bool pdfa, const PdfSignatureInfo *sgn)
{
	return GetDrawingToPdfFn() && report.GetCount() ?
	      (*GetDrawingToPdfFn())(report.GetPages(), report.GetPage(0).GetSize(),
	                             Nvl(report.GetMargins().x, 200), pdfa, sgn)
	      : String();
}

void ReportWindow::Pdf()
{
	static FileSel fs;
	static bool b;
	if(!b) {
		fs.Type(t_("PDF file"), "*.pdf");
		fs.AllFilesType();
	}
	if(!fs.ExecuteSaveAs(t_("Output PDF file")))
		return;
	SaveFile(~fs, UPP::Pdf(*report));
}

void ReportWindow::SetButton(int i, const char *label, int id)
{
	LogPos p = cancel.GetPos();
	Button& b = button.At(i);
	b.SetLabel(label);
	b.RightPos(p.x.GetA(), p.x.GetB()).BottomPos(p.y.GetA() + (p.y.GetB() + 6) * (i + 1), p.y.GetB());
	b <<= Breaker(id);
	for(int i = 0; i < button.GetCount(); i++)
		button[i].Remove();
	for(int i = 0; i < button.GetCount(); i++)
		AddChildBefore(&button[i], &cancel);
}

void ReportWindow::ShowPage()
{
	if(pg.Get())
		lbl = Format("%d / %d", pg.GetFirst() + 1, pg.Get()->GetCount());
}

int ReportWindow::Perform(Report& report, int zoom, const char *caption)
{
	this->report = &report;
//	if(report.dortf)
//		WriteClipboard(GetClipboardFormatCode("Rich text format"), report.rtf.Get());
	pg.Set(report);
	Size sz;
	Rect area = Ctrl::GetWorkArea();
	sz = area.Size() * 4 * abs(zoom) / 500;
	SetRect(sz);

	ActiveFocus(pg);
	Title(caption);
	Open();
	for(;;) {
		ShowPage();
		int c = Run();
		switch(c) {
		case IDCANCEL:
			return false;
		case 999995:
			return Print(report, pg.GetFirst(), caption);
		}
		if(c > 0)
			return c;
	}
}

bool Perform(Report& r, const char *caption)
{
	return ReportWindow().Perform(r, 100, caption);
}

bool QtfReport0(const String& qtf, const char *name, bool pagenumbers, Size pagesize)
{
	Report r;
	if(!IsNull(pagesize))
		r.SetPageSize(pagesize);
	if(pagenumbers) {
		Report rr;
		rr.Footer("[1> $$P]");
		rr << qtf;
		r.Footer("[1> $$P/" + AsString(rr.GetCount()) + "]");
	}
	r << qtf;
	return Perform(r, name);
}

bool QtfReport(const String& qtf, const char *name, bool pagenumbers)
{
	return QtfReport0(qtf, name, pagenumbers, Null);
}

bool QtfReport(Size pagesize, const String& qtf, const char *name, bool pagenumbers)
{
	return QtfReport0(qtf, name, pagenumbers, pagesize);
}


}
