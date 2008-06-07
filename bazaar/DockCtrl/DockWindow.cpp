#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockWindowImages
#include <Draw/iml.h>

//===============================================
// DockWindow class:
// A dockable window widget.
//===============================================

DockWindow::DockWindow() : DockableCtrl()
{
	SetType(TYPE_DOCKWINDOW);

	ownertab		= NULL;
	ownerbar		= NULL;	
	menubar			= NULL;
	toolbar			= NULL;
	statusbar		= NULL;
	hasstatusbar	= false;
	hasbarbuttons	= false;
	hastoolbar		= false;
	hasmenubar		= false;

	shutbutton		<< THISBACK(OnShutButton);
	autohidebutton	<< THISBACK(OnAutoHideButton);
	menubutton		<< THISBACK(OnMenuButton);
	
	dragbar.SetOwner(this);
	SetStyle(DockCtrlChStyle::StyleDefault());
	SetFrame(FieldFrame());
	ShowDragBar();
	ShowDragBarButtons(true);
	Sizeable().Zoomable().ToolWindow().TopMost(false).MinimizeBox().MaximizeBox().BackPaint();
	WhenContextMenu	= THISBACK(DockWindowMenu);		

}

DockWindow::~DockWindow()
{
}

void DockWindow::Dock(int alignment, int state, int position, bool sizecheck)
{
	if(Alignment() == alignment && (!IsHidden() && !IsAutoHidden())) return;
	else if(IsTabbed())	GetOwnerTab()->Detach(*this);
	else if(IsFloating()) ShutWindow();
	else if(IsAutoHidden())	GetBase().GetHideBar(Alignment()).Detach(*this);
	else 
	{
		if(sizecheck) ReSize();
		GetBase().Detach(*this);
	}
	DockingStyle(alignment, state, position);
	ShowDragBar();
	GetBase().Dock(*this);
}

void DockWindow::FloatEx(Rect r)
{
	if(IsTabbed() && IsChild()) GetOwnerTab()->Detach(*this); 
	else if(IsAutoHidden())	GetBase().GetHideBar(Alignment()).Detach(*this);
	else if(!IsShut()) GetBase().Detach(*this);
	OpenWindow(r.left, r.top, r.right, r.bottom);
	DockingStyle(DOCK_NONE, STATE_SHOW, 0);
	GetBase().RefreshPanel();
}

void DockWindow::Float()
{
	if(IsFloating()) return;
	Rect  r  =  GetScreenView();
	Point p  = GetMousePos();

	if(IsTabbed())
	{
		GetOwnerTab()->Detach(*this); 	
		Size sizehint = GetSizeHint();
		r.left 	 = p.x - 4;
		r.top	 = p.y + 4;
		r.right  = p.x + sizehint.cx;
		r.bottom = p.y + sizehint.cy;
	}
	else
	{
		if(r.top > p.y) r.top -= r.top - p.y - 4;
		r.right	 = r.left + r.Width();
		r.bottom = r.top + r.Height();
	}
	if(IsAutoHidden())	GetBase().GetHideBar(Alignment()).Detach(*this);
	if(!IsShut()) GetBase().Detach(*this);
	OpenWindow(r);
	DockingStyle(DOCK_NONE, STATE_SHOW, 0);
	GetBase().RefreshPanel();
}

void DockWindow::Show()
{
	if(!IsHidden() || IsTabbed()) return;
	Dock(Alignment(), STATE_SHOW, Position());
	GetBase().RefreshPanel();
}

void DockWindow::Hide()
{
	if(IsTabbed()) return;
	if(IsHidden())
	{
		Show();
		return;
	}
	ShutWindow();
	GetBase().Detach(*this);
	DockingStyle(Alignment(), STATE_HIDE, Position());
	GetBase().RefreshPanel();
}

void DockWindow::AutoHide()
{
	if(IsFloating()) return;
	if(IsAutoHidden())
	{
		GetBase().GetHideBar(Alignment()).Detach(*this);
		GetBase().Dock(this->DockingStyle(Alignment(), STATE_SHOW, Position()));
		return;
	}
	int alignment 	= Alignment();
	int state		= STATE_AUTO;
	int position	= Position();
	
	if(IsTabbed())
	{
		TabWindow* tabwindow = GetOwnerTab()->GetBaseTab();
		if(tabwindow->IsFloating()) return;
		alignment 	= tabwindow->Alignment();
		position	= tabwindow->Position();
		GetOwnerTab()->Detach(*this);
	}
	else
	{
		ReSize();
		GetBase().Detach(*this);
	}
	DockingStyle(alignment, state, position);
	GetBase().Dock(*this);
	GetBase().RefreshPanel();
}

