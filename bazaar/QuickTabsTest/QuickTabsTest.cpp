#include "QuickTabsTest.h"

Image IdeFileImage(const String& t, bool a)
{
	return Null;
}
  
App::App()
{
	t_tabs.SetTop();
/*	l_tabs.SetLeft();
	r_tabs.SetRight();
	b_tabs.SetBottom();
*/	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();
	
//	AddFrame(t_tabs);
/*	AddFrame(r_tabs);
	AddFrame(l_tabs);
	AddFrame(b_tabs);
*/	AddFrame(ViewFrame());
/*	
	l_tabs.Crosses(true);	
	l_tabs.Add("/ala/Test.cpp");
	l_tabs.Add("/ala/Test.h");
	l_tabs.Add("/ala/SuperProgram.cpp", true);
	l_tabs.Add("/kasia/SuperProgram.h");
	l_tabs.Add("/kasia/Synchronize.cpp");
	l_tabs.Add("/kasia/Synchronize.h");
	l_tabs.Add("/test/Test.cpp");
	l_tabs.Add("/test/Test.h");
	l_tabs.Grouping(true);

	r_tabs.Crosses(true);	
	r_tabs.Add("/ala/Test.cpp");
	r_tabs.Add("/ala/Test.h");
	r_tabs.Add("/ala/SuperProgram.cpp", true);
	r_tabs.Add("/kasia/SuperProgram.h");
	r_tabs.Add("/kasia/Synchronize.cpp");
	r_tabs.Add("/kasia/Synchronize.h");
	r_tabs.Add("/test/Test.cpp");
	r_tabs.Add("/test/Test.h");
	r_tabs.Grouping(true);

	b_tabs.Crosses(true);	
	b_tabs.Add("/ala/Test.cpp");
	b_tabs.Add("/ala/Test.h");
	b_tabs.Add("/ala/SuperProgram.cpp", true);
	b_tabs.Add("/kasia/SuperProgram.h");
	b_tabs.Add("/kasia/Synchronize.cpp");
	b_tabs.Add("/kasia/Synchronize.h");
	b_tabs.Add("/test/Test.cpp");
	b_tabs.Add("/test/Test.h");
	b_tabs.Grouping(true);
*/
	AddFrame(t_tabs);
	t_tabs.Stacking(true);
	t_tabs.Crosses(true);	
	t_tabs.Add("/ala/Test.cpp");
	t_tabs.Add("/ala/SuperProgram.cpp", true);
	t_tabs.Add("/ala/Test.h");
	t_tabs.Add("/ala/SuperProgram.h");
	t_tabs.Add("/MyApp/MyApp.cpp");
	t_tabs.Add("/MyApp/MyApp.h");
	t_tabs.Add("/MyApp/MyApp.lay");
	t_tabs.Add("/MyApp/MyApp.iml");
	t_tabs.Add("/MyApp/main.cpp");	
	t_tabs.Add("/kasia/Synchronize.cpp");
	t_tabs.Add("/kasia/Synchronize.h");
	t_tabs.Add("/CtrlLib/LabelBase.h");
	t_tabs.Add("/CtrlLib/LabelBase.cpp");
	t_tabs.Add("/CtrlLib/ScrollBar.h");
	t_tabs.Add("/CtrlLib/ScrollBar.cpp");	

	t_tabs.Grouping(true);
	t_tabs <<= THISBACK(OnFileCursor);
	
	stacking <<= t_tabs.IsStacking();
	stacking <<= THISBACK(OnStacking);
}

void App::OnFileCursor()
{
	file <<= ~t_tabs;
}

void App::OnStacking()
{
	t_tabs.Stacking(stacking);
}

void App::GoLastVis()
{
//	const Vector<Tab>& t = tabs.GetTabs();
//	Size sz = GetSize();
//	int x = 0;//tabs.GetSbPos();
//	int current = 0;
//	for(int i = 0; i < t.GetCount(); i++)
//	{
//		if(x < sz.cx && x + t[i].cx > sz.cx)
//		{
//			current = i;
//			break;
//		}
//		x += t[i].cx;
//	}
//	
//	tabs.SetCursor(current);
}


GUI_APP_MAIN
{
	App().Run();
}

