#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	MenuBar menu;

	MyAppWindow() {
		Title("My application with menu").Sizeable();
		AddFrame(menu);
		menu.Set(
			[=](Bar& bar) {
				bar.Sub("Menu",
						[=](Bar& bar) {
							bar.Add("Exit",
								[=] {
									if(PromptOKCancel("Exit MyApp?"))
										Break();
								}
							);
						}
				);
			}
		);
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
