#include "DockCtrl.h"

PaneFrame& PaneFrame::Attach(DockableCtrl& ctrl)
{
	if(!GetBase().IsSideAllowed(GetType()))
	{
		// if not allowed, then find an allowed pane.
		for(int i = 0; i < 4; i++)
			if(GetBase().IsSideAllowed(i)) 
			{
				GetBase().Dock(ctrl.DockingStyle(i, ctrl.State(), 0));
				break;
			}
	}
	else if(ctrl.IsTabbed()) 
		return AttachAsTab(ctrl);
	else if(ctrl.IsAutoHidden())
		return AttachAsAuto(ctrl);
	else if(ctrl.IsDocked())
		return AttachAsDock(ctrl);
	return *this;
}

PaneFrame& PaneFrame::AttachAsDock(DockableCtrl& ctrl)
{
	if(DnDHasSource())
	{
		int position = dndpos;
		DnDSourceoutofRange();
		if(dockcontainer.IsAnimating()) dockcontainer.StopAnimation();
	}
	if(ctrl.IsDocked() && !ctrl.IsHidden())	ShowFrame(ctrl.GetSizeHint());
	dockcontainer.AddChildDock(ctrl, dndpos ? dndpos : ctrl.Position());
	dndpos = 0;
	return RefreshPaneFrame();
}

PaneFrame& PaneFrame::AttachAsTab(DockableCtrl& ctrl)
{
	int position = ctrl.Position();
	if(DnDHasSource())
	{
		position = dndpos;
		DnDSourceoutofRange();
		if(HasTabWindow(GetActiveTabWindow()))
			if(GetActiveTabWindow()->IsTabAnimating())
				GetActiveTabWindow()->StopTabAnimation();
	}
	else
	{
		DockableCtrl* targetctrl = dockcontainer.GetChildAt(position);

		if(!targetctrl)
		{
			int cc = dockcontainer.GetChildCount();
			if(cc == 0) 
			{
				ctrl.DockingStyle(GetType(), DockableCtrl::STATE_SHOW, 0);
				return AttachAsDock(ctrl);
			}
			else
			{
				targetctrl = dockcontainer.GetChildAt(cc);
				if(targetctrl) position = targetctrl->Position();
				else return *this;
			}
		}
	}
	
	AddtoTabWindow(ctrl.Posit(position));
	SetActiveTabWindow(NULL);
	dndpos = 0;
	RefreshTabWindowList();
	return RefreshPaneFrame();
}

PaneFrame& PaneFrame::AttachAsAuto(DockableCtrl& ctrl)
{
	bool vertical = GetType() == LEFT || GetType() == RIGHT;
	AutoHideBar& bar = GetBase().GetHideBar(GetType());
	Size sz = ctrl.GetSizeHint();
	Size newsz;
	Size hidesz;
	hidesz += GetBase().GetHideBar(GetType()).GetHeight() + 4;
	
	if(vertical) {
		if (sz.cx < hidesz.cx) {
			newsz = Size(maxsize, sz.cy);
			ctrl.SetSizeHint(newsz);
		}
	} else {
		if (sz.cy < hidesz.cy) {
			newsz = Size(sz.cx, maxsize);
			ctrl.SetSizeHint(newsz);
		}
	}
	bar.Attach(ctrl);
	GetBase().RefreshWidgetLayout();
	return RefreshPaneFrame();
}

PaneFrame& PaneFrame::Detach(DockableCtrl& ctrl)
{
	if(!ctrl.IsDocked()) return *this;
	if(HasChild(ctrl) && !ctrl.IsShut()) 
		dockcontainer.RemoveChildDock(ctrl);
	if(!dockcontainer.HasChild())  
		HideFrame(); 
	return RefreshPaneFrame();		
}

DockableCtrl* PaneFrame::GetChild(int position)
{
	return dockcontainer.GetChildAt(position);
}

int PaneFrame::GetCount()
{
	return dockcontainer.GetChildCount();
}

bool PaneFrame::HasChild(DockableCtrl& ctrl)
{
	if(dockcontainer.HasChild())
		for(int i = 1; i <= dockcontainer.GetCount(); i++)
			if(&ctrl == dockcontainer.GetChildAt(i))	
				return true;
	return false;
}

