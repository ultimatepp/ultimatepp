#include <CtrlLib/CtrlLib.h>

struct App : TopWindow {
	void LeftDown(Point p, dword keyflags)
    {
        Refresh(Rect(0,0,6,GetSize().cy));
    }

    void Paint(Draw &w)
    {
        Size sz = GetSize();
        Rect rc = w.GetClip();
        static bool first = true;

		DUMP(rc);

        if(first)
        {
            w.DrawRect(sz, SGreen);
            first = false;
        }
        else
        for(int i = rc.left; i <= rc.right; i++)
            w.DrawLine(i, rc.top , i, rc.bottom, 0,  (i & 1) ? SWhite : SBlack);
    }
};

GUI_APP_MAIN
{
	App().Run();
}
