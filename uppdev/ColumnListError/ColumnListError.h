#ifndef _ColumnListError_ColumnListError_h
#define _ColumnListError_ColumnListError_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;


#define IMAGECLASS ColumnListErrorImg
#define IMAGEFILE <ColumnListError/ColumnListError.iml>
#include <Draw/iml.h>


class MyList : public ColumnList, private Display {
	public:
		typedef MyList CLASSNAME;
		MyList();
		void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		Size GetStdSize();
};

class ListViewer : public Ctrl {

	private:
		ToolBar tb;
		MyList cl;

	public:
		typedef ListViewer CLASSNAME;
		ListViewer();
		void UpdateToolbar() 				{tb.Set(THISBACK(SetToolBar));}
		void SetToolBar(Bar& tb);
		void Paint(Draw& w);
		void Layout();
		void InsertCL(Value v) 					{cl.Insert(cl.GetCount(),v);}
		void DoNothing() {}
};

class ColumnListError : public TopWindow { //public WithColumnListErrorLayout<TopWindow> {
	private:
		ListViewer lv;
		ColumnList cl;
		ToolBar tb;
public:
	typedef ColumnListError CLASSNAME;
	ColumnListError();

	void UpdateToolbar() 				{tb.Set(THISBACK(SetToolBar));}
	void SetToolBar(Bar& tb);
	void DoNothing() {}
	void Paint(Draw &w);
	void AddEntries();
	void ShowCl();
	void Layout();

};


#endif