void DockWindow::Shut()
{
	if(IsShut()) return;
	ShutWindow();
	if(IsTabbed()) GetOwnerTab()->Detach(*this);
	else if(IsAutoHidden())	GetBase().GetHideBar(Alignment()).Detach(*this);
	else if(IsFloating()) Remove(); 	
	else GetBase().Detach(*this);
	DockingStyle(DOCK_NONE, STATE_SHUT, 0);
	GetBase().RefreshPanel();
}

void DockWindow::Menu()
{
	ContextMenu();
}

DockWindow& DockWindow::AddMenuBar(MenuBar& bar)
{
	AddChildBar(bar); 
	menubar = &bar; 
	hasmenubar = true;
	return *this;
}

DockWindow& DockWindow::AddToolBar(ToolBar& bar)
{
	AddChildBar(bar); 
	toolbar = &bar; 
	hastoolbar = true;
	return *this;
}

DockWindow& DockWindow::AddStatusBar(StatusBar& bar)
{
	AddChildBar(bar.Height(18)); 
	statusbar = &bar; 
	hasstatusbar = true;
	return *this;
}

void DockWindow::HideDragBar()
{
	if(!HasDragBar()) return;
	RemoveFrame(dragbar);
	DockableCtrl::HideDragBar();	
}

void DockWindow::ShowDragBar()
{
	if(HasDragBar()) return;
	InsertFrame(0, dragbar); 
	DockableCtrl::ShowDragBar();
}

Rect DockWindow::GetCtrlRect()
{
	if(!HasDragBar()) return GetScreenRect();
	Rect bar = dragbar.GetScreenRect();
	Rect wnd = GetScreenRect();
	wnd.top  = bar.top;
	return wnd;
}

Size DockWindow::GetCtrlSize()
{
	return GetCtrlRect().GetSize();
}

void DockWindow::Paint(Draw& d)
{
	TopWindow::Paint(d);
}

DockWindow& DockWindow::SetStyle(const DockCtrlChStyle::Style& st)
{
	style = &st;
	dragbar.SetStyle(st);
	shutbutton.SetImage(style->barshut[0], style->barshut[1], style->barshut[2]);
	menubutton.SetImage(style->barmenu[0],style->barmenu[1], style->barmenu[2]);
	autohidebutton.SetImage(style->barhide[0], style->barhide[1], style->barhide[2]);
	RefreshLayoutDeep();
	return *this;
}

const DockCtrlChStyle::Style* DockWindow::GetStyle()
{
	return style ? style : &DockCtrlChStyle::StyleDefault();
}

void DockWindow::MouseMove(Point p, dword keyflags)
{
	TopWindow::MouseMove(p, keyflags);
}

void DockWindow::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if(HasDragBar() && child == &dragbar)
	{
		if(event == LEFTDRAG) StartWindowDrag();
		if(event == LEFTDOUBLE && IsAutoHidden()) Dock(Alignment(), STATE_SHOW, Position());
		if(event == RIGHTDOWN  && !hasbarbuttons) ContextMenu();
	}
	TopWindow::ChildMouseEvent(child, event, p, zdelta, keyflags);
}

void DockWindow::OnMenuButton()
{
	ContextMenu();
}

void DockWindow::OnShutButton()
{
	Shut();
}

void DockWindow::OnAutoHideButton()
{
	AutoHide();
}

DockWindow& DockWindow::ShowDragBarButtons(bool b)
{
	if(b && hasbarbuttons) return *this;
	else if(b && !hasbarbuttons)
	{
		hasbarbuttons = b;
		int cx 	= style->barheight - 8;
		int cy 	= style->barheight - 8;	

		dragbar	<< shutbutton.RightPos(2, cy).TopPos(2, cy).Tip(t_("Close")); 
		dragbar	<< autohidebutton.RightPos(cy + 4, cy).TopPos(2, cy).Tip(t_("Auto Hide"));
		dragbar	<< menubutton.RightPos((2 * cy) + 6, cy).TopPos(2, cy).Tip(t_("Window Position")); 
	}
	else if(!b && hasbarbuttons)
	{
		hasbarbuttons = b;
		shutbutton.Remove();
		autohidebutton.Remove();
		menubutton.Remove();
	}
	dragbar.ShowButtons(b);
	return *this;
}

