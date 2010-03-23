#ifndef _Fonts_Fonts_h
#define _Fonts_Fonts_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Fonts/Fonts.lay>
#include <CtrlCore/lay.h>



class Fonts : public WithFontsLayout<TopWindow> {
public:
	typedef Fonts CLASSNAME;
	
	void Copy();
	void Reload();
	
	Fonts();
};

#endif

