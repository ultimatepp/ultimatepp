#ifndef _CtrlCore_CocoMM_h_
#define _CtrlCore_CocoMM_h_

#include <Core/config.h>

#ifdef PLATFORM_COCOA

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
String  ToString(CFStringRef s);

inline WString ToWString(NSString *s) { return ToWString((CFStringRef)s); }
inline String  ToString(NSString *s)  { return ToString((CFStringRef)s); }

inline CGContextRef GetCG(SystemDraw& w) { return (CGContextRef)w.GetCGHandle(); }

#define      cgHandle   (CGContextRef)handle

struct PointCG {
	int x, y;

	operator CGPoint() const { return CGPointMake(x, y); }
	
	PointCG(int x, int y) : x(x), y(y) {}
	PointCG();
};

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

struct Upp::MMCtrl {
	static void SyncRect(CocoView *view);
};

struct Upp::CocoTop {
	NSWindow *window = NULL;
	CocoView *view = NULL;
	Ptr<Ctrl> owner;
};

void SyncRect(CocoView *view);

inline Upp::Rect MakeRect(const CGRect& r) {
	return Upp::RectC(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

inline CGRect MakeCGRect(const Upp::Rect& r) {
	return CGRectMake(r.left, r.top, r.GetWidth(), r.GetHeight());
}

#endif

#endif
