#ifndef _TiffThumbsCtrl_h_
#define _TiffThumbsCtrl_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// forward declaration
class TiffCtrl;

//#############################################################################################
// Tiff Thumbnails ctrl class
class TiffThumbsCtrl : public Ctrl
{
	public :
		typedef TiffThumbsCtrl CLASSNAME;
		
	private :
	
		// associated TiffCtrl widget
		TiffCtrl *tiffCtrl;
		
		// scrollbar
		ScrollBar scrollBar;
		
		// thumbs scale -- in percent of real size
		int thumbsScale;
		
		// array of thumbnails positions and sizes
		Vector<Rect> thumbsRects;
		
		// scrollbar handler
		void OnScroll(void);
	
		// paint routine
		virtual void Paint(Draw &d);
	
	public :
	
		// constructor
		TiffThumbsCtrl(TiffCtrl *t);
		
		// destructor
		~TiffThumbsCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);		
		
} ; // END class TiffThumbsCtrl

#endif
