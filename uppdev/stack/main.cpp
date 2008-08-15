#include <CtrlLib/CtrlLib.h>

struct App : public TopWindow
{
	TreeCtrl tree;
	ArrayCtrl array;
	Splitter splitter;

	App()
	{
		splitter.Horz(tree, array);
		Add(splitter);
		splitter.SetPos(1500); // << SetPos seems to be guilty of the crashes.
		Sizeable().Zoomable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
