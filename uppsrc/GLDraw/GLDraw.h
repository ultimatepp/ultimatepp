#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <Draw/Draw.h>
#include <GL/gl.h>

namespace Upp {

class GLDraw : SDraw {
	void SetColor(Color c);

	uint64   context;

public:
	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
	void InitGL(Size sz, uint64 context = 0);
	
	static void ClearCache();
	static void ResetCache();
	
	GLDraw() { context = 0; }
};

};

#endif
