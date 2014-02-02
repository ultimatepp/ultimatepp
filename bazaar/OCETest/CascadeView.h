#ifndef _CascadeView_h_
#define _CascadeView_h_

#include "CascadeDocument.h"

#include <V3d_View.hxx>

using namespace Upp;

class CascadeView : public OCECtrl
{
	private:
	
		// Panning active flag
		bool isPanning;
		
		// View rotation active flag
		bool isRotating;
		
		// Start point in pan operations
		Point panStartPoint, panPrevPoint;

	protected:
			
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
		CascadeView();
	
		// Destructor class CascadeView
		~CascadeView();

}; // END Class CascadeView


#endif
