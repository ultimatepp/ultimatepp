#include "WindowBackupRestore.h"



MainWindow::MainWindow()
{
	CtrlLayout(*this, "Backup/Restore test");
	show_dialog <<= THISBACK(OnShowDialog);
	
	CtrlLayoutOKCancel(dlg, t_("Test Dialog"));
}

void MainWindow::OnShowDialog()
{
	dlg.Backup();
	if(dlg.Execute()!=IDOK)
//		dlg.Restore();
		;
}


GUI_APP_MAIN
{
	MainWindow().Run();
}

