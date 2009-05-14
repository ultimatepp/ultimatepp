#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>

using namespace Upp;

struct App : public TopWindow
{
	DropGrid drop;

	App()
	{
		Sizeable().Zoomable();
		SetRect(Size(600, 100));
		Title("DropGrid");
		
		drop.ClearButton();
		drop.AddPlus(THISBACK(Action));

		Add(drop.LeftPosZ(20, 350).TopPosZ(20, 19));

		drop.AddColumn("ID");
		drop.AddColumn("Value");
		drop.AddColumn("Description");
		
		drop.AddText("Add person", THISBACK(Action));
		drop.AddText("Add client", THISBACK(Action)).Left().SetImage(GridImg::Append());
		drop.AddSelect(THISBACK(Action)).Left();
		
		for(int i = 0; i < 20; i++)
			drop.Add(i, Format("Hello %d", i), Format("How are you mr Hello %d", i));

		drop.Width(300);
		drop.DisplayAll(0);/*NoHeader(0)*///NotNull();
		drop.ColorRows();
		drop.SetDropLines(15);
		drop.SetValueColumn(1);
		drop.AddValueColumn(0).AddValueColumn(1);
	}

	void Action()
	{
	}
	
	typedef App CLASSNAME;
};

GUI_APP_MAIN
{
	App().Run();
}
