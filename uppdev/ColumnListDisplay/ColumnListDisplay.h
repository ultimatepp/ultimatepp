#ifndef _ColumnListDisplay_ColumnListDisplay_h
#define _ColumnListDisplay_ColumnListDisplay_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ColumnListDisplay/ColumnListDisplay.lay>
#include <CtrlCore/lay.h>


class MyColumnList : public ColumnList, private Display {
	public:
		MyColumnList();
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;

		Size GetStdSize();
};

class ColumnListDisplay : public WithColumnListDisplayLayout<TopWindow> {
private:
	MyColumnList list;
public:
	typedef ColumnListDisplay CLASSNAME;
	ColumnListDisplay();

};


#endif
