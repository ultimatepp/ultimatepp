#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct RandomRectCtrl : public Ctrl {
	Rect  rect;
	Color color;
	
	void Paint(Draw& w) override {
		Size sz = GetSize();
		
		w.DrawRect(sz, White());
		if (!rect.IsEmpty()) {
			w.DrawRect(rect, color);
		}
	}
	
	void RandomizeRect() {
		Size sz = GetSize();
		
		int length = 50;
		int x = Random() % (sz.cx - length);
		int y = Random() % (sz.cy - length);
		
		rect = Rect(x, y, x + length, y + length);
		color = Color(Random() % 256, Random() % 256, Random() % 256);
	}
};

#define LAYOUTFILE <Gui23/myapp.lay>
#include <CtrlCore/lay.h>

struct MyAppWindow : public WithMyAppLayout<TopWindow> {
	MyAppWindow() {
		CtrlLayout(*this, "MyApp");
		
		start_stop_btn << [=] { OnStartStop(); };
	}
	
	~MyAppWindow() {
		// Not needed will be automatically call upon Ctrl destruction.
		// KillTimeCallback();
	}
	
	void OnStartStop() {
		if (random_rect_ctrl.rect.IsEmpty()) {
			start_stop_btn.SetLabel("Stop!");
			
			// When the user clicks the "Start" button, OnTimer() is immediately executed after
			// all related events have been processed, to generate and display a random rectangle.
			SetTimeCallback(0, [=] { OnTimer(); });
			
			// Calls OnTimer() every two seconds to update the random position of a rectangle.
			SetTimeCallback(-2000, [=] { OnTimer(); });
		} else {
			KillTimeCallback();
			
			start_stop_btn.SetLabel("Start!");
			random_rect_ctrl.rect.Clear();
			random_rect_ctrl.Refresh();
		}
	}
	
	void OnTimer() {
		random_rect_ctrl.RandomizeRect();
		random_rect_ctrl.Refresh();
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
