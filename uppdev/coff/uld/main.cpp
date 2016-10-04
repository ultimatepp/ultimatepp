#include "uld.h"
#pragma hdrstop
#include "obj.h"

void TryMain()
{
//	PutConsole("ULD::TryMain");
	int start = msecs();
	LinkJob linkjob;
	String cfgfile = LoadFile(ForceExt(GetExeFilePath(), ".cfg"));

#ifndef flagTEST
	if(CommandLine().IsEmpty())
	{
		PutStdOut(LinkJob::Usage());
		return;
	}

	linkjob.ReadCommand(cfgfile, false);
	linkjob.ReadCommand(CommandLine(), true);
	linkjob.Link();
#else
//*
	linkjob.ReadCommand(
//	"-dump:J:\\temp\\uppnew\\console\\uld\\MINGW-CONSOLE-DEBUG-DEBUG_FULL-GCC-MAIN-SO-ST-WIN32\\uld.exe "
//	"-dump:J:\\temp\\uppnew\\console\\uld\\VC71-CONSOLE-DEBUG-DEBUG_FULL-MAIN-MSC71-SO-ST-WIN32\\uld.exe"
//	"-dump:J:\\temp\\uppnew\\console\\uld\\MINGW-CONSOLE-DEBUG-DEBUG_FULL-GCC-MAIN-SO-ST-WIN32\\uld.exe"
//	"-dump:j:\\temp\\uuld.exe"

"-nologo "
//"-map "
//"-dumpcollect "
"-dumpresource "
//"-dumpsec:all "
"-machine:I386 "
"-pdb:j:\\temp\\ideuld.pdb "
"-out:j:\\temp\\ideuld.exe "
"-incremental:yes "
"-debug "
"-OPT:NOREF "
"-subsystem:windows "
"-LIBPATH:F:\\Dev71\\Vc7\\lib "
"-LIBPATH:F:\\Dev71\\Vc7\\PlatformSDK\\Lib "
"-LIBPATH:f:\\w\\ssl\\openssl-0.9.7b\\out32 "
"-LIBPATH:f:\\soft\\mysql\\lib\\opt "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/UppDlg.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Template.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/languages.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/FindInFiles.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Console.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/ide.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/idefile.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Setup.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/SetupKeys.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Custom.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/idetool.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/idebar.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Uninstall.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/idewin.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/About.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Util.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Methods.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/OutputMode.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Host.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Build.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/Debug.obj "
"j:/temp/uppnew/ide/VC71.Debug.Debug_full.Gui.Main.Msc71.St.Uld.Win32/ide$rc.obj "
"j:\\temp\\uppnew\\ide\\Common\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Common.lib "
"j:\\temp\\uppnew\\ide\\Editor\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Editor.lib "
"j:\\temp\\uppnew\\ide\\LayDes\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\LayDes.lib "
"j:\\temp\\uppnew\\ide\\IconDes\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\IconDes.lib "
"j:\\temp\\uppnew\\ide\\Builders\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Builders.lib "
"j:\\temp\\uppnew\\ide\\Debuggers\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Debuggers.lib "
"j:\\temp\\uppnew\\ide\\Browser\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Browser.lib "
"j:\\temp\\uppnew\\Topic\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Topic.lib "
"j:\\temp\\uppnew\\CtrlLib\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\CtrlLib.lib "
"wsock32.lib "
"j:\\temp\\uppnew\\Web\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Web.lib "
"j:\\temp\\uppnew\\plugin\\bz2\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\bz2.lib "
"j:\\temp\\uppnew\\Esc\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Esc.lib "
"advapi32.lib "
"winmm.lib "
"shell32.lib "
"j:\\temp\\uppnew\\Core\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Core.lib "
"j:\\temp\\uppnew\\RichEdit\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\RichEdit.lib "
"j:\\temp\\uppnew\\RichText\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\RichText.lib "
"j:\\temp\\uppnew\\Image\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Image.lib "
"j:\\temp\\uppnew\\plugin\\png\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\png.lib "
"user32.lib "
"gdi32.lib "
"j:\\temp\\uppnew\\Draw\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\Draw.lib "
"j:\\temp\\uppnew\\plugin\\z\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\z.lib "
"j:\\temp\\uppnew\\plugin\\gif\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\gif.lib "
"j:\\temp\\uppnew\\plugin\\tif\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\tif.lib "
"j:\\temp\\uppnew\\plugin\\jpg\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\jpg.lib "
"j:\\temp\\uppnew\\coff\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\coff.lib "
"j:\\temp\\uppnew\\docpp\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\docpp.lib "
"advapi32.lib "
"comdlg32.lib "
"comctl32.lib "
"j:\\temp\\uppnew\\CtrlCore\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\CtrlCore.lib "
"j:\\temp\\uppnew\\DocTypes\\VC71.Debug.Debug_full.Gui.Msc71.St.Win32\\DocTypes.lib "
"shell32.lib "

	, false);
	linkjob.Link();
#endif
}

CONSOLE_APP_MAIN
{
/*
	{
		RTIMING("VectorMap");
		VectorMap<int, int> temp;
		for(int i = 0; i < 1000000; i++)
			temp.Add(i, -1);
	}
*/

//	static void *hovno;
//	__asm int 3
//	__asm jmp [hovno]

//	PutConsole("ULD::Main");

	try
	{
		TryMain();
	}
	catch(Exc e)
	{
		PutStdOut(e);
		SetExitCode(1);
	}
}
