#ifndef _telupp_telupp_h_
#define _telupp_telupp_h_

#include <RichText/RichText.h>

using namespace Upp;

struct TelDraw : public SDraw {
public:
	virtual void  PutImage(Point p, const Image& img, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
public:	
	enum Code {
		RECT = 0,
		IMAGE = 1,
		SETIMAGE = 2,
	};
	
	static Index<int64>           img_index;
	
	static int GetImageI(TelDraw& w, const Image& img);
	static void ResetI()          { img_index.Clear(); }

	StringBuffer result;

	void Put8(int x)              { result.Cat(x); }
	void Put16(int x);
	void Put(Point p);
	void Put(Size sz);
	void Put(const Rect& r);
};

#endif
