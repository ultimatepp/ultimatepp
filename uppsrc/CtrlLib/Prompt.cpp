#include "CtrlLib.h"

NAMESPACE_UPP

struct PromptDlgWnd__ : TopWindow {
	bool    esc;
	Button *b;

	virtual bool HotKey(dword key) {
		if(TopWindow::HotKey(key))
			return true;
		if(IsAlpha(key))
			return TopWindow::HotKey(K_ALT_A + ToUpper((int)key) - 'A');
		if(key == K_ESCAPE && esc)
			b->PseudoPush();
		return false;
	}
};

static void sAdd(Ctrl& dlg, int fcy, int bcy, int& bx, int bcx, int gap, Button& b, const char *button, const Image& img)
{
	if(button) {
		dlg << b.BottomPos(fcy, bcy).LeftPos(bx, bcx);
		b.SetLabel(button);
		if(!IsNull(img))
			b.SetImage(img);
		bx += gap + bcx;
	}
}

void sExecutePrompt(PromptDlgWnd__ *dlg, int *result)
{
	dlg->Open();
	Vector<Ctrl *> wins = Ctrl::GetTopWindows();
	for(int i = 0; i < wins.GetCount(); i++) {
		TopWindow *w = dynamic_cast<TopWindow *>(wins[i]);
		if(w && w->IsTopMost()) {
			dlg->TopMost();
			break;
		}
	}
	*result = dlg->RunAppModal();
	dlg->Close();
}
                        
RedirectPromptFn RedirectPrompt;

void RedirectPrompts(RedirectPromptFn r)
{
	RedirectPrompt = r;
}

int Prompt(Callback1<const String&> WhenLink,
           const char *title, const Image& iconbmp, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx,
		   Image im1, Image im2, Image im3)
{
	if(RedirectPrompt)
		return (*RedirectPrompt)(WhenLink, title, iconbmp, qtf, okcancel,
                                 button1, button2, button3,
                                 cx, im1, im2, im3);
	int fcy = Draw::GetStdFontCy();
	EnterGuiMutex(); // Ctrl derived classes can only be initialized with GuiLock
	PromptDlgWnd__ dlg;
	RichTextCtrl qtfctrl;
	Icon         icon;
	qtfctrl.WhenLink = WhenLink;
	icon.SetImage(iconbmp);
	Button b1, b2, b3;
	qtfctrl.SetQTF(String("[G1 ") + qtf, GetRichTextStdScreenZoom());
	int bcy = Ctrl::VertLayoutZoom(24);
	int bcx = Ctrl::HorzLayoutZoom(72);
	if(button1)
		bcx = max(2 * fcy + GetTextSize(button1, Draw::GetStdFont()).cx, bcx);
	if(button2)
		bcx = max(2 * fcy + GetTextSize(button2, Draw::GetStdFont()).cx, bcx);
	if(button3)
		bcx = max(2 * fcy + GetTextSize(button3, Draw::GetStdFont()).cx, bcx);
	Size bsz = icon.GetStdSize();
	if(cx == 0) {
		cx = qtfctrl.GetWidth();
		if(!cx)
			cx = 350;
		cx += 2 * fcy;
		if(bsz.cx)
			cx += bsz.cx + fcy;
	}
	int nbtn = !!button1 + !!button2 + !!button3;
	dlg.esc = okcancel && nbtn == 1;
	cx = min(550, max(nbtn * bcx + (1 + nbtn) * fcy, cx));
	int qcx = cx - 2 * fcy;
	if(bsz.cx)
		qcx -= bsz.cx + fcy;
	int ccy = qtfctrl.GetHeight(qcx);
	int qcy = min(400, ccy);
	if(qcy <= ccy) {
		qcx += ScrollBarSize() + fcy;
		cx += ScrollBarSize() + fcy;
	}
	int mcy = max(qcy, bsz.cy);
	int cy = mcy + 48 * fcy / 10;
	dlg.SetRect(Size(cx, cy));
	dlg << icon.TopPos(fcy, bsz.cy).LeftPos(fcy, bsz.cx);
	dlg << qtfctrl.TopPos(fcy + (mcy - qcy) / 2, qcy).RightPos(fcy, qcx);
	if(okcancel) {
		b1.Ok();
		if(nbtn == 2)
			b2.Cancel();
		if(nbtn == 3)
			b3.Cancel();
	}
	b1.WhenAction = dlg.Breaker(1);
	b2.WhenAction = dlg.Breaker(0);
	b3.WhenAction = dlg.Breaker(-1);
	dlg.b = &b1;
	int bx = bcx;
	int gap = fcy / 2;
	fcy = 8 * fcy / 10;
	if(button2)
		bx += gap + bcx;
	if(button3)
		bx += gap + bcx;
	bx = (cx - bx) / 2;
	if(SwapOKCancel()) {
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b2, button2, im2);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b3, button3, im3);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b1, button1, im1);
	}
	else {
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b1, button1, im1);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b2, button2, im2);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b3, button3, im3);
	}
	dlg.WhenClose = dlg.Breaker(button3 ? -1 : 0);
	dlg.Title(title);
	LeaveGuiMutex();
	int result;
	Ctrl::Call(callback2(sExecutePrompt, &dlg, &result));
	return result;
}

