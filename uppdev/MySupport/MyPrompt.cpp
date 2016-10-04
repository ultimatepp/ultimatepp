#include "support.h"

int Prompt(const char *title, const Image& iconbmp, const char *qtf, Callback HelpDlg, bool okcancel,
           const char *button1, const char *button2, const char *button3, const char *help,
		       int cx)
{
	int fcy = Draw::GetStdFontCy();
	PromptDlgWnd__ dlg;
	RichTextCtrl qtfctrl;
	Icon         icon;
	//qtfctrl.WhenLink = WhenLink;
	icon.SetImage(iconbmp);
	Button b1, b2, b3, bh;
	promptTopic = help;
	
	qtfctrl.SetQTF(String("[G1 ") + qtf, GetRichTextStdScreenZoom());	
	int bcy = 2 * fcy;
	int bcx = 6 * fcy;
	Size bsz = icon.GetStdSize();
	if(cx == 0) {
		cx = qtfctrl.GetWidth();
		if(!cx)
			cx = 350;
		cx += 2 * fcy;
		if(bsz.cx)
			cx += bsz.cx + fcy;
	}
	int nbtn = !!button1 + !!button2 + !!button3 + !!help;
	cx = min(550, max(nbtn * bcx + (1 + nbtn) * fcy, cx));
	int qcx = cx - 2 * fcy;
	if(bsz.cx)
		qcx -= bsz.cx + fcy;
	int ccy = qtfctrl.GetHeight(qcx);
	int qcy = min(400, ccy);
	if(qcy <= ccy) {
		qcx += ScrollBarSize() + fcy;
		cx += ScrollBarSize();
	}
	int mcy = max(qcy, bsz.cy);
	int cy = mcy + 4 * fcy;
	dlg.SetRect(Size(cx, cy));
	fcy /= 2;
	dlg << icon.TopPos(fcy + (mcy - bsz.cy) / 2, bsz.cy).LeftPos(fcy, bsz.cx);
	dlg << qtfctrl.TopPos(fcy + (mcy - qcy) / 2, qcy).LeftPos(2 * fcy + bsz.cx, qcx);
	if(okcancel)
		b1.Ok();
	b1.SetLabel(button1);
	b1.WhenAction = dlg.Breaker(1);
	b2.WhenAction = dlg.Breaker(0);
	b3.WhenAction = dlg.Breaker(-1);
	bh.WhenAction = HelpDlg;
	
	int nextx = 4;

	if(help) {
		bh.SetLabel(t_("?"));
		dlg << bh.RightPosZ(nextx, 80).BottomPosZ(4, 22);
		nextx += 84;
	}

	if(button2) {
		b2.SetLabel(button2);
		if(button3) {
			dlg << b3.SetLabel(button3).RightPosZ(nextx, 80).BottomPosZ(4, 22);
			nextx += 84;
			dlg << b2.RightPosZ(nextx, 80).BottomPosZ(4, 22);
			nextx += 84;
			
			if(okcancel)
				b3.Cancel();
		}
		else {
			dlg << b2.RightPosZ(nextx, 80).BottomPosZ(4, 22);
			nextx += 84;
			
			if(okcancel)
				b2.Cancel();
		}
		dlg << b1.RightPosZ(nextx, 80).BottomPosZ(4, 22);
		nextx += 84;
	}
	else {
		dlg << b1.RightPosZ(nextx, 80).BottomPosZ(4, 22);
		nextx += 84;	  
	}
	dlg.WhenClose = dlg.Breaker(button3 ? -1 : 0);
	dlg.Open();
	dlg.Title(title);
	return dlg.RunAppModal();
}

int Prompt(const char *title, const Image& iconbmp, const char *qtf, Callback HelpDlg,
           const char *button1, const char *button2, const char *button3, const char *help,
		       int cx)
{
	return Prompt(title, iconbmp, qtf, HelpDlg, true, button1, button2, button3, help, cx);
}

void PromptOK(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::information(), qtf, HelpDlg, t_("OK"), NULL, NULL, help);
}

void Exclamation(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, HelpDlg, t_("OK"), NULL, NULL, help);
}

void ShowExc(const Exc& exc, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), DeQtf(exc), HelpDlg, t_("OK"), NULL, NULL, help);
}

int PromptOKCancel(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, HelpDlg, t_("OK"), t_("Cancel"), NULL, help);
}

int PromptYesNo(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, HelpDlg, false, t_("&Yes"), t_("&No"), NULL, help);
}

int PromptYesNoCancel(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, HelpDlg, true, t_("&Yes"), t_("&No"), t_("Cancel"), help);
}

int PromptAbortRetry(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, HelpDlg, false, t_("&Abort"), t_("&Retry"), NULL, help);
}

int PromptRetryCancel(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, HelpDlg, true, t_("&Retry"), t_("Cancel"), NULL, help);
}

int PromptAbortRetryIgnore(const char *qtf, Callback HelpDlg, const char *help) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, HelpDlg, 
		          false, t_("&Abort"), t_("&Retry"), t_("&Cancel"), help);
}
