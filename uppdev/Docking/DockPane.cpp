$uvs: PENDING CONFLICT
#include "DockPane.h"

int DockPane::ClientToPos(const Size &p)
{
	int w = (width>>1)*pos.GetCount()-1;
	return minmax(vert ? 10000 * p.cy / (GetSize().cy - w) : 10000 * p.cx / (GetSize().cx - w), 0, 9999);
}

$uvs: REPOSITORY INSERT
void DockPane::CumulativePos(Vector<int> &p) const
{
	for (int i = 1; i < p.GetCount(); i++)
		p[i] += p[i-1];
}

void DockPane::NonCumulativePos(Vector<int> &p) const
{
	for (int i = p.GetCount()-1; i > 0; i--)
		p[i] -= p[i-1];
}

void DockPane::StartAnimate(int ix, Size sz, bool restore)
{
	if (restore) {
		ASSERT(savedpos.GetCount() == pos.GetCount());
		animpos <<= savedpos;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
void DockPane::StartAnimate(int ix, Size sz)
{
	int tsz = ClientToPos(sz) - NormalPos(ix);
	int msz = GetMinPos(ix);

	if (msz < 10000 && msz+tsz > 10000) {
		Ctrl *c = FindCtrl(ix);
		int min = ClientToPos(c->GetMinSize());
		if (min < tsz) {
			int std = ClientToPos(c->GetStdSize());
			tsz = (std < tsz) ? std : min;
		}
	}

	animix = ix;
	animtick = 0;
	animinc = tsz / animmaxticks;
	SmartRepos(ix, tsz % animmaxticks);

	SetTimeCallback(-animinterval, THISBACK(AnimateTick), TIMEID_ANIMATE);
}

void DockPane::AnimateTick()
{
	SmartRepos(animix, animinc);
	animtick++;
	if (animtick == animmaxticks)
		EndAnimate();
	Layout();
}

void DockPane::EndAnimate()
{
	animix = -1;
	animtick = 0;
	KillTimeCallback(TIMEID_ANIMATE);
	if (dummy.GetParent())
		Undock(dummy, false);
	else {
		FixChildSizes();
		Layout();
	}
}

void DockPane::SmartRepos(int ix, int inc)
{
	int cnt = pos.GetCount();
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];

	int n = 0;
	int msz = 0;
	int tsz = 0;
	Vector<int>minpos;
	minpos.SetCount(cnt);

	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
			minpos[n] = min(ClientToPos(c->GetMinSize()), pos[n]);
			msz += minpos[n];
			tsz += pos[n];
		}
		n++;
	}
	int dif = tsz - inc - msz;
	tsz -= msz;

	pos[ix] += inc;
	if (tsz != 0 && dif != 0) {
		if (tsz <= 0) dif = -dif;
		int isz = pos[ix];
		for (int i = 0; i < cnt; i++) {
			if (i != ix)
				pos[i] = minpos[i] + (dif * (pos[i] - minpos[i])) / tsz;
		}
	}
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];
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

/*
void DockPane::SmartRepos(int ix, int inc)
{
	int rtot = 0;
	int cnt = pos.GetCount();
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];

	int dif = 10000 - inc;

	if (dif < 0) {
		// Not enough space
		int rem = dif % cnt;
		dif /= cnt;
		for (int i = 0; i < cnt; i++)
			pos[i] += dif;
		pos[ix] += rem;
$uvs: END REPOSITORY DELETE
	}
	else {
$uvs: REPOSITORY INSERT
		int tsz = ClientToPos(sz);
		animpos <<= pos;
		if (tsz) {
			int msz = GetMinPos(ix);
			if (msz < 10000 && msz+tsz > 10000) {
				Ctrl *c = FindCtrl(ix);
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
	if (dummy.GetParent()) {
		pos.Remove(FindIndex(dummy));
		dummy.Remove();
	}
	//FixChildSizes();
	Layout();
}

void DockPane::SmartReposDown(Vector<int> &p, int ix)
{
	int n = 0;
	int sum = 0;
	int dif = 0;
	int maxsize = 0;
	int totalsize = 0;
	int sparesize = 0;
	int cnt = p.GetCount();
	Vector<int> maxpos;
	maxpos.SetCount(cnt);

	if (p.GetCount() == 1) {
		p[0] = 0;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
		pos[ix] += inc;
		if (dif > 0)
			for (int i = 0; i < cnt; i++) {
				if (i != ix)
					pos[i] = (dif * pos[i]) / 10000;
			}
	}
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];
}

void DockPane::SmartRepos(int ix, int inc)
{
	Vector<int> ratio;
	int n = 0;
	int cnt = pos.GetCount();
	int psz = ClientToPos(GetSize());;
	int msz = 0;
	int rtot = 0;
	if (cnt == 1) {
		pos[0] = 10000;
$uvs: END REPOSITORY DELETE
		return;
	}

$uvs: REPOSITORY INSERT
	// Convert pos to non-cumulative sizes
	NonCumulativePos(p);

	// Find max, and total sizes (in pos units)
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	ratio.SetCount(cnt);
	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];

$uvs: END REPOSITORY DELETE
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
$uvs: REPOSITORY INSERT
			maxpos[n] = max(ClientToPos(c->GetMaxSize()), p[n]);
			maxsize += maxpos[n];
			totalsize += p[n];
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
			ratio[n] = ClientToPos(c->GetMinSize());
			msz += ratio[n];
			ratio[n] = max(pos[n] - ratio[n], 0);
			rtot += ratio[n];
$uvs: END REPOSITORY DELETE
		}
		n++;
	}
$uvs: REPOSITORY INSERT

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

void DockPane::SmartReposUp(Vector<int> &p, int ix, int sz)
{
	int n = 0;
	int sum = 0;
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

void DockPane::SimpleRepos(Vector<int> &p, int ix, int inc)
{
	int cnt = p.GetCount();
	if (cnt == 1) {
		p[0] = 10000;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	ratio[ix] = 0;

	int dif = psz - msz;
	if (dif < 0) {
		// Not enough space
		int rem = dif % cnt;
		dif /= cnt;
		for (int i = 0; i < cnt; i++)
			pos[i] += dif;
		pos[ix] += rem;
	}
	else if (dif > 0) {;
		int d = (inc > dif) ? dif : inc;
		pos[ix] += d;
		dif -= d;

		int p = pos[0];
		int r = ratio[0];

		if (dif > 0)
			for (int i = 0; i < cnt; i++) {
				ratio[i] = rtot ? ((ratio[i] * 10000) / rtot) : (10000 / cnt);
				pos[i] = (dif * ratio[i]) / 10000;
			}

		p = pos[0];
		r = 0;
	}

	if (ix > 0) {
		int p1 = pos[0];
		int p2 = pos[1];
		int i = 0;
	}
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];
}
*/
void DockPane::FixChildSizes()
{
	int cnt = pos.GetCount();
	if (!cnt) return;
	if (cnt == 1) {
		pos[0] = 10000;
$uvs: END REPOSITORY DELETE
		return;
	}
$uvs: REPOSITORY INSERT

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
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	int sum = pos[0];

	for (int i = 1; i < cnt; i++)
		sum += pos[i] - pos[i-1];
	sum -= 10000;
	int rem = sum % cnt;
	sum /= cnt;
	for (int i = 1; i < cnt; i++)
		pos[i] -= sum;
	pos[cnt-1] += rem;
}

