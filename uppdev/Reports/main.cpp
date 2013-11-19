#include <Report/Report.h>

using namespace Upp;

void Background(Report *r)
{
	LOG("Background");
	DDUMP(r->GetPages().GetCount());
	Draw& w = *r;
	Size sz = r->GetSize();
	Image m = CtrlImg::exclamation();
	Size isz = m.GetSize();
	for(int x = -isz.cx / 2; x < sz.cx; x += isz.cx)
		for(int y = -isz.cy / 2; y < sz.cy; y += isz.cy)
			w.DrawImage(x, y, m);
}

GUI_APP_MAIN
{
	Report r;
	r.OnPage(callback1(Background, &r)).Header("THIS IS HEADER!");
	r << LoadFile(GetHomeDirFile("smlouva.qtf"));
	
	Perform(r);
}
