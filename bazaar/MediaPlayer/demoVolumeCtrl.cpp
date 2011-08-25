#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/MediaPlayer.h>
#include <Media/VolumeCtrl.h>

#define LAYOUTFILE <Media_demo/demoVolumeCtrl.lay>
#include <CtrlCore/lay.h>

class VolumeCtrlDemo : public WithMainVolumeCtrl<TopWindow> {
public:
	typedef VolumeCtrlDemo CLASSNAME;
	VolumeCtrlDemo();
};


VolumeCtrlDemo::VolumeCtrlDemo() {
	CtrlLayout(*this, "VolumeCtrl demo");
	Zoomable().Sizeable();
}

void Run_VolumeCtrl() {
	VolumeCtrlDemo().Run();
}
