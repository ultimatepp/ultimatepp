#include "Docking.h"

namespace Upp {

/*
int DockPane::ClientToPos(const Size& p)
{
	int w = (width>>1)*pos.GetCount()-1;
	return minmax(vert ? 10000 * p.cy / (GetSize().cy - w) : 10000 * p.cx / (GetSize().cx - w), 0, 9999);	
}
*/
void DockPane::CumulativePos(Vector<int>& p) const
{
	for (int i = 1; i < p.GetCount()-1; i++)
		p[i] += p[i-1];		
	p.Top() = 10000;
}

void DockPane::NonCumulativePos(Vector<int>& p) const
{
	for (int i = p.GetCount()-1; i > 0; i--)
		p[i] -= p[i-1];		
}

void DockPane::StartAnimate(int ix, Size sz, bool restore)
{
	if (restore) {
		ASSERT(savedpos.GetCount() == pos.GetCount());
		animpos <<= savedpos;
	}
	else {
		int tsz = ClientToPos(sz);
		animpos <<= pos;
		if (tsz) {
			int msz = GetMinPos(ix);
			if (msz < 10000 && msz+tsz > 10000) {
				Ctrl *c = GetIndexChild(ix);
				int min = ClientToPos(c->GetMinSize());
				if (min < tsz) {
					int std = ClientToPos(c->GetStdSize());
					tsz = (std < tsz) ? std : min;
				}
			}
			SmartReposUp(animpos, ix, tsz);
		}
		else
			SmartReposDown(animpos, ix);
	}
	animtick = 0;	
	SetTimeCallback(-animinterval, THISBACK(AnimateTick), TIMEID_ANIMATE);
	AnimateTick();
}

void DockPane::AnimateTick()
{
	for (int i = 0; i < animpos.GetCount(); i++)
		pos[i] += ((animpos[i] - pos[i]) * animtick) / animmaxticks;
	animtick++;
	if (animtick == animmaxticks)
		EndAnimate();
	else
		Layout();
}

void DockPane::EndAnimate()
{
	animtick = 0;
	pos <<= animpos;
	animpos.Clear();
	KillTimeCallback(TIMEID_ANIMATE);
	Remove(dummy);
/*	if (dummy.GetParent()) {
		pos.Remove(FindIndex(dummy));	
		dummy.Remove();
	}*/
	//FixChildSizes(); 
	Layout();
}

void DockPane::SmartReposDown(Vector<int>& p, int ix) 
{
	int n = 0;
	int dif = 0;
	int maxsize = 0;
	int totalsize = 0;
	int sparesize = 0;
	int cnt = p.GetCount();
	Vector<int> maxpos;
	maxpos.SetCount(cnt);

	if (p.GetCount() == 1) {
		p[0] = 0;
		return;
	}

	// Convert pos to non-cumulative sizes
	NonCumulativePos(p);

	// Find max, and total sizes (in pos units)
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
			maxpos[n] = max(ClientToPos(c->GetMaxSize()), p[n]);
			maxsize += maxpos[n];
			totalsize += p[n];
		}
		n++;
	}
	
	totalsize = max(totalsize, 1);
	
	sparesize = p[ix];
	p[ix] = 0;		
	for (int i = 0; i < cnt; i++)
		p[i] += iscale(sparesize, p[i], totalsize);
		
	// Restrict to max size
	if (maxsize > 10000) {
		totalsize = 0;
		dif = 0;
		for (int i = 0; i < cnt; i++)
			if (i != ix) {
				if (p[i] > maxpos[i]) {
					dif += p[i] - maxpos[i];
					p[i] = maxpos[i];
				}
				else	
					totalsize += p[i];
			}
		// Share out extra spare space to ctrls that are less than maxsize
		while (dif > 1) {
			int sum = 0;
			for (int i = 0; i < cnt; i++) {
				if (i != ix && p[i] < maxpos[i]) { 
					int t = max(1, min((p[i]*dif) / totalsize, maxpos[i] - p[i]));
					p[i] += t;
					sum += t;
				}
			}
			dif -= sum;
		}
	}

	// Do remainder
	dif = Sum(p, 0) - 10000;
	if (dif) 
		p[ix] += dif;
	// Return to cumulative sizes	
	CumulativePos(p);	
}

