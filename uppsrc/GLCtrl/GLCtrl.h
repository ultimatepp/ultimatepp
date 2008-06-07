#ifndef _GLCtrl_GLCtrl_h
#define _GLCtrl_GLCtrl_h

#include <CtrlLib/CtrlLib.h>

#include <GL/gl.h>
#include <GL/glu.h>

NAMESPACE_UPP

#ifdef PLATFORM_X11

#include <GL/glx.h>

class GLCtrl : public DHCtrl
{
	private:
	
		// OpenGL Context
		GLXContext WindowContext;

		// Number of instances
		static int Instances;
		
		// Current instance number
		int InstanceNum;
		
		// OpenGL parameters
		int DepthSize;
		int StencilSize;
		int NumberOfSamples;
		bool DoubleBuffering;
		bool MultiSampleBuffering;
		
		// Currently activated context number
		static int ContextActivated;

		// Activates current OpenGL context
		void ActivateContext();
		
		// Ovverridden method to choose the correct visual
		virtual XVisualInfo *CreateVisual(void);
		
		// Overridden method for attribute setting
		virtual void SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &attr);
		
		// Overridden method to create and destroy OpenGL context
		virtual void AfterInit(bool Error);
		virtual void BeforeTerminate(void);
		
		// Overridden method to resize GL windows
		virtual void Resize(int w, int h);

		// Internal OpenGL Paint method
		void doPaint(void);

		// Paint method - with graphic context
		// Called from DHCtrl - Graphic context is *not* uses
		virtual void Paint(Draw &/*draw*/);
		
	protected:
	
		// Overridable methods for derived controls
		
		// Called after succesful OpenGL initialization
		virtual void GLInit() {};
		
		// Called just before OpenGL termination
		virtual void GLDone() {};	
		
		// Called on resize events
		virtual void GLResize( int w, int h ) {};
		
		// Called on paint events
		virtual void GLPaint() {};

	public:
	
		typedef GLCtrl CLASSNAME;

		// Constructor class GLCtrl
		GLCtrl(	int		depthsize            = 24, 
	    		int		stencilsize          = 0, 
	    		bool	doublebuffer         = true, 
				bool	multisamplebuffering = false, 
				int		numberofsamples      = 0 );
	
		// Destructor class GLCtrl
		~GLCtrl();
		
		// Initializes OpenGL context to a standard view
		void StdView();
		
		// Forces control repaint
		void PostPaintGL(); // same as Refresh()
		
		// Forces control resize
		void PostResizeGL();
	
}; // END Class GLCtrl

#else

class GLCtrl : public DHCtrl {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	HDC   hDC;
	HGLRC hRC;

	void OpenGL();
	void CloseGL();

public:
	virtual void GLPaint();

	Callback WhenGLPaint;

	void    StdView();

	HDC     GetDC() const            { return hDC; }
	HGLRC   GetHGLRC() const         { return hRC; }

	GLCtrl();
	~GLCtrl();
};
#endif

END_UPP_NAMESPACE

#endif
