#include "Layouts.h"

#define LAYOUTFILE <Layouts/layoutfile2.lay> // Layouts will be local to main.cpp
#include <CtrlCore/lay.h>

FirstTabDlg::FirstTabDlg() // Constructor needs to be in .cpp
{
	CtrlLayout(*this); // Places widgets into positions
}

struct SecondTabDlg : WithSecondTabLayout<ParentCtrl> { // Local to main.cpp
	typedef SecondTabDlg CLASSNAME;
	SecondTabDlg();
};

SecondTabDlg::SecondTabDlg()
{
	CtrlLayout(*this);
}

struct MainDlg : WithMainLayout<TopWindow> { // Local to main.cpp
	typedef MainDlg CLASSNAME;
	
	FirstTabDlg  tab1;
	SecondTabDlg tab2;
	
	void DoDialog();
	
	MainDlg();
};

void MainDlg::DoDialog()
{
	WithDialogLayout<TopWindow> dlg; // Variant without class, good for simple modal dialogs
	CtrlLayoutOK(dlg, "Dialog"); // OK means dialog has normal 'ok' button that needs setting up
	dlg.info = String().Cat() << "Today is: " << GetSysDate();
	if(dlg.Execute() != IDOK)
		return;
}

MainDlg::MainDlg()
{
	CtrlLayout(*this, "Layouts");
	tabs.Add(tab1.SizePos(), "Tab1"); // SizePos() means dialog fills the whole are of tab
	tabs.Add(tab2.SizePos(), "Tab2");

	tab1.dialog << [=] { DoDialog(); }; // When pushing 'dialog' button, DoDialog is invoked
}

GUI_APP_MAIN
{
	MainDlg().Run();
}
