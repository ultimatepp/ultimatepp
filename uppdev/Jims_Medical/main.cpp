#include <CtrlLib/CtrlLib.h>

class App : public TopWindow {
	LineEdit   ed1,ed2;
	Splitter   horz;

	FrameRight<StaticRect> sr;
	Option                 btn;

public:
	typedef App CLASSNAME;
	void MoveSplitter();
	App();
};

void App::MoveSplitter()
{
	if(btn)
		horz.SetPos(1000);
	else
		horz.SetPos(5000);
}

App::App()
{
	horz.Add(ed1);
	horz.Add(ed2);

	Add(horz.Horz().VSizePos(35,35));
	ed1.SetFrame(0, sr);
	sr.Width(20);
	ed1.AddFrame(FieldFrame());
	sr.Add(btn.LeftPos(5, 20).TopPos(0, 20));

	btn.WhenAction=THISBACK(MoveSplitter);
	Sizeable().Zoomable();
}


GUI_APP_MAIN
{
	App().Run();
}
