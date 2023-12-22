#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterImage/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterImage2/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;

	DropList method;
	DropList image;
	Option   border;
	Option   over;
	
	int rotate = 20;

	void LeftDown(Point p, dword keyflags) override
	{
		rotate = p.y;
		Refresh();
	}

	void Paint(Draw& w) override
	{
		Image img = TestImg::test();
		ImagePainter p(32, 128);
		p.Clear(White());
		w.DrawRect(GetSize(), WhiteGray());
		
		int y = 1;
		
		for(double sz : { 4, 8, 12, 2, 3, 5, 6, 8, 9 }) {
			p.Rectangle(1, y, sz, sz)
			 .ImageFilter(~method)
			 .Fill(img, Xform2D::Scale((double)sz / 4) * Xform2D::Translation(1, y), FILL_PAD);

			w.DrawImage(32 * 16, 16 * y, Magnify(RescaleFilter(TestImg::test(), sz, sz, ~method), 16, 16));

			y += sz + 1;
		}
		
//		 .Fill(img, 1, 1, 1, 4);
		Image m = p;
		
		w.DrawImage(0, 0, Magnify(m, 16, 16));
		
/*
		Size sz = GetSize();
		Image img = ~image == 0 ? TestImg::test2() : TestImg::test();
		Size isz = img.GetSize();
		{
			DrawPainter p(w, sz);
			p.Clear(White());
//			p.Co();
			int q = 8 * !!border;
			p.Rectangle(20 - q, 20 - q, isz.cx + 2 * q, isz.cy + 2 * q)
			 .ImageFilter(~method)
//			 .Fill(LtRed());
			 .Fill(img, 20, 20, isz.cx + 20, 20);

			DLOG("==============");
			p.Rectangle(20 - q, isz.cy + 40 - q, 2 * isz.cx + 2 * q, 2 * isz.cy + 2 * q)
			 .ImageFilter(~method)
			// .Fill(LtRed())
			 .Fill(img, 20, isz.cy + 40, 2 * isz.cx + 20, isz.cy + 40, FILL_HPAD|FILL_VPAD);
		}
		w.DrawImage(over ? 100 : 2 * isz.cx + 60, isz.cy + 40, RescaleFilter(img, 2 * isz, ~method));
*/
	}
	
	void Sync()
	{
		Refresh();
	}

	MyApp() {
		Sizeable().Zoomable();
	//	SetRect(0, 0, 40 + 4 * 180, 40 + 4 * 180);
		method.Add(FILTER_NEAREST, "Nearest");
		method.Add(FILTER_BILINEAR, "Bilinear");
		method.Add(FILTER_BSPLINE, "Bspline");
		method.Add(FILTER_COSTELLA, "Costello");
		method.Add(FILTER_BICUBIC_MITCHELL, "Bicubic Mitchell");
		method.Add(FILTER_BICUBIC_CATMULLROM, "Bicubic Catmull Rom");
		method.Add(FILTER_LANCZOS2, "Lanczos 2");
		method.Add(FILTER_LANCZOS3, "Lanczos 3");
		method.Add(FILTER_LANCZOS4, "Lanczos 4");
		method.Add(FILTER_LANCZOS5, "Lanczos 5");
		method <<= THISBACK(Sync);
		Add(method.BottomPos(0, STDSIZE).RightPos(0, 200));
		method <<= FILTER_NEAREST;
		border.SetLabel("Extend border");
		Add(border.BottomPos(0, STDSIZE).LeftPosZ(0, 200));
		Add(image.BottomPos(0, STDSIZE).LeftPosZ(200, 200));
		image.Add(0, "Box");
		image.Add(1, "Bee");
		image <<= 0;
		image << [=] { Refresh(); };
		border << [=] { Refresh(); };
		over.SetLabel("Over border");
		Add(over.BottomPos(0, STDSIZE).LeftPosZ(420, 200));
		over << [=] { Refresh(); };
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
