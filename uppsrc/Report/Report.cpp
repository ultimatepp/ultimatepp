#include "Report.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)

Report::Report()
{
	mg.x = mg.y = Null;
	Clear();
}

Report::Report(int cx, int cy)
{
	mg.x = mg.y = Null;
	SetPageSize(cx, cy);
}

Report::Report(const Size &sz)
{
	mg.x = mg.y = Null;
	SetPageSize(sz);
}

Report& Report::SetPageSize(Size sz)
{
	page.Clear();
	header = footer = Null;
	headercy = footercy = headerspc = footerspc = 0;
	pagei = -1;
	Create(sz);
	Page(0);
	Create(sz);
	StartPage(0);
	return *this;
}

void Report::Clear()
{
	SetPageSize(3968, 6074);
}

Rect Report::GetPageRect()
{
	Rect r = GetSize();
	r.top += headercy + headerspc;
	r.bottom -= footercy + footerspc;
	return r;
}

void Report::PaintHF(Draw& w, int y, const char *qtf, int i)
{
	RichText txt = ParseQTF(FormatHF(qtf, i));
	PaintInfo pi; // need pi so that darktheme is false
	txt.Paint(w, 0, y, GetSize().cx, pi);
}

void Report::Flush()
{
	if(pagei >= 0) {
		Drawing dw = GetResult();
		page.At(pagei).Append(dw);
		Create(GetSize());
	}
}

void Report::StartPage(int i)
{
	DrawingDraw dw(GetSize());
	page.At(i) = dw;
	LLOG("Start page " << i);
	Create(GetSize());
	WhenPage();
	LLOG("Paint header");
	PaintHF(*this, 0, header, i);
	LLOG("Paint footer");
	PaintHF(*this, GetSize().cy - footercy, footer, i);
	y = GetPageRect().top;
}

Draw& Report::Page(int i)
{
	ASSERT(i >= 0);
	if(i != pagei) {
		Flush();
		pagei = i;
		while(page.GetCount() <= pagei)
			StartPage(page.GetCount());
		y = GetPageRect().top;
	}
	return *this;
}

String  Report::FormatHF(const char *s, int pageno)
{
	String result;
	while(*s) {
		if(s[0] == '$' && s[1] == '$') {
			if(s[2] == 'P') {
				result << pageno + 1;
				s += 3;
			}
			else
			if(s[2] == 'D') {
				result.Cat(Format(GetSysDate()));
				s += 3;
			}
		}
		result.Cat(*s++);
	}
	return result;
}

int Report::GetHeightHF(const char *s)
{
	RichText txt = ParseQTF(FormatHF(s, 9999));
	return txt.GetHeight(GetSize().cx);
}

Report& Report::Header(const char *qtf, int spc)
{
	header = qtf;
	headerspc = spc;
	headercy = qtf ? GetHeightHF(qtf) : 0;
	RestartPage();
	return *this;
}

Report& Report::Footer(const char *qtf, int spc)
{
	footer = qtf;
	footerspc = spc;
	footercy = qtf ? GetHeightHF(qtf) : 0;
	RestartPage();
	return *this;
}

Report& Report::OnPage(Callback whenpage)
{
	WhenPage = whenpage;
	RestartPage();
	return *this;
}

void Report::RestartPage()
{
	page.SetCount(pagei + 1);
	StartPage(pagei);
}

Report& Report::Landscape()
{
	Size sz = GetSize();
	SetPageSize(sz.cy, sz.cx);
	return *this;
}

void Report::Put(const RichText& txt, void *context)
{
	PageY py(pagei, y);
	LLOG("Put RichText, py: " << py << ", pagerect: " << GetPageRect());
	PaintInfo paintinfo;
	paintinfo.top = PageY(0, 0);
	paintinfo.bottom = PageY(INT_MAX, INT_MAX);
	paintinfo.indexentry = Null;
	paintinfo.hyperlink = Null;
	paintinfo.context = context;
	txt.Paint(*this, py, GetPageRect(), paintinfo);
	py = txt.GetHeight(py, GetPageRect());
	LLOG("Final pos: " << py);
	Page(py.page);
	y = py.y;
}

void Report::Put(const char *qtf)
{
	Put(ParseQTF(qtf));
}

bool Report::ChoosePrinter(const char *jobname)
{
	printerjob.Create();
	printerjob->Name(jobname);
	if(!printerjob->Execute()) {
		printerjob.Clear();
		return false;
	}
	SetPageSize(printerjob->GetDraw().GetPageSize());
	return true;
}

bool Report::ChooseDefaultPrinter(const char *jobname)
{
	printerjob.Create();
	printerjob->Name(jobname);
	Size sz = printerjob->GetDraw().GetPageSize();
	if(sz.cx == 0 || sz.cy == 0)
		return false;
	SetPageSize(sz);
	return true;
}

}
