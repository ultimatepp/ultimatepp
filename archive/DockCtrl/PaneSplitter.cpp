#include "DockCtrl/DockCtrl.h"


PaneSplitter::PaneSplitter()
{
	animating = false;
	SetAnimationType(PANEANIMATION);		// by default.
	x = 0;
	y = 0;
}

PaneSplitter::~PaneSplitter()
{
	animating = false;
}

void PaneSplitter::AddChildDock(DockableCtrl& dock)
{
	Splitter::Add((Ctrl &)dock.SizePos());
	ReposChilds();
}

void PaneSplitter::AddChildDock(DockableCtrl& dock, int position)
{
	int count = GetCount();
	if(!HasChild() || position > count) AddChildDock(dock);
	else AddChildBefore((Ctrl*) &dock, (Ctrl *) GetChildAt(position));
	ReposChilds();
}

void PaneSplitter::RemoveChildDock(DockableCtrl& dock)
{
	RemoveChildDock(dock.Position());
}

void PaneSplitter::RemoveChildDock(int position)
{
	DockableCtrl* ctrl = GetChildAt(position);
	if(ctrl) ctrl->Remove();
	ReposChilds();
}

void PaneSplitter::RemoveChilds()
{
	if(!HasChild()) return;
	for(int i = 1; i < GetCount(); i++)
		GetChildAt(i)->Shut();
}

void PaneSplitter::ReposChilds()
{
	if(!HasChild()) return;
	int i = 1;
	DockableCtrl* ctrl = (DockableCtrl*) GetFirstChild();
	ctrl->Posit(i);
	while(ctrl = (DockableCtrl*) ctrl->GetNext())
		ctrl->Posit(++i);
	if(IsOpen()) 
		Layout();
}

DockableCtrl* PaneSplitter::GetChildAt(int position)
{
	if(HasChild())
	{
		Ctrl* ctrl = GetFirstChild();
		while(ctrl)
		{
			if(ctrl != &animationctrl)
				if(((DockableCtrl*)ctrl)->Position() == position) 
					return (DockableCtrl*) ctrl;
			ctrl = ctrl->GetNext();
		}
	}
	return NULL;
}

int PaneSplitter::GetChildCount()
{
	int childcount = GetCount();
	return IsAnimating() ?  childcount - 1 : childcount;
}

bool PaneSplitter::HasChild()
{
	return GetChildCount();
}

void PaneSplitter::StartAnimation(int position)
{	
	if(animationctrl.IsOpen()) return;
	if(!HasChild() || position > GetCount()) 
		Splitter::Add(animationctrl.SizePos());
	else 
		AddChildBefore(&animationctrl.SizePos(), (Ctrl *) GetChildAt(position));

	animating = true;

	if(animationtype == TABANIMATION) 
	{
		Ctrl* ctrl = GetChildAt(GetZoom() + 1);
		if(ctrl)
		{
			const DockCtrlChStyle::Style& s = DockCtrlChStyle::StyleDefault();
			Size sz = animationctrl.GetSize();
			ImageDraw img(sz.cx, sz.cy);
			ctrl->DrawCtrlWithParent(img, 0, 0);
			img.DrawImage(sz,s.tabhighlight); 
			Image i = img;
			animationctrl.SetAnimImage(i);
		}
	}
	Animate(position);
}

void PaneSplitter::StartAnimation(DockableCtrl& dock, int position)
{
	StartAnimation(position);
}

void PaneSplitter::StopAnimation()
{
   	if(!animationctrl.IsOpen()) return;
	animationctrl.Type(animationtype);
   	animationctrl.Remove();
   	animating = false; 
}

void PaneSplitter::Animate(int position)
{
}

void PaneSplitter::AnimateCallback()
{
}

void PaneSplitter::Layout()
{
	Splitter::Layout();
}

void PaneSplitter::AnimationCtrl::Paint(Draw& d)
{ 
	if(ctrltype != PaneSplitter::TABANIMATION)
		d.DrawRect(GetSize(), GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow); 
	else
		d.DrawImage(GetSize(), image);
}

//----------------------------------------------------------------------------------------------

void PaneSplitter::AnimationCtrl::AnimateHorz()
{
	if(!IsVisible()) Show();
	Ctrl::LeftPos(0, 0).Layout();
}
void PaneSplitter::AnimationCtrl::AnimateVert()
{
	if(!IsVisible()) Show();
	Ctrl::TopPos(0, 0).Layout();
}
