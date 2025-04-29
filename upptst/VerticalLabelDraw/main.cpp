#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
	
		w.DrawRect(GetSize(), LtGray());

		int x = 0, y = 0;
		DrawLabel dl;
		dl.text = "Some text\nanother line";

		Color col1 = White();
		Color col2 = WhiteGray();
		auto Test = [&](int align) {
			w.DrawRect(x, y, 200, 200, col1);
			Swap(col1, col2);
			dl.align = align;
			dl.Paint(w, x, y, 200, 200);
			x += 200;
		};
		
		auto Test3 = [&](int valign) {
			dl.valign = valign;
			Test(ALIGN_LEFT);
			Test(ALIGN_CENTER);
			Test(ALIGN_RIGHT);
		};
		
		auto Test9 = [&](int orientation) {
			dl.orientation = orientation;
			Test3(ALIGN_TOP);
			Test3(ALIGN_CENTER);
			Test3(ALIGN_BOTTOM);
			x = 0;
			y += 200;
		};
	
		for(int pass = 0; pass < 2; pass++) {
			Test9(ORIENTATION_NORMAL);
			Test9(ORIENTATION_CLOCKWISE);
			Test9(ORIENTATION_ANTICLOCKWISE);
			dl.limg = CtrlImg::Computer();
			dl.rimg = CtrlImg::Dir();
			dl.text = "\1[*/ QTF text!]&another line";
		}
	};
};

GUI_APP_MAIN
{
	MyApp().Run();
}
