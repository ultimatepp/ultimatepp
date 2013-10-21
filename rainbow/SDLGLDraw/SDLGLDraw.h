#ifndef _SDLGLDraw_SDLGLDraw_h
#define _SDLGLDraw_SDLGLDraw_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SDLGLDraw/SDLGLDraw.lay>
#include <CtrlCore/lay.h>

class SDLGLDraw : public TopWindow {
public:
	virtual void Paint(Draw& w);

	typedef SDLGLDraw CLASSNAME;
	SDLGLDraw();
};

#endif
