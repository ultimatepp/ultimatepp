#include "MenuImp.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

NAMESPACE_UPP

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

static ColorF xpmenuborder[] = {
	(ColorF)3,
	&SColorShadow, &SColorShadow, &SColorShadow, &SColorShadow,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
};

GLOBAL_VARP(BorderFrame, XPMenuFrame, (xpmenuborder));

CtrlFrame& MenuFrame()
{
	return GUI_GlobalStyle() >= GUISTYLE_XP ? (CtrlFrame&)XPMenuFrame() : (CtrlFrame&)OutsetFrame();
}

CH_STYLE(MenuBar, Style, StyleDefault)
{
	topitem[0] = Null;
	topitem[1] = topitem[2] = item = SColorHighlight();
	topitemtext[0] = menutext = SColorMenuText();
	topitemtext[1] = topitemtext[2] = itemtext = SColorHighlightText();
	topbar = SColorFace();
	maxiconsize = Size(16, 16);
	arealook = Null;
	look = SColorFace();
	separator = breaksep = SeparatorCtrl::StyleDefault();
	ImageBuffer ib(7, 7);
	Fill(ib, SColorShadow(), 7 * 7);
	for(int i = 0; i < 5; i++)
		Fill(ib[i + 1] + 1, SColorMenu(), 5);
	ib.SetHotSpot(Point(3, 3));
	ib.Set2ndSpot(Point(3, 3));
	popupframe = Image(ib);
	popupbody = SColorMenu();
	popupiconbar = Null;
	leftgap = Zx(26);
	textgap = Zx(6);
	lsepm = rsepm = Zx(2);
	pullshift.x = 0;
	pullshift.y = -1;
}

MenuBar::MenuBar()
{
	LLOG("MenuBar " << Name());
	submenu = parentmenu = NULL;
	submenuitem = NULL;
	doeffect = true;
	font = StdFont();
	leftgap = Null;
	lock = 0;
	SetStyle(StyleDefault());
	arealook = -1;
	maxiconsize = Null;
	nodarkadjust = false;
}

MenuBar& MenuBar::SetStyle(const Style& s)
{
	style = &s;
	lsepm = style->lsepm;
	rsepm = style->rsepm;
	sepstyle = &style->separator;
	Layout();
	Refresh();
	return *this;
}

void MenuBar::Paint(Draw& w)
{
	if(IsTransparent())
		return;
	if(GetParent())
		if(!IsNull(style->arealook) && (arealook < 0 ? InFrame() : arealook)) {
			PaintBarArea(w, this, style->arealook, (GetAlign() != BAR_TOP) ? GetScreenRect().bottom : Null);
			PaintBarArea(w, this, style->arealook);
			PaintBar(w, style->breaksep, Null);
		}
		else
			PaintBar(w, style->breaksep, style->look);
	else
		PaintBar(w, style->breaksep, style->popupbody, style->popupiconbar, Nvl(leftgap, style->leftgap));
}

Value MenuBar::GetBackground() const
{
	return parentmenu || !IsChild() ? Null : style->topbar;
}

void MenuBar::Clear()
{
	if(lock) return;
	lock++;
	SetActiveSubmenu(NULL, NULL);
	item.Clear();
	BarCtrl::Clear();
	lock--;
}

Bar::Item& MenuBar::AddItem(Callback cb)
{
	LLOG("MenuBar::AddItem " << Name());
	MenuItemBase *q;
	if(IsChild())
		q = new TopMenuItem;
	else
		q = new MenuItem;
	item.Add(q);
	pane.Add(q, Null);
	q->SetFont(font);
	q->LeftGap(Nvl(leftgap, style->leftgap));
	q->TextGap(style->textgap);
	q->Style(style);
	q->MaxIconSize(IsNull(maxiconsize) ? style->maxiconsize : maxiconsize);
	*q <<= THISBACK(CloseMenu);
	*q << cb;
	return *q;
}

Bar::Item& MenuBar::AddSubMenu(Callback1<Bar&> proc)
{
	LLOG("MenuBar::AddSubMenu " << Name());
	SubMenuBase *w;
	MenuItemBase *q;
	if(IsChild()) {
		TopSubMenuItem *a = new TopSubMenuItem;
		q = a;
		w = a;
	}
	else {
		SubMenuItem *a = new SubMenuItem;
		q = a;
		w = a;
	}
	item.Add(q);
	pane.Add(q, Null);
	q->SetFont(font);
	q->LeftGap(Nvl(leftgap, style->leftgap));
	q->TextGap(style->textgap);
	q->Style(style);
	q->MaxIconSize(IsNull(maxiconsize) ? style->maxiconsize : maxiconsize);
	w->SetParent(this);
	w->Set(proc);
	return *q;
}