void PaneFrame::ShowFrame()
{
	SetSize(stdsize <= 4 ? maxsize : stdsize); 
	RefreshPaneFrame().Enable();
}

void PaneFrame::ShowFrame(Size sz)
{
	bool horizontal = GetType() == LEFT || GetType() == RIGHT;
	
	if(!dockcontainer.HasChild())
	{
		if(horizontal)
			SetSize((sz.cx > 4 && sz.cx < maxsize) ? sz.cx : maxsize);
		else
			SetSize((sz.cy > 4 && sz.cy < maxsize) ? sz.cy : maxsize);
	}
	else SetSize(size);
	RefreshPaneFrame();
	if(!IsEnabled()) Enable();  
}

void PaneFrame::HideFrame()
{
	stdsize = GetSize();
	SetSize(0);
	RefreshPaneFrame().Disable();
}

void PaneFrame::LockFrame()
{
	if(IsLocked()) return;
}

void PaneFrame::UnlockFrame()
{
	if(!IsLocked()) return;
}
Vector<TabWindow*>& PaneFrame::GetTabWindows()
{
	return tabwindows;
}

TabWindow* PaneFrame::AddTabWindow()
{
	Vector<TabWindow*>& tl = GetBase().GetTabWindows();
	TabWindow* tabwindow = NULL;
	tl.Add(tabwindow = new TabWindow());
	tabwindow->SetStyle(*GetBase().GetStyle());
	tabwindow->SetBase(&GetBase());
	GetBase().RefreshTabWindowList();
	return tabwindow;
}

TabWindow* PaneFrame::AddTabWindow(DockableCtrl& ctrl)
{
	Size tabnewsize;
	Vector<TabWindow*>& tl = GetBase().GetTabWindows();
	
	int position = ctrl.Position();
	if(position == 0) ++position;
	TabWindow *tabwindow = FindTabWindow(position);
	if(tabwindow) return activetabwindow = tabwindow;
	tl.Add(tabwindow = new TabWindow());
	tabwindow->SetStyle(*GetBase().GetStyle());
	tabwindow->SetBase(&GetBase()).DockingStyle(GetType(), DockableCtrl::STATE_SHOW, position);

	if(dockcontainer.HasChild())
	{
		DockableCtrl* child = dockcontainer.GetChildAt(position);
		if(child)
		{
			child->Shut();
			tabwindow->Attach(*child);
			tabnewsize = child->GetSizeHint();
		}
	}
	if(!tabwindow->IsTabbed())
		if(tabwindow->IsDocked() && !tabwindow->IsHidden()) ShowFrame(tabnewsize);
		dockcontainer.AddChildDock(*tabwindow, (dndpos == 0 || tabwindow->Position()) ? tabwindow->Position() : dndpos);
	GetBase().RefreshTabWindowList();
	return activetabwindow = tabwindow;	
}

void PaneFrame::RemoveTabWindow(TabWindow* tabwindow)
{
	//TODO: Remove.
}

void PaneFrame::RemoveTabWindow(int position)
{
	//TODO: Remove.
}

void PaneFrame::RemoveTabWindows()
{
	//TODO: Remove.
}

void PaneFrame::RefreshTabWindowList()
{
	GetBase().RefreshTabWindowList();
	SetActiveTabWindow(NULL);
}

void PaneFrame::AddtoTabWindow(DockableCtrl& ctrl)
{
	TabWindow* tabwindow 	=  FindTabWindow(ctrl.Position());
	if(!tabwindow) tabwindow = AddTabWindow(ctrl);
	tabwindow->Attach(ctrl);
}

void PaneFrame::RemovefromTabWindow(DockableCtrl& ctrl)
{
	TabWindow* tabwindow = FindTabWindow(ctrl.Position());
	if(!tabwindow) return;
	ctrl.Shut();
	if(tabwindow && tabwindow->IsShut())
		delete tabwindow;
}

void PaneFrame::RemovefromTabWindow(int position)
{
}

