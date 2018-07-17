#ifndef _CtrlCore_CocoMM_h_
#define _CtrlCore_CocoMM_h_

#ifdef flagCOCOA

#define Point NS_Point
#define Rect  NS_Rect
#define Size  NS_Size
#include <AppKit/AppKit.h>
#undef  Point
#undef  Rect
#undef  Size

#include "CtrlCore.h"

namespace Upp {

template <class T>
struct CFRef {
	T ptr;
	T operator~()   { return ptr; }
	operator T()    { return ptr; }
	T  operator->() { return ptr; }
	T  Detach()     { T h = ptr; ptr = NULL; return h; }
	CFRef(T p)      { ptr = p; }
	~CFRef()        { if(ptr) CFRelease(ptr); }
};

struct AutoreleasePool {
	NSAutoreleasePool *pool;

	AutoreleasePool() {
		pool = [[NSAutoreleasePool alloc] init];
	}
	~AutoreleasePool() {
	    [pool release];
	}
};

WString ToWString(CFStringRef s);
String ToString(CFStringRef s);

#define      cgHandle   (CGContextRef)handle

struct RectCG {
	int x, y, cx, cy;

	operator CGRect() const { return CGRectMake(x, y, cx, cy); }
	
	RectCG(int x, int y, int cx, int cy) : x(x), y(y), cx(cx), cy(cy) {}
	RectCG();
};

}

@interface CocoView : NSView <NSWindowDelegate>
{
	@public
	Upp::Ctrl *ctrl;
}
@end

struct Upp::CocoTop {
	NSWindow *window = NULL;
	CocoView *view = NULL;
};

void SyncRect(CocoView *view);

inline Upp::Rect MakeRect(const CGRect& r) {
	return Upp::RectC(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

#endif

#endif
