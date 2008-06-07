#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>

using namespace Upp;

struct App : public TopWindow
{
	DropGrid drop, dg;
	DropList classic_drop, dl;
	GridCtrl grid;
	ArrayCtrl array;

	App()
	{
		//CtrlLayout(*this, "DropGrid");
		Sizeable().Zoomable();
		Title("DropGrid");

		dl.Add("Item 0").Add("Item 1").Add("Item 2");
		dg.Add("Item 0").Add("Item 1").Add("Item 2");

		//Add(drop.LeftPos(20, 150).TopPos(20, 19));
		Add(grid.LeftPos(20, 300).TopPos(100, 150));
		Add(array.LeftPos(350, 300).TopPos(100, 150));

//		array.AddColumn("Col 0").Edit(dg);
//		array.AddColumn("Col 1").Edit(dl);
		//array.AddColumn("Col 2").Option();
		array.Appending();
		grid.Searching();

//		grid.AddColumn("Col 0").Edit(dg);
//		grid.AddColumn("Col 1").Edit(dl);
//		grid.AddColumn("Col 2").Option();
		grid.Appending();

		drop.AddColumn("ID");
		drop.AddColumn("Value");
		drop.AddColumn("Description");
		for(int i = 0; i < 20; i++)
			drop.Add(i, Format("Hello %d", i), "How are you now?");

		drop.Width(300);
		drop.DisplayAll(0);/*NoHeader(0)*///NotNull();
		drop.ColorRows();
		//drop.NotNull();
		drop.SetDropLines(15);
		drop.SetValueColumn(1);
		drop.AddValueColumn(0).AddValueColumn(1);
		Add(classic_drop.LeftPos(20, 150).TopPos(60, 19));
		classic_drop.Add("ALA").Add("ELA").Add("BABA");
	}
};

#ifdef flagMAIN

GUI_APP_MAIN
{
	App().Run();
}

#endif
