#include <CtrlLib/CtrlLib.h>
#define IMAGEFILE  <ArrayTest/Images.iml>
#define IMAGECLASS AImg
#include <Draw/iml.h>

using namespace Upp;

struct App : public TopWindow
{
	enum {ASCENDING, DESCENDING, NOSORT};
	typedef App CLASSNAME;

	ArrayCtrl arr;
	Vector<int> vStates;
	int nActive;

	void AddColumn(ArrayCtrl &arr, Vector<int> &vStates, const char *sName)
	{
		vStates.Add(NOSORT);
		arr.AddColumn(sName).HeaderTab().WhenAction = THISBACK1(TabAction, vStates.GetCount() - 1);
	}

	void TabAction(int n)
	{
		if (n != nActive)
		{
			vStates[nActive] = NOSORT;
			arr.HeaderTab(nActive).SetLeftImage(AImg::ImgSortNone(), 0);
			nActive = n;
		}

		switch (vStates[n])
		{
			case NOSORT:
				vStates[n] = ASCENDING;
				arr.HeaderTab(n).SetLeftImage(AImg::ImgSortAsc(), 5);
				arr.Sort(n);
				break;
			case ASCENDING:
				vStates[n] = DESCENDING;
				arr.HeaderTab(n).SetLeftImage(AImg::ImgSortDsc(), 5);
				// arr.ReverseSort(n) <-- it's easier than define my own compare function eg. StdReverseValueCompare
				break;
			case DESCENDING:
				vStates[n] = NOSORT;
				arr.HeaderTab(n).SetLeftImage(AImg::ImgSortNone(), 0);
				// second problem <-- How to set original row order ?
				break;
		}

	}

	void Action()
	{
	}

	void Test()
	{
		PromptOK(DeQtf(AsString(arr.GetClickPos())));
	}

	App()
	{
		arr.HSizePos(0,0).VSizePos(0,0);
		Sizeable().Zoomable();
		Title("ArrayTest");
		vStates.Clear();
		Add(arr);

		nActive = 0;
		AddColumn(arr, vStates, "Name");
		AddColumn(arr, vStates, "Surname");
		AddColumn(arr, vStates, "Age");

		arr.Set(0,0,"Daniel");
		arr.Set(0,1,"Kos");
		arr.Set(0,2,"28");
		arr.Set(1,0,"Marek");
		arr.Set(1,1,"Blaszewski");
		arr.Set(1,2,"31");
		arr.Set(2,0,"Lucek");
		arr.Set(2,1,"Poniewiera");
		arr.Set(2,2,"90");
		arr.NoCursor();
		arr.WhenLeftDouble = THISBACK(Test);
	}

};


GUI_APP_MAIN
{
	App().Run();
}