void DockWindow::DockWindowMenu(Bar& bar)
{
	bool floatingbase = false;
	TabWindow* c = GetOwnerTab();
	if(c) floatingbase = c->GetBaseTab()->IsFloating();

	bar.Add(t_("Dock"),	THISBACK(DockWindowDockMenu));
	bar.Add(!IsFloating(), t_("Float"),THISBACK(Float));
	bar.Separator();
	bar.Add(IsDocked(), IsHidden() ? t_("Show") : t_("Hide"), THISBACK(Hide));
	bar.Add(!IsFloating() && !floatingbase, t_("Auto Hide"), THISBACK(AutoHide));	
	bar.Separator();
	bar.Add(t_("Settings"), THISBACK(Settings));
	bar.Separator();
	bar.Add(t_("Close"), DockWindowImages::CClose(), THISBACK(Shut));	
}

void DockWindow::DockWindowDockMenu(Bar& bar)
{
	if(GetBase().IsSideAllowed(DOCK_TOP)) 		bar.Add(Alignment() == DOCK_TOP  ? 0 : 1, t_("Top"), THISBACK4(Dock, (int)DOCK_TOP, (int)STATE_SHOW, Position(), 0));
	if(GetBase().IsSideAllowed(DOCK_LEFT)) 		bar.Add(Alignment() == DOCK_LEFT  ? 0 : 1, t_("Left"), THISBACK4(Dock, (int)DOCK_LEFT, (int)STATE_SHOW, Position(), 0));
	if(GetBase().IsSideAllowed(DOCK_RIGHT)) 	bar.Add(Alignment() == DOCK_RIGHT ? 0 : 1, t_("Right"), THISBACK4(Dock, (int)DOCK_RIGHT, (int)STATE_SHOW, Position(), 0));
	if(GetBase().IsSideAllowed(DOCK_BOTTOM)) 	bar.Add(Alignment() == DOCK_BOTTOM ? 0 : 1, t_("Bottom"), THISBACK4(Dock, (int)DOCK_BOTTOM, (int)STATE_SHOW, Position(), 0));	
}

void DockWindow::Serialize(Stream& s)
{
	int 	type		= GetType();
	int 	alignment 	= Alignment();
	int 	state		= State();
	int 	position	= Position();
	int 	id 			= -1;
	Size	sz			= GetSizeHint();
	Rect	r			= GetRect();
	
	if(s.IsStoring())
	{
		if(IsAutoHidden()) SetSizeHint(Size(0, 0));
		id = GetBase().GetCtrlRecord(*this)->id;
		s / type;
		s / id;
		s / alignment / state / position; 
		sz.Serialize(s);
		if(IsFloating()) 
		{	
			r.Serialize(s);
			SerializePlacement(s, true);
		}
	}
	if(s.IsLoading())
	{
		TabWindow* tabwindow = GetOwnerTab();
		s / alignment / state / position;
		sz.Serialize(s);
		SetSize(sz);
		if(tabwindow) tabwindow->Attach(*this);
		else if(alignment == DOCK_NONE && state != STATE_SHUT)
		{
			r.Serialize(s);
			Shut();
			FloatEx(Rect(r.left, r.top, 10, 10));
			SerializePlacement(s, true);
		}
		else 
		{
			Shut();
			Dock(alignment, state, position, false);
		}
	}
}

//===============================================
// DockWindow::DragBarButton
// Nested helper class for DockWindow
//===============================================

Ctrl& DockWindow::DragBarButton::SetImage(const Image& normal, const Image& highlighted, const Image& pushed)
{
	nimage = normal; 
	himage = highlighted; 
	pimage = pushed;
	return *this;
}

void DockWindow::DragBarButton::Paint(Draw& d)
{
	Size sz = GetSize();
	d.DrawImage(0 , 0, sz.cx, sz.cy, (IsPush() && HasMouse() ? pimage : (!IsPush() && HasMouse() ? himage : nimage)));
}

//===============================================
// DockWindow::DragBar
// Nested helper class for DockWindow
//===============================================

