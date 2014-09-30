#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	String text = "Original text";
	if(EditText(text, "Please edit a text", "Text"))
		PromptOK(text);

	String text2;
	if(EditTextNotNull(text2, "Please enter non-empty text", "Text"))
		PromptOK(text2);

	int number = 10;
	if(EditNumber(number, "Please enter a number 1..100", "Number", 1, 100, true))
		PromptOK(AsString(number));
	
	Date dt = GetSysDate();
	if(EditDateDlg(dt, "Please enter a date", "Date"))
		PromptOK(AsString(dt));
}

