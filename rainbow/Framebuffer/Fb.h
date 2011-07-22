#ifndef _Framebuffer_Fb_h_
#define _Framebuffer_Fb_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class TopWindowFrame : public Ctrl {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& w);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);

private:
	Point  dir;
	Point  startpos;
	Rect   startrect;

	bool   maximized;
	Rect   overlapped;
	
	Point GetDragMode(Point p);
	Image GetDragImage(Point dragmode);
	void  StartDrag();
	Rect  Margins() const;
	Rect  ComputeClient(Rect r);

	typedef TopWindowFrame CLASSNAME;

public:
	String title;
	Button close, maximize;
	Image  icon;
	Size   minsize;
	bool   sizeable;
	TopWindow *window;

	void SetTitle(const String& s)           { title = s; Refresh(); }
	Rect GetClient() const;
	void SetClient(Rect r);
	void GripResize();

	void  Maximize();
	void  Overlap();
	void  ToggleMaximize();
	bool  IsMaximized() const                 { return maximized; }
	void  SyncRect();
	
	TopWindowFrame();
};

END_UPP_NAMESPACE

#endif
