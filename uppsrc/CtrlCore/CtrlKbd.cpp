#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)  // DLOG(x)

Ptr<Ctrl> Ctrl::focusCtrl;
Ptr<Ctrl> Ctrl::focusCtrlWnd;
Ptr<Ctrl> Ctrl::lastActiveWnd;
Ptr<Ctrl> Ctrl::caretCtrl;
Rect      Ctrl::caretRect;
bool      Ctrl::ignorekeyup;

Ptr<Ctrl>           Ctrl::defferedSetFocus;
Vector< Ptr<Ctrl> > Ctrl::defferedChildLostFocus;


#ifdef flagSO
Ptr<Ctrl> Ctrl::FocusCtrl() { return focusCtrl; }
void Ctrl::FocusCtrl(Ptr<Ctrl> fc) { focusCtrl = fc; }
#endif

static bool s_hotkey;

void Ctrl::RefreshAccessKeys()
{
	GuiLock __;
	if(GetAccessKeys())
		Refresh();
	for(Ctrl *ctrl = GetFirstChild(); ctrl; ctrl = ctrl->GetNext())
		ctrl->RefreshAccessKeys();
}

void Ctrl::RefreshAccessKeysDo(bool vis)
{
	GuiLock __;
	if(GUI_AltAccessKeys() && vis != akv) {
		akv = vis;
		RefreshAccessKeys();
	}
}

bool Ctrl::DispatchKey(dword keycode, int count)
{
	GuiLock __;
	if(GUI_AltAccessKeys()) {
		bool alt = GetAlt();
		Ctrl *c = GetActiveCtrl();
		if(c)
			c->RefreshAccessKeysDo(alt);
	}
//	RLOGBLOCK("Ctrl::DispatchKey");
//	RLOG("DispatchKey: focusCtrl = " << FormatIntHex((int)~focusCtrl) << ", wnd = " << FormatIntHex((int)~focusCtrlWnd) << ")");
	LLOG("DispatchKey " << keycode << " (0x" << Sprintf("%08x", keycode)
		<< ", " << GetKeyDesc(keycode) << "), count:" << count
		<< " focusCtrl:" << UPP::Name(focusCtrl) << " focusCtrlWnd:" << UPP::Name(focusCtrlWnd));
	if((keycode & K_KEYUP) && ignorekeyup)
	{
		ignorekeyup = false;
		return true;
	}
	for(int i = 0; i < keyhook().GetCount(); i++)
		if((*keyhook()[i])(focusCtrl, keycode, count))
			return true;
	dword k = keycode;
	word l = LOWORD(keycode);
	if(!(k & K_DELTA) && l >= 32 && l != 127 && GetDefaultCharset() != CHARSET_UNICODE)
		k = MAKELONG((word)FromUnicode(l, CHARSET_DEFAULT), HIWORD(keycode));
	if(!focusCtrl)
		return false;
	Ptr<Ctrl> p = focusCtrl;
	if(Ini::user_log) {
		String kl;
		dword k = keycode;
		const char *l = "";
		if(k < 65536) {
			kl << "CHAR \'" << ToUtf8((wchar)keycode) << "\' (" << keycode << ')';
			l = "  ";
		}
		else {
			kl << "KEY";
			if(k & K_KEYUP) {
				kl << "UP";
				k &= ~K_KEYUP;
				l = "  ";
			}
			kl << " " << GetKeyDesc(k);
		}
		USRLOG(l << kl);
	}
	for(;;) {
		LLOG("Trying to DispatchKey: p = " << Desc(p));
		if(p->IsEnabled() && p->Key(p->unicode ? keycode : k, count))
		{
			LLOG("Ctrl::DispatchKey(" << FormatIntHex(keycode) << ", " << GetKeyDesc(keycode)
				<< "): eaten in " << Desc(p));
			if(Ini::user_log)
				USRLOG("  -> " << Desc(p));
			eventCtrl = focusCtrl;
			return true;
		}
		s_hotkey = true;
		if(!p->GetParent()) {
			if(p->HotKey(keycode)) {
				eventCtrl = focusCtrl;
				return true;
			}
			return false;
		}
		p = p->GetParent();
	}

	USRLOG("  key was ignored");

	return false;
}

bool Ctrl::HotKey(dword key)
{
	GuiLock __;
	LLOG("HotKey " << GetKeyDesc(key) << " at " << Name(this));
	if(!IsEnabled() || !IsVisible()) return false;
	for(Ptr<Ctrl> ctrl = firstchild; ctrl; ctrl = ctrl->next)
	{
		LLOG("Trying HotKey " << GetKeyDesc(key) << " at " << Name(ctrl));
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled() && ctrl->HotKey(key))
		{
			if(Ini::user_log && s_hotkey) {
				USRLOG("  HOT-> " << UPP::Name(ctrl));
				s_hotkey = false;
			}
			LLOG("ACCEPTED");
			return true;
		}
	}
	return false;
}

