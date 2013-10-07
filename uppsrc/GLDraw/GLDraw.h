#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <Draw/Draw.h>
#include <GL/gl.h>

namespace Upp {
	
// #define USE_VBO 1

class GLDraw : public SDraw {
	void SetColor(Color c);

	uint64   context;

#ifdef USE_VBO
	GLuint   vbo, tbo;
	
#endif

public:
	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
	void Init(Size sz, uint64 context = 0);
	
	static void ClearCache();
	static void ResetCache();
	
	GLDraw()  { context = 0; /* vbo = 0; tbo = 0; */ }

	~GLDraw();
};

};

#endif
