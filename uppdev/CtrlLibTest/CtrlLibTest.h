#ifndef _CtrlLibTest_CtrlLibTest_h
#define _CtrlLibTest_CtrlLibTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlLibTest/CtrlLibTest.lay>
#include <CtrlCore/lay.h>
/*
class MagickRaster : public Raster
{
public:
	virtual Size    GetSize()			{ return sz; }
	virtual Line    GetLine(int line);
	virtual Info    GetInfo();
	
	MagickRaster(Magick::Image img);
	MagickRaster(Magick::Image img, int x, int y, int w, int h);
private:	
	Magick::PixelPacket *	pixels;
	Size 					sz;
	
	void SetImage(Magick::Image, int x, int y, int w, int h);
};

MagickRaster::MagickRaster(Magick::Image)
{
	SetImage(img, 0, 0, img.columns(), img.rows()); 
}

MagickRaster::MagickRaster(Magick::Image, int x, int y, int w, int h)
{
	SetImage(img, x, y, w, h);
}

void MagickRaster::SetImage(Magick::Image, int x, int y, int w, int h)
{
	pixels = Magick::getPixels(x, y, w, h);
	sz = Size(w, h);
}

Line MagickRaster::GetLine(int line)
{
	if (Magick::QuantumDepth == 8)
		return Line((const RGBA *)~lines[line], false);
	else
		return Line(~lines[line], this, false)
}

Info MagickRaster::GetInfo()
{
	Info f;
	f.bpp = Magick::QuantumDepth;
	f.colors = (1 << f.bpp)*3;
	f.dots = Size(0, 0);
	f.hotspot = Point(0, 0);
	f.kind = IMAGE_ALPHA;
	return f;	
}
*/

class ChildWnd : public WithCtrlLibTestLayout<TopWindow> {
public:
	typedef ChildWnd CLASSNAME;
	ChildWnd();
};

class CtrlLibTest : public WithCtrlLibTestLayout<TopWindow> {
public:
	ChildWnd wnd;

	typedef CtrlLibTest CLASSNAME;
	CtrlLibTest();
	
	void OnPush1();
	void OnPush2();
};


#endif

