#include <DropGrid/DropGrid.h>

using namespace Upp;

struct App : public TopWindow {
	DropGrid drop;

	App()
	{
		Sizeable().Zoomable();
		SetRect(Size(Zx(600), Zy(100)));
		Title("DropGrid");
		
		drop.ClearButton();
		drop.AddPlus([=] { PromptOK("Action!"); });

		Add(drop.LeftPosZ(20, 350).TopPosZ(20, 19));

		drop.AddColumn("ID");
		drop.AddColumn("Value");
		drop.AddColumn("Description");
		
		drop.AddText("Add person", [=] { PromptOK("Person!"); });
		drop.AddText("Add client", [=] { PromptOK("Client!"); }).Left().SetImage(GridImg::Append());
		drop.AddSelect([=] { PromptOK("Select!"); }).Left();
		
		for(int i = 0; i < 20; i++)
			drop.Add(i, Format("Hello %d", i), Format("How are you mr Hello %d", i));

		drop.Width(300);
		drop.DisplayAll(0);
		drop.ColorRows();
		drop.SetDropLines(15);
		drop.SetValueColumn(1);
		drop.AddValueColumn(0).AddValueColumn(1);
		drop << [=] { PromptOK(AsString(drop.GetKey())); };
	}

	typedef App CLASSNAME;
};

GUI_APP_MAIN
{
	App().Run();
}