void Ctrl::DoDeactivate(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl)
{
	GuiLock __;
	if(pfocusCtrl) {
		Ptr<Ctrl> ptop = pfocusCtrl->GetTopCtrl();
		Ctrl *ntop = nfocusCtrl ? nfocusCtrl->GetTopCtrl() : NULL;
		LLOG("DoDeactivate " << UPP::Name(ptop) << " in favor of " << UPP::Name(ntop));
		if(ntop != ptop && !ptop->destroying) {
			ptop->DeactivateBy(ntop);
			ptop->Deactivate();
			if(ptop)
				ptop->StateH(DEACTIVATE);
			if(ptop)
				ptop->RefreshAccessKeysDo(false);
		}
	}
}

void Ctrl::DoKillFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl)
{
	GuiLock __;
	if(pfocusCtrl && !pfocusCtrl->destroying) {
		pfocusCtrl->StateH(FOCUS);
		LLOG("LostFocus: " << Name(pfocusCtrl));
		pfocusCtrl->LostFocus();
	}
	if(pfocusCtrl && pfocusCtrl->parent && !pfocusCtrl->parent->destroying)
		pfocusCtrl->parent->ChildLostFocus();
	SyncCaret();
}

void Ctrl::DoSetFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl, bool activate)
{
	GuiLock __;
	if(activate && focusCtrl == nfocusCtrl && nfocusCtrl) {
		Ctrl *top = nfocusCtrl->GetTopCtrl();
		if((!pfocusCtrl || pfocusCtrl->GetTopCtrl() != top) && !top->destroying) {
			top->StateH(ACTIVATE);
			top->Activate();
			top->RefreshAccessKeysDo(top->VisibleAccessKeys());
		}
	}

	if(focusCtrl == nfocusCtrl && nfocusCtrl && !nfocusCtrl->destroying) {
		nfocusCtrl->GotFocus();
		nfocusCtrl->StateH(FOCUS);
	}
	if(focusCtrl == nfocusCtrl && nfocusCtrl && nfocusCtrl->parent &&
	   !nfocusCtrl->parent->destroying)
		nfocusCtrl->parent->ChildGotFocus();
	SyncCaret();
}

/*
bool Ctrl::SetFocus0(bool activate)
{
	GuiLock __;
	if(IsUsrLog())
		UsrLogT(6, String().Cat() << "SETFOCUS " << Desc(this));
	LLOG("Ctrl::SetFocus " << Desc(this));
	LLOG("focusCtrlWnd " << UPP::Name(focusCtrlWnd));
	LLOG("Ctrl::SetFocus0 -> deferredSetFocus = NULL; was: " << UPP::Name(defferedSetFocus));
	defferedSetFocus = NULL;
	if(focusCtrl == this) return true;
	if(!IsOpen() || !IsEnabled() || !IsVisible()) return false;
	Ptr<Ctrl> pfocusCtrl = focusCtrl;
	Ptr<Ctrl> topwindow = GetTopWindow();
	Ptr<Ctrl> topctrl = GetTopCtrl();
	Ptr<Ctrl> _this = this;
	if(!topwindow) topwindow = topctrl;
	LLOG("SetFocus -> SetWndFocus: topwindow = " << UPP::Name(topwindow) << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd));
	if(!topwindow->HasWndFocus() && !topwindow->SetWndFocus()) return false;// cxl 31.1.2004
	if(activate) // Dolik/fudadmin 2011-5-1
		topctrl->SetWndForeground();  // cxl 2007-4-27
	LLOG("SetFocus -> focusCtrl = this: " << FormatIntHex(this) << ", _this = " << FormatIntHex(~_this) << ", " << UPP::Name(_this));
	focusCtrl = _this;
	focusCtrlWnd = topwindow;
	DoKillFocus(pfocusCtrl, _this);
	LLOG("SetFocus 2");
	DoDeactivate(pfocusCtrl, _this);
	DoSetFocus(pfocusCtrl, _this, activate);
	if(topwindow)
		lastActiveWnd = topwindow;
	return true;
}
*/

