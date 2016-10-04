#ifndef _PinA_aggdraw_h_
#define _PinA_aggdraw_h_

#include <Draw/Draw.h>

// Testing svn rev 329...

namespace Upp
{
	class AggDraw : public Draw 
	{
		public:
		
		AggDraw(int cx, int cy);
		AggDraw(Size sz);
		~AggDraw();
	
		virtual void BeginOp();
		virtual void EndOp();
		virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
		
		operator Image();
		
		private:
		
		struct AggDrawPrivate;
		AggDrawPrivate * data;
		
	};
}
#endif
