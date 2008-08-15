$uvs: PENDING CONFLICT
#ifndef _Docking_DockableCtrl_h_
#define _Docking_DockableCtrl_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS DockingImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_header.h>

class DockCont;

class DockableCtrl : public ParentCtrl
{
public:
	typedef DockableCtrl CLASSNAME;

	struct Style : ChStyle<Style> {
		CtrlFrame *frame;
		Value handle[2];
		Rect handle_margins;
		bool handle_vert;
		Font title_font;
		Color title_ink[2];
		Value close[4];
		Value windowpos[4];
		Value autohide[4];
$uvs: REPOSITORY DELETE
		int btnsize;
$uvs: END REPOSITORY DELETE
		Value highlight[2]; // Opaque, transparent
	};
	static const Style& StyleDefault();
	static const Style& StyleDefaultVert();

private:
	Size			minsize;
	Size			maxsize;
	Size			stdsize;
	Image 			icon;
	WString 		title;
	String 			group;
	bool			dockable[4];

	const Style *	style;

	DockCont *		GetContainer() const;

$uvs: REPOSITORY DELETE

$uvs: END REPOSITORY DELETE
public:
	Callback1<Bar &> WhenMenuBar;

	virtual void WindowMenu(Bar &bar) 				{ WhenMenuBar(bar); }

	const Image &	GetIcon()						{ return icon; }
	DockableCtrl &	Icon(const Image& m)			{ icon = m; return *this; }
	DockableCtrl &	Title(const char *_title)		{ title = _title; if (GetParent()) GetParent()->Refresh(); return *this; }
	DockableCtrl &	Title(const WString& _title)	{ title = _title; if (GetParent()) GetParent()->Refresh(); return *this; }
	const WString & GetTitle()						{ return title; }

	DockableCtrl &	SizeHint(const Size &min, const Size &max = Null, const Size &std = Null);
	void       		SetMinSize(Size sz) 			{ minsize = sz; }
	void       		SetMaxSize(Size sz) 			{ maxsize = sz; }
	void 			SetStdSize(Size sz) 			{ stdsize = sz; }
	virtual Size	GetMinSize() const				{ return minsize; }
	virtual Size	GetMaxSize() const				{ return maxsize.IsNullInstance() ? Ctrl::GetMaxSize() : maxsize; }
	virtual Size	GetStdSize() const				{ return stdsize.IsNullInstance() ? minsize : stdsize; }

	DockableCtrl &	SetStyle(const Style &s)		{ style = &s; RefreshParentLayout(); return *this; }
	const Style &	GetStyle()						{ return style ? *style : StyleDefault(); }

	virtual const String &GetGroup() const			{ return group; }
	virtual DockableCtrl &SetGroup(const String &g);

	DockableCtrl &	AllowDockAll()					{ return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom(); }
	DockableCtrl &	AllowDockNone()					{ return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false); }
	DockableCtrl &	AllowDockLeft(bool v = true)	{ dockable[0] = v; return *this; }
	DockableCtrl &	AllowDockTop(bool v = true)		{ dockable[1] = v; return *this; }
	DockableCtrl &	AllowDockRight(bool v = true)	{ dockable[2] = v; return *this; }
	DockableCtrl &	AllowDockBottom(bool v = true)	{ dockable[3] = v; return *this; }
	DockableCtrl &	AllowDock(int a, bool v = true)	{ ASSERT(a >= 0 && a < 4); dockable[a] = v; return *this; }
	bool			IsDockAllowed(int a)			{ ASSERT(a >= 0 && a < 4); return dockable[a]; }

	bool 			IsFloating() const;
	bool 			IsDocked() const;
	bool 			IsAutoHide() const;
	bool 			IsTabbed() const;
	bool 			IsHidden() const;
	int 			GetDockAlign() const;

	void			Highlight();

	DockableCtrl();
};

#endif

