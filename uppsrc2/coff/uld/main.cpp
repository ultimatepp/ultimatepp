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

"--subsystem "
"windows "
"-Bstatic "
"-o "
"C:\\upp0802\\out\\MINGW.Gui\\HelloWorld.exe "
"c:/upp0802/mingw/bin/../lib/gcc/mingw32/4.3.0/../../../crt2.o "
"c:/upp0802/mingw/bin/../lib/gcc/mingw32/4.3.0/crtbegin.o "
"-LC:\\upp0802\\mingw\\lib "
"-Lc:/upp0802/mingw/bin/../lib/gcc/mingw32/4.3.0 "
"-Lc:/upp0802/mingw/bin/../lib/gcc "
"-Lc:/upp0802/mingw/bin/../lib/gcc/mingw32/4.3.0/../../.. "
"-s "
"-O "
"2 "
"C:/upp0802/out/HelloWorld/MINGW.Gui.Main\\hello.o "
"C:/upp0802/out/CtrlLib/MINGW.Gui\\CtrlLib.o "
"C:/upp0802/out/CtrlCore/MINGW.Gui\\CtrlCore.o "
"C:/upp0802/out/RichText/MINGW.Gui\\RichImage.o "
"C:/upp0802/out/plugin/bmp/MINGW.Gui\\BmpReg.o "
"C:/upp0802/out/plugin/png/MINGW.Gui\\pngreg.o "
"--start-group "
"C:/upp0802/out/CtrlLib/MINGW.Gui\\CtrlLib.a "
"-ladvapi32 "
"-lcomdlg32 "
"-lcomctl32 "
"C:/upp0802/out/CtrlCore/MINGW.Gui\\CtrlCore.a "
"C:/upp0802/out/RichText/MINGW.Gui\\RichText.a "
"-luser32 "
"-lgdi32 "
"C:/upp0802/out/Draw/MINGW.Gui\\Draw.a "
"C:/upp0802/out/plugin/bmp/MINGW.Gui\\bmp.a "
"-ladvapi32 "
"-lshell32 "
"-lwinmm "
"-lmpr "
"-lole32 "
"-loleaut32 "
"-luuid "
"C:/upp0802/out/Core/MINGW.Gui\\Core.a "
"C:/upp0802/out/plugin/z/MINGW.Gui\\z.a "
"C:/upp0802/out/plugin/png/MINGW.Gui\\png.a "
"--end-group "
"-lstdc++ "
"-lmingw32 "
"-lgcc "
"-lmoldname "
"-lmingwex "
"-lmsvcrt "
"-lgdi32 "
"-lcomdlg32 "
"-luser32 "
"-lkernel32 "
"-ladvapi32 "
"-lshell32 "
"-lmingw32 "
"-lgcc "
"-lmoldname "
"-lmingwex "
"-lmsvcrt "
"c:/upp0802/mingw/bin/../lib/gcc/mingw32/4.3.0/crtend.o "

	, true);
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
