#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Button         back;
	FileList       list;
	Array<NetNode> netstack;
	Array<NetNode> netnode;
	
	void LoadNet();
	void Network();
	void Select();
	void Back();
	
	typedef App CLASSNAME;
	
	App();
};

void App::Back()
{
	if(stack.GetCount())
		stack.Drop();
	if(stack.GetCount()) {
		netnode = stack.Top().Enum();
		LoadNet();
	}
	else {
		netnode = NetNode::EnumRoot();
		LoadNet();
	}
}

App::App()
{
	Add(list.SizePos());
	back.SetLabel("..");
	Add(back.RightPos(30, 30).TopPos(30, 30));
	list.WhenLeftDouble = THISBACK(Select);
	back <<= THISBACK(Back);
}

GUI_APP_MAIN
{
	App app;
	app.Network();
	app.Run();
}

