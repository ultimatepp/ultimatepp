#include <CtrlLib/CtrlLib.h>
#include <TabBar/TabBar.h>

using namespace Upp;

struct TabBarTest : TopWindow
{
	TabBar l_tabs;
	TabBar r_tabs;
	TabBar t_tabs;
	TabBar b_tabs;
	
	typedef TabBarTest CLASSNAME;
	
	bool Stack(Value a, Value b)
	{
		String as = a;
		String bs = b;
		as = as.Left(as.Find('.'));
		bs = bs.Left(bs.Find('.'));
		return as == bs;
	}
	
	TabBarTest()
	{
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
		
		t_tabs.Crosses(true);	
		t_tabs.Add("Test.cpp");
		t_tabs.Add("Test.h");
		t_tabs.Add("/ala/SuperProgram.cpp", true);
		t_tabs.Add("/kasia/SuperProgram.h");
		t_tabs.Add("/kasia/Synchronize.cpp");
		t_tabs.Add("/kasia/Synchronize.h");
		t_tabs.Add("/test/Test.cpp");
		t_tabs.Add("/test/Test.h");
		t_tabs.StackingFunc(THISBACK(Stack));
		//t_tabs.Grouping(true);
		//t_tabs.Stacking(true);
		
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
		
		AddFrame(t_tabs);
		AddFrame(l_tabs);
		AddFrame(r_tabs);
		AddFrame(b_tabs);

		l_tabs.SetLeft();
		l_tabs.AutoScrollHide();
		r_tabs.SetRight();
		r_tabs.AutoScrollHide(); //FIXME: Doesn't work
		b_tabs.SetBottom();
		b_tabs.AutoScrollHide(); //FIXME: If not called bottom scrollbar is not shown

		Sizeable().Zoomable();
	}
	
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White);
	}
};

GUI_APP_MAIN
{
	TabBarTest().Run();
}