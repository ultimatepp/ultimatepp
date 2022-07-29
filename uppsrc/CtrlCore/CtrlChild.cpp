#include "CtrlCore.h"

namespace Upp {

#define LLOG(x)   // DLOG(x)

void Ctrl::DeleteTop()
{
	if(top && utop) {
		delete utop;
		utop = nullptr;
		top = false;
	}
}

void Ctrl::SetParent(Ctrl *parent)
{
	if(top && utop) {
		Close();
		DeleteTop(); // if Close did not work as expected...:
	}
	uparent = parent;
	top = false;
}

bool Ctrl::IsDHCtrl() const {
	return dynamic_cast<const DHCtrl *>(this);
}

void Ctrl::AddChild(Ctrl *q, Ctrl *p)
{
	GuiLock __;
	ASSERT(q);
	LLOG("Add " << UPP::Name(q) << " to: " << Name());
	if(p == q) return;
	bool updaterect = true;
	Ctrl *qparent = q->GetParent();
	if(qparent) {
		ASSERT(!q->inframe);
		if(qparent == this) {
			RemoveChild0(q);
			updaterect = false;
		}
		else
			qparent->RemoveChild(q);
	}
	
	if(children) {
		if(!p) p = GetLastChild();
		ASSERT(p->GetParent() == this);
		q->prev_sibling = p;
		q->next_sibling = p->next_sibling;
		p->next_sibling->prev_sibling = q;
		p->next_sibling = q;
	}
	else {
		ASSERT(!p);
		children = q->next_sibling = q->prev_sibling = q;
	}
	q->SetParent(this);

	q->CancelModeDeep();
	if(updaterect)
		q->UpdateRect();
	ChildAdded(q);
	q->ParentChange();
	if(updaterect && GetTopCtrl()->IsOpen())
		q->StateH(OPEN);
}

void Ctrl::AddChild(Ctrl *child)
{
	AddChild(child, GetLastChild());
}

void Ctrl::AddChildBefore(Ctrl *child, Ctrl *insbefore)
{
	if(insbefore)
		AddChild(child, insbefore->GetPrev());
	else
		AddChild(child);
}

void  Ctrl::RemoveChild0(Ctrl *q)
{
	GuiLock __;
	ChildRemoved(q);
	if(!q->GetParent()) return; // ChildRemoved can remove q
	q->DoRemove();
	if(!q->GetParent()) return; // DoRemove can remove q
	q->SetParent(NULL);

	if(q == children) {
		children = q->next_sibling;
		if(children == q)
			children = NULL;
	}
	
	q->prev_sibling->next_sibling = q->next_sibling;
	q->next_sibling->prev_sibling = q->prev_sibling;
	q->next_sibling = q->prev_sibling = NULL;
}

void  Ctrl::RemoveChild(Ctrl *q)
{
	GuiLock __;
	if(q->GetParent() != this) return;
	q->RefreshFrame();
	RemoveChild0(q);
	q->ParentChange();
	if(GetTopCtrl()->IsOpen())
		q->StateH(CLOSE);
}

void  Ctrl::Remove()
{
	GuiLock __;
	Ctrl *parent = GetParent();
	if(parent)
		parent->RemoveChild(this);
}

int Ctrl::GetChildIndex(const Ctrl *child) const
{
	GuiLock __;
	int i = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if(c == child) return i;
		i++;
	}
	return -1;
}

int Ctrl::GetChildCount() const
{
	GuiLock __;
	int n = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		n++;
	return n;
}

Ctrl * Ctrl::GetIndexChild(int ii) const
{
	GuiLock __;
	Ctrl *c = GetFirstChild();
	for(int i = 0; i < ii && c; i++)
		c = c->GetNext();
	return c;
}

int Ctrl::GetViewChildIndex(const Ctrl *child) const
{
	GuiLock __;
	int i = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		if(!c->InFrame()) {
			if(c == child) return i;
			i++;
		}
	return -1;
}

int Ctrl::GetViewChildCount() const
{
	GuiLock __;
	int n = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		if(!c->InFrame())
			n++;
	return n;
}

Ctrl * Ctrl::GetViewIndexChild(int ii) const
{
	GuiLock __;
	int i = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		if(!c->InFrame()) {
			if(i == ii)
				return c;
			i++;
		}
	return NULL;
}

bool Ctrl::HasChild(Ctrl *q) const
{
	GuiLock __;
	return q && q->GetParent() == this;
}

