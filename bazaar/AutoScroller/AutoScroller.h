#ifndef _AutoScroller_AutoScroller_h_
#define _AutoScroller_AutoScroller_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

template<class C = ParentCtrl>
class AutoScroller : public C
{
public:
	typedef AutoScroller<C> CLASSNAME;
	AutoScroller();
	
public:
	virtual void Layout();
	
	void EnableScroll(bool b = true);
	void DisableScroll() { EnableScroll(false); }
	bool IsEnabledScroll() const { return scroll.x.IsEnabled() || scroll.y.IsEnabled(); }

	void AddPane(Ctrl& c) { ClearPane(); pane = &c; C::Add(c); }
	Ctrl* GetPane() const { return pane; }
	inline bool HasPane() const { return (~pane != NULL); }
	void ClearPane() { if(! ~pane) return; pane->Remove(); pane = NULL; }

	void Scroll(const Point& p);
	void OnScroll();

	Callback WhenScrolled;
	ScrollBars scroll;

protected:
	Ptr<Ctrl> pane;
};


#endif
