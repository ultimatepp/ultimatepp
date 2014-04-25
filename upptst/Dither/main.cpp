#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Image image;
	Image image2;
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawImage(0, 0, image);
		w.DrawImage(0, 500, image2);
		DrawFrame(w, 0, 500, 512, 32, Black());
	}
	
	MyApp() {
		ImageBuffer ib(512, 32);
		RGBA *t = ~ib;
		for(int y = 0; y < 32; y++)
			for(int x = 0; x < 512; x++)
				*t++ = GrayColor(x / 2);
		image2 = ib;
		image2 = Dither(image2);
	}

};

GUI_APP_MAIN
{
	MyApp app;
	app.image = Dither(StreamRaster::LoadFileAny(GetDataFile("test.jpg")));
	app.Run();
}
