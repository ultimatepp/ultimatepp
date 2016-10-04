#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Test : public TopWindow {
public:
	typedef Test CLASSNAME;
	Test();
	
private:
	TreeCtrl   		structure;
	ArrayCtrl		info;
};

Test::Test()
{
	Title("Test");
	TreeCtrl::Node node(info);
	node.SetSize(Size(300, 50));
	// node.margin = 20;
		
	info.AddColumn("Property");
	info.AddColumn("Value");
	info.AddColumn("Meaning");
	info.SizePos();
	
	int parent_node = 0;
	structure.SetRoot(TreeCtrl::Node("ROOT"));
	parent_node = structure.Add(0, node);

	Add(structure.SizePos());
	Sizeable().Zoomable();
	SetRect(100, 100, 370, 100);

}

GUI_APP_MAIN
{
	Test().Run();
}