bool Ctrl::SetFocus0(bool activate)
{
	GuiLock __;
	USRLOG("      SETFOCUS " << Desc(this));
	LLOG("Ctrl::SetFocus " << Desc(this));
	LLOG("focusCtrlWnd " << UPP::Name(focusCtrlWnd));
	LLOG("Ctrl::SetFocus0 -> deferredSetFocus = NULL; was: " << UPP::Name(defferedSetFocus));
	defferedSetFocus = NULL;
	if(focusCtrl == this) return true;
	if(!IsOpen() || !IsEnabled() || !IsVisible()) return false;
	Ptr<Ctrl> pfocusCtrl = focusCtrl;
	Ptr<Ctrl> topwindow = GetTopWindow();
	Ptr<Ctrl> topctrl = GetTopCtrl();
	Ptr<Ctrl> _this = this;
	if(!topwindow) topwindow = topctrl;
	LLOG("SetFocus -> SetWndFocus: topwindow = " << UPP::Name(topwindow) << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd));
	if(!topwindow->HasWndFocus() && !topwindow->SetWndFocus()) return false;// cxl 31.1.2004
#ifdef PLATFORM_OSX11 // ugly temporary hack - popups not behaving right in MacOS
	// before 2012-9-2 was #ifdef GUI_X11, but that caused issues in most linux distros (cxl)
	// as parent window of popup always manages focus/keyboard for popup in X11
	if(activate) // Dolik/fudadmin 2011-5-1
		topctrl->SetWndForeground();
#else
	topwindow->SetWndForeground();  // cxl 2007-4-27
#endif
	LLOG("SetFocus -> focusCtrl = this: " << FormatIntHex(this) << ", _this = " << FormatIntHex(~_this) << ", " << UPP::Name(_this));
	focusCtrl = _this;
	focusCtrlWnd = topwindow;
	DoKillFocus(pfocusCtrl, _this);
	LLOG("SetFocus 2");
	DoDeactivate(pfocusCtrl, _this);
	DoSetFocus(pfocusCtrl, _this, activate);
	if(topwindow)
		lastActiveWnd = topwindow;
	return true;
}


bool Ctrl::SetFocus()
{
	GuiLock __;
	LLOG("Ctrl::SetFocus(" << Name() << ")");
	return SetFocus0(true);
}

void Ctrl::ActivateWnd()
{
	GuiLock __;
	// notification, don't set physical focus here
	LLOG("ActivateWnd " << Name());
	Ptr<Ctrl> nfocusCtrl = this;
	Ptr<Ctrl> pfocusCtrl = focusCtrl;
	LLOG("About to set focus: " << UPP::Name(nfocusCtrl));
	DoDeactivate(pfocusCtrl, nfocusCtrl);
	focusCtrl = nfocusCtrl;
	focusCtrlWnd = this;
	DoKillFocus(pfocusCtrl, nfocusCtrl);
	DoSetFocus(pfocusCtrl, nfocusCtrl, true);
	LLOG("Focus: " << UPP::Name(focusCtrl) << " FocusWnd:" << UPP::Name(focusCtrlWnd));
}

void Ctrl::SetFocusWnd()
{
	GuiLock __;
	// notification, don't set host platform focus here
	LLOG("Ctrl::SetFocusWnd");
	if(focusCtrlWnd != this) {
		LLOG("Ctrl::SetFocusWnd->ActivateWnd");
		ActivateWnd();
	}
}

void Ctrl::KillFocusWnd()
{
	GuiLock __;
	// notification, don't set host platform focus here
	LLOG("KillFocusWnd " << Name());
	if(this == ~focusCtrlWnd) {
		Ptr<Ctrl> pfocusCtrl = focusCtrl;
		DoDeactivate(pfocusCtrl, NULL);
		focusCtrl = focusCtrlWnd = NULL;
		DoKillFocus(pfocusCtrl, NULL);
	}
}

void Ctrl::ClickActivateWnd()
{
	GuiLock __;
	LLOG("Ctrl::ClickActivateWnd");
	if(this == ~focusCtrlWnd && focusCtrl && focusCtrl->GetTopCtrl() != this) {
		LLOG("Ctrl::ClickActivateWnd -> ActivateWnd");
		ActivateWnd();
	}
}

void Ctrl::DefferedFocusSync()
{
	GuiLock __;
	while(defferedChildLostFocus.GetCount() || defferedSetFocus) {
		LLOG("Ctrl::DeferredFocusSync, defferedSetFocus = " << UPP::Name(defferedSetFocus));
		Vector< Ptr<Ctrl> > b = pick(defferedChildLostFocus);
		defferedChildLostFocus.Clear();
		for(int i = 0; i < b.GetCount(); i++)
			if(b[i]) {
				LLOG("Ctrl::DeferredFocusSync -> ChildLostFocus " << UPP::Name(b[i]));
				b[i]->ChildLostFocus();
			}
		if(defferedSetFocus) {
			LLOG("Ctrl::DeferredFocusSync -> SetFocus " << UPP::Name(defferedSetFocus));
			defferedSetFocus->SetFocus();
		}
		defferedSetFocus = NULL;
		SyncCaret();
	}
}

