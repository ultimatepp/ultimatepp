#ifndef _GLCtrl_glad_GLCtrl_glad_h
#define _GLCtrl_glad_GLCtrl_glad_h

#include <CtrlCore/CtrlCore.h>

#if defined(GUI_X11) || defined(GUI_GTK)
#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture
#define Status  int
#endif

#include <plugin/glad/Glad.h>

//#include <GL/gl.h>
#include <GL/glu.h>

#if defined(GUI_X11) || defined(GUI_GTK)

//#include <GL/glx.h>

#undef  Status
#undef  Picture
#undef  Time
#undef  Font
#undef  Display
#endif

namespace Upp {

static void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ){
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),type, severity, message );
}


class GLCtrl_glad : public Ctrl {
	typedef GLCtrl_glad CLASSNAME;

public:
	Image  MouseEvent(int event, Point p, int zdelta, dword keyflags) override;
#ifdef PLATFORM_POSIX
	void   Paint(Draw& w) override;
#endif
	
private:
#ifdef PLATFORM_WIN32
	struct GLPane : DHCtrl {
		friend class GLCtrl_glad;
		
		GLCtrl_glad *ctrl;

		void DoGLPaint();
		
	public:
		GLPane() { NoWantFocus(); }
		
		virtual void    State(int reason);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual Image   MouseEvent(int event, Point p, int zdelta, dword keyflags);
		
		void Init();
		void Destroy();
		
		void ActivateContext();

		void ExecuteGL(HDC hdc, Event<> paint, bool swap_buffers);
		void ExecuteGL(Event<> paint, bool swap_buffers);
	};
#endif

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
	
	GLCtrl_glad& RedirectMouse(Ctrl *target)           { mouseTarget = target; return *this; }
	
	void ExecuteGL(Event<> gl, bool swap_buffers = false);

	GLCtrl_glad()                                      { Init(); }

#ifdef PLATFORM_WIN32
	void Refresh()                                { pane.Refresh(); }
#endif

	// deprecated (these settings are now static, as we have just single context)
	GLCtrl_glad& DepthBits(int n)               { depthSize = n; return *this; }
	GLCtrl_glad& StencilBits(int n)             { stencilSize = n; return *this; }
	GLCtrl_glad& DoubleBuffering(bool b = true) { doubleBuffering = b; return *this; }
	GLCtrl_glad& MSAA(int n = 4)                { numberOfSamples = n; return *this; }

	GLCtrl_glad(int  depthsize, int  stencilsize = 8, bool doublebuffer = true,
	       bool multisamplebuffering = false, int  numberofsamples = 0)
	{ Init(); DepthBits(depthsize).StencilBits(stencilsize).DoubleBuffering(doublebuffer).MSAA(numberofsamples); }

	// deprecated (fixed pipeline is so out of fashion...)
	void StdView();
};

}

#endif
