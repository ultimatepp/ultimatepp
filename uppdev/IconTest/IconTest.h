#ifndef _IconTest_IconTest_h
#define _IconTest_IconTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <IconTest/IconTest.lay>
#include <CtrlCore/lay.h>



class IconTest : public WithIconTestLayout<TopWindow> {
public:
	typedef IconTest CLASSNAME;
	IconTest();
	
	Image img;
	
	virtual void Paint(Draw &w) {
		w.DrawRect(GetSize(), SColorFace);
		w.DrawImage(HorzLayoutZoom(4), VertLayoutZoom(4), img);
	}	
};

#endif

