#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow
{
	TimeCallback cb;
	void Back() {
		LOG("Back");
		Overlap();
	}
	
	virtual bool Key(dword key, int) {
		if(key == K_CTRL_M)
			Maximize();
		if(key == K_CTRL_N) {
			Minimize();
			cb.Set(2000, THISBACK(Back));
		}
		if(key == K_CTRL_B)
			Overlap();
		if(key == K_CTRL_F)
			FullScreen(!IsFullScreen());
		return false;
	}
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		if(IsMaximized())
			w.DrawText(0, 0, "Maximized!");
		else
		if(IsMinimized())
			w.DrawText(0, 0, "Minimized!");
	}
	
	void Sync() {
		if(IsMaximized())
			Title("Maximized!");
		else
		if(IsMinimized())
			Title("Minimized!");
		else
			Title("Normal");
	}
	
	typedef MyApp CLASSNAME;
	
	void Serialize(Stream& s)
	{
		SerializePlacement(s, true);
	}
	
	MyApp() {
		Sizeable().Zoomable();
		SetTimeCallback(-100, THISBACK(Sync));
	}
};

GUI_APP_MAIN
{
	MyApp app;
	LoadFromFile(app);
	app.Run();
	StoreToFile(app);
}

