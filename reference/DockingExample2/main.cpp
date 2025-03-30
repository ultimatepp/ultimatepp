#include <Docking/Docking.h>
using namespace Upp;
/*
** This is an advanced example designed to show most of the available Docking features, and how
**  might be used in a real world application.
**
** Features used:
**	Serailization
**  Including a default layout as binary data in the exe.
**  Layout templates with DockableCtrls
**	Dock Manager (Group/layout management)
**  Standard Docking Menu
**  Permitted/denied docking alignments per window
*/

#define LAYOUTFILE <DockingExample2/DockingExample.lay>
#include <CtrlCore/lay.h>

#define TOPICFILE <DockingExample2/app.tpp/all.i>
#include <Core/topic_group.h>

// This sets up our backup serialization data (see Upp documentation for how .brc files work)
#include <DockingExample2/DefaultLayout.brc>

/*
** Class declaration. Notice that we inherit from DockWindow (not TopWindow)
*/
class DockingExample : public DockWindow {
public:
	typedef DockingExample CLASSNAME;
	DockingExample();

	virtual void Close();	
private:
	// Some DockableCtrls using Layout template
	WithDockingToolsLayout<DockableCtrl> 	toolwindow;
	WithDockingOptionsLayout<DockableCtrl> 	optionswindow;
	// Some data ctrls (will be added with Dockable())
	ArrayCtrl 	arrayctrl1, arrayctrl2;
	TreeCtrl 	treectrl1, 	treectrl2;
	Button		button;
	// A menu bar to hold our windows 
	MenuBar menu;
	
	
	// Our initialization function
	virtual void DockInit();

	// MenuBar callback function
	void MainMenu(Bar &bar);
	// Option callback handler
	void OnDockAllow(int align, Option *ctrl);
	// Button callback handler
	void OnUserGuide();

	// Functions to put data in our ctrls (fluff)
	void FillArray(ArrayCtrl &array);
	void FillTree(TreeCtrl &tree);
};

/*
** Source
*/
DockingExample::DockingExample()
{
	Title("DockingExample2 : Advanced Docking").Sizeable(true).Zoomable(true);
	
	// Layout out windows
	CtrlLayout(toolwindow, 		"Tools");
	CtrlLayout(optionswindow, 	"Options");
	
	// Add our MenuBar using the standard Docking menu
	AddFrame(menu); // menu must be added before Set or it thinks it's a context menu
	menu.Set(THISFN(MainMenu));
	
	// Set Option callbacks. The option ctrls will permit/deny docking in different directions
	//  for optionswindow only. The (int) is for the benefit of GCC
	// You can also do this for the whole window (see DockWindow::AllowDockLeft() etc)
	optionswindow.dockleft   << [=] { OnDockAllow((int)DOCK_LEFT, 	&optionswindow.dockleft); };
	optionswindow.docktop    << [=] { OnDockAllow((int)DOCK_TOP, 		&optionswindow.docktop); };
	optionswindow.dockright  << [=] { OnDockAllow((int)DOCK_RIGHT, 	&optionswindow.dockright); };
	optionswindow.dockbottom << [=] { OnDockAllow((int)DOCK_BOTTOM,	&optionswindow.dockbottom); };
	
	// Now we put some bogus data in our controls
	FillArray(arrayctrl1);
	FillTree(treectrl1);
	FillArray(arrayctrl2);
	FillTree(treectrl2);
	// And add the User-Guide button
	Add(button.SetLabel("User Guide").LeftPosZ(4, 100).TopPosZ(4, 23));
	button << [=] { OnUserGuide(); };
}

void DockingExample::DockInit()
/* This is the docking initialisation function. It gets called after the window is opened,
**  and it's here you should do your docking/serialization, if you do it in the contructor
**  you get various problems (incorrect layout, floating windows not opening).
*/ 
{
	// Because we are going to use Serialization and will always have a Layout available we
	//	don't need to Dock/Float windows manually. Instead we just Register them. 
	// First our windows. These don't need size hints as they were set already by CtrlLayout
	Register(toolwindow.Icon(CtrlImg::copy()));
	Register(optionswindow.Icon(CtrlImg::paste()));
	// And now our data ctrls
	Register(Dockable(arrayctrl1, "ArrayCtrl 1").SizeHint(Size(300, 200)));
	Register(Dockable(treectrl1,  "TreeCtrl 1").SizeHint(Size(300, 200)));
	Register(Dockable(arrayctrl2, "ArrayCtrl 2").SizeHint(Size(300, 200)));
	Register(Dockable(treectrl2,  "TreeCtrl 2").SizeHint(Size(300, 200)));
	// Now we're ready to Serialize. The first time you run this example there won't be a data 
	//  file, but I have included serialized copy in the .EXE to use as Default
	// Note that we use SerializeWindow, not Serialize.
	FileIn in(GetDataFile("docklayout.dat"));
	bool badfile = !in.IsOpen();
	if (!badfile) {
		SerializeWindow(in);
		badfile = in.IsError();
	}
	// If the file didn't open or was corrupt, use default layout
	if (badfile) {
		MemStream in(default_layout_bin, default_layout_bin_length);
		SerializeWindow(in);
	}
}

void DockingExample::Close()
{
	// Save our current layout & settings
	FileOut out(GetDataFile("docklayout.dat"));
	if (out.IsOpen())
		SerializeWindow(out);
	TopWindow::Close();
}

void DockingExample::MainMenu(Bar &bar)
{
	// Add a Windows menu.
	// We're using a the standard DockWindow menu, though you can also roll your own.
	bar.Sub("Windows", Image(), [=](Bar& bar) { DockWindowMenu(bar); });	
}

void DockingExample::OnDockAllow(int align, Option *ctrl)
// Permit/deny docking in different directions
//  for optionswindow only.
// You can also do this for the whole window (see DockWindow::AllowDockLeft() etc)
{
	optionswindow.AllowDock(align, !(bool)~(*ctrl));
}




/*
** Everything after here is fluff
*/
void DockingExample::OnUserGuide()
{
	HelpWindow help;
	help.GoTo("topic://DockingExample2/app/userguide$en-us");
	help.Run();
}

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

