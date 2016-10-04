#include "CtrlLib/CtrlLib.h"

void DrawFileName(Draw& w, int x, int y, int wcx, int cy, const WString& mname, bool isdir, Font font,
                  Color ink, Color extink, const WString& desc, Font descfont, bool justname);

struct Test : TopWindow {
	void Paint(Draw &w) {
		Size sz = GetSize();
		w.DrawRect(sz, White);
		String x = WString("CENÍK2.DOC").ToString();
		DrawFileName(w, 0, 0, 410, 40, WString(x), false,
		             Arial(17), Black, Black, WString(), Arial(17), false);
	}
};

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);
	Test tst;
/*	FindFile ff("d:/doc/*.*");
	while(ff) {
		DUMP(ff.GetName());
		ff.Next();
	}*/
	tst.Run();
}
