#include "Utils.h"

#define DOCK	"gsettings set org.onboard.window docking-enabled true"
#define BOTTOM	"gsettings set org.onboard.window docking-edge 'bottom'"

using namespace Upp;

void Upp::ShowKeyboard(bool show)
{
	if (show){
		Size sz = GetScreenSize();
		int e = system(Format(DOCK ";" BOTTOM ";onboard -l Phone -x %d -y %d -s %d`x%d &", 0, sz.cy - sz.cy / 4, sz.cx, sz.cy / 4));
	}else{
		Sys("killall onboard");
	}
}