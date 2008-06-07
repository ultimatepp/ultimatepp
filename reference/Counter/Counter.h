#ifndef _T_Counter_Counter_h_
#define _T_Counter_Counter_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class CounterCtrl : public Ctrl {
public:
	virtual Value GetData() const;
	virtual void  SetData(const Value& v);
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point, dword);

private:
	int   number;
	Font  font;
	Color ink, paper;

public:
	CounterCtrl& SetFont(Font f)   { font = f; Refresh(); return *this; }
	CounterCtrl& Ink(Color c)      { ink = c; Refresh(); return *this; }
	CounterCtrl& Paper(Color c)    { paper = c; Refresh(); return *this; }

	CounterCtrl();
	~CounterCtrl();
};

#endif