TabWindow* PaneFrame::FindTabWindow(int position)
{
	if(position <= 0) return NULL;
	TabWindow * tabwindow = NULL;

	Vector<TabWindow*>& tl = GetBase().GetTabWindows();	
		
	for(int i = 0; i < tl.GetCount(); i++)
	{
		tabwindow = tl.At(i);
		if(tabwindow->Position() == position && 
		tabwindow->Alignment() == GetType() && 
		tabwindow->State() != DockableCtrl::STATE_TABBED)
		return tabwindow;
	}
	return NULL;
}

TabWindow* PaneFrame::FindTabWindow(DockableCtrl& ctrl)
{
	return NULL;
}

TabWindow* PaneFrame::GetActiveTabWindow()
{
	return activetabwindow;
}

void PaneFrame::SetActiveTabWindow(TabWindow* tabwindow)
{
	activetabwindow = tabwindow;
}

bool PaneFrame::HasTabWindow(TabWindow* tabwindow)
{
	Vector<TabWindow*>& tl = GetBase().GetTabWindows();
	for(int i = 0; i < tl.GetCount(); i++)
		if(tabwindow == tl.At(i)) return true;
	return false;
}


bool PaneFrame::HasCtrlInRange(DockableCtrl& ctrl, Point p)
{
	if(!GetBase().IsSideAllowed(GetType())) return false;
	Ctrl* first = NULL;
	Ctrl* last  = NULL;

	if(!dockcontainer.HasChild())
		return CalculateEmptyRange(ctrl, p);	
	if(!(first = dockcontainer.GetFirstChild()) || !(last = dockcontainer.GetLastChild())) 
		return false;
	if((first == last) && (first != dockcontainer.GetAnimationCtrl()))
		return CalculateCtrlRange(ctrl, *first, p);
	else
	{
		Ctrl* ctrl0 = first;
		while(ctrl0)
		{
			if(ctrl0 != dockcontainer.GetAnimationCtrl())
				if(CalculateCtrlRange(ctrl, *ctrl0, p)) return true;
			ctrl0 = ctrl0->GetNext();
		}
	}
	return false;
}

bool PaneFrame::CalculateCtrlRange(DockableCtrl& t, Ctrl& c, Point p)
{
	DockWindow* ctrl = reinterpret_cast<DockWindow*>(&c);
	ASSERT(ctrl);
	Rect r = ctrl->GetCtrlRect();
	int	 position 	= ctrl->Position();
	int	 hrange  	= r.GetWidth()  / 3;
	int	 vrange		= r.GetHeight() / 3;

	if(!DnDHasSource())
	{
		if(!GetActiveTabWindow() || 
			(GetActiveTabWindow() && 
			!GetActiveTabWindow()->IsTabAnimating()))
		{
			if(Rect(r.left + hrange, r.top + vrange, r.right - hrange, r.bottom - vrange).Contains(p))
			{
				DnDSourceinRange();
				StartTabWindowAnimation(*ctrl, position);
				t.SetDropTarget(GetType(), DockableCtrl::STATE_TABBED);
				return true;
			}
		}
		if(dockcontainer.IsVert() && 
			!GetActiveTabWindow() && 
			!dockcontainer.IsAnimating())
		{
			if(Rect(r.left, r.top, r.right, r.top + vrange).Contains(p) || 
		   	Rect(r.left, r.bottom - vrange, r.right, r.bottom).Contains(p))
		    {
				DnDSourceinRange();
				StartPaneFrameAnimation(*ctrl, p.y > r.bottom - vrange ? ++position : position); 
				t.SetDropTarget(GetType(), DockableCtrl::STATE_SHOW);
				return true;
		    }
		}
		if(dockcontainer.IsHorz() && 
			!GetActiveTabWindow() && 
			!dockcontainer.IsAnimating())
		{
			if(Rect(r.left, r.top, r.left + hrange, r.bottom).Contains(p) || 
			Rect(r.right - hrange, r.top, r.right, r.bottom).Contains(p))
			{
				DnDSourceinRange();
				StartPaneFrameAnimation(*ctrl, p.x < r.left + hrange ? position : ++position);
				t.SetDropTarget(GetType(), DockableCtrl::STATE_SHOW);
		   		return true;
		  	}
		}
		return false;
	}
	if(DnDHasSource())
	{
		if(dockcontainer.IsHorz() && dockcontainer.IsAnimating() && 
			dockcontainer.AnimationArea().Contains(p)) return true;
		if(dockcontainer.IsVert())
		{
			r = dockcontainer.AnimationArea();
			r.bottom += 5;
			r.top 	 -= 5;
			if(dockcontainer.IsAnimating() && r.Contains(p)) return true;
		}

		if(GetActiveTabWindow())
			if(GetActiveTabWindow()->IsTabAnimating() && 
				GetActiveTabWindow()->HasCtrlInRange()) return true;
		DnDSourceoutofRange(); 
		DnDAnimate(position);
		return false;
			
	}
	return false;
}

