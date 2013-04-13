#include "bicubic.h"

Image img;

Image Rescale2(const Image& m, int cx, int cy)
{
	ImagePainter iw(cx, cy);

	iw.RectPath(0, 0, cx, cy);
	double sw = (double)cx / m.GetWidth();
	double sh = (double)cy / m.GetHeight();
	iw.Fill(m, Xform2D::Scale(sw, sh)/* * Xform2D::Translation(x - sw * src.left, y - sh * src.top)*/,
	        FILL_EXACT);

	return iw;
}

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;
	
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtGray());
	//	w.DrawImage(0, 0, Rescale(img, GetSize()));
		w.DrawImage(0, 0, Rescale(img, 480, 84));
		w.DrawImage(0, 100, Rescale2(img, 480, 84));
		w.DrawImage(0, 200, RescaleBicubic2(img, 480, 84));
//		w.DrawImage(0, 300, RescaleBicubic(img, Size(480, 84), Rect(img.GetSize()).Deflated(20)));
		w.DrawImage(0, 300, RescaleBicubic3(img, 480, 84));
		w.DrawImage(0, 400, RescaleBicubic4(img, 480, 84));
//		for(int i = 1; i < 5; i++)
//			w.DrawImage(0, 200 + 100 * i, RescaleBicubic(img, 480 / (i + 3), 84 / (i + 3)));
	//	w.DrawImage(0, 400, RescaleBicubic3(img, 480, 84));
	}

	MyApp() {
		SetRect(0, 0, 600, 800);
		Sizeable();
	}
};

GUI_APP_MAIN
{
	img = StreamRaster::LoadFileAny("c:/xxx/202728.jpg");
	MyApp().Run();
#ifndef _DEBUG
	for(int i = 0; i < 10; i++) {
		{
			RTIMING("Rescale");
			Rescale(img, 480, 84);
		}
		{
			RTIMING("Rescale2");
			Rescale2(img, 480, 84);
		}
		{
			RTIMING("RescaleBicubic");
			RescaleBicubic(img, 480, 84);
		}
		{
			RTIMING("RescaleBicubic2");
			RescaleBicubic2(img, 480, 84);
		}
		{
			RTIMING("RescaleBicubic3");
			RescaleBicubic3(img, 480, 84);
		}
		{
			RTIMING("RescaleBicubic4");
			RescaleBicubic3(img, 480, 84);
		}
	}
#endif
}
