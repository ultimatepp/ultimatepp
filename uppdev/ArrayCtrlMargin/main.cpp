#include <CtrlLib/CtrlLib.h>

class TestArrayMargin : public TopWindow {
	ArrayCtrl array;
	EditString eString;
public:
	typedef TestArrayMargin CLASSNAME;
	TestArrayMargin();
};

TestArrayMargin::TestArrayMargin()
{
	array.SetRect(0,0,500,100);
	Add(array);
	array.AddColumn("test");
	for(int
	array.Add("item 1");
}

struct MarginDisplay : Display {
	Display *b;
	int      margin;

	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		b->Paint(w, r.Deflated(margin, 0), q, ink, paper, style);
	}

}

GUI_APP_MAIN
{
	TestArrayMargin().Run();
}
