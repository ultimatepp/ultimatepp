#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);
	FileSel fs;
	String fn;
	fs.AllFilesType()
	  .Type( t_("archive FIDE (txt)"), "*.txt") //type 1
	  .Type( t_("archive VEG (csv)"), "*.csv;*.veg") //type 2
	  .Type( t_("national archive with Fixed Length (txt)"), "*.txt")   //type 3
	  .Type( t_("archive FSI Italy (csv)"), "*.csv");  //type 4
	fs.DefaultExt("hhh");
	
//	fs.NoMkDirOption();
	fs.Multi();
//	fs.ActiveDir("/usr/include");
//	fs.NoEditFileName();
	fs.ActiveDir("c:/");
	fs.PreSelect("U:/log.txt");
	fs.ActiveType(3);
	for(;;) {
		if(!fs.ExecuteSaveAs())
			break;
		PromptOK(DeQtf(~fs));
	}
}
