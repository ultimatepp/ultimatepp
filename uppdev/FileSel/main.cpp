#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);
	FileSel fs;
	String fn;
	fs.AllFilesType();
	fs.Type("jpeg", "*.jpg");
	fs.ExecuteOpen();
	return;
	
	for(;0;) {
		if(!fs.ExecuteSaveAs())
			break;
		PromptOK(DeQtf(~fs));
	}
	fs.Multi();
	for(;;) {
		if(!fs.ExecuteOpen())
			break;
		String h;
		for(int i = 0; i < fs.GetCount(); i++) {
			if(i)
				h << '&';
			h << fs[i];
		}
		PromptOK(h);
	}
	
}
