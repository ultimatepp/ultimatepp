#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <AAA/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <AAA/test.iml>
#include <Draw/iml_source.h>
Image Upscale(const Image& simg);
Image Upscale2(const Image& simg);
Image UpscaleA(const Image& simg);
Image UpscaleA2(const Image& simg);

struct Test : TopWindow {
	virtual void Paint(Draw& w);
	
	EditIntSpin passes, amount;
	
	void DoRefresh() { Refresh(); }

	void PutImage(Draw& w, int& y, const Image& m)
	{
		w.DrawImage(20, y, m);
		y += m.GetSize().cy + 10;
	}
	
	typedef Test CLASSNAME;
	
	Test();
};

void Test::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
	Image simg = TestImg::test2();
	int y = 10;
	PutImage(w, y, simg);

	Size isz = simg.GetSize();
	w.DrawImage(10, 150 + 12 * isz.cy, Magnify(simg, 8, 8));
	
	simg = Unmultiply(simg);
	

	Image r = UpscaleA2(simg);
	w.DrawImage(10, 100, r);

	w.DrawImage(10, 150 + 2 * isz.cy, Magnify(r, 4, 4));
	
#if 0
	for(int i = FILTER_NEAREST; i <= FILTER_LANCZOS5; i++) {
		Image img = RescaleFilter(simg, 2 * simg.GetSize(), i);
		w.DrawImage(10, y, img);
		img = Unmultiply(img);
		for(int j = 0; j < (int)~passes; j++)
			img = Sharpen(img, ~amount);
		w.DrawImage(300, y, Premultiply(img));
		w.DrawText(600, y + 10, AsString(i));
		
		y += img.GetSize().cy + 20;
	}
#endif
}

Test::Test()
{
	amount <<= 50;
	passes <<= 2;
	Add(amount.BottomPos(2, 20).LeftPos(2, 50));
	Add(passes.BottomPos(2, 20).LeftPos(60, 50));

//	amount <<= THISBACK(Amount);
//	passes <<= THISBACK(Passes);
	Zoomable().Sizeable();
}

using namespace std;

GUI_APP_MAIN
{
	Test().Run();
}
