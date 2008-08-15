$uvs: PENDING CONFLICT
#ifndef _Docking_DockPane_h_
#define _Docking_DockPane_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class DockPane : public Splitter
{
public:
	typedef DockPane CLASSNAME;

private:
	struct DummyCtrl : public Ctrl
	{
		DummyCtrl() { minsize = stdsize = Null; Transparent(); NoWantFocus(); }
		Size minsize;
		Size stdsize;
		virtual Size GetMinSize() const { return minsize; }
		virtual Size GetStdSize() const { return stdsize; }
	};
$uvs: REPOSITORY INSERT
	DummyCtrl 	dummy;
	int 		animtick;
	int 		animinterval;
	int 		animmaxticks;
	Vector<int>	animpos;
	Vector<int>	savedpos;

	void 	StartAnimate(int ix, Size sz, bool restore);
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	DummyCtrl dummy;
	int 	animix;
	int 	animtick;
	int 	animinc;
	int 	animinterval;
	int 	animmaxticks;

	void 	StartAnimate(int ix, Size sz);
$uvs: END REPOSITORY DELETE
	void 	AnimateTick();
	void 	EndAnimate();

$uvs: REPOSITORY INSERT
	void 	SmartReposUp(Vector<int> &p, int ix, int sz);
	void 	SmartReposDown(Vector<int> &p, int ix);
	void 	SimpleRepos(Vector<int> &p, int ix, int sz);
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	void 	SmartRepos(int ix, int pos);
$uvs: END REPOSITORY DELETE
	int 	GetMinPos(int notix);
	void 	FixChildSizes();

	int 	NormalPos(int ix) const			{ return (ix > 0) ? pos[ix] - pos[ix-1] : pos[ix]; }
	int		ClientToPos(const Size &sz);
$uvs: REPOSITORY INSERT
	void	CumulativePos(Vector<int> &p) const;
	void	NonCumulativePos(Vector<int> &p) const;
$uvs: END REPOSITORY INSERT

	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
           TIMEID_COUNT };
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	int 	FindIndex(Ctrl &child);
	Ctrl *	FindCtrl(int ix);
	int 	GetCount() const						{ return pos.GetCount(); }

	void	Swap(Ctrl &child, Ctrl &newctrl);
$uvs: REPOSITORY INSERT
	void	Dock(Ctrl &newctrl, Size sz, int pos, bool animate, bool save = false);
	void 	Undock(Ctrl &newctrl, bool animate, bool restore = false);

	void	SavePos()								{ savedpos <<= pos; }
	void	RestorePos()							{ ASSERT(savedpos.GetCount() == pos.GetCount()); pos <<= savedpos; Layout(); }
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	void	Dock(Ctrl &newctrl, Size sz, int pos, bool animate);
	void 	Undock(Ctrl &newctrl, bool animate);
	void 	QuickDock(Ctrl &ctrl, int sz);
$uvs: END REPOSITORY DELETE

	int 	GetCtrlSize(int i) const				{ return NormalPos(i); }
	void	SetCtrlSize(int i, int sz)				{ pos[i] = (i == 0) ? sz : sz + pos[i-1]; }

$uvs: REPOSITORY INSERT
	void 	Clear()									{ pos.Clear(); savedpos.Clear(); }

	bool	IsAnimating() const	 					{ return animpos.GetCount(); }
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	void 	Clear()									{ pos.Clear(); }

	bool	IsAnimating()		 					{ return animix >= 0; }
$uvs: END REPOSITORY DELETE
	void	SetAnimateRate(int ticks, int ms) 		{ animmaxticks = max(1, ticks);	animinterval = max(0, ms); }
	int		GetAnimMaxTicks() const					{ return animmaxticks; }
	int		GetAnimInterval() const					{ return animinterval; }

$uvs: REPOSITORY INSERT
	virtual void Serialize(Stream &s)				{ s % pos; if (s.IsLoading()) FixChildSizes(); }

$uvs: END REPOSITORY INSERT
	DockPane();
};

#endif