void MenuBar::CloseMenu()
{
	LLOG("CloseMenu");
	MenuBar *q = GetLastSubmenu();
	while(q && q->IsPopUp()) {
		if(q->IsOpen()) {
			q->doeffect = true;
			q->Close();
			if(q->parentmenu)
				q->parentmenu->WhenSubMenuClose();
		}
		MenuBar *w = q;
		q = q->parentmenu;
		w->submenu = w->parentmenu = NULL;
	}
	if(q)
		q->submenu = NULL;
	while(q) {
		q->SyncState();
		q->doeffect = true;
		q = q->parentmenu;
	}
}

MenuBar *MenuBar::GetMasterMenu()
{
	MenuBar *q = this;
	while(q->parentmenu)
		q = q->parentmenu;
	return q;
}

MenuBar *MenuBar::GetLastSubmenu()
{
	MenuBar *q = this;
	while(q->submenu)
		q = q->submenu;
	return q;
}

void MenuBar::SetupRestoreFocus()
{
	MenuBar *q = GetMasterMenu();
	while(q) {
		if(q->HasFocusDeep()) return;
		q = q->submenu;
	}
	restorefocus = GetFocusCtrl();
	LLOG("SetupRestoreFocus -> " << UPP::Name(restorefocus));
}

void MenuBar::PostDeactivate()
{
	LLOG("PostDeactivate");
	MenuBar *q = GetLastSubmenu();
	MenuBar *u = q;
	while(q && q->IsPopUp()) {
		if(q->IsOpen() && q->HasFocusDeep())
			return;
		q = q->parentmenu;
	}
	if(u->IsPopUp()) {
		while(u->parentmenu && u->parentmenu->IsPopUp())
			u = u->parentmenu;
		Ctrl *w = GetActiveCtrl();
		while(w) {
			if(w == u) return;
			w = w->GetOwner();
		}
	}
	CloseMenu();
}

void MenuBar::Deactivate()
{
	LLOG("Deactivate");
	if(IsOpen())
		SetTimeCallback(0, THISBACK(PostDeactivate), TIMEID_STOP);
}

void MenuBar::SyncState()
{
	if(lock) return;
	for(int i = 0; i < item.GetCount(); i++)
		item[i].SyncState();
}

void MenuBar::ChildGotFocus()
{
	if(submenu && !submenuitem->HasFocusDeep() && !ExistsTimeCallback()) {
	   	if(submenu->IsOpen())
			submenu->DelayedClose();
	}
	KillDelayedClose();
	SyncState();
}

void MenuBar::ChildLostFocus()
{
	SyncState();
}

bool PullMenu(Ctrl *q)
{
	if(q->IsEnabled()) {
		TopSubMenuItem *w = dynamic_cast<TopSubMenuItem *>(q);
		if(w) {
			w->Pull();
			return true;
		}
	}
	return false;
}

static bool s_doaltkey;

bool MenuKeyHook(Ctrl *ctrl, dword key, int count)
{
	if(key != K_ALT_KEY && key != (K_ALT_KEY|K_KEYUP))
		s_doaltkey = false;
	return false;
}

bool MenuMouseHook(Ctrl *ctrl, bool inframe, int event, Point p,
	               int zdelta, dword keyflags)
{
	if(findarg(event & Ctrl::ACTION, Ctrl::MOUSEWHEEL, Ctrl::DOWN, Ctrl::UP) >= 0)
		s_doaltkey = false;
	return false;
}

bool MenuStateHook(Ctrl *ctrl, int reason)
{
	if(reason == Ctrl::ACTIVATE)
		s_doaltkey = false;
	return false;
}

INITBLOCK
{
	Ctrl::InstallKeyHook(MenuKeyHook);
	Ctrl::InstallStateHook(MenuStateHook);
	Ctrl::InstallMouseHook(MenuMouseHook);
}

