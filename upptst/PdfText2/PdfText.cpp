#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

void DoPaint(Draw& w, int hg, int x1, int x2, int n = INT_MAX)
{
	int cy = 0;
	w.DrawImage(x2, 20, 100, 100, CtrlImg::exclamation());
	for(int face = Font::SERIF; face < min(n, Font::GetFaceCount()); face++)
	{
		Font fnt(face, hg);
		Cout() << fnt << "\n";
		LOG(face << ' ' << fnt << ", TTF: " << fnt.IsTrueType());
		String txt = AsString(fnt) + " 訓民正音 (훈민정음) 😜 🤪 ";
		w.DrawRect(0, cy, GetTextSize(txt, fnt).cx, fnt.GetAscent(), Blend(White(), LtBlue()));
		w.DrawRect(0, cy + fnt.GetAscent(), GetTextSize(txt, fnt).cx, fnt.GetDescent(), Blend(White(), LtRed()));
		w.DrawText(0, cy, txt, fnt, Black);
		w.DrawText(x1, cy, AsString(fnt) << ' ' << face, StdFont(hg), Black);
		cy += fnt.GetLineHeight();
		if(cy > 6000) {
			auto *pdf = dynamic_cast<PdfDraw *>(&w);
			if(pdf) {
				pdf->EndPage();
				pdf->StartPage();
			}
			cy = 0;
		}
	}
}

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		DoPaint(w, 24, sz.cx / 2, sz.cx - 100, 10);
	}
};

GUI_APP_MAIN
{
	PdfDraw pdf;
	DoPaint(pdf, 100, 3000, 5000);
	String p = GetHomeDirFile("pdf.pdf");
	String s = pdf.Finish();
	DDUMP(s.GetCount());
	SaveFile(p, s);
	LaunchWebBrowser(p);
	MyApp().Run();
}