int DockPane::FindIndex(Ctrl &child)
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

void DockPane::Swap(Ctrl &child, Ctrl &newctrl)
{
	newctrl.SetRect(child.GetRect());
	Ctrl::AddChildBefore(&newctrl, &child);
	Ctrl::RemoveChild(&child);
}

void DockPane::Dock(Ctrl &newctrl, Size sz, int ps, bool animate)
$uvs: END REPOSITORY DELETE
{
	int cnt = pos.GetCount();
$uvs: REPOSITORY INSERT
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
	int p = pos[cnt-1];
	ASSERT(pos[cnt-1] == 10000);
}

int DockPane::FindIndex(Ctrl &child)
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

void DockPane::Swap(Ctrl &child, Ctrl &newctrl)
{
	newctrl.SetRect(child.GetRect());
	Ctrl::AddChildBefore(&newctrl, &child);
	Ctrl::RemoveChild(&child);
}

void DockPane::Dock(Ctrl &newctrl, Size sz, int ps, bool animate, bool save)
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	if (!cnt) animate = false;
	int tsz = cnt ? ClientToPos(sz) : 10000;
	ps = min(ps, cnt-1);
	if (ps >= 0) {
		Ctrl *c = GetFirstChild();
		for (int i = 0; i < ps; i++)
			c = c->GetNext();
		Ctrl::AddChildBefore(&newctrl, c);
		pos.Insert(ps);
		pos[ps] = (ps > 0) ? pos[ps-1] : 0;
	}
	else {
		Splitter::Add(newctrl);
		Splitter::pos.Add(0);
		ps = cnt;
		if (ps > 0) pos[ps] = pos[ps-1];
	}
	if (animate)
		StartAnimate(ps, sz);
	else {
		if (cnt)
			SmartRepos(ps, tsz);
		else
			pos[ps] = 10000;
		Layout();
	}
}

void DockPane::Undock(Ctrl &child, bool animate)
$uvs: END REPOSITORY DELETE
{
	if (IsAnimating())
		EndAnimate();

$uvs: REPOSITORY INSERT
	int cnt = pos.GetCount();
	if (!cnt) animate = false;
	int tsz = cnt ? ClientToPos(sz) : 10000;
	ps = min(ps, cnt-1);
	if (ps >= 0) {
		Ctrl *c = GetFirstChild();
		for (int i = 0; i < ps; i++)
			c = c->GetNext();
		Ctrl::AddChildBefore(&newctrl, c);
		pos.Insert(ps);
		pos[ps] = (ps > 0) ? pos[ps-1] : 0;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	int ix = FindIndex(child);
	if (animate) {
		dummy.Remove();
		Swap(child, dummy);
		StartAnimate(ix, Size(0, 0));
$uvs: END REPOSITORY DELETE
	}
	else {
$uvs: REPOSITORY INSERT
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
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
		child.Remove();
		pos.Remove(ix);
		FixChildSizes();
$uvs: END REPOSITORY DELETE
		Layout();
	}
}

$uvs: REPOSITORY INSERT
void DockPane::Undock(Ctrl &child, bool animate, bool restore)
{
	if (IsAnimating())
		EndAnimate();

	int ix = FindIndex(child);
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
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
void DockPane::QuickDock(Ctrl &ctrl, int sz)
{
	Add(ctrl);
	int cnt = pos.GetCount();
	pos.Add(cnt ? pos[cnt-1] + sz : sz);
$uvs: END REPOSITORY DELETE
}

DockPane::DockPane()
{
$uvs: REPOSITORY INSERT
	animtick = 0;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	animix = -1;
	animtick = 0;
	animinc = 0;
$uvs: END REPOSITORY DELETE
	animinterval = 20; // milliseconds
	animmaxticks = 10;
	pos.Clear();
}

