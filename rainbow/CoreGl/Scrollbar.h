#ifndef _CoreGl_Scrollbar_h_
#define _CoreGl_Scrollbar_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

struct AlignedFrame : FrameCtrl<Ctrl>
{
	int layout;
	int framesize;
	int border;
public:	
	enum
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	
	AlignedFrame() : layout(TOP), framesize(0), border(0) {}
	
	virtual void FrameAddSize(Size& sz);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameLayout(Rect& r);
	
	bool		  IsVert() const	{ return (layout & 1) == 0; }
	bool		  IsHorz() const	{ return layout & 1; }
	bool		  IsTL() const		{ return layout < 2; }
	bool		  IsBR() const		{ return layout >= 2; }
	
	AlignedFrame& SetAlign(int align) { layout = align; FrameSet(); RefreshParentLayout(); return *this; }
	AlignedFrame& SetLeft()		{ return SetAlign(LEFT); }
	AlignedFrame& SetTop()		{ return SetAlign(TOP); }
	AlignedFrame& SetRight()	{ return SetAlign(RIGHT); }
	AlignedFrame& SetBottom()	{ return SetAlign(BOTTOM); }	
	AlignedFrame& SetFrameSize(int sz, bool refresh = true);
		
	int 		  GetAlign() const		{ return layout; }
	int			  GetFrameSize() const 	{ return framesize; }
	int			  GetBorder() const		{ return border; }
protected:
	void Fix(Size& sz);
	void Fix(Point& p);
	Size Fixed(const Size& sz);
	Point Fixed(const Point& p);
	
	bool		  HasBorder()				{ return border >= 0; }
	AlignedFrame& SetBorder(int _border)	{ border = _border; return *this; }
	
	virtual	void  FrameSet()				{ }
};

class SlimScrollBar : public AlignedFrame
{
	private:
		int total;
		int t;
		bool isend;
		bool isbegin;
		double pos, prev_ps, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		virtual void UpdatePos(bool update = true, bool whenscroll = true);
		void RefreshScroll();
		bool ready;
		Size sz;
	public:
		SlimScrollBar();

		virtual void Paint(Draw& w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void Layout();

		int  GetPos() const;
		void SetPos(int p, bool dontscale = false, bool whenscroll = true);
		void AddPos(int p, bool dontscale = false, bool whenscroll = true);
		int  GetTotal() const;
		void AddTotal(int t);
		void SetTotal(int t, bool whenscroll = true);
		void GoEnd(bool whenscroll = true);
		void GoBegin(bool whenscroll = true);
		bool IsEnd();
		bool IsBegin();
		void Clear();
		void Set(const SlimScrollBar& t);
		bool IsScrollable() const;
		Callback WhenScroll;
};

END_UPP_NAMESPACE

#endif
