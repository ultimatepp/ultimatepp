#ifndef _WindowBackupRestore_WindowBackupRestore_h
#define _WindowBackupRestore_WindowBackupRestore_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <WindowBackupRestore/WindowBackupRestore.lay>
#include <CtrlCore/lay.h>



class MainWindow : public WithMainWindowLayout<TopWindow> {
public:
	typedef MainWindow CLASSNAME;
	MainWindow();
protected:
	WithTestDialogLayout<TopWindow> dlg;
	void OnShowDialog();
};

#endif