void DockPane::SmartReposUp(Vector<int>& p, int ix, int sz) 
{
	int n = 0;
	int dif = 0;
	int minsize = 0;
	int maxsize = 0;
	int totalsize = 0;
	int cnt = p.GetCount();
	int resizemin = max(10000 / (cnt*4), PosToClient(30));
	Vector<int> minpos;
	Vector<int> maxpos;
	maxpos.SetCount(cnt);
	minpos.SetCount(cnt);
		
	// Convert pos to non-cumulative sizes
	NonCumulativePos(p);
	
	// Find min, max, and total sizes (in pos units)
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		minpos[n] = min(ClientToPos(c->GetMinSize()), p[n]);
		maxpos[n] = max(ClientToPos(c->GetMaxSize()), p[n]);
		maxsize += maxpos[n];		
		if (n != ix)
			minsize += minpos[n];
		n++;
	}
	totalsize = 10000 - sz - minsize;
	
	for (int i = 0; i < cnt; i++)
		if (i != ix)
			p[i] = minpos[i] + iscale(p[i], totalsize, 10000);
	p[ix] = sz;
		
	// Enforce an absolute minimum size
	dif = 0;
	minsize += minpos[ix];
	for (int i = 0; i < cnt; i++) {
		if (p[i] < resizemin && minpos[i] > p[i]) {
			dif += resizemin - p[i];
			p[i] = resizemin;	
		}
	}
	if (dif) {
		totalsize = 10000 - cnt*resizemin;
		for (int i = 0; i < cnt; i++) {
			int t = p[i] - resizemin;
			if (t > 0)
				p[i] -= (t*dif) / totalsize;
		}
	}

	// Restrict to max size
	if (maxsize > 10000) {
		totalsize = 0;
		dif = 0;
		for (int i = 0; i < cnt; i++)
			if (i != ix) {
				if (p[i] > maxpos[i]) {
					dif += p[i] - maxpos[i];
					p[i] = maxpos[i];
				}
				else	
					totalsize += p[i];
			}
			else if (p[i] < maxpos[i])
				totalsize += p[i];
		// Share out extra spare space to ctrls that are less than maxsize
		while (dif > 1) {
			int sum = 0;
			for (int i = 0; i < cnt; i++) {
				if (p[i] < maxpos[i]) { 
					int t = max(1, min((p[i]*dif) / totalsize, maxpos[i] - p[i]));
					p[i] += t;
					sum += t;
				}
			}
			dif -= sum;
		}
	}	
	
	// Do remainder
	dif = Sum(p, 0) - 10000;
	if (dif)
		p[ix] += dif;	
	// Return to cumulative sizes
	CumulativePos(p);
}

void DockPane::SimpleRepos(Vector<int>& p, int ix, int inc)
{
	int cnt = p.GetCount();
	if (cnt == 1) {
		p[0] = 10000;
		return;
	}

	// Convert pos to non-cumulative sizes
	NonCumulativePos(p);
	
	int n = 0;
	int tsz = 0;
	
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix)
			tsz += p[n];
		n++;
	}
	int dif = tsz - inc;
	int sum = 0;
	p[ix] += inc;
	for (int i = 0; i < cnt; i++) {
		if (i != ix)
			p[i] = (p[i]*dif) / tsz ;
		sum += p[i];
	}	
	dif = sum - 10000;
	if (dif)
		p[ix] += dif;
	// Return to cumulative sizes
	CumulativePos(p);
}

int DockPane::GetMinPos(int notix)
{
	int n = 0;
	int msz = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != notix) msz += ClientToPos(c->GetMinSize());
		n++;
	}
	return msz;	
}

