#ifndef _WinGl_Console_h_
#define _WinGl_Console_h_

#include <CtrlLib/CtrlLib.h>
#include "Scrollbar.h"

NAMESPACE_UPP

struct Console : Ctrl
{
	Ctrl* parent;
	SlimScrollBar sb;
	
	struct LineInfo : Moveable<LineInfo> {
		Color ink;
		String text;
	};
	
	bool init;
	bool resize;
	Point pos;
	Point tl;
	Size rs;
	int currLine;
	int scrollLine;
	int maxLine;
	VectorMap<int, LineInfo> fixedText;
	Vector<LineInfo> floatText;
	void Init(Ctrl& parent);
	void SetConsoleRect(int x, int y, int cx = -1, int cy = -1);
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keys);
	virtual void LeftUp(Point p, dword keys);
	virtual void MouseMove(Point p, dword keys);
	virtual void Layout();
	
	void Log(int line, const char* text, Color ink = White);
	void Log(const char* text, Color ink = White);
	
	void SetLines(int lines);
	void Show(bool b = true);
	
	typedef Console CLASSNAME;
	Console();
};

END_UPP_NAMESPACE

#endif
