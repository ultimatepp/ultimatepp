#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	PromptOK(DeQtf(SelectFileOpen("Text files\t*.txt\nAll files\t*.*")));

	PromptOK(DeQtf(SelectFileSaveAs("Image files\t*.png *.jpg\nAll files\t*.*")));
	
	SelectFileIn in("Text files\t*.txt");
	if(in) {
		int n = 0;
		while(!in.IsEof()) {
			in.GetLine();
			n++;
		}
		PromptOK("There is " + AsString(n) + " lines in the file.");
	}
	
	PromptOK("[{1} \1" + SelectLoadFile("Text files\t*.txt"));
}
