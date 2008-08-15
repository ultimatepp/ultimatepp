#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class ImageDrawTest : public TopWindow {
public:
	typedef ImageDrawTest CLASSNAME;
	Image img;
	
	ImageDrawTest() {
		Sizeable();
		GenerateImage();
	}
	
	void GenerateImage() {
		Size sz(100, 100);
		ImageDraw w(sz);
//		w.Alpha().DrawRect(sz, Black());
//		w.Alpha().DrawRect(30, 30, 40, 40, White());
		w.DrawRect(sz, Red);
		img = w;
	}
	
	virtual void Paint(Draw &w) {
		w.DrawRect(GetSize(), SColorFace());
		w.DrawImage(0, 0, img);	
	}
	
	virtual void LeftUp(Point p, dword keyflags) {
		GenerateImage();	
		Refresh();
	}
};

GUI_APP_MAIN
{
	ImageDrawTest().Run();
}