DockWindow::DragBar::DragBar()
{
	SetStyle(DockCtrlChStyle::StyleDefault());
	SetFrame(FieldFrame());
	SetSize(style->barheight);
	ShowButtons();
}

int DockWindow::DragBar::GetWidth()
{
	Size sz = GetTextSize(GetOwner().GetLabel(), style->font);
	return BAR_MARGIN * 2 + sz.cx;
}

int DockWindow::DragBar::GetHeight()
{
	return style->barheight + 4;
}

void DockWindow::DragBar::FrameAddSize(Size& sz)
{
	sz += GetHeight();
}

void DockWindow::DragBar::Paint(Draw& d)
{
	Size sz = GetSize();
	
	Image icon = GetOwner().GetIcon();
	bool hasicon = false;
	
	ImageDraw skin(sz.cx, sz.cy);
	ChPaint(skin, 0, 0, GetSize().cx, GetSize().cy, style->barbackground[0]);
	Image imgs = skin;
	((GUI_GlobalStyle() >= GUISTYLE_XP) && (style != &DockCtrlChStyle::StyleClassic())) ?
		// See CtrlsImg for proper rectangle value.
		d.DrawImage(GetRect(), imgs, Rect(4, 4, 2, 14)) :
		d.DrawRect(GetSize(), SColorFace());


	if(!icon.IsNullInstance())
	{
		hasicon = true;
		d.DrawImage(BAR_MARGIN, GetHeight() / icon.GetSize().cy, style->barheight - 8, style->barheight - 8, icon);
	}
	d.DrawText(BAR_MARGIN + (BAR_FILEICON + BAR_SPACEICON) * int(hasicon), GetHeight() / 12, GetOwner().GetLabel(), style->font);
}

//----------------------------------------------------------------------------------------------

TabWindow::TabWindow()
{
	SetType(TYPE_TABWINDOW);
	
	destroyed	= false;
	animating	= false;
	groupwindow = false;
	position	= -1;
	previous	= -1;
	nestlevel	=  0;
	childcount	=  0;
	activectrl	= NULL;

	AddFrame(tabs.SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM));
	
	tabs.WhenSelect 	= THISBACK(OnActiveTab);
	tabs.WhenClose  	= THISBACK(OnTabClose);
	tabs.WhenContext 	= THISBACK(OnTabContextMenu);
	tabs.WhenDrag		= THISBACK(OnTabDrag);
	tabs.WhenActive		= THISBACK(OnTabActive);
}

TabWindow::~TabWindow()
{
	position 	= -1;
	previous	= -1;
	nestlevel	=  0;
	childcount	=  0;
	groupwindow = false;
	activectrl 	= NULL;
}

void TabWindow::Attach(DockableCtrl& ctrl, int makeactive)
{
	if(ctrl.IsTabWindow())
	{
		if(GetBase().DockBase::tabsnested || IsGroupWindow()) AttachNested(ctrl, makeactive);				 
		else AttachNormal(ctrl, makeactive);
	}
	else Attach0(ctrl, makeactive);
}


void TabWindow::AttachNormal(DockableCtrl& ctrl, int makeactive)
{
	TabWindow& tabwindow = reinterpret_cast<TabWindow&>(ctrl);
	int ncount =  tabwindow.ChildCount();

	DockableCtrl* lc = tabwindow.GetChildAt(ncount);

	if(lc)
	{
		bool transfered = false;
		for(int i = 0; i < ncount - 1 ; i++)
		{
			DockableCtrl* c = tabwindow.GetChildAt(i + 1);
			if(c) 
			{
				Attach(*c);
				transfered = true;
			}
		}
		if(transfered)
		{
			Attach(*lc);
		}
		tabwindow.TabWindow::destroyed = true;
	} 
}

void TabWindow::AttachNested(DockableCtrl& ctrl, int makeactive)
{
	TabWindow& child = reinterpret_cast<TabWindow&>(ctrl);
	child.SetNestLevel(GetNestLevel() + 1);
	Attach0(ctrl, makeactive);
}

void TabWindow::Attach0(DockableCtrl& ctrl, int makeactive)
{
	reinterpret_cast<DockWindow&>(ctrl).SetOwnerTab(this);
	ctrl.HideDragBar();
	Add(ctrl.DockingStyle(DOCK_TABBED, STATE_TABBED, 0).SetBase(&GetBase()).SizePos());
	ReposChilds();
	tabs.Add(ctrl, makeactive);
	childcount++;
	
	SetActive(GetActiveTab());
	GetBase().AddCtrlRecord(ctrl);
	RefreshTabWindowLabel(ctrl);
	GetBase().RefreshWidgetLayout();
	GetBase().RefreshPanel();
}


