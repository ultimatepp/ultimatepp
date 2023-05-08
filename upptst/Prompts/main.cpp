#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	PromptOK("Test");
	PromptOKCancel("Test");
	PromptOKCancelAll("Test");
	PromptYesNo("Test");
	PromptYesNoCancel("Test");
	PromptYesNoAll("Test");
	PromptRetryCancel("Test");
	PromptAbortRetry("Test");
	PromptAbortRetryIgnore("Test");
	PromptSaveDontSaveCancel("Test");
	
	Exclamation("Test");
	
	ErrorOK("Test");
	ErrorOKCancel("Test");
	ErrorYesNo("Test");
	ErrorYesNoCancel("Test");
	ErrorYesNoAll("Test");
	ErrorRetryCancel("Test");
	ErrorAbortRetry("Test");
	ErrorAbortRetryIgnore("Test");
}
