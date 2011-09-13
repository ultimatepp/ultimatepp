#ifndef _CtrlFinder_CtrlFinder_h
#define _CtrlFinder_CtrlFinder_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class CtrlFinder : public ParentCtrl
{
public:
	typedef CtrlFinder CLASSNAME;
	typedef Callback3<Ctrl*&, Point&, int&> CtrlFilterType;

	enum
	{
		VISIBLE = 0x1,
		INVISIBLE = 0x2,
		
		ENABLED = 0x4,
		DISABLED = 0x8,

		VIEW = 0x10,
		FRAME = 0x20,
		
		DEEP = 0x80,
		
		DEF = VISIBLE | INVISIBLE | ENABLED | DISABLED | VIEW | FRAME | DEEP,
	};

	CtrlFinder() : flags(DEF), filter(STDBACK(StdCtrlFilter)) { WantFocus(); }

	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	virtual void Updated();

	void SetSource(Ctrl* c) { if(pctrl != c) { if(c) ASSERT(c->GetParent()); pctrl = c; ctrl = NULL; UpdatedSource(); } Update(); }
	Ctrl* GetSource() const { return pctrl; }
	void ClearSource() { SetSource(NULL); }

	void SetCtrl(Ctrl* c) { if(ctrl != c) { if(pctrl && c) { ASSERT(IsParentR(pctrl, c)); } ctrl = c; } Update(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }
	
	static void StdCtrlFilter(Ctrl*& q, Point& pt, int& f);	
	static Ctrl* ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& filt);
	static Ctrl* GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& filt);
	static bool IsParentR(const Ctrl* p, const Ctrl* c);

	Callback3<Ctrl&, Point, dword> WhenLeftDown;
	Callback3<Ctrl&, Point, dword> WhenRightDown;
	Callback1<Bar&> WhenBar;

	CtrlFilterType filter; //set to NULL if should not be handled, change flags if desired
	int flags;

protected:
	void UpdatedSource();

	Ctrl* pctrl; //the parent we search in
	Ctrl* ctrl; //the current found child
};

#endif

