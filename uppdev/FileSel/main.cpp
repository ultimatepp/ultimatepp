#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DUMP(StdFont().Info().GetHeight());
	DUMP(StdFont().Info().GetDescent());
	DUMP(StdFont().Bold().Info().GetHeight());
	DUMP(StdFont().Bold().Info().GetDescent());
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
	for(;;) {
		if(!fs.ExecuteSaveAs())
			break;
		PromptOK(DeQtf(~fs));
	}
}
