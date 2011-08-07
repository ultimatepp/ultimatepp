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

	CtrlFinder() : flags(DEF), filter(STDBACK(StdCtrlFilter)) {}

	virtual void UpdatedSource();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	void SetSource(Ctrl* c) { if(c) ASSERT(c->GetParent()); pctrl = c; ctrl = NULL; UpdatedSource(); Enable(); }
	Ctrl* GetSource() const { return pctrl; }
	void ClearSource() { SetSource(NULL); }

	void SetCtrl(Ctrl* c) { if(c && pctrl) ASSERT(c->GetParent() == ~pctrl); ctrl = c; UpdateRefresh(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(~ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }
	
	static void StdCtrlFilter(Ctrl*& q, Point& pt, int& f);	
	static Ctrl* ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& fil);
	static Ctrl* GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& fil);

	Callback3<Ctrl&, Point, dword> WhenLeftDown;
	Callback3<Ctrl&, Point, dword> WhenRightDown;

	CtrlFilterType filter; //set to NULL if should not be handled, change flags if desired
	int flags;

protected:
	Ptr<Ctrl> pctrl; //the parent we search in
	Ptr<Ctrl> ctrl; //the current found child
};

#endif

