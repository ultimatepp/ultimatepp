#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

using namespace Upp;

struct App : TopWindow
{
	typedef App CLASSNAME;

	GridCtrl grid;
	EditString name;
	EditInt age;

	App()
	{
		Sizeable().Zoomable();

		Add(grid.SizePos());

		grid.AddColumn("Name").Edit(name);
		grid.AddColumn("Age").Edit(age);

		grid.Add("Ann", 21)
		    .Add("Jack", 34)
		    .Add("David", 15);

		grid.Set(0, 0, "Daniel");
		grid.SetFixed(0, 1, "Age of person");

		grid.Indicator();
		grid.WhenLeftDouble = THISBACK(ShowInfo);

		/* uncomment this to get random color of rows */
		//grid.WhenChangeRow = THISBACK(SetRandomColor);

		grid.ColorRows();
		grid.EditCell();
		grid.EnterLikeTab();
		grid.GoBegin();
	}

	void ShowInfo()
	{
		PromptOK(Format("%s is %d years old", grid(0), grid(1)));
	}

	void SetRandomColor()
	{
		grid.GetRow().Bg(Color(rand() % 255, rand() % 255, rand() % 255));
	}
};

GUI_APP_MAIN
{
	App().Run();
}