int Prompt(Callback1<const String&> WhenLink,
           const char *title, const Image& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(WhenLink, title, icon, qtf, okcancel, button1, button2, button3, cx, Null, Null, Null);
}

int Prompt(const char *title, const Image& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(callback(LaunchWebBrowser), title,
	              icon, qtf, okcancel, button1, button2, button3, cx, Null, Null, Null);
}

int Prompt(const char *title, const Image& icon, const char *qtf,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(title, icon, qtf, true, button1, button2, button3, cx);
}

void PromptOK(const char *qtf) {
	BeepInformation();
	Prompt(Ctrl::GetAppName(), CtrlImg::information(), qtf, t_("OK"));
}

void Exclamation(const char *qtf) {
	BeepExclamation();
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, t_("OK"));
}

void ShowExc(const Exc& exc) {
	BeepExclamation();
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), DeQtf(exc), t_("OK"));
}

void ErrorOK(const char *qtf) {
	BeepError();
	Prompt(Ctrl::GetAppName(), CtrlImg::error(), qtf, t_("OK"));
}

int PromptOKCancel(const char *qtf) {
	BeepQuestion();
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, t_("OK"), t_("Cancel"));
}

int ErrorOKCancel(const char *qtf) {
	BeepError();
	return Prompt(Ctrl::GetAppName(), CtrlImg::error(), qtf, t_("OK"), t_("Cancel"));
}

CH_IMAGE(YesButtonImage, Null);
CH_IMAGE(NoButtonImage, Null);
CH_IMAGE(AbortButtonImage, Null);
CH_IMAGE(RetryButtonImage, Null);

int PromptYesNo(const char *qtf) {
	BeepQuestion();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::question(), qtf, false,
	              t_("&Yes"), t_("&No"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNo(const char *qtf) {
	BeepError();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::error(), qtf, false,
	              t_("&Yes"), t_("&No"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoCancel(const char *qtf) {
	BeepQuestion();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::question(), qtf, true,
	              t_("&Yes"), t_("&No"), t_("Cancel"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoCancel(const char *qtf) {
	BeepError();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::error(), qtf, true,
	              t_("&Yes"), t_("&No"), t_("Cancel"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptAbortRetry(const char *qtf) {
	BeepExclamation();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, false,
	              t_("&Abort"), t_("&Retry"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetry(const char *qtf) {
	BeepError();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::error(), qtf, false,
	              t_("&Abort"), t_("&Retry"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptRetryCancel(const char *qtf) {
	BeepExclamation();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, true,
	              t_("&Retry"), t_("Cancel"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int ErrorRetryCancel(const char *qtf) {
	BeepError();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::error(), qtf, true,
	              t_("&Retry"), t_("Cancel"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int PromptAbortRetryIgnore(const char *qtf) {
	BeepExclamation();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, false,
	              t_("&Abort"), t_("&Retry"), t_("&Ignore"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryIgnore(const char *qtf) {
	BeepError();
	return Prompt(callback(LaunchWebBrowser),
	              Ctrl::GetAppName(), CtrlImg::error(), qtf, false,
	              t_("&Abort"), t_("&Retry"), t_("&Ignore"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

END_UPP_NAMESPACE
