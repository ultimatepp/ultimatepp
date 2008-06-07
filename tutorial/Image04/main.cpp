#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	Image   img;
	FileSel fs;

	void Open();

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point, dword) { Open(); }

	typedef MyApp CLASSNAME;
	MyApp();
};

MyApp::MyApp()
{
	fs.Type("Image file", "*.bmp;*.png;*.tif;*.tiff;*.jpg;*.jpeg;*.gif");
	Sizeable();
}

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White);
	if(img)
		w.DrawImage(0, 0, img);
	else
		w.DrawText(0, 0, "No image loaded!", Arial(30).Italic());
}

void MyApp::Open()
{
	if(fs.ExecuteOpen("Choose the image file to open")) {
		img = StreamRaster::LoadFileAny(~fs);
		Refresh();
	}
}

GUI_APP_MAIN
{
	MyApp app;
	app.Open();
	app.Run();
}
