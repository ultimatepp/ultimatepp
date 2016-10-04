#ifndef _paintbox_h_
#define _paintbox_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct PaintBox : public Ctrl 
{
	public:
	
	Callback1< Draw & > 				onPaint;
	Callback2< Point , dword > 			onLeftDown;
	Callback2< Point , dword > 			onLeftUp;
	Callback2< Point , dword >			onMouseMove;
	Callback3< Point , int , dword > 	onMouseWheel;
	Callback1< dword > 					onKeyPress;
	
	PaintBox( void );
	
	virtual void 		Paint( Draw & w );
	virtual void 		MouseMove( Point p, dword keyflags );
	virtual void   		LeftUp( Point p, dword keyflags );
	virtual void 		LeftDown( Point p, dword keyflags );
	virtual void   		MouseWheel( Point p, int zdelta, dword keyflags );
	virtual bool		Key( dword key, int count );

	private:
	
	
	
};

#endif
