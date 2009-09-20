#ifndef _PixRasterThumbsCtrl_h_
#define _PixRasterThumbsCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "ImageCache.h"

#include "PixRasterBaseCtrl.h"

NAMESPACE_UPP

// forward declaration
class PixRasterCtrl;

///////////////////////////////////////////////////////////////////////////////////////////////
// Tiff Thumbnails ctrl class
class PixRasterThumbsCtrl : public PixRasterBaseCtrl
{
	public :
		typedef PixRasterThumbsCtrl CLASSNAME;
		
	private :
		// calculates image scale factor
		virtual int CalcScale(int imageScale, int rasterWidth, int maxPageHeight);

	public:
		// constructor
		PixRasterThumbsCtrl(PixRasterCtrl *t);
		
		// destructor
		~PixRasterThumbsCtrl();
		
		// left click handler --> shows requested page in viewer
		virtual void LeftDown(Point p, dword keyflags);

} ; // END class PixRasterThumbsCtrl

END_UPP_NAMESPACE

#endif
