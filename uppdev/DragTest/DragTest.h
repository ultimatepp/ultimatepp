#ifndef _DragTest_DragTest_h
#define _DragTest_DragTest_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

struct DragCtrl : public Ctrl
{
	typedef DragCtrl CLASSNAME;
	virtual void Paint(Draw& draw) {draw.DrawRect(GetSize(), White());}
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	void OnBar(Bar &bar);
	void OnBarItem();
};

class DragTest : public TopWindow {
public:
	typedef DragTest CLASSNAME;
	DragTest();

	DragCtrl ctrl;
};

#endif
