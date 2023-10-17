#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	Image   img;
	FileSel fs;

	MyApp() {
		fs.Type("Image file", "*.bmp;*.png;*.tif;*.tiff;*.jpg;*.jpeg;*.gif");
		Sizeable();
	}

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White);
		if(img) {
			w.DrawImage(0, 0, img);
		}
		else {
			w.DrawText(0, 0, "No image loaded!", Arial(30).Italic());
		}
	}
	
	void LeftDown(Point, dword) override {
		Open();
	}
	
	void Open() {
		if(fs.ExecuteOpen("Choose the image file to open")) {
			img = StreamRaster::LoadFileAny(~fs);
			Refresh();
		}
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.Open();
	app.Run();
}
