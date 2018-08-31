#ifndef _FontCJK_FontCJK_h
#define _FontCJK_FontCJK_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <FontCJK/FontCJK.lay>
#include <CtrlCore/lay.h>

class FontCJK : public WithFontCJKLayout<TopWindow> {
public:
	typedef FontCJK CLASSNAME;
	FontCJK();
};

#endif
