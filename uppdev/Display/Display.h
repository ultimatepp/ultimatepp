#ifndef _T_Display_Display_h_
#define _T_Display_Display_h_

#include <CtrlLib/CtrlLib.h>

class DisplayCtrl : public Ctrl {
public:
	virtual Value GetData() const;
	virtual void  SetData(const Value& v);
	virtual void  Paint(Draw& w);

	virtual void operator = (const Value& v)    { SetData(v); }

private:
	Value val;
	Font  font;
	Color ink, paper;

public:
	DisplayCtrl& SetFont(Font f)   { font = f; Refresh(); return *this; }
	DisplayCtrl& Ink(Color c)      { ink = c; Refresh(); return *this; }
	DisplayCtrl& Paper(Color c)    { paper = c; Refresh(); return *this; }

	DisplayCtrl();
	~DisplayCtrl();
};

#endif
