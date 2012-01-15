#ifndef _CascadeView_h_
#define _CascadeView_h_

#include <GLCtrl/GLCtrl.h>
using namespace Upp;

#include "CascadeDocument.h"

#include <OCE/OCE.h>
#include <V3d_View.hxx>

class CascadeView : public DHCtrl
{
	private:
	
		// The associated document
		CascadeDocument *Document;
		
		// The view object
		Handle(V3d_View) View;
		
		// Panning active flag
		bool isPanning;
		
		// View rotation active flag
		bool isRotating;
		
		// Start point in pan operations
		Point PanStartPoint, PanPrevPoint;

	protected:
			
#ifdef PLATFORM_X11
		// Method to choose the correct visual
		virtual XVisualInfo *CreateVisual(void);
		
		// Method for attribute setting
		virtual void SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &attr);
		
		// These is called just after initialization
		virtual void AfterInit(bool Error) ;

		// These is called just before termination
		virtual void BeforeTerminate(void) ;
#else
		// state handler, to catch init and terminate
		void State(int reason);
#endif
		
		// initializes view after platform-dependent init
		void InitView(void);
		
		// Handle layout events and propagates to embedded window
		virtual void Layout(void) ;

		// Paint function with context
		virtual void Paint(Draw &draw) ;
		
		////////////////////////////////////////////////////////////////////////////////////////////
		// mouse events handling
		
		// middle drag - pan or 3dorbit if in addition to ctrl-shift keys
		virtual void MiddleDrag(Point p, dword keyflags);
		
		// mouse wheel - zoom
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
		// double middle click - zoom extents
		virtual void MiddleDouble(Point p, dword keyflags);

		// middle up - resets pan/3dorbit behaviour
		virtual void MiddleUp(Point p, dword keyflags);

		// mouse move handler
		virtual void MouseMove(Point p, dword keyflags);

		// mouse enter handler
		virtual void   MouseEnter(Point p, dword keyflags);

		// mouse leave handler
		virtual void   MouseLeave();

		////////////////////////////////////////////////////////////////////////////////////////////

	public:
		typedef CascadeView CLASSNAME;

		// Constructor class CascadeView
		CascadeView(CascadeDocument *Doc);
	
		// Destructor class CascadeView
		~CascadeView();

		// Document property
		CascadeDocument const &getDocument(void) { return *Document; };
	
		// Fit all in view
		void FitAll();

}; // END Class CascadeView


#endif
