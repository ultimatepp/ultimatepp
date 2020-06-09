#include "Test_painting.h"



#define IMAGECLASS Test_paintingImg
#define IMAGEFILE <Test_painting/Test_painting.iml>
#include <Draw/iml_source.h>

Test_painting::Test_painting()
{
	CtrlLayout(*this, "Window title");
	ctrl1_draw._drawMode = -1;
	ctrl1_noaa._drawMode = MODE_NOAA;
	ctrl1_aa._drawMode = MODE_ANTIALIASED;
	ctrl1_subpix._drawMode = MODE_SUBPIXEL;
	
	Image img = Test_paintingImg::testImg();
	
	ctrl1_draw._ctrlBckgndStyle = img;
	ctrl1_noaa._ctrlBckgndStyle = img;
	ctrl1_aa._ctrlBckgndStyle = img;
	ctrl1_subpix._ctrlBckgndStyle = img;


	ctrl2_draw._drawMode = -1;
	ctrl2_noaa._drawMode = MODE_NOAA;
	ctrl2_aa._drawMode = MODE_ANTIALIASED;
	ctrl2_subpix._drawMode = MODE_SUBPIXEL;
	
	ctrl2_draw._ctrlBckgndStyle = img;
	ctrl2_noaa._ctrlBckgndStyle = img;
	ctrl2_aa._ctrlBckgndStyle = img;
	ctrl2_subpix._ctrlBckgndStyle = img;


	ctrl3_draw._drawMode = -1;
	ctrl3_noaa._drawMode = MODE_NOAA;
	ctrl3_aa._drawMode = MODE_ANTIALIASED;
	ctrl3_subpix._drawMode = MODE_SUBPIXEL;
	
	ctrl3_draw._ctrlBckgndStyle = img;
	ctrl3_noaa._ctrlBckgndStyle = img;
	ctrl3_aa._ctrlBckgndStyle = img;
	ctrl3_subpix._ctrlBckgndStyle = img;
}

GUI_APP_MAIN
{
	Image img = Test_paintingImg::testImg();
	const RGBA *s = ~img;
	Index<RGBA> rgba;
	for(const RGBA& c : img)
		rgba.FindAdd(c);
	DDUMP(rgba);
	Test_painting().Run();
}



