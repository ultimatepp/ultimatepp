#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

void DoPaint(Draw& w, int hg, int x1, int y1, int x2, int y2)
{
	int cy = 0;
	w.DrawImage(x2, 20, 100, 100, CtrlImg::exclamation());
	Font fnt = Serif(hg);
	for(int i = 0; i < 3600; i += 450) {
		String txt = "Text with emoji: 訓음 😜🤪 " + AsString(i);
		w.DrawText(x1, y1, i, txt, fnt, Black);
		w.DrawText(x2, y2, i, txt, fnt().Italic(), Black);
	}
}

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		DoPaint(w, 24, sz.cx / 4, sz.cy / 2, sz.cx - sz.cx / 4, sz.cy / 2);
	}
};

GUI_APP_MAIN
{
	PdfDraw pdf;
	DoPaint(pdf, 100, 2000, 2000, 2200, 5000);
	String p = GetHomeDirFile("pdf.pdf");
	String s = pdf.Finish();
	SaveFile(p, s);
	LaunchWebBrowser(p);
	MyApp().Run();
}
