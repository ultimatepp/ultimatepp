#include "PlatformRef.h"

struct PlatformRef : public TopWindow {
public:
	String mouse, keyboard;

	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword flags);
	virtual bool Key(dword key, int count);

	typedef PlatformRef CLASSNAME;

	PlatformRef();
};

void PlatformRef::MouseMove(Point p, dword flags)
{
	mouse = AsString(p);
	Refresh();
	Title(mouse);
}

void PlatformRef::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, White);
	w.DrawText(0, 0, keyboard);
	Size tsz = GetTextSize(mouse, StdFont());
	w.DrawText(sz.cx - tsz.cx, 0, mouse);
	
	ImageBuffer ib(128, 128);
	
}

bool PlatformRef::Key(dword key, int count)
{
	keyboard = GetKeyDesc(key);
	Refresh();
	return false;
}

PlatformRef::PlatformRef()
{
}

GUI_APP_MAIN
{
	PlatformRef().Run();
}