bool MenuBar::Key(dword key, int count)
{
	LLOG("KEY " << GetKeyDesc(key));
	bool horz = IsChild();
	if((horz ? key == K_RIGHT : key == K_DOWN)) {
		Ctrl *ctrl = GetFocusChildDeep();
		LLOG("MenuBar::Key(" << key << ") -> IterateFocusForward for " << UPP::Name(ctrl) << ", pane " << UPP::Name(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(ctrl && IterateFocusForward(ctrl, &pane, false, false, true))
			return true;
		Ctrl *f = pane.GetFirstChild();
		if(!f) return true;
		if(f->IsEnabled()) {
			f->SetFocus();
			return true;
		}
		if(IterateFocusForward(pane.GetFirstChild(), &pane, false, false, true)) return true;
	}
	else
	if((horz ? key == K_LEFT : key == K_UP)) {
		Ctrl *ctrl = GetFocusChildDeep();
		LLOG("MenuBar::Key(" << key << ") -> IterateFocusBackward for " << UPP::Name(ctrl) << ", pane " << UPP::Name(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(ctrl && IterateFocusBackward(ctrl, &pane, false, true))
			return true;
		Ctrl *f = pane.GetLastChild();
		if(!f) return true;
		if(f->IsEnabled()) {
			f->SetFocus();
			return true;
		}
		if(IterateFocusBackward(pane.GetLastChild(), &pane, false, true)) return true;
	}
	else
	if(parentmenu && !parentmenu->IsChild() && key == K_LEFT || key == K_ESCAPE) {
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(parentmenu && parentmenu->submenu)
			parentmenu->submenuitem->SetFocus();
		else
		if(IsChild() && HasFocusDeep()) {
			if(restorefocus)
				restorefocus->SetFocus();
			doeffect = true;
			return true;
		}
		if(IsPopUp()) {
			SubmenuClose();
			return true;
		}
		doeffect = true;
	}
	if(parentmenu && parentmenu->IsChild() && parentmenu->GetActiveSubmenu() &&
	   parentmenu->pane.GetFirstChild() && parentmenu->submenuitem) {
		Ctrl *smi = parentmenu->submenuitem;
		Ctrl *q = smi;
		q->Refresh();
		if(key == K_RIGHT)
			for(;;) {
				q = q->GetNext();
				if(!q)
					q = parentmenu->pane.GetFirstChild();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					q->Refresh();
					SyncState();
					return true;
				}
			}
		if(key == K_LEFT)
			for(;;) {
				q = q->GetPrev();
				if(!q)
					q = parentmenu->pane.GetLastChild();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					q->Refresh();
					SyncState();
					return true;
				}
			}
	}
	LLOG("MenuBar::Key -> HotKey");
	return HotKey(key);
}

void MenuBar::LeftDown(Point, dword)
{
	if(IsChild() && HasFocusDeep()) {
		if(restorefocus)
			restorefocus->SetFocus();
	}
	WhenLeftClick();
}

bool MenuBar::HotKey(dword key)
{
	if(Ctrl::HotKey(key))
		return true;
	if(IsChild()) {
		if((key == (K_ALT_KEY|K_KEYUP) || key == K_F10) && (submenu || HasFocusDeep())) {
			LLOG("CloseMenu()");
			CloseMenu();
			if(restorefocus)
				restorefocus->SetFocus();
			s_doaltkey = false;
			return true;
		}
		if(key == K_ALT_KEY) {
			LLOG("K_ALT_KEY");
			s_doaltkey = true;
			return true;
		}
		if((key == K_F10 || key == (K_ALT_KEY|K_KEYUP) && s_doaltkey)
		   && !submenu && !HasFocusDeep() && GetTopWindow() && GetTopWindow()->IsForeground()) {
			LLOG("Open menu by F10 or ALT-UP");
			SetupRestoreFocus();
			for(Ctrl *q = pane.GetFirstChild(); q; q = q->GetNext())
				if(q->SetFocus()) return true;
		}
	}
	LLOG("MenuBar::HotKey");
	return (key == K_LEFT || key == K_RIGHT) && parentmenu ? parentmenu->Key(key, 1) : false;
}

void MenuBar::SetActiveSubmenu(MenuBar *sm, Ctrl *item)
{
	if(submenu && submenu != sm) {
		submenu->Close();
		WhenSubMenuClose();
		submenu->parentmenu = NULL;
	}
	if(submenuitem)
		submenuitem->Refresh();
	submenu = sm;
	submenuitem = item;
}

void MenuBar::SubmenuClose()
{
	if(parentmenu && parentmenu->GetActiveSubmenu() == this)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	else {
		Close();
		if(parentmenu)
			parentmenu->WhenSubMenuClose();
	}
	parentmenu = NULL;
	Clear();
}

void MenuBar::DelayedClose()
{
	SetTimeCallback(400, THISBACK(SubmenuClose), TIMEID_SUBMENUCLOSE);
}

void MenuBar::KillDelayedClose()
{
	KillTimeCallback(TIMEID_SUBMENUCLOSE);
	KillTimeCallback(TIMEID_STOP);
}

void MenuBar::Set(Callback1<Bar&> menu)
{
	if(lock) return;
	Clear();
	lock++;
	menu(*this);
	SyncBar();
	DistributeAccessKeys();
	lock--;
}

void MenuBar::Post(Callback1<Bar&> bar)
{
	KillTimeCallback(TIMEID_POST);
	SetTimeCallback(0, THISBACK1(Set, bar), TIMEID_POST);
}

#ifdef PLATFORM_WIN32

DWORD WINAPI PlaySoundThread(LPVOID)
{
#ifdef PLATFORM_WINCE
	PlaySound(L"MenuPopup", NULL, SND_ASYNC|SND_NODEFAULT);
#else
	PlaySound("MenuPopup", NULL, SND_ASYNC|SND_NODEFAULT);
#endif
	return 0;
}

#endif

void MenuBar::PopUp(Ctrl *owner, Point p, Size rsz)
{
	bool szcx = true;
	bool szcy = true;
	bool szx = false;
	bool szy = false;
	if(parentmenu) {
		if(parentmenu->IsChild())
			szcx = false;
		else
			szcy = false;
		WhenHelp = parentmenu->WhenHelp;
	}
	Rect r = GetWorkArea(p);
	restorefocus = GetFocusCtrl();
	LLOG("PopUp " << UPP::Name(this) << " set restorefocus:" << UPP::Name(restorefocus));
	DistributeAccessKeys();
	frame.Set(style->popupframe);
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		SetFrame(frame);
	else
		SetFrame(OutsetFrame());
	pane.SubMenu();
	Size sz = pane.Repos(false, r.Height());
	pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
	Size sz0 = sz;
	sz = AddFrameSize(sz);
	if(p.y + sz.cy > r.bottom) {
		if(p.y - r.top > r.bottom - p.y) {
			int y0;
			if (parentmenu && parentmenu->GetActiveSubmenu() && parentmenu->submenuitem) 
				y0 = parentmenu->submenuitem->GetScreenRect().BottomRight().y + 2 + rsz.cy;
			else
				y0 = p.y + rsz.cy;
			szy = szcy;
			sz = pane.Repos(false, y0 - max(y0 - sz.cy, r.top) - (sz.cy - sz0.cy));
			pane.RightPos(0, sz.cx).TopPos(0, sz.cy);
			sz = AddFrameSize(sz);
			p.y = y0 - sz.cy;
		}
		else {
			sz = pane.Repos(false, r.bottom - p.y - (sz.cy - sz0.cy));
			pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
			sz = AddFrameSize(sz);
		}
	}
	if(p.x + sz.cx > r.right) {
		p.x = max(p.x + rsz.cx - sz.cx, r.left);
		szx = szcx;
		pane.LeftPos(0, sz.cx);
	}
	bool eff = parentmenu == NULL || parentmenu->doeffect;
	if(eff && GUI_PopUpEffect() == GUIEFFECT_SLIDE)
		SetRect(szx ? p.x + sz.cx : p.x, szy ? p.y + sz.cy : p.y, parentmenu ? sz.cx : 1, 1);
	else
		SetRect(p.x, p.y, sz.cx, sz.cy);
#ifdef PLATFORM_WIN32
	DWORD dummy;
	CloseHandle(CreateThread(NULL, 0, PlaySoundThread, NULL, 0, &dummy));
#endif
	doeffect = true;
	Ctrl::PopUp(owner, true, true, GUI_DropShadows(), !owner);
	GuiPlatformAfterMenuPopUp();
	if(eff)
		Animate(*this, p.x, p.y, sz.cx, sz.cy);
}

void MenuBar::Execute(Ctrl *owner, Point p)
{
	static Vector<Ctrl *> ows; // Used to prevent another open local menu for single owner to be opened (repeated right-click)
	int level = ows.GetCount();
	if(IsEmpty() || FindIndex(ows, owner) >= 0)
		return;
	ows.Add(owner);
	PopUp(owner, p);
	EventLoop(this);
	CloseMenu();
	ows.SetCount(level);
}

void MenuBar::Execute(Ctrl *owner, Callback1<Bar&> proc, Point p)
{
	MenuBar bar;
	proc(bar);
	bar.Execute(owner, p);
}

int MenuBar::GetStdHeight(Font font)
{
	return TopMenuItem::GetStdHeight(font);
}

void MenuBar::CancelMode()
{
}

MenuBar::~MenuBar()
{
	lock++;
	LLOG("~MenuBar " << Name());
	LLOG("Parent menu " << UPP::Name(parentmenu));
	if(parentmenu)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	LLOG("~MenuBar 1");
}

END_UPP_NAMESPACE
