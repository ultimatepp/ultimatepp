#ifndef _CBPMain_DropPane_h_
#define _CBPMain_DropPane_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class DropPane: public Ctrl
{
private:
	Font  font;
	Color ink, paper;
	ButtonOption btnDrop;
	Label lblText;
	
	int lastHeight;
	void Togle();
public:
	typedef DropPane CLASSNAME;

	DropPane& SetFont(Font f)   { font = f; Refresh(); return *this; }
	DropPane& Ink(Color c)      { ink = c; Refresh(); return *this; }
	DropPane& Paper(Color c)    { paper = c; Refresh(); return *this; }

	DropPane();
	~DropPane();

	virtual void SetRect(int x, int y, int cx, int cy);
	void Paint(Draw& w);
	void SetLabel(const char* label);
};

#endif
