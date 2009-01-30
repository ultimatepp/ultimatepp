#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	const Vector<String>& cmdline = CommandLine();
	SetDefaultCharset(CHARSET_WIN1250);
	if(cmdline.IsEmpty())
	{
		Exclamation("[* UPDATER] should be run from another applications");
		return;
	}
	String name = cmdline[0];
	UpdateFile(name);
	String exec = GetExeDirFile(name);
	for(int i = 1; i < cmdline.GetCount(); i++)
		if(cmdline[i].Find(' ') >= 0)
			exec << " \"" << cmdline[i] << "\"";
		else
			exec << " " << cmdline[i];
	WinExec(exec, SW_SHOWNORMAL);
}
