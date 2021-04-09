#include <CodeEditor/CodeEditor.h>

using namespace Upp;

struct MyCtrl : Ctrl {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), Blue());
	}
	void LeftHold(Point p, dword keyflags) override
	{
		Exclamation("Left Hold!");
	}
	void RightHold(Point p, dword keyflags) override
	{
		Exclamation("Right Hold!");
	}
	void MiddleHold(Point p, dword keyflags) override
	{
		Exclamation("Middle Hold!");
	}
	void LeftTriple(Point p, dword keyflags) override
	{
		Exclamation("Left Triple!");
	}
	void RightTriple(Point p, dword keyflags) override
	{
		Exclamation("Right Triple!");
	}
	void MiddleTriple(Point p, dword keyflags) override
	{
		Exclamation("Middle Triple!");
	}
};

GUI_APP_MAIN
{
	MyCtrl ctrl;
	TopWindow win;
	win << ctrl.RightPos(100, 100).TopPos(200, 200);
	win.Run();
}
