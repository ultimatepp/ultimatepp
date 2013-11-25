#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);
	FileSel fs;
	String fn;
	fs.AllFilesType();
	for(;;) {
		if(!fs.ExecuteSaveAs())
			break;
		PromptOK(DeQtf(~fs));
	}
}