void TabWindow::Detach(DockableCtrl& ctrl)
{
	Size sz = GetBaseTab()->GetSizeHint();
	if(!ctrl.IsFloating()) ctrl.ShowDragBar();
	ctrl.Remove();
	int id = tabs.Find(ctrl);
	if(id >= 0) tabs.Close(id);
	else if(position >= 0) tabs.Close(position);
	childcount--;
	ReposChilds();
	ctrl.SetSizeHint(sz);
	RemoveTabWindow();
	reinterpret_cast<DockWindow&>(ctrl).SetOwnerTab(NULL);
	GetBase().RefreshPanel();
	position = -1;
}

void TabWindow::DetachAll()
{
	if(IsDestroyed()) return;
	Shut();
	DockableCtrl *childctrl = NULL;
	Vector<DockableCtrl*> childs;
	childs.Clear();
	int i = 0;
	int nest = GetNestLevel();
	for(i = 0; i < ChildCount(); i++)
	{
		childctrl = GetChildAt(i + 1);
		if(childctrl) childs.Add(childctrl);
	}
	int ncount = childs.GetCount();
	i = 0;
	for(i = 0; i < (ncount * 2); i++)
	{
		if(i < ncount)	
		{
			childctrl = childs.At(i);
			ASSERT(childctrl);
			if(childctrl->IsTabWindow()) dynamic_cast<TabWindow*>(childctrl)->DetachAll();
		}
		else
		{
			childctrl = childs.At(i - ncount);
			ASSERT(childctrl);
			childctrl->Shut();
		}
	}
	Remove();
	RemoveFrame(tabs);
	SetOwnerTab(NULL);
	childcount = 0;
	destroyed  = true;
	GetBase().RefreshPanel();
}

bool TabWindow::RemoveTabWindow()
{
	bool tabbed = false;
	TabWindow *tabwindow = NULL;
	ReSize();
	Size sizehint = GetBaseTab()->GetSizeHint();
	
	if(ChildCount() == 1)
	{
		DockWindow* lastctrl = reinterpret_cast<DockWindow*>(GetChildAt(1));
		tabs.CloseAll();
		RemoveFrame(tabs);
		if(lastctrl)
		{
			int a = Alignment();
			int s = State();
			int p = Position();
			
			lastctrl->Remove();
			childcount = 0;
			if(IsShut()) lastctrl->Shut();
			if(IsTabbed())
			{
				TabInterface& tabs = GetOwnerTab()->GetTabs();
				GetOwnerTab()->Attach(*lastctrl);
				Shut();
			}
			else if(IsFloating())
			{
				lastctrl->Shut();
				SetRect(GetBaseTab()->GetScreenRect());
				Rect r = GetRect();
				lastctrl->OpenWindow(r);
				lastctrl->DockingStyle(DOCK_NONE, STATE_SHOW, 0);
				Shut();
				lastctrl->SetOwnerTab(NULL);
			}
			else
			{
				Shut();
				lastctrl->ShowDragBar();
				lastctrl->SetSizeHint(sizehint);
				lastctrl->Dock(a, s, p);
				lastctrl->SetOwnerTab(NULL);
			}
			DockingStyle(DOCK_NONE, STATE_SHUT, 0);
			return destroyed = true;
		}
	}
	if(GetActiveCtrl()) RefreshTabWindowLabel(*GetActiveCtrl());
return false;
}

DockableCtrl* TabWindow::GetChildAt(int position)
{
	for(Ctrl* c = Ctrl::GetFirstChild(); c; c = c->GetNext())
	{
		DockableCtrl* ctrl = dynamic_cast<DockableCtrl*>(c);
		if(ctrl) if(ctrl->Position() == position)
			return ctrl; 
	}
	return NULL;
}

void TabWindow::ReposChilds()
{
	int i = 0;
	for(Ctrl* c = Ctrl::GetFirstChild(); c; c = c->GetNext())
	{
		DockableCtrl* ctrl = dynamic_cast<DockableCtrl*>(c);
		if(ctrl) ctrl->Posit(++i);
	}
}

