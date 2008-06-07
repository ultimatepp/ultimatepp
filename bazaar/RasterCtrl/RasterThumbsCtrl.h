#ifndef _RasterThumbsCtrl_h_
#define _RasterThumbsCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "ImageCache.h"

#include "RasterBaseCtrl.h"

using namespace Upp;

// forward declaration
class RasterCtrl;

///////////////////////////////////////////////////////////////////////////////////////////////
// Tiff Thumbnails ctrl class
class RasterThumbsCtrl : public RasterBaseCtrl
{
	public :
		typedef RasterThumbsCtrl CLASSNAME;
		
	private :
		// calculates image scale factor
		virtual int CalcScale(int imageScale, int rasterWidth, int maxPageHeight);

	public:
		// constructor
		RasterThumbsCtrl(RasterCtrl *t);
		
		// destructor
		~RasterThumbsCtrl();
		
		// left click handler --> shows requested page in viewer
		void LeftDown(Point p, dword keyflags);

} ; // END class RasterThumbsCtrl

#endif
