#ifndef _CtrlFinder_CtrlFinder_h
#define _CtrlFinder_CtrlFinder_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class CtrlFinder : public ParentCtrl
{
public:
	typedef CtrlFinder CLASSNAME;
	typedef Callback2<Ctrl*&, int&> CtrlFilterType;

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
	} FLAGS;

	CtrlFinder() : flags(DEF), filter(STDBACK(StdCtrlFilter)), multi(true) { WantFocus(); }

	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	virtual void Updated();

	void SetSource(Ctrl* c) { if(pctrl != c) { if(c) ASSERT(c->GetParent()); pctrl = c; ctrls.Clear(); UpdatedSource(); } Update(); }
	Ctrl* GetSource() const { return pctrl; }
	void ClearSource() { SetSource(NULL); }

	void SetCtrl(Ctrl* c);
	void SetCtrls(pick_ Vector<Ctrl*>& _ctrls) { ctrls = _ctrls; Update(); }

	Ctrl* GetCtrl() const { return ctrls.IsEmpty() ? NULL : ctrls.Top(); }
	Vector<Ctrl*> GetCtrls() const { return Vector<Ctrl*>(ctrls, 1); }
	bool IsEmpty() const { return ctrls.IsEmpty(); }

	void ClearCtrl() { ctrls.Clear(); Update(); }

	virtual Value GetData() const { return RawPickToValue(Vector<Ctrl*>(ctrls, 1)); }
	virtual void SetData(const Value& v) { SetCtrls(Vector<Ctrl*>(RawValue<Vector<Ctrl*> >::Extract(v), 1)); }
	
	void Multiselect(bool b = true) { multi = b; if(!b && ctrls.GetCount() > 1) { ctrls[0] = ctrls.Top(); ctrls.SetCount(1); Update(); } }
	bool IsMultiselect() const { return multi; }
	
	static void StdCtrlFilter(Ctrl*& q, int& f);	

	static Ctrl* ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& filt);
	static Ctrl* GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& filt);

	static Ctrl* ChildInRect(Ctrl& par, Rect& rect, int& f, const CtrlFilterType& filt);
	static Ctrl* GetCtrl(Ctrl& c, Rect& rect, int& f, const CtrlFilterType& filt);

	static Vector<Ctrl*> ChildrenInRect(Ctrl& par, Rect& rect, int& f, const CtrlFilterType& filt);
	static Vector<Ctrl*> GetCtrls(Ctrl& c, Rect& rect, int& f, const CtrlFilterType& filt);
	
	static bool IsParentR(const Ctrl* p, const Ctrl* c);

	static void AddParentViewOffset(const Ctrl& c, Point& p);
	static void AddContextOffset(const Ctrl& c, const Ctrl& q, Point& p);
	static Point GetContextOffset(const Ctrl& c, const Ctrl& q);
	static Point GetOffset(const Ctrl& ctxuser, const Ctrl& finalctx);
	static Rect SurroundingRect(const Vector<Ctrl*>& ctrls);

	Callback3<Ctrl&, Point, dword> WhenLeftSelect;
	Callback3<Ctrl&, Point, dword> WhenRightSelect;
	
	Callback3<const Vector<Ctrl*>*, Rect, dword> WhenLeftSelectMulti;
	Callback3<const Vector<Ctrl*>*, Rect, dword> WhenRightSelectMulti;
	
	Callback1<Bar&> WhenBar;

	CtrlFilterType filter; //set to NULL if should not be handled, change flags if desired
	int flags;

protected:
	void UpdatedSource();

	bool multi;

	Ctrl* pctrl; //the parent we search in
	Vector<Ctrl*> ctrls; //the current found children
};

#endif

