#include "Udoc.h"

#define IMAGEFILE <Udoc/Udoc.iml>
#include <Draw/iml_source.h>

void Udoc::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	editor.SerializeSettings(s);
	s % left_right % package_topic;
}

void Udoc::Exit()
{
}

void Udoc::EditBar(Bar& bar)
{
	bar.Add("Exit", THISBACK(Exit));
}

void Udoc::MainMenu(Bar& bar)
{
	bar.Add("Edit", THISBACK(EditBar));
}

void Udoc::MainTool(Bar& bar)
{
	editor.DefaultBar(bar);
}

Udoc::Udoc()
{
	AddFrame(menu);
	editor.InsertFrame(0, TopSeparatorFrame());
	editor.InsertFrame(1, tool);
	menu.Set(THISBACK(MainMenu));
	tool.Set(THISBACK(MainTool));
	int dcy = lang.GetStdSize().cy;
	right.Add(lang.LeftPos(0, 150).TopPos(0, dcy));
	right.Add(title.HSizePos(150, 0).TopPos(0, dcy));
	right.Add(editor.VSizePos(dcy + 4, 0).HSizePos());
	Add(left_right.Horz(left, right).SizePos());
	left_right.SetPos(2000);
	left.Add(group.TopPos(0, dcy).HSizePos());
	left.Add(package_topic.Vert(package.NoRoundSize(), topic.NoRoundSize())
	         .VSizePos(dcy + 4).HSizePos());
	Zoomable().Sizeable();
	ActiveFocus(editor);
}

void Udoc::ClearPackages()
{
	package.Clear();
}

void Udoc::AddPackage(const char *name, const char *dir)
{
	package.Add(name);
	packagedir.Add(dir);
}

void Udoc::Flush()
{
}

GUI_APP_MAIN
{
	Udoc udoc;
	LoadFromFile(udoc);
	udoc.AddPackage("test", "e:/test/test.dpp");
	udoc.Run();
	StoreToFile(udoc);
}
