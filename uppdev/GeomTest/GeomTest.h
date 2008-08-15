#ifndef _GeomTest_GeomTest_h
#define _GeomTest_GeomTest_h

#include <CtrlLib/CtrlLib.h>





class GeomTest : public TopWindow {
public:
	Vector<Pointf> polygon;
	Point          point;
	bool           paintin;

	virtual void LeftDown(Point p, dword);
	virtual void MouseMove(Point p, dword);
	virtual void Paint(Draw& w);

	typedef GeomTest CLASSNAME;
	GeomTest();
};

#endif
