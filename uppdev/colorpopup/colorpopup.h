#ifndef _colorpopup_colorpopup_h
#define _colorpopup_colorpopup_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <colorpopup/colorpopup.lay>
#include <CtrlCore/lay.h>



class colorpopup : public WithcolorpopupLayout<TopWindow> {
public:
	typedef colorpopup CLASSNAME;
	colorpopup();
};

#endif
