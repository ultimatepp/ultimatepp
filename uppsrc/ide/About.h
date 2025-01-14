#ifndef _ide_About_h_
#define _ide_About_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp {

class SplashCtrl final : public Ctrl
{
public:
	static String GenerateVersionInfo(bool qtf = false);
	static String GenerateVersionNumber();
	static Size   MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl);
	
public:
	SplashCtrl();
	
private:
	Array<Ctrl> ctrl;
};

}

#endif
