#include <CtrlLib/CtrlLib.h>

struct Date { int da, b, c; };

Date aaa(int x, int y);

Date ddd(1, 2, 3);

struct App : public TopWindow
{
	TabCtrl tab;

	ArrayCtrl arr0;
	ArrayCtrl arr1;

	App();
};

void foo()
{
	Date qqq;
	Date xxx(int a, int b, int c);
	Date q;
	Title("Disapearing scrollbar");
	tab.HSizePos(0,0).
	    VSizePos(0,0);
	arr0.HSizePos(0,0).
	     VSizePos(0,0);
	arr0.Track(false).
	     AutoHideSb();
	arr0.HeaderObject().Absolute();

	arr1.HSizePos(0,0).
	     VSizePos(0,0);
	arr1.Track(false).
	     AutoHideSb();

	arr1.HeaderObject().Absolute();

	tab.Add(arr0, "Sheet 1");
	tab.Add(arr1, "Sheet 2");

	for (int i = 0; i < 10; i++)
	{
		arr0.AddColumn("Column" + AsString(i+1), 100);
		arr1.AddColumn("Column" + AsString(i+1), 100);
	}

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
		{
			String str = "Value" + AsString(j) + "." + AsString(i);
			arr0.Set(j, i, str);
			arr1.Set(j, i, str);
		}

	Sizeable().Zoomable();

	Add(tab);
}

GUI_APP_MAIN
{
	App().Run();
}
