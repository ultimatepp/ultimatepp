#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White);
		w.DrawText(10, 10, "ultimate++是一个性能优良的C++GUI库");
		w.DrawText(10, 100, "X\357\274\214X");
		WString h("x\357\274\214X");
		w.DrawText(10, 150, h);
		DDUMP(h.GetLength());
	}
};

GUI_APP_MAIN
{
	DDUMP(WString("\357\274\214")[0]);
	DDUMP(WString("X\357\274\214X"));
//	for(int i = 0; i < Font::GetFaceCount(); i++)
//		LOG(Font::GetFaceName(i));
	MyApp().Run();
}

