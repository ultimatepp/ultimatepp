#include "PyShell.h"

// Arkon, 09 July 2004 RageStorm
// http://ragestorm.net
// Feel free to do with this whatever you want!
// Thanks goes to Sagiv Malihi for this great idea and help.
//http://www.ragestorm.net/tutorial?id=21
//http://www.ragestorm.net/sample?id=79

void PyShell::Proc()
{
	/*
	The following code is what we run below, it will open PyShell using TkInter.
	it runs blocking
	*/
	AtomicInc(a);
	int ret = PyRun_SimpleString(
		"from Tkinter import Tk\n"
		"from idlelib.PyShell import PyShell, PyShellFileList, fixwordbreaks\n"
		"import idlelib.PyShell\n"
		"\n"
		"idlelib.PyShell.use_subprocess = False\n"
		"root = Tk()\n"
		"fixwordbreaks(root)\n"
		"flist = PyShellFileList(root)\n"
		"flist.pyshell = PyShell(flist)\n"
		"\n"
		"root.withdraw()\n"
		"flist.pyshell.begin()\n"
		"root.mainloop()\n"
		"root.destroy()\n"
	);
	AtomicDec(a);
}
