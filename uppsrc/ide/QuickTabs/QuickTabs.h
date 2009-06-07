#ifndef _QuickTabs_QuickTabs_h
#define _QuickTabs_QuickTabs_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_header.h>

enum
{
	QT_MARGIN = 6,
	QT_SPACE = 10,
	QT_SBHEIGHT = 4,
	QT_SBSEPARATOR = 1,
	QT_FILEICON = 16,
	QT_SPACEICON = 5
};

class TabScrollBar : public Ctrl
{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		void UpdatePos(bool update = true);
		bool ready;
		Size sz;

	public:
		TabScrollBar();

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void Layout();

		int  GetPos() const;
		void SetPos(int p, bool dontscale = false);
		void AddPos(int p, bool dontscale = false);
		int  GetTotal() const;
		void AddTotal(int t);
		void SetTotal(int t);
		void GoEnd();
		void GoBegin();
		void Set(const TabScrollBar& t);
		Callback WhenScroll;
};

struct Tab : Moveable<Tab>
{
	String name;
	String group;
	bool visible;
	int x, cx;
	int y, cy;
	int id;
	Size tsz;
	Tab() : visible(true), id(-1)
	{}
	int Right() { return x + cx; }
	bool HasMouse(const Point& p);
	bool HasMouseCross(const Point& p, int h);
};

struct Group : Moveable<Group>
{
	Group()	{}
	String name;
	String path;
	int active;
	int count;
	int first;
	int last;
};

class QuickTabs : public FrameCtrl<Ctrl>
{
	public:
		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keysflags);
		virtual void LeftUp(Point p, dword keysflags);
		virtual void RightDown(Point p, dword keyflags);
		virtual void MiddleDown(Point p, dword keyflags);
		virtual void MiddleUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keysflags);
		virtual void MouseLeave();
		virtual void FrameLayout(Rect &r);
		virtual void FrameAddSize(Size& sz);
		virtual void FramePaint(Draw& w, const Rect& r);
		virtual void DragAndDrop(Point p, PasteClip& d);
		virtual void LeftDrag(Point p, dword keyflags);
		virtual void DragEnter();
		virtual void DragLeave();
		virtual void DragRepeat(Point p);
		virtual void CancelMode();
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	private:

		int id;
		FrameBottom<TabScrollBar> sc;
		void Scroll();

		Vector<Group> groups;
		Vector<Tab> tabs;
		int highlight;
		int active;
		int target;
		int cross;
		bool crosses;
		bool file_icons;
		bool isctrl;
		bool isdrag;
		bool grouping;
		Point mouse, oldp;
		int group;

		const TabCtrl::Style *style;


		void DrawTab(Draw &w, int i);
		void Repos();
		int  Find(int id);
		int  GetNext(int n);
		int  GetPrev(int n);

		int GetWidth(int n);
		int GetTargetTab(Point p);

		int GetWidth();
		int GetHeight();

		void Menu(Bar& bar);
		void GroupMenu(Bar &bar, int n);

		void CloseAll();

		void MakeGroups();
		int  FindGroup(const String& g);

		void DoGrouping(int n);
		void DoCloseGroup(int n);

		String GetGroupName() const      { return group == 0 ? Null : groups[group].path; }
		int  SetGroup(const String &s)   { group = max(0, FindGroup(s)); return group; }
		int  SetGroup(int c)             { group = c; return group; }
		int  GetGroup() const            { return group;                }
		void SetActiveGroup(int id)      { groups[group].active = id;   }
		int  GetActiveGroup() const      { return groups[group].active; }
		int  GetFirst() const            { return groups[group].first;  }
		int  GetLast() const             { return groups[group].last;   }
		bool IsGroupAll() const          { return group == 0;           }

	public:
		Callback WhenCloseRest;

		QuickTabs();

		void Add(const char *name, bool make_active = false);
		void Close(int n);
		void Clear();

		QuickTabs& FileIcons(bool b = true);
		QuickTabs& Crosses(bool b = true);
		QuickTabs& Grouping(bool b = true);

		typedef QuickTabs CLASSNAME;

		int    GetCount() const { return tabs.GetCount(); }
		int    GetCursor() const { return active; }
		String GetFile(int n) const { return tabs[n].name; }
		bool   FindSetFile(const String& fn);
		void   SetAddFile(const String& fn);
		void   RenameFile(const String& fn, const String& nn);
		void   Set(const QuickTabs& t);

		QuickTabs& SetStyle(const TabCtrl::Style& s)  { style = &s; Refresh(); return *this; }
		static const TabCtrl::Style& StyleDefault();

		int    GetNextId();

		const Vector<Tab>& GetTabs() { return tabs; }
		int GetPos() { return sc.GetPos(); }

		void SetCursor(int n);
		
		void SerializeTabs(Stream& s);
};

#endif
