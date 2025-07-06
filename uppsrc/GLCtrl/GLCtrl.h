#ifndef _GLCtrl_GLCtrl_h
#define _GLCtrl_GLCtrl_h

#include <CtrlCore/CtrlCore.h>

#if defined(GUI_X11) || defined(GUI_GTK)
#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture
#define Status  int
#endif

#define GLEW_STATIC

#include <plugin/glew/glew.h>

#ifdef PLATFORM_WIN32
#include <plugin/glew/wglew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#if defined(GUI_X11) || defined(GUI_GTK)

#include <GL/glx.h>

#undef  Status
#undef  Picture
#undef  Time
#undef  Font
#undef  Display
#endif

namespace Upp {

void InitializeGlew();


class GLCtrl : public Ctrl {
public:
	Image  MouseEvent(int event, Point p, int zdelta, dword keyflags) override;
#ifdef PLATFORM_POSIX
	void   Paint(Draw& w) override;
#endif
	
private:
#ifdef PLATFORM_WIN32
	struct GLPane : DHCtrl {
		friend class GLCtrl;
		
		GLCtrl *ctrl;

		void DoGLPaint();
		
	public:
		GLPane() { NoWantFocus(); }
		
		virtual void    State(int reason);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual Image   MouseEvent(int event, Point p, int zdelta, dword keyflags);
		virtual void    Pen(Point p, const PenInfo& pen, dword keyflags);
		
		void Init();
		void Destroy();
		
		void ActivateContext();

		void ExecuteGL(HDC hdc, Event<> paint, bool swap_buffers);
		void ExecuteGL(Event<> paint, bool swap_buffers);
	};
#endif

	friend struct GLPane;

#ifdef PLATFORM_POSIX // we assume X11 or GTK
	unsigned long win = 0;
	bool visible;
	Rect position;

	void Create();
	void Sync();
	void Destroy();
	
	void State(int reason) override;
#else
	GLPane pane;
#endif

	static int  depthSize;
	static int  stencilSize;
	static bool doubleBuffering;
	static int  numberOfSamples;

	static void MakeGLContext();
	void        DoGLPaint();

	static Size current_viewport; // because we need to set different viewports in drawing code

	Ptr<Ctrl> mouseTarget = NULL;

protected:
	// Called on paint events
	virtual void GLPaint() { WhenGLPaint(); }
	
	void Init();

public:
	Callback WhenGLPaint;

	static void SetDepthBits(int n)               { depthSize = n; }
	static void SetStencilBits(int n)             { stencilSize = n; }
	static void SetDoubleBuffering(bool b = true) { doubleBuffering = b; }
	static void SetMSAA(int n = 4)                { numberOfSamples = n; }
	
	static void CreateContext();
	
	static Size CurrentViewport()                 { return current_viewport; }
	static void SetCurrentViewport(); // intended to restore viewport after changing it in e.g. TextureDraw
	
	GLCtrl& RedirectMouse(Ctrl *target)           { mouseTarget = target; return *this; }
	
	void ExecuteGL(Event<> gl, bool swap_buffers = false);

	GLCtrl()                                      { Init(); }

#ifdef PLATFORM_WIN32
	void Refresh()                                { pane.Refresh(); }
#endif

	// deprecated (these settings are now static, as we have just single context)
	GLCtrl& DepthBits(int n)               { depthSize = n; return *this; }
	GLCtrl& StencilBits(int n)             { stencilSize = n; return *this; }
	GLCtrl& DoubleBuffering(bool b = true) { doubleBuffering = b; return *this; }
	GLCtrl& MSAA(int n = 4)                { numberOfSamples = n; return *this; }

	GLCtrl(int  depthsize, int  stencilsize = 8, bool doublebuffer = true,
	       bool multisamplebuffering = false, int  numberofsamples = 0)
	{ Init(); DepthBits(depthsize).StencilBits(stencilsize).DoubleBuffering(doublebuffer).MSAA(numberofsamples); }

	// deprecated (fixed pipeline is so out of fashion...)
	void StdView();
};

}

#endif
