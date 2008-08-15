#include <CtrlLib/CtrlLib.h>

struct MyAppWindow : TopWindow {
	typedef MyAppWindow CLASSNAME;
    Point  p;
    String text;
    //StaticRect s1;
    StaticRect c1;
    Button button;
    virtual void LeftDown(Point pos, dword flags) {
		c1.SetRect(20,30,80,50);
		c1.Add(button.SetLabel("Button").LeftPosZ(10, 64).TopPosZ(10, 24));
        c1.PopUp(this,false,true,false,true);
        p = pos;
        Refresh();
    }

    virtual void MouseMove(Point pos, dword flags) {
        text = Format("[%d:%d]", pos.x, pos.y);
        Refresh();
    }

    virtual void Paint(Draw& w) {
        w.DrawRect(GetSize(), SWhite);
        w.DrawText(p.x, p.y, text, Arial(20), Magenta);
    }

	void finish()
	{
	    c1.Close();
	}

	MyAppWindow() {
		p.x = p.y = 0;
		button <<= THISBACK(finish);
    }
};

GUI_APP_MAIN
{
	MyAppWindow w;
	w.Run();
}