bool PaneFrame::CalculateEmptyRange(Ctrl& c, Point p)
{
	DockWindow* ctrl = reinterpret_cast<DockWindow*>(&c);
	ASSERT(ctrl);
	Rect r = GetScreenRect();
	if(!DnDHasSource() && !dockcontainer.HasChild() && !dockcontainer.IsAnimating())
	{
		bool state = false;
		switch(GetType())
		{
			case LEFT:
				((p.x < r.right + 20) && (p.x > r.left) && (p.y > r.top && p.y < r.bottom)) ?
					state = true : state = false; break;
			case RIGHT:
				((p.x > r.left - 20) && (p.x < r.right) && (p.y > r.top && p.y < r.bottom)) ?
					state = true : state = false; break;
			case TOP:
				((p.y < r.bottom + 20) && (p.y > r.top) && (p.x > r.left && p.x < r.right))  ? 
					state = true : state = false; break;
			case BOTTOM:
				((p.y > r.top - 20) && (p.y < r.bottom) && (p.x > r.left && p.x < r.right)) ?
					state = true : state = false; break;
		}
		if(state) 
		{
			ShowFrame(ctrl->GetSizeHint());
			DnDSourceinRange(); 
			StartPaneFrameAnimation(reinterpret_cast<DockableCtrl&>(c), 1);
			ctrl->SetDropTarget(GetType(), DockableCtrl::STATE_SHOW);
			return true;
		}
		return false;
	}

	if(DnDHasSource())
	{
	
		if(dockcontainer.IsAnimating() && dockcontainer.AnimationArea().Contains(p)) return true;
		HideFrame();
		DnDSourceoutofRange(); 
		StopPaneFrameAnimation();
		return false;
	}
	return false;
}


//----------------------------------------------------------------------------------------------

void PaneFrame::StartPaneFrameAnimation(DockableCtrl& ctrl, int position)
{
	if(DnDHasSource() && !dockcontainer.IsAnimating())
	{
		if(HasTabWindow(GetActiveTabWindow()))
	   		if(GetActiveTabWindow()->IsTabAnimating())
	   			GetActiveTabWindow()->StopTabAnimation();
		dockcontainer.StartAnimation(position);
		dndpos = position;
	}
	RefreshPaneFrame();
}

void PaneFrame::StartTabWindowAnimation(DockableCtrl& ctrl, int position)
{
	if(DnDHasSource() && &ctrl && (ctrl.Position() == position)) 
	{
		RefreshTabWindowList();
		if(!HasTabWindow(GetActiveTabWindow())) 
				activetabwindow = AddTabWindow(ctrl);
		if(!GetActiveTabWindow()->IsTabAnimating())
		{
			if(dockcontainer.IsAnimating())	dockcontainer.StopAnimation();
			GetActiveTabWindow()->StartTabAnimation();
		}
		dndpos = position;
	}
	RefreshPaneFrame();
}

void PaneFrame::StopPaneFrameAnimation()
{
	if(!DnDHasSource() && dockcontainer.IsAnimating())
		dockcontainer.StopAnimation();
	RefreshPaneFrame();
}

void PaneFrame::StopTabWindowAnimation()
{
	if(!DnDHasSource() && HasTabWindow(GetActiveTabWindow()))
		if(GetActiveTabWindow()->IsTabAnimating())
		{
			GetActiveTabWindow()->StopTabAnimation();
			RefreshTabWindowList();
		}
	RefreshPaneFrame();
}



