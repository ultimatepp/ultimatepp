#ifndef _MySupport_MyPrompt_h_
#define _MySupport_MyPrompt_h_

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
//#include <Topic/Topic.h>

struct PromptDlgWnd__ : TopWindow {
	virtual bool HotKey(dword key) {
		if(TopWindow::HotKey(key))
			return true;
		if(IsAlpha(key))
			return TopWindow::HotKey(K_ALT_A + ToUpper((int)key) - 'A');
		return false;
	}
};

int Prompt(const char *title, const Image& icon, const char *qtf, Callback HelpDlg, bool okcancel,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL, const char *help = NULL,
		   		 int cx = 0);

int Prompt(const char *title, const Image& icon, const char *qtf, Callback HelpDlg,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL, const char *help = NULL,
		       int cx = 0);

void PromptOK(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptOKCancel(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptYesNo(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptYesNoCancel(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptRetryCancel(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptAbortRetry(const char *qtf, Callback HelpDlg, const char *help = NULL);
int  PromptAbortRetryIgnore(const char *qtf, Callback HelpDlg, const char *help = NULL);
void Exclamation(const char *qtf, Callback HelpDlg, const char *help = NULL);

#endif