void DockPane::FixChildSizes()
{
	int cnt = pos.GetCount();
	if (!cnt) return;
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}
	int sum = pos[0];
	for (int i = 1; i < cnt; i++)
		sum += pos[i] - pos[i-1];	
	sum -= 10000;
	int rem = sum % cnt;
	sum /= cnt;
	if (sum)
		for (int i = 0; i < cnt; i++)
			pos[i] -= sum*(i+1);
	pos[cnt-1] -= rem;
	ASSERT(pos[cnt-1] == 10000);
}

/*
int DockPane::FindIndex(Ctrl& child)
{
	int ix = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (c == &child) return ix;
		ix++;
	}
	return -1;
}

Ctrl * DockPane::FindCtrl(int ix)
{
	Ctrl *c = GetFirstChild();
	for (int i = 0; i < ix && c; i++)
		c = c->GetNext();
	return c;
}

void DockPane::Swap(Ctrl& child, Ctrl& newctrl)
{
	newctrl.SetRect(child.GetRect());
	Ctrl::AddChildBefore(&newctrl, &child);
	Ctrl::RemoveChild(&child);
}
*/
void DockPane::Dock(Ctrl& newctrl, Size sz, int ps, bool animate, bool save)
{
	if (IsAnimating())
		EndAnimate();
		
	int cnt = pos.GetCount();
	if (!cnt) animate = false;
	int tsz = cnt ? ClientToPos(sz) : 10000;
	ps = min(ps, cnt);
	if (ps >= 0) {
		if (ps && ps == cnt)
			Ctrl::AddChild(&newctrl, GetLastChild());
		else {			
			Ctrl *c = GetFirstChild();
			for (int i = 0; i < ps; i++)
				c = c->GetNext();
			Ctrl::AddChildBefore(&newctrl, c);
		}
		pos.Insert(ps);
		pos[ps] = (ps > 0) ? pos[ps-1] : 0;
	}
	else {
		Splitter::Add(newctrl);
		Splitter::pos.Add(0);
		ps = cnt;
		if (ps > 0) pos[ps] = pos[ps-1];
	}
	if (save) 
		SavePos();
	if (animate)
		StartAnimate(ps, sz, false);
	else {
		if (cnt)
			SmartReposUp(pos, ps, tsz);
		else
			pos[ps] = 10000;
		Layout();
	}
}

void DockPane::Undock(Ctrl& child, bool animate, bool restore)
{
	if (IsAnimating())
		EndAnimate();	
	
	int ix = GetChildIndex(&child);
	if (animate && GetFirstChild() != GetLastChild()) {
		dummy.Remove();
		Swap(child, dummy);
		StartAnimate(ix, Size(0, 0), restore);
	}
	else {
		if (restore) 
			RestorePos();
		SmartReposDown(pos, ix);
		child.Remove();
		pos.Remove(ix);	
		Layout();	
	}
}

Rect DockPane::GetFinalAnimRect(Ctrl& ctrl)
{
	ASSERT(ctrl.GetParent() == this);
	if (!IsAnimating())
		return ctrl.GetRect();
	int ix = GetChildIndex(&ctrl);
	ASSERT(ix >= 0 && ix < animpos.GetCount());
	
	Rect r = GetRect();
	int prev = ix ? animpos[ix-1] : 0;
	int width = GetSplitWidth();
	if (IsHorz()) {
		r.left += PosToClient(prev) + width*(ix);
		r.right = r.left + PosToClient(animpos[ix] - prev);				
	}
	else {
		r.top += PosToClient(prev) + width*(ix);
		r.bottom = r.top + PosToClient(animpos[ix] - prev);
	}
	return r;	
}

DockPane::DockPane()
{
	animtick = 0;
	animinterval = 20; // milliseconds
	animmaxticks = 10;	
	pos.Clear();
}

}
