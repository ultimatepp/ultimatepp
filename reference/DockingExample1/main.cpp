#include <Docking/Docking.h>
using namespace Upp;
/*
** This is very simple example designed to show the basics of setting up a 
**  docking window without many of the compicated features
*/

/*
** Class declaration. Notice that we inherit from DockWindow (not TopWindow)
*/
class DockingExample : public DockWindow {
public:
	typedef DockingExample CLASSNAME;
	DockingExample();
	
private:
	ArrayCtrl arrayctrl1, arrayctrl2;
	TreeCtrl treectrl1, treectrl2;
	Button button;

	// Our initialization function
	virtual void DockInit();

	// Functions to put data in our ctrls
	void FillArray(ArrayCtrl &array);
	void FillTree(TreeCtrl &tree);
};

/*
** Source
*/
DockingExample::DockingExample()
{
	Title("DockingExample1 : Simple Docking");
	Sizeable().MaximizeBox();
		
	// This a button so that we can open the Dock Manager
	Add(button.SetLabel("Manager").LeftPosZ(4, 100).TopPosZ(4, 23));
	button << [=] { DockManager(); };
	
	// Now we just put some bogus data in our controls
	FillArray(arrayctrl1);
	FillTree(treectrl1);
	FillArray(arrayctrl2);
	FillTree(treectrl2);
}

void DockingExample::DockInit()
/* This is the docking initialisation function. It gets called after the window is opened,
**  and it's here you should do your docking/serialization, if you do it in the contructor
**  you get various problems (incorrect layout, floating windows not opening).
*/ 
{
	// Here we add our dockable controls in the simplest way possible
	// It's usually best to set a minimum size as Upp ctrls aren't very good 
	//  at guessing by themselves
	DockLeft(Dockable(arrayctrl1, "ArrayCtrl 1").SizeHint(Size(300, 200)));
	DockLeft(Dockable(treectrl1, "TreeCtrl 1").SizeHint(Size(300, 200)));
	DockTop(Dockable(arrayctrl2, "ArrayCtrl 2").SizeHint(Size(300, 200)));
	DockRight(Dockable(treectrl2, "TreeCtrl 2").SizeHint(Size(300, 200)));
	/*
	** Not bad for four lines of code.
	** 
	** Some notes about the above:
	**	Before a control can be dockable it must be added to a DockableCtrl.
	**  Passing a ctrl to Dockable() does this for us and returns the DockableCtrl so
	**		so that we can set size hints etc. If you wanted to use a Upp Layout you 
	**		could declare it as WithMyLayout<DockableCtrl> and use that instead.
	**	We then pass this DockableCtrl to DockLeft/DockTop (or right/bottom) to add 
	**   it to the window
	*/	
	
}

/*
** Everything after here is fluff
*/
void DockingExample::FillArray(ArrayCtrl &array)
{
	array.AddColumn("Number");
	array.AddColumn("Roman numbers");
	array.MultiSelect();
	for(int i = 0; i < 200; i++)
		array.Add(i, FormatIntRoman(i, true));
}

void DockingExample::FillTree(TreeCtrl &tree) 
{
	Vector<int> parent, parent2;
	parent.Add(0);
	tree.SetRoot(Image(), "The Tree");
	for(int i = 1; i < 10000; i++) {
		parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
		            FormatIntRoman(i, true)));
		if((rand() & 3) == 0)
			tree.Open(parent.Top());
	}
	tree.Open(0);					
}

GUI_APP_MAIN
{
	DockingExample().Run();
}

