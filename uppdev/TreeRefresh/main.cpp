#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyDisplay : public Display {
  virtual void MyDisplay::Paint(Draw& w,const Rect& r,
                                const Value& q, Color ink,
                                Color paper,dword style) const
  {
    w.DrawRect(r, paper);
    w.DrawText(r.left, r.top, (String)q, StdFont(), Blue()); // <- the color I want goes in here
  }
 };

GUI_APP_MAIN
{
//	Ctrl::ShowRepaint(100);
	TopWindow win;
	TreeCtrl tree;
	tree.SetDisplay(Single<MyDisplay>());
	tree.SetRoot(Null, "Tree");
	Array<EditString> s;
	tree.BackPaint();
	for(int i = 0; i < 30; i++) {
		int id = tree.Add(0, Null, "Node " + AsString(i));
		for(int j = 0; j < 20; j++) {
			tree.Add(id, s.Add());
			s.Top() <<= AsString(j);
		}
	}
	win.Add(tree.SizePos());
	win.Run();
}
