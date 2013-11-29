#ifndef _telupp_telupp_h_
#define _telupp_telupp_h_

#include <Draw/Draw.h>

using namespace Upp;

enum Code {
	DRAW_RECT = 0,
	DRAW_IMAGE = 1,
};

struct TelDraw : public SDraw {
public:
	virtual void  PutImage(Point p, const Image& img, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
public:	
	StringBuffer result;

	void Put8(int x)              { result.Cat(x); }
	void Put16(int x);
	void Put(Point p);
	void Put(Size sz);
	void Put(const Rect& r);

};

#endif
