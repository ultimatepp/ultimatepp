#include "FileSel.h"

GUI_APP_MAIN
{
	DDUMP("Test");
	SetDefaultCharset(CHARSET_UTF8);
	FileSel fs;
	String fn;
	fs.Type( t_("archive FIDE (txt)"), "*.txt") //type 1
	  .Type( t_("archive VEG (csv)"), "*.csv;*.veg") //type 2
	  .Type( t_("national archive with Fixed Length (txt)"), "*.txt")   //type 3
	  .Type( t_("archive FSI Italy (csv)"), "*.csv");  //type 4
	fs.NoMkDirOption();
	fs.Multi();
//	fs.NoEditFileName();
	for(;;) {
		if(!fs.ExecuteOpen())
			break;
		PromptOK(~fs);
	}
}

