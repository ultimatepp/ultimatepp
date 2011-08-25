#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Media_demo/demo.lay>
#include <CtrlCore/lay.h>

void Run_MediaPlayer();
void Run_VolumeCtrl();

struct Media_Demo : public WithMain<TopWindow> {
typedef Media_Demo CLASSNAME;	
	void OnRun() {
		String demo = grid.Get(0);
		if (demo == "MediaPlayer")
			Run_MediaPlayer();
		else if (demo == "VolumeCtrl")
			Run_VolumeCtrl();
	}
	Media_Demo() {
		CtrlLayout(*this, "Media Examples");
		
		butRun.WhenAction = THISBACK(OnRun);
		
		grid.AddColumn("Demo", 20);
		grid.AddColumn("Description", 60);
		grid.Add("MediaPlayer", "MediaPlayer control demo");
		grid.Add("VolumeCtrl", "Simple VolumeCtrl control demo");
		grid.SetLineCy(int(1.4*StdFont().GetCy()));
		grid.WhenLeftDouble = THISBACK(OnRun);
	}
};

GUI_APP_MAIN {
	Media_Demo().Run();
}
