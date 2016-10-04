#include "CtrlLib/CtrlLib.h"

using namespace Upp;

class G965 : public TopWindow {
public:
	virtual Image CursorImage(Point p, dword keyflags) {
		return GetTickCount() & 128 ? Image::Arrow() : Image::IBeam();
	}
};

GUI_APP_MAIN
{
	G965().Run();
}
