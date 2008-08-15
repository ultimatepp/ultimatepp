#ifndef _Docking_TitleFrame_h_
#define _Docking_TitleFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class TitleFrame : public CtrlFrame, public Ctrl {
public:
	Callback WhenContext;

	typedef TitleFrame CLASSNAME;
	virtual void FrameAdd(Ctrl& parent)		{ parent.Add(*this); }
	virtual void FrameRemove()				{ this->Ctrl::Remove(); }
	virtual void FrameAddSize(Size& sz)		{ if (IsShown()) sz.cy += TitleSz(); }
	virtual void FrameLayout(Rect& r)		{ LayoutFrameTop(r, this, TitleSz()); }
	virtual void Paint(Draw& draw);

	virtual void RightDown(Point p, dword keyflags) { WhenContext(); }

	virtual void ChildAdded(Ctrl *child) 	{ SetChildPos(); }
	virtual void ChildRemoved(Ctrl *child) 	{ SetChildPos(); }
public:
	struct Style : ChStyle<Style> {
		Value	background[2];
		Color	text[2];
		Font	font;
	};
	static const Style& StyleDefault();

private:
	Image 			img;
	WString 		title;
	bool			focuslook:1;

	const Style *	style;
	const Style *	St() const					{ return style ? style : &StyleDefault(); }

	int 			TitleSz() const				{ return St()->font.GetHeight()+4; }
	void			SetChildPos();
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	TitleFrame & 	SetStyle(const Style &s)		{ style = &s; SetChildPos(); RefreshParentLayout(); return *this; }
	TitleFrame & 	SetFocusLook(bool _focus = true){ if (focuslook != _focus) {focuslook = _focus; Refresh();} return *this; }
	bool			IsFocusLook()					{ return focuslook; }
	TitleFrame & 	SetTitle(const WString &_title) { title = _title; Refresh(); return *this; }
	const WString &	GetTitle() const				{ return title; }
	TitleFrame &	SetImage(const Image &image)	{ img = image; Refresh(); return *this; }
	const Image &	GetImage() const				{ return img; }
	TitleFrame();
};

#endif
