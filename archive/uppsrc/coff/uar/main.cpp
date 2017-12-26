#include "uar.h"
#pragma hdrstop

#include "lib.h"

static void TryMain()
{
#ifndef flagTEST
	if(CommandLine().IsEmpty())
	{
		PutStdOut(UARArchiveJob::Usage());
		return;
	}
#endif

	UARArchiveJob archjob;
	archjob.ReadCommand(
#ifdef flagTEST

"-sr "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\CtrlLib.a\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\LabelBase.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Button.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Switch.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\EditField.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Text.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\LineEdit.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\DocEdit.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ScrollBar.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\HeaderCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ArrayCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\MultiButton.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\PopupTable.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\DropList.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\DropChoice.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Static.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Splitter.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\FrameSplitter.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\SliderCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ColumnList.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Progress.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\AKeys.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\RichTextView.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Prompt.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Help.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\DateTimeCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Bar.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\MenuItem.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\MenuBar.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ToolButton.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ToolBar.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ToolTip.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\StatusBar.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\TabCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\TreeCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\DlgColor.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ColorPopup.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ColorPusher.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\FileList.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\FileSel.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\PrinterJob.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Windows.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Win32.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\TrayIconWin32.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\TrayIconX11.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\CtrlUtil.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Update.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\LNGCtrl.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\Ch.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ChWin32.o\" "
"\"C:/upp0802/out/CtrlLib/MINGW.Gui\\ChGtk.o\" "

#else
	CommandLine()
#endif
	);
	archjob.Build();
}

CONSOLE_APP_MAIN
{
	try
	{
		TryMain();
	}
	catch(Exc e)
	{
		puts(e);
		SetExitCode(1);
	}
}
