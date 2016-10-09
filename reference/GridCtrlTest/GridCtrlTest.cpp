#include "GridCtrlTest.h"

void MakeGrid(One<Ctrl>& ctrl)
{
	GridCtrl &c = ctrl.Create<GridCtrl>();
	c.AddColumn("Column 1");
	c.AddColumn("Column 2");
	c.AddColumn("Column 3");
	c.MovingCols();
	c.MovingRows();
	c.Add("Hello", 1, 2);
	c.Add("Daniel", 12, 10);
	c.Add("Ala", 17, 23);
	c.Add("Baby", 22, 8);
}

App::App()
{
	CtrlLayout(*this, "Grid Test Panel");
	
	tab.Add(testMain.SizePos(), "Main");
	tab.Add(testExcelGrid.SizePos(), "Excel grid");
	tab.Add(testPropertyGrid.SizePos(), "Property grid");
	tab.Add(testArrayCtrl.SizePos(), "ArrayCtrl");
	tab.Add(testFocusLost.SizePos(), "Focus lost");
	
	tab << [=] { TabChange(); };
	resort << [=] { Resort(); };
	toxml << [=] { ToXml(); };
	fromxml << [=] { FromXml(); };
	
	Sizeable().Zoomable();//.BackPaint(!false);	
}

void App::Init()
{
	testMain.Init();
	testExcelGrid.Init();
	testPropertyGrid.Init();
	testFocusLost.Init();
	testArrayCtrl.Init();

	panel.Init(testMain.grid);	
}

void App::TabChange()
{
	int t = tab.Get();
	if(t == 0)
		panel.Init(testMain.grid);
	else if(t == 1)
		panel.Init(testExcelGrid.grid);
	else if(t == 2)
		panel.Init(testPropertyGrid.grid);
	else if(t == 4)
		panel.Init(testFocusLost.grid0);
	
	dlog->Clear();
}
		
void App::Serialize(Stream &s)
{
	SerializePlacement(s);
	s % panel.level;
	if(s.IsLoading())
		dlev = ~panel.level;		
}

void App::Resort()
{
	panel.grid->ReSort();
}

void App::ToXml()
{
	String s = StoreAsXML(*panel.grid, "grid-values");
	FileOut f(ConfigFile("grid.xml"));
	f << s;
	f.Close();
}

void App::FromXml()
{
	String s = LoadFile(ConfigFile("grid.xml"));
	LoadFromXML(*panel.grid, s);
}

GUI_APP_MAIN
{
	SetLanguage(LNGC_('E','N','E','N', CHARSET_UTF8));
	App app;
	LoadFromFile(app);
	app.Init();
	app.Run();
	StoreToFile(app);
}
