#include <CtrlLib/CtrlLib.h>

#ifdef VIRTUALGUI

namespace Upp {

class TopWindowFrame : public Ctrl {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& w);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  LeftHold(Point p, dword keyflags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  CancelMode();
	virtual void  LeftUp(Point p, dword keyflags);

private:
	Point  dir;
	Point  startpos;
	Rect   startrect;

	bool   maximized;
	Rect   overlapped;
	
	bool   holding;
	TimeCallback hold;
	
	Point GetDragMode(Point p);
	Image GetDragImage(Point dragmode);
	void  StartDrag();
	Rect  Margins() const;
	Rect  ComputeClient(Rect r);
	void  Hold();

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

extern VirtualGui *VirtualGuiPtr;

}

#endif