TabWindow* TabWindow::GetBaseTab()
{
	TabWindow *to = GetOwnerTab();
	TabWindow *tb = NULL;
	if(!IsTabbed() || !to) return this;
	while(to) 
	{
		tb = to;
		to = to->GetOwnerTab();
	}
	ASSERT(tb);
	return tb;
}

void TabWindow::OnMenuButton()
{
	DockWindow::OnMenuButton();
	if(IsShut()) DetachAll();
}

void TabWindow::OnShutButton()
{
	DetachAll();
	Shut();
}

void TabWindow::OnAutoHideButton()
{
	DockWindow::OnAutoHideButton();
}

void TabWindow::OnShutWindow()
{
	OnShutButton();
}


void TabWindow::StartTabAnimation()
{
	if(IsTabAnimating()) return;
	childcount++;
	String title = t_("Tabbed Window");

	Size canvassize = GetSize();
	ImageDraw canvas(canvassize.cx, canvassize.cy);
	GetActiveCtrl()->DrawCtrlWithParent(canvas, 0, 0);
	canvas.DrawImage(canvassize, GetStyle()->tabhighlight);
	
	previous = GetActiveTab();
	GetBase().RefreshWidgetLayout();
	tabs.Add(title, true);
	SetLabel(title);

	animimage = canvas;
	animating = true;
}

void TabWindow::StopTabAnimation()
{
	if(!IsTabAnimating()) return;
	tabs.Close(tabs.GetActiveTab());
	childcount--;
	animating = false;
	animimage.Clear();
	if(!RemoveTabWindow())
	{
		SetActive(previous);
		if(GetActiveTab())
		SetLabel(GetTabs()
				.GetTabs()
				.At(GetActiveTab()).dock->GetLabel());
		return;
	}
	position = -1;
}


bool TabWindow::HasCtrlInRange()
{
	Rect r = GetCtrlRect();
	int hrange = r.Width() 	/ 3;
	int vrange = r.Height()	/ 3;
	return Rect(r.left + hrange, r.top + vrange, r.right - hrange, r.bottom - vrange).Contains(GetMousePos());
}

bool TabWindow::HasTabbedDock()
{
	return (bool) ChildCount();
}

int TabWindow::ChildCount()
{
	return childcount;
}

void TabWindow::SetNestLevel(int n)
{
	nestlevel = n;
}

void TabWindow::SetActive(int index)
{
	if(index >= ChildCount()) return;
	if(index != GetActiveTab()) tabs.SetActiveTab(index);

	for(int i = 0; i < tabs.GetTabs().GetCount(); i++)
	{
		TabInterface::Tab t = tabs.GetTabs().At(i);
		if(t.dock)
		{
			if(i == index) t.dock->Ctrl::Show();
			else t.dock->Ctrl::Hide();
		}
	}
	RefreshTabWindowLabel(*this);
}

void TabWindow::SetActive(DockableCtrl& ctrl)
{
}

int TabWindow::GetActiveTab()
{
	return tabs.GetActiveTab();
}

DockableCtrl* TabWindow::GetActiveCtrl()
{
	return GetChildAt(GetActiveTab() + 1);
}

void TabWindow::OnActiveTab()
{
	if(!IsTabAnimating()) SetActive(tabs.GetActiveTab());
}

void TabWindow::OnTabClose(int id, DockableCtrl& ctrl)
{
	position = id;
	ctrl.Shut();
}

void TabWindow::OnTabDrag(int id, DockableCtrl& ctrl)
{
	position = id;
	ctrl.StartWindowDrag();
}

void TabWindow::OnTabContextMenu(int id, DockableCtrl& ctrl)
{
	position = id;
	reinterpret_cast<DockWindow&>(ctrl).OnMenuButton();
}

void TabWindow::OnTabActive(int id, DockableCtrl& ctrl)
{
	position = id;
	RefreshTabWindowLabel(ctrl);
}

