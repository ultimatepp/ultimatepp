#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ScrollBar      sb;
	int            count;

	int GetLineHeight() { return Arial(20).Info().GetHeight(); }

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SWhite);
		int fcy = GetLineHeight();
		int i = sb / fcy;
		int y = i * fcy - sb;
		while(i < count && y < sz.cy) {
			w.DrawText(0, y, AsString(i++, true), Arial(20));
			y += fcy;
		}
	}

	virtual void Layout()
	{
		sb.SetPage(GetSize().cy);
	}

	virtual void MouseWheel(Point, int zdelta, dword)
	{
		sb.Wheel(zdelta);
	}

	bool Key(dword key, int)
	{
		return sb.VertKey(key);
	}

	void SetCount(int n)
	{
		count = n;
		sb.SetTotal(n * GetLineHeight());
	}

	App() {
		Sizeable().Zoomable().BackPaint();
		AddFrame(sb);
		sb.WhenScroll = [=] { Refresh(); };
		sb.SetLine(GetLineHeight());
	}
};

GUI_APP_MAIN
{
	App app;
	app.SetRect(0, 0, 100, 100);
	app.SetCount(15);
	app.Run();
}
