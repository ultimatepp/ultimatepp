#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TestSplitter : public TopWindow
{
	private:
		// pages column list
		ColumnList pageList;
		
		// page container
		StaticRect pagePane;

		// the splitter
		Splitter splitter;
		
	public:
		TestSplitter();
};

TestSplitter::TestSplitter()
{
	// adds the splitter
//	SizePos();
	Add(splitter);
	
	// setup pagelist appearance
	pageList.Columns(1).SizePos();
	
	// adds the columnlist on left
	splitter.Horz(pageList, pagePane).SetPos(100);
}
	
GUI_APP_MAIN
{
	Ctrl::GlobalBackPaint();
	TestSplitter().Run();
}