void PaneFrame::DnDAnimate(int position)
{
	if(!(dockcontainer.IsAnimating() && dockcontainer.GetAnimationCtrl()->GetScreenRect().Contains(GetMousePos())))
		StopPaneFrameAnimation();
	if(HasTabWindow(GetActiveTabWindow()))
		if(!(GetActiveTabWindow()->IsTabAnimating() && GetActiveTabWindow()->HasCtrlInRange()))
			StopTabWindowAnimation();
}


//----------------------------------------------------------------------------------------------

PaneFrame& PaneFrame::SetLayout(DockBase* basectrl, int alignment, int _maxsize)
{
	ASSERT(basectrl);
	base = basectrl;
	MaxSize(_maxsize);
	
	switch(alignment)
	{
		case LEFT:
			Left(dockcontainer.SizePos(), 0);
			break;
		case TOP:
			Top(dockcontainer.SizePos(), 0);
			break;
		case RIGHT:
			Right(dockcontainer.SizePos(), 0);
			break;
		case BOTTOM:
			Bottom(dockcontainer.SizePos(), 0);
			break;
	}
	HideFrame();
	created = true;
	return *this;
}

void PaneFrame::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void PaneFrame::FrameRemove()
{
	this->Ctrl::Remove();
}

void PaneFrame::FrameAddSize(Size& sz)
{
	if(IsShown()) 
		if(type == LEFT || type == RIGHT)  
		{
			sz.cx += size;
			maxsize = sz.cx / 3;
		}
		else
		{
			sz.cy += size;
			maxsize = sz.cy / 3;
		}
}

int  PaneFrame::BoundSize()
{
	int maxsize = max(0, (type == LEFT || type == RIGHT ? parentsize.cx : parentsize.cy) - sizemin);
	return IsShown() ? max(1, minmax(size, minsize - 1, maxsize)) : 0;
}

void PaneFrame::FrameLayout(Rect& r)
{
	Rect rr = r;
	parentsize = r.GetSize();
	int sz = BoundSize();
	switch(type) {
	case LEFT:
		r.left += sz;
		rr.right = r.left;
		break;
	case RIGHT:
		r.right -= sz;
		rr.left = r.right;
		break;
	case TOP:
		r.top += sz;
		rr.bottom = r.top;
		break;
	case BOTTOM:
		r.bottom -= sz;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);
}

void PaneFrame::Paint(Draw& draw)
{
	draw.DrawRect(
		Ctrl::GetSize(),
		HasCapture() ? GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow
		             : SColorFace()
	);
}

void PaneFrame::LeftDown(Point p, dword)
{
	SetCapture();
	Refresh();
	ref = GetMousePos();
	size0 = BoundSize();
}

void PaneFrame::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	p = GetMousePos();
	switch(type) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	RefreshParentLayout();
}

void PaneFrame::LeftUp(Point p, dword keyflags)
{
	Refresh();
}

Image PaneFrame::CursorImage(Point p, dword keyflags)
{
	return type == LEFT || type == RIGHT ? Image::SizeHorz() : Image::SizeVert();
}

PaneFrame& PaneFrame::Set(Ctrl& c, int _size, int _type)
{
	while(GetFirstChild())
		GetFirstChild()->Remove();
	type = _type;
	size = _size;
	Add(c.SizePos());
	switch(type) {
	case LEFT: c.HSizePos(0, 4); break;
	case RIGHT: c.HSizePos(4, 0); break;
	case TOP: c.VSizePos(0, 4); break;
	case BOTTOM: c.VSizePos(4, 0); break;
	}
	RefreshParentLayout();
	return *this;
}

PaneFrame::PaneFrame()
{
	type = LEFT;
	size = size0 = 1;
	minsize = 0;
	maxsize = 0;
	stdsize = 0;
	sizemin = 0;
	dndpos	= 0;
	ncount	= 0;
	locked 	= false;
	created = false;
	activetabwindow = NULL; 
}

PaneFrame::~PaneFrame()
{
}
