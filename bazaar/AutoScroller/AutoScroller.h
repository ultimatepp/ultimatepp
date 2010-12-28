#ifndef _AutoScroller_AutoScroller_h_
#define _AutoScroller_AutoScroller_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

template<class C = ParentCtrl>
class AutoScroller
	: public ParentCtrl
{
public:
	typedef AutoScroller CLASSNAME;
	AutoScroller();
	
public:
	virtual void Layout();
	
	void SetPaneSize(const Size & sz);
	Size GetPaneSize() const { return pane.GetSize(); }

	void EnableScroll(bool b = true);
	void DisableScroll() { EnableScroll(false); }
	bool IsEnabledScroll() const { return scroll.x.IsEnabled() || scroll.y.IsEnabled(); }

	C pane;

protected:
	void OnScroll();

private:	
	ScrollBars scroll;
};


#endif
