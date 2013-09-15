#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <GLCtrl/GLCtrl.h>

namespace Upp {

struct GLDraw : SDraw {
	void SetColor(Color c);
//	void SetVertexRect(const Rect& r);

	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
	void InitGL(const Rect& r);
	
	static void ClearCache();
	static void ResetCache();
};

};

#endif
