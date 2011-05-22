#ifndef _FrameTest_ExpandFrame_h_
#define _FrameTest_ExpandFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class ExpandFrame : public CtrlFrame, public Ctrl {
public:
	typedef ExpandFrame CLASSNAME;
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual void FrameAddSize(Size& sz);
	virtual void FrameLayout(Rect& r);
	virtual void Paint(Draw& draw);
	
	void PaintBorder(Draw& w);
	
	virtual void ChildGotFocus() 	{ if (!childfocus) { childfocus = true; if (!IsExpanded()) Expand(true); Refresh(); } }
	virtual void ChildLostFocus() 	{ if (childfocus) { childfocus = false; Refresh(); } }
public:	
	struct Style : ChStyle<Style> {
		Value	handle_look[4]; // NORMAL, HIGHLIGHT, OPEN (PRESSED), DISABLED
		int     handle_margin[4];
		Rect 	handle_border[4];
		int		handle_size; 
		Font	font[4];
		Color	textcolor[4];
		int		textalign;
		Image	image[2]; // Normal/Expanded
		int 	imagealign;
		const ColorF *border[4];
		Rect 	border_inset;
		Rect 	border_outset;
	};	
	static const Style& StyleDefault();	
	static const Style& StyleFlat();		

	enum { LEFT, TOP, RIGHT, BOTTOM };		
private:
	struct ExpandButton : public FlatButton
	{
		bool 	expand;
		int 	align;
		const ExpandFrame::Style *style;
		
		virtual void Paint(Draw& w);
		int GetVisualState() const;
		ExpandButton() { Transparent(); }
	};
	
	ExpandButton	handle;
	Image 			img;
	int				child_size;
	bool			childfocus:1;
	bool			ignorefocus:1;
	bool			hasarrow:1;
	
	int				ExpandSz() const;
	Rect			BorderSz() const;
	int				HandleSz() const				{ return handle.style->handle_size + handle.style->handle_border[0].top + handle.style->handle_border[0].bottom; }
	
	void			UpdateButton();
	void			SetChildPos();
	bool			IsSet()	const					{ return GetLastChild() != &handle; }
	void 			Expand0(bool _expand)			{ if (_expand != handle.expand) { Expand(_expand); Action(); } }
public:

	ExpandFrame&  	SetStyle(const Style& s)		{ handle.style = &s; UpdateButton(); Refresh(); return *this; }

	ExpandFrame&  	SetTitle(const char *_title) 	{ handle.SetLabel(_title); return *this; }
	String 			GetTitle() const				{ return handle.GetLabel(); }

	ExpandFrame& 	SetImage(const Image& image)	{ img = image; Refresh(); return *this; }
	const Image& 	GetImage() const				{ return img; }

	ExpandFrame& Set(Ctrl& c, int size, int _type);
	ExpandFrame& Left(Ctrl& c, int size = -1)   	{ return Set(c, (size < 0) ? c.GetMinSize().cx : HorzLayoutZoom(size), LEFT); }
	ExpandFrame& Top(Ctrl& c, int size = -1)    	{ return Set(c, (size < 0) ? c.GetMinSize().cy : VertLayoutZoom(size), TOP); }
	ExpandFrame& Right(Ctrl& c, int size = -1)  	{ return Set(c, (size < 0) ? c.GetMinSize().cx : HorzLayoutZoom(size), RIGHT); }
	ExpandFrame& Bottom(Ctrl& c, int size = -1) 	{ return Set(c, (size < 0) ? c.GetMinSize().cy : VertLayoutZoom(size), BOTTOM); }
	ExpandFrame& SetStyle(const Style *_style) 		{ handle.style = _style; SetChildPos(); UpdateButton(); RefreshParentLayout(); return *this; }
	ExpandFrame& IgnoreFocus(bool ignore = true) 	{ ignorefocus = ignore; if (!childfocus) Refresh(); }

	ExpandFrame& Expand(bool _expand = true);
	ExpandFrame& Collapse()							{ return Expand(false); }
	void		 Toggle()							{ Expand(!IsExpanded()); }
	bool 		 IsExpanded() const					{ return handle.expand; }
	
	virtual Size GetMinSize() const;
	virtual Size GetStdSize() const;
	
	void 		Show(bool show = true)				{ Ctrl::Show(show); }
	void 		Hide()								{ Ctrl::Hide(); }
	bool 		IsShown() const					  	{ return Ctrl::IsShown(); }

	ExpandFrame();
};

class ExpanderCtrl : public ParentCtrl
{
public:
	virtual void Layout();					
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) { scroll.Wheel(zdelta); }
	virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);

private:
	Array<ExpandFrame> 		exp;
	ScrollBar 				scroll;
	Scroller				scroller;
	bool 					horz;
	
	bool 	IsHorz() const					{ return horz; }
	int		Hv(const Size& sz) const		{ return IsHorz() ? sz.cx : sz.cy; }
	void	UpdateScrollBar();
	void 	OnScroll();
	void 	OnExpand();
	void 	Repos();
public:
	typedef ExpanderCtrl CLASSNAME;
	
	ExpandFrame& 	AddExpander(Ctrl& c, int size = -1)		{ return AddExpander(c, true, size); }
	ExpandFrame& 	AddExpander(Ctrl& c, bool expand, int size = -1);
	
	ExpandFrame&  	Get(int i)								{ return exp[i]; }
	const ExpandFrame&  Get(int i) const					{ return exp[i]; }
	ExpanderCtrl& 	Horz(bool v = true);
	ExpanderCtrl& 	Vert(bool v = true)						{ return Horz(!v); }
	
	void 			Clear()									{ exp.Clear(); scroller.Clear(); }
	int	 			GetCount()								{ return exp.GetCount(); }
	
	ExpanderCtrl();
};

#endif
