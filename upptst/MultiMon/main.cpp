#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	RichTextCtrl view;

	void Layout() {
		Array<Rect> a;
		GetWorkArea(a);

		String text = "[g \1"
			"Rect: " << GetRect() << "\n" <<
			"ScreenRect: " << GetScreenRect() << "\n" <<
			"WorkArea: " << GetWorkArea() << "\n" <<
			"WorkAreas: " << a << "\n" <<
			"PrimaryWorkArea: " << GetPrimaryWorkArea() << "\n"
		;
		for(int i = 0; i < 200; i++)
			text << "\n";
		view.SetQTF(text);
	}
	
	void Serialize(Stream& s)
	{
		SerializePlacement(s);
	}
	
	MyApp() {
		Sizeable().Zoomable();
		Add(view.SizePos());
		view.WhenLeftClick = [=] {
			PromptOK("Test");
		};
	}
};

GUI_APP_MAIN
{
	MyApp app;
	LoadFromFile(app);
	app.Run();
	Rect r = app.GetRect();
	StoreToFile(app);
	PromptOK("OK \1" + AsString(r));
}
