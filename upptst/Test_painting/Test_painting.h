#ifndef _Test_painting_Test_painting_h
#define _Test_painting_Test_painting_h

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS Test_paintingImg
#define IMAGEFILE <Test_painting/Test_painting.iml>
#include <Draw/iml_header.h>

class TestCtrl_1 : public Ctrl {
	public:
	typedef TestCtrl_1 CLASSNAME;
	
	int _drawMode;
	Image _ctrlBckgndStyle;
	
	TestCtrl_1() {
		Transparent();
		BackPaintHint();
	}
	virtual ~TestCtrl_1() {}

	void Paint2(ImageBuffer& ib) {
		RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
		Upp::Fill( ib.Begin(), bckgColor, ib.GetLength() );
		BufferPainter bp(ib, _drawMode);
		bp.Clear(RGBAZero());
		bp.DrawRect(Rect(Point(0,0), Point( GetSize().cx, GetSize().cy/2)), White());
		bp.DrawRect(Rect(Point(0,GetSize().cy/2), Point( GetSize().cx, GetSize().cy)), Blue());
		ChPaint(bp, GetSize(), _ctrlBckgndStyle );
		bp.DrawLine( Point(0,0), Point( GetSize().cx, GetSize().cy), 1 , Red() );
	}

	virtual void Paint(Draw& dw) {
		ImageBuffer ib(GetSize());
		Paint2(ib);
		dw.DrawImage(GetSize(), ib);
	}

};

class TestCtrl_2 : public TestCtrl_1 {
	public:
	typedef TestCtrl_2 CLASSNAME;
	
	TestCtrl_2() {}
	virtual ~TestCtrl_2() {}


	virtual void Paint(Draw& dw) {
		ImageBuffer ib(GetSize());
		Paint2(ib);
		Premultiply(ib);
		dw.DrawImage(GetSize(), ib);
	}
};

class TestCtrl_3 : public TestCtrl_1 {
	public:
	typedef TestCtrl_3 CLASSNAME;
	
	
	TestCtrl_3() {}
	virtual ~TestCtrl_3() {}


	virtual void Paint(Draw& dw) {
		ImageBuffer ib(GetSize());
		RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
		Upp::Fill( ib.Begin(), bckgColor, ib.GetLength() );
		BufferPainter bp(ib, _drawMode);
		
		ImageBuffer ib2(GetSize());
		Paint2(ib2);
		
		bp.DrawImage(GetSize(), ib2);
		dw.DrawImage(GetSize(), ib);
	}
};


#define LAYOUTFILE <Test_painting/Test_painting.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <Test_painting/Test_painting.iml>
#include <Draw/iml_header.h>

class Test_painting : public WithTest_paintingLayout<TopWindow> {
public:
	typedef Test_painting CLASSNAME;
	Test_painting();
};

#endif
