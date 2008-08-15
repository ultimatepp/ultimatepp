#include "ColumnListDisplay.h"

MyColumnList::MyColumnList() {
	SetDisplay(*this);
}

void MyColumnList::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
	//when is this called? my debugger tells me: NEVER
	//how can i change the look of the list else?
	w.DrawRect(r,SBlack());
}

Size MyColumnList::GetStdSize() {
	return Size(100,100);
}

ColumnListDisplay::ColumnListDisplay()
{
	CtrlLayout(*this, "Window title");
	list.SetRect(10,10,100,100);
	list.Insert(0,Value("*1*"));
	list.Insert(1,Value("*2*"));
	list.Insert(2,Value("*3*"));
	list.Insert(3,Value("*4*"));
	Add(list);
}

GUI_APP_MAIN
{
	ColumnListDisplay().Run();
}