void Ctrl::RefreshCaret()
{
	GuiLock __;
	if(caretCtrl)
		caretCtrl->Refresh(caretCtrl->caretx, caretCtrl->carety,
		                   caretCtrl->caretcx, caretCtrl->caretcy);
}

Ctrl *Ctrl::GetActiveWindow()
{
	GuiLock __;
	Ctrl *q = GetActiveCtrl();
	return q ? q->GetTopWindow() : NULL;
}

bool  Ctrl::HasFocusDeep() const
{
	GuiLock __;
	if(HasFocus() || HasChildDeep(FocusCtrl())) return true;
	Ctrl *a = GetActiveCtrl();
	if(!a || !a->IsPopUp()) return false;
	a = a->GetOwnerCtrl();
	return a && HasChildDeep(a);
}

String GetKeyDesc(dword key)
{
	String desc;
//	key &= 0xFFFF;

	if(key == 0)
		return desc;

	if(key & K_CTRL)  desc << t_("key\vCtrl+");
	if(key & K_ALT)   desc << t_("key\vAlt+");
	if(key & K_SHIFT) desc << t_("key\vShift+");
	if(key & K_KEYUP) desc << t_("key\vUP ");


	key &= ~(K_CTRL | K_ALT | K_SHIFT | K_KEYUP);
	if(key < K_DELTA && key > 32 && key != K_DELETE)
		return desc + ToUtf8((wchar)key);
	if(key >= K_NUMPAD0 && key <= K_NUMPAD9)
		desc << "Num " << (char)(key - K_NUMPAD0 + '0');
	else if(key >= K_0 && key <= K_9)
		desc << (char)('0' + key - K_0);
	else if(key >= K_A && key <= K_Z)
		desc << (char)('A' + key - K_A);
	else if(key >= K_F1 && key <= K_F12)
		desc << Format("F%d", (int)key - K_F1 + 1);
	else {
		static struct {
			dword key;
			const char *name;
		} nkey[] = {
			{ K_TAB, tt_("key\vTab") }, { K_SPACE, tt_("key\vSpace") },
			{ K_RETURN, tt_("key\vEnter") }, { K_BACKSPACE, tt_("key\vBackspace") },
			{ K_CAPSLOCK, tt_("key\vCaps Lock") }, { K_ESCAPE, tt_("key\vEsc") },
			{ K_PAGEUP, tt_("key\vPage Up") }, { K_PAGEDOWN, tt_("key\vPage Down") },
			{ K_END, tt_("key\vEnd") }, { K_HOME, tt_("key\vHome") },
			{ K_LEFT, tt_("key\vLeft") }, { K_UP, tt_("key\vUp") },
			{ K_RIGHT, tt_("key\vRight") }, { K_DOWN, tt_("key\vDown") },
			{ K_INSERT, tt_("key\vInsert") }, { K_DELETE, tt_("key\vDelete") },{ K_BREAK, tt_("key\vBreak") },
			{ K_MULTIPLY, tt_("key\vNum[*]") }, { K_ADD, tt_("key\vNum[+]") }, { K_SUBTRACT, tt_("key\vNum[-]") }, { K_DIVIDE, tt_("key\vNum[/]") },
			{ K_ALT_KEY, tt_("key\vAlt") }, { K_SHIFT_KEY, tt_("key\vShift") }, { K_CTRL_KEY, tt_("key\vCtrl") },
			{ K_PLUS, tt_("key\v[+]") }, { K_MINUS, tt_("key\v[-]") }, { K_COMMA, tt_("key\v[,]") }, { K_PERIOD, tt_("key\v[.]") },
			{ K_SEMICOLON, tt_("key\v[;]") }, { K_SLASH, tt_("key\v[/]") }, { K_GRAVE, tt_("key\v[`]") }, { K_LBRACKET, tt_("key\v[[]") },
			{ K_BACKSLASH, tt_("key\v[\\]") }, { K_RBRACKET, tt_("key\v[]]") }, { K_QUOTEDBL, tt_("key\v[']") },
			{ 0, NULL }
		};
		for(int i = 0; nkey[i].key; i++)
			if(nkey[i].key == key) {
				desc << GetLngString(nkey[i].name);
				return desc;
			}
		desc << Format("%04x", (int)key);
	}
	return desc;
}

END_UPP_NAMESPACE
