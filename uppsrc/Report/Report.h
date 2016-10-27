#ifndef _Report_Report_h
#define _Report_Report_h

#include <CtrlLib/CtrlLib.h>
#include <RichText/RichText.h>

namespace Upp {

#define  LAYOUTFILE <Report/Report.lay>
#include <CtrlCore/lay.h>

class Report : public DrawingDraw, public PageDraw {
public:
	virtual Draw& Page(int i);
	virtual Size  GetPageSize() const    { return DrawingDraw::GetPageSize(); } // avoid CLANG warning
	virtual void  StartPage()            { DrawingDraw::StartPage(); } // avoid CLANG warning

private:
	Array<Drawing>      page;
	int                 pagei;
	int                 y;
	String              header, footer;
	int                 headercy, headerspc, footercy, footerspc;
	Point               mg;
	One<PrinterJob>     printerjob;

	void    Flush();
	String  FormatHF(const char *s, int pageno);
	int     GetHeightHF(const char *s);
	void    PaintHF(Draw& w, int y, const char *qtf, int i);
	void    StartPage(int i);
	void    RestartPage();

	Callback              WhenPage;
	
public:

	int                   GetCount()                  { Flush(); return page.GetCount(); }
	Drawing               GetPage(int i)              { Flush(); return page[i]; }
	Drawing               operator[](int i)           { return GetPage(i); }
	const Array<Drawing>& GetPages()                  { Flush(); return page; }

	void                  Clear();

	Rect                  GetPageRect();
	Size                  GetPageSize()               { return GetPageRect().Size(); }

	void                  SetY(int _y)                { y = _y; }
	int                   GetY() const                { return y; }

	void                  NewPage()                   { Page(pagei + 1); }
	void                  RemoveLastPage()            { if(page.GetCount()) page.Drop(); pagei = -1; }

	void                  Put(const RichText& txt, void *context = NULL);
	void                  Put(const char *qtf);
	Report&               operator<<(const char *qtf) { Put(qtf); return *this; }

	void                  SetRichTextLayoutTracer(RichTextLayoutTracer& l) { tracer = &l; }

	Point                 GetMargins() const          { return mg; }

	bool                  ChoosePrinter(const char *jobname = t_("Report"));
	bool                  ChooseDefaultPrinter(const char *jobname = t_("Report"));
	PrinterJob           *GetPrinterJob()             { return ~printerjob; }

	Report&               SetPageSize(Size sz);
	Report&               SetPageSize(int cx, int cy) { return SetPageSize(Size(cx, cy)); }
	Report&               Landscape();
	Report&               Margins(int top, int left)  { mg.y = top; mg.x = left; return *this; }
	Report&               Margins(int m)              { return Margins(m, m); }
	Report&               Header(const char *qtf, int spc = 150);
	Report&               Footer(const char *qtf, int spc = 150);
	Report&               OnPage(Callback whenpage);
	Report&               NoHeader()                  { return Header(NULL, 0); }
	Report&               NoFooter()                  { return Footer(NULL, 0); }

	Report();
	Report(int cx, int cy);
	Report(const Size &sz);
};

class ReportView : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual bool Key(dword key, int);
	virtual void LeftDown(Point p, dword);
	virtual void MouseWheel(Point, int zdelta, dword);

protected:
	ScrollBar sb;
	Report   *report;
	Image     page[64];
	int       pagei[64];
	Size      pagesize;
	int       vsize;

	int       pm;
	int       pvn;
	bool      numbers;
	int       pages;

	Image     GetPage(int i);

	void      Init();
	void      Sb();
	void      Numbers()               { Refresh(); }
	Size      GetReportSize();

public:
	Callback  WhenGoPage;

	enum Pages {
		PG1, PG2, PG4, PG16
	};
	ReportView& Pages(int pags)       { pages = pags; Layout(); return *this; }
	ReportView& Numbers(bool nums)    { numbers = nums; Refresh(); return *this; }

	void      Set(Report& report);
	Report   *Get()                   { return report; }
	int       GetFirst() const        { return sb / pagesize.cy * pvn; }

	void      ScrollInto(int toppage, int top, int bottompage, int bottom);

	typedef ReportView CLASSNAME;

	ReportView();
};

class ReportWindow : public WithReportWindowLayout<TopWindow> {
	void Pages()   { pg.Pages(~sw); }
	void Numbers() { pg.Numbers(numbers); }
	void GoPage()  { sw <<= ReportView::PG1; Pages(); }
	void Pdf();
	void ShowPage();

	Array<Button>         button;
	Report               *report;

public:
	ReportView pg;

	typedef ReportWindow CLASSNAME;

	static void SetPdfRoutine(String (*pdf)(const Report& report, int margin));

	void SetButton(int i, const char *label, int id);

	int  Perform(Report& report, int zoom = 100, const char *caption = t_("Report"));

	ReportWindow();
};

String Pdf(Report& report, bool pdfa = false, const PdfSignatureInfo *sign = NULL);
void   Print(Report& r, PrinterJob& pd);
bool   DefaultPrint(Report& r, int i, const char *_name = t_("Report"));
bool   Print(Report& r, int i, const char *name = t_("Report"));
bool   Perform(Report& r, const char *name = t_("Report"));
bool   QtfReport(const String& qtf, const char *name = "", bool pagenumbers = false);
bool   QtfReport(Size pagesize, const String& qtf, const char *name = "", bool pagenumbers = false);

}

#endif
