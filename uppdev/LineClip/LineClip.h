#ifndef _LineClip_LineClip_h
#define _LineClip_LineClip_h

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

class LineClip : public TopWindow {
public:
	double x1, y1, x2, y2;
	Rectf  cliprect;

	bool Clip(Painter& sw, double x1, double y1, double x2, double y2);

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);

	typedef LineClip CLASSNAME;
	LineClip();
};

#endif

