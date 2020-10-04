#ifndef _AutoScroller_AutoScroller_h_
#define _AutoScroller_AutoScroller_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp
{
	template<class C = ParentCtrl>
	class AutoScroller : public C
	{
	public:
		using CLASSNAME = AutoScroller<C>;
		AutoScroller();
		
	public:
		virtual void Layout() override;
		
		void EnableScroll(bool b = true);
		void DisableScroll()         { EnableScroll(false); }
		bool IsScrollEnabled() const { return scroll.x.IsEnabled() || scroll.y.IsEnabled(); }
	
		void AddPane(Ctrl& c)        { ClearPane(); pane = &c; C::Add(c); }
		Ctrl* GetPane() const        { return pane; }
		inline bool HasPane() const  { return (~pane != nullptr); }
		void ClearPane()             { if(! ~pane) return; pane->Remove(); pane = nullptr; }
	
		void Scroll(const Point& p);
		void OnScroll();
	
		Callback WhenScrolled;
		ScrollBars scroll;
	
	protected:
		Ptr<Ctrl> pane;
	};
}

#endif
