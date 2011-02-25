#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SplitterFrame1/StandardLayout.lay>
#include <CtrlCore/lay.h>

struct MyApp : WithStandardLayout<TopWindow> {
	SplitterFrame sf;
	ArrayCtrl list,list1;
	typedef MyApp CLASSNAME;
	TopWindow app;

	MyApp() {
		CtrlLayoutOKCancel(*this, "Test SplitterFrame");
		AddFrame(sf.Left(list, 200));
		list.AddColumn("List");
		for(int i = 0; i < 100; i++)
			list.Add(FormatIntRoman(i, true));
		AddFrame(InsetFrame());
		sf.SizeMin(200).MinSize(100);
		Sizeable();		
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
