#include <CtrlLib/CtrlLib.h>

struct GridCtrl : Ctrl
{
	int cols;
	double cx;
	bool ready;

	GridCtrl()
	{
		cols = 5;
		cx = 1;
		ready = false;
	}

	void ComputeSize()
	{
		Size sz = GetSize();
		cx = sz.cx / double(cols);
	}

	virtual void State(int reason)
	{
		if(reason == OPEN)
		{
			ComputeSize();
			ready = true;
		}
	}

	virtual void Paint(Draw &w)
	{
		Size sz = GetSize();

		for(int i = 0; i < cols; i++)
		{
			Color c = (i & 1) ? Red : Yellow;
			w.DrawRect(int(i * cx), 0, fceil(cx), sz.cy, c);
		}

		ready = true;
	}

	virtual void Layout()
	{
		if(ready)
			ComputeSize();
	}
};


struct App : TopWindow
{
	GridCtrl grid, grid1;
	EditString edit, edit1;
	Splitter spl, spl1;
	TabCtrl tab;

	App()
	{
		Add(tab.SizePos());

	//	tab.Add(spl.Horz(grid, edit), "Sheet 1");
		tab.Add(spl1, "Sheet 2");

		Sizeable().Zoomable();
	}

	virtual bool Key(dword key, int count)
	{
		if(key == K_SPACE)
		{
			spl1.Horz(grid1.SizePos(), edit1);
		}
		return false;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