void TabWindow::RefreshTabWindowLabel(DockableCtrl& ctrl)
{
	int id = 0;
	AutoHideBar* hidebar 	= NULL;
	TabWindow*	 tabwindow	= NULL;

	if(IsTabbed())
	{
		TabWindow* tabwindow = GetOwnerTab();
		while(tabwindow)
		{
			int id = tabwindow->GetActiveTab();
			if(id < 0) break;
			TabInterface::Tab& sourcetab  = GetTabs().GetTabs().At(GetActiveTab());
			TabInterface::Tab& targettab  =	tabwindow->GetTabs().GetTabs().At(id);
			String& label 	= targettab.name = sourcetab.name;
			Image& icon 	= targettab.icon = sourcetab.icon;
			tabwindow->GetTabs().ReposTabs();
			if(!tabwindow->GetOwnerTab())
			{
				if(tabwindow->IsAutoHidden())
				{
					hidebar = tabwindow->GetOwnerBar();
					id = hidebar->Find((DockableCtrl&)*tabwindow);
					if(id >= 0)
					{
						TabInterface::Tab& t = hidebar->GetTabs().At(id);
						t.name = label;
						t.icon = icon;
						hidebar->ReposTabs();
						hidebar->Refresh();
					}
						
				}
				tabwindow->SetLabel(label);
				tabwindow->SetIcon (icon);
			}
			tabwindow->RefreshLayoutDeep();
			tabwindow = tabwindow->GetOwnerTab();
		}
	}
	else 
	{
		if(IsAutoHidden())
		{
			hidebar = GetOwnerBar();
			id = hidebar->Find((DockableCtrl&)*this);
			if(id >= 0)
			{
				TabInterface::Tab& t = hidebar->GetTabs().At(id);
				t.name = ctrl.GetLabel();
				t.icon = ctrl.GetIcon();
				hidebar->ReposTabs();
				hidebar->Refresh();
			}
		}
		DockableCtrl::SetLabel(ctrl.GetLabel());
		DockableCtrl::SetIcon(ctrl.GetIcon());
		GetTabs().ReposTabs();
		RefreshLayoutDeep();
	}
}

void TabWindow::Paint(Draw& d)
{
	IsTabAnimating() ? d.DrawImage(GetSize(), animimage) :	TopWindow::Paint(d);
}

void TabWindow::Serialize(Stream& s)
{
		int type 		= 0;
		int alignment	= 0;
		int state		= 0;
		int position	= 0;
		int id			= 0;
		int childcount 	= 0;
		int activetab	= 0;
		Size newsize;
		Rect r			= GetRect();		
		
	if(s.IsStoring())
	{
		ReSize();
		type 		= TYPE_TABWINDOW;
		alignment	= GetBaseTab()->Alignment();
		state		= GetBaseTab()->State();
		position	= GetBaseTab()->Position();
		id			= 0;
		newsize		= GetBaseTab()->GetSizeHint();
		childcount 	= ChildCount();
		activetab	= GetActiveTab();
		
		s / type; 
		s / id;
		s / alignment / state / position;
		newsize.Serialize(s);
		s / childcount;
		s / activetab;
		s % groupwindow;
		s / groupname;
		if(!GetOwnerTab() && IsFloating()) 
		{
			r.Serialize(s);
			SerializePlacement(s, true);
		}

		for(int i = 0; i < childcount; i++)
			GetChildAt(i + 1)->Serialize(s);
	}
	if(s.IsLoading())
	{
		TabWindow* tabwindow = GetOwnerTab();
		s / alignment / state / position;
		newsize.Serialize(s);
		s / childcount; 
		s / activetab;
		s % groupwindow;
		s / groupname;

		if(!tabwindow)
		{
			if(alignment == DOCK_NONE  && state != STATE_SHUT)
			{
				r.Serialize(s);
				FloatEx(Rect(r.left, r.top, 10, 10));
				SerializePlacement(s, true);
			}
			else
			{
				Shut();
				SetSize(newsize);
				Dock(alignment, state, position, false);
			}
		}
		else tabwindow->Attach(*this);
		
		for(int i = 0; i < childcount; i++)
		{
			s / type / id;
			if(type == TYPE_DOCKWINDOW)
			{
				DockWindow* child = dynamic_cast<DockWindow*>(GetBase().GetCtrlRecordFromId(id)->ctrl);
				ASSERT(child);
				child->SetOwnerTab(this);
				child->Serialize(s);
			}
			if(type == TYPE_TABWINDOW)
			{
				TabWindow* tabwindow = GetBase().GetPaneFrame(0).AddTabWindow();
				tabwindow->SetOwnerTab(this);
				tabwindow->Serialize(s);
			}
		}
		SetActive(activetab);
	}
}
