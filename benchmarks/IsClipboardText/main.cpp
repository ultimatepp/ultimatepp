#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		for(int i = 0; i < 100; i++) {
			{
				RTIMING("IsClipboardAvailableText");
				IsClipboardAvailableText();
			}
			{
				RTIMING("IsClipboardAvailableImage");
				IsClipboardAvailableImage();
			}
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
