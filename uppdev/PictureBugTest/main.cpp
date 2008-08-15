#include "PictureBugTest.h"



PictureBugTest::PictureBugTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable();
	open <<= THISBACK(Open);

	pict.KeepRatio();
}

void PictureBugTest::Open()
{
	FileSelector fs;
	fs
		.Type(t_("Pictures (*.png; *.bmp; *.jpg)"), "*.png;*.bmp;*.jpg")
		.AllFilesType()
	;
	if(!fs.ExecuteOpen())
		return;
	String filename = fs;
	
	Image img = StreamRaster::LoadFileAny(filename);
	if(img.IsEmpty())
	{
		Exclamation(t_("File is not an image file!"));
		return;
	}

	DrawingDraw d( img.GetWidth(), img.GetHeight());
	d.DrawImage(0,0, img.GetWidth(), img.GetHeight(), img);
	pict.Set(d);
}


GUI_APP_MAIN
{
	PictureBugTest().Run();
}