bool Ctrl::HasChildDeep(Ctrl *q) const
{
	GuiLock __;
	while(q && q->IsChild()) {
		Ctrl *qparent = q->GetParent();
		if(qparent == this) return true;
		q = qparent;
	}
	return false;
}

static bool IterateFocusFw(Ctrl *ctrl, bool noframe, bool init, bool all)
{
	LLOG("IterateFocusFw(" << UPP::Name(ctrl) << ")");
	while(ctrl) {
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled()) {
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->SetFocus();
					return true;
				}
				if((!init || ctrl->IsInitFocus()) && ctrl->SetWantFocus())
					return true;
			}
			if(IterateFocusFw(ctrl->GetFirstChild(), noframe, init, all))
				return true;
		}
		ctrl = ctrl->GetNext();
	}
	return false;
}

bool Ctrl::IterateFocusForward(Ctrl *ctrl, Ctrl *top, bool noframe, bool init, bool all)
{
	GuiLock __;
	LLOG("IterateFocusForward(" << UPP::Name(ctrl) << ", top " << UPP::Name(top) << ", noframe " << noframe << ", init " << init << ")");
	if(!ctrl) return false;
	if(IterateFocusFw(ctrl->GetFirstChild(), noframe, init, all))
		return true;
	if(ctrl->GetNext() && IterateFocusFw(ctrl->GetNext(), noframe, init, all))
		return true;
	while(ctrl->GetParent() != top && (ctrl = ctrl->GetParent()) != NULL)
		if(IterateFocusFw(ctrl->GetNext(), noframe, init, all))
			return true;
	return false;
}

static bool IterateFocusBw(Ctrl *ctrl, bool noframe, bool all)
{
	while(ctrl) {
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled()) {
			if(IterateFocusBw(ctrl->GetLastChild(), noframe, all))
				return true;
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->SetFocus();
					return true;
				}
				if(ctrl->SetWantFocus())
					return true;
			}
		}
		ctrl = ctrl->GetPrev();
	}
	return false;
}

bool Ctrl::IterateFocusBackward(Ctrl *ctrl, Ctrl *top, bool noframe, bool all)
{
	GuiLock __;
	if(!ctrl || ctrl == top) return false;
	if(IterateFocusBw(ctrl->GetPrev(), noframe, all))
		return true;
	while(ctrl->GetParent() != top) {
		ctrl = ctrl->GetParent();
		if(ctrl->SetWantFocus())
			return true;
		if(IterateFocusBw(ctrl->GetPrev(), noframe, all))
			return true;
	}
	return false;
}

Ctrl *Ctrl::GetTopCtrl()
{
	GuiLock __;
	Ctrl *q = this;
	for(;;) {
		Ctrl *qparent = q->GetParent();
		if(!qparent)
			return q;
		q = qparent;
	}
}

const Ctrl *Ctrl::GetTopCtrl() const      { return const_cast<Ctrl *>(this)->GetTopCtrl(); }
const Ctrl *Ctrl::GetOwner() const        { return const_cast<Ctrl *>(this)->GetOwner(); }
Ctrl       *Ctrl::GetTopCtrlOwner()       { return GetTopCtrl()->GetOwner(); }
const Ctrl *Ctrl::GetTopCtrlOwner() const { return GetTopCtrl()->GetOwner(); }

Ctrl       *Ctrl::GetOwnerCtrl()          { GuiLock __; return !IsChild() && top && utop ? utop->owner : NULL; }
const Ctrl *Ctrl::GetOwnerCtrl() const    { return const_cast<Ctrl *>(this)->GetOwnerCtrl(); }

TopWindow *Ctrl::GetTopWindow()
{
	GuiLock __;
	Ctrl *q = this;
	while(q) {
		q = q->GetTopCtrl();
		TopWindow *w = dynamic_cast<TopWindow *>(q);
		if(w) return w;
		q = q->GetOwner();
	}
	return NULL;
}

const TopWindow *Ctrl::GetTopWindow() const
{
	return const_cast<Ctrl *>(this)->GetTopWindow();
}

TopWindow *Ctrl::GetMainWindow()
{
	GuiLock __;
	Ctrl *q = GetTopCtrl();
	for(;;) {
		Ctrl *w = q->GetOwner();
		if(!w)
			return dynamic_cast<TopWindow *>(q);
		q = w;
	}
}

const TopWindow *Ctrl::GetMainWindow() const
{
	return const_cast<Ctrl *>(this)->GetMainWindow();
}

}
