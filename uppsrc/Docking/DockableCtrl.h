#define IMAGECLASS DockingImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_header.h>

class DockCont;

class DockableCtrl : public ParentCtrl {
public:
	typedef DockableCtrl CLASSNAME;
	
	struct Style : ChStyle<Style> {
		Value handle[2];
		Rect handle_margins;
		bool handle_vert;
		Font title_font;
		Color title_ink[2];
		Value close[4];
		Value windowpos[4];
		Value autohide[4];
		Value pin[4];
		Value highlight[2]; // Opaque, transparent
	};	
	static const Style& StyleDefault();		
	static const Style& StyleDefaultVert();		

	enum { TIMEID_HIGHLIGHT = ParentCtrl::TIMEID_COUNT, TIMEID_COUNT };
private:
	Size			minsize;
	Size			maxsize;
	Size			stdsize;
	Image 			icon;
	WString 		title;
	String 			group;
	bool			dockable[4];
	dword			keycode;
	KeyInfo&(*		keyinfo)();

	const Style    *style;
	
	void			StopHighlight() 				{ Refresh(); }
protected:
	DockCont       *GetContainer() const;
public:
	Callback1<Bar&> WhenMenuBar;
	Callback		WhenState;

	virtual void WindowMenu(Bar& bar) 				{ WhenMenuBar(bar); }
	
	const Image& 	GetIcon()						{ return icon; }
	DockableCtrl& 	Icon(const Image& m);
	DockableCtrl& 	Title(const char *_title)		{ return Title((WString)_title); }
	DockableCtrl& 	Title(const WString& _title);
	const WString&  GetTitle()						{ return title; }

	DockableCtrl& 	Set(const Image& icon, const char *_title, String group = Null);
	DockableCtrl&	SetHotKey(dword key)			{ keycode = key; return *this; }
	DockableCtrl&	SetHotKey(KeyInfo&(*key)())		{ keyinfo = key; return *this; }
	bool			IsHotKey(dword key);

	DockableCtrl& 	SizeHint(const Size& min, const Size& max = Null, const Size& std = Null);
	void       		SetMinSize(Size sz) 			{ minsize = sz; }
	void       		SetMaxSize(Size sz) 			{ maxsize = sz; }	
	void 			SetStdSize(Size sz) 			{ stdsize = sz; }
	virtual Size	GetMinSize() const				{ return minsize; }
	virtual Size	GetMaxSize() const				{ return maxsize.IsNullInstance() ? Ctrl::GetMaxSize() : maxsize; }	
	virtual Size	GetStdSize() const				{ return stdsize.IsNullInstance() ? minsize : stdsize; }
		
	DockableCtrl& 	SetStyle(const Style& s)		{ style = &s; RefreshParentLayout(); return *this; }
	const Style& 	GetStyle()						{ return style ? *style : StyleDefault(); }
	
	virtual const String& GetGroup() const			{ return group; }
	virtual DockableCtrl& SetGroup(const String& g);
	
	DockableCtrl& 	AllowDockAll()					{ return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom(); }
	DockableCtrl& 	AllowDockNone()					{ return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false); }
	DockableCtrl& 	AllowDockLeft(bool v = true)	{ dockable[0] = v; return *this; }
	DockableCtrl& 	AllowDockTop(bool v = true)		{ dockable[1] = v; return *this; }
	DockableCtrl& 	AllowDockRight(bool v = true)	{ dockable[2] = v; return *this; }
	DockableCtrl& 	AllowDockBottom(bool v = true)	{ dockable[3] = v; return *this; }
	DockableCtrl& 	AllowDock(int a, bool v = true)	{ ASSERT(a >= 0 && a < 4); dockable[a] = v; return *this; }	
	bool			IsDockAllowed(int a) const		{ ASSERT(a >= 0 && a < 4); return dockable[a]; }
	bool            IsDockAllowedLeft() const  		{ return dockable[0]; }
	bool            IsDockAllowedTop() const  		{ return dockable[1]; }
	bool            IsDockAllowedRight() const  	{ return dockable[2]; }
	bool            IsDockAllowedBottom() const  	{ return dockable[3]; }
	bool            IsDockAllowedAny() const  		{ return dockable[0] || dockable[1] || dockable[2] || dockable[3]; }
	bool            IsDockAllowedNone() const  		{ return !IsDockAllowedAny(); } 
	
	bool 			IsFloating() const;
	bool 			IsDocked() const;
	bool 			IsAutoHide() const;
	bool 			IsTabbed() const;
	bool 			IsHidden() const;
	int 			GetDockAlign() const;
	
	void			Highlight();
	void			TimedHighlight(int ms);
	
	virtual Value 	GetSortValue() { return GetTitle(); }	

	DockableCtrl();
};

struct DockableParent : public DockableCtrl
{
	virtual void Paint(Draw& w) { w.DrawRect(GetSize(), SColorFace); }
};
