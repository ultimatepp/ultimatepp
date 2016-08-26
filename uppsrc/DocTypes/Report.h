namespace Upp {

#define LAYOUTFILE <DocTypes/Report.lay>
#include <CtrlCore/lay.h>

class DocReport : public DrawingDraw, public DocOut {
public:
	virtual void    Put(const Paragraph& p);
	virtual void    Put(const Table& p);
	virtual void    Page();

	struct RValueRect : public ValueRect {
		int toppage;
		int bottompage;
	};

protected:
	String          name;
	Array<RValueRect> vr;
	Array<Drawing>  page;
	Size            pgsz;
	Size            size;
	Drawing         header;
	Drawing         footer;
	String          qtfheader;
	String          qtffooter;
	int             ylim, lm;
	int             pgn, pgnx, pgny, pageno;
	const char     *pgnformat;
	Font            pgnfont;
	QtfParser       parser;
	RtfDocOut       rtf;

	bool            dortf;
	bool            valrects;

	int             ypos;

	struct TableSet;

	void          StartPage();
	void          EndPage();
	String        HFormat(const char *s);
	bool          Print0(int page, const char *name);
	void          AddVR(const Vector<ValueRect>& v);

	friend class ReportDlg;

public:
	Callback      WhenNewPage;

	void          NextPage();

	DocReport&       SetPageSize(int cx, int cy);
	DocReport&       SetPageSize(Size sz);
	DocReport&       SetPageno(int x, int y, const char *format = "%d",
		                    Font font = Roman(-100), int fp = 1);
	DocReport&       SetPageNo(int pageno);
	DocReport&       SetHeader(const Drawing& h);
	DocReport&       SetFooter(const Drawing& f);

	DocReport&       Landscape()                         {	return SetPageSize(pgsz.cy, pgsz.cx); }


	int           GetPageNo() const                   { return pageno; }

	DocReport&       SetHeader(const char *qtf);
	DocReport&       SetFooter(const char *qtf);

	Size          GetPageSize() const                 { return pgsz; }
	Size          GetSize() const                     { return size; }
	String        GetHeader() const                   { return qtfheader; }
	String        GetFooter() const                   { return qtffooter; }

	int           GetYPos() const                     { return ypos; }
	void          SetYPos(int y)                      { ypos = y; }
	void          AddYPos(int dy)                     { ypos += dy; }
	int           GetYLim() const                     { return ylim; }
	int           GetYRest() const                    { return GetYLim() - ypos; }
	void          SetLm(int cx)                       { lm = cx; }

	void          SkipBefore(int cy);
	void          SkipAfter(int cy);

	int           GetCount() const                    { return page.GetCount(); }
	int           GetPageCount() const                { return page.GetCount() + 1; }
	Drawing       GetPage(int i) const                { return page[i]; }
	const Array<Drawing>& GetPages() const            { return page; }
	void          SetCount(int n);

	void          Clear();

	void          KillLastPage();

	void          Finish()                            { EndPage(); }

	bool          Print(int page = 0, const char *name = NULL);
	bool          Perform(const char *caption = NULL);

	const ValueRect& GetValueRect(int i) const        { return vr[i]; }
	int              GetValueRectCount() const        { return vr.GetCount(); }

	String           GetRtf()                         { return rtf.Get(); }

	DocReport&       Rtf(bool b = true)               { dortf = b; return *this; }
	DocReport&       NoRtf()                          { return Rtf(false); }
	DocReport&       RtfImageScale(double d)          { rtf.ImageScale(d); return *this; }
	DocReport&       ValueRects(bool b = true)        { valrects = true; return *this; }

	DocReport(const char *name = t_("DocReport"));
	virtual ~DocReport();
};

String Pdf(const DocReport& report, bool pdfa = false);

class ReportCtrl : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual bool Key(dword key, int);
	virtual void LeftDown(Point p, dword);
	virtual void MouseWheel(Point, int zdelta, dword);

protected:
	ScrollBar sb;
	DocReport   *report;
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

public:
	Callback  WhenGoPage;

	enum Pages {
		PG1, PG2, PG4, PG16
	};
	ReportCtrl& Pages(int pags)       { pages = pags; Layout(); return *this; }
	ReportCtrl& Numbers(bool nums)    { numbers = nums; Refresh(); return *this; }

	void      Set(DocReport& p);
	int       GetFirst() const        { return sb / max(pagesize.cy * pvn, 1); }

	void      ScrollInto(int toppage, int top, int bottompage, int bottom);
	void      ScrollInto(const DocReport::RValueRect& vr);

	typedef ReportCtrl CLASSNAME;

	ReportCtrl();
};

class ReportDlg : public WithReportLayout<TopWindow> {
	void Pages()   { pg.Pages(~sw); }
	void Numbers() { pg.Numbers(numbers); }
	void GoPage()  { sw <<= ReportCtrl::PG1; Pages(); }
	void Pdf();

	Array<Button>  button;
	DocReport        *report;

public:
	ReportCtrl pg;

	typedef ReportDlg CLASSNAME;

	static void SetPdfRoutine(String (*pdf)(const DocReport& report, int margin));

	void SetButton(int i, const char *label, int id);

	int  Perform(DocReport& report, int zoom = 100, const char *caption = t_("DocReport"));

	ReportDlg();
};

void PutQTF(DocReport& r, const char *qtf);

}
