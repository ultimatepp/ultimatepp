#ifndef _WebWord_WebWord_h
#define _WebWord_WebWord_h

#ifdef flagTURTLEGUI
#include <Turtle/Turtle.h>
#else
#include <CtrlLib/CtrlLib.h>
#endif

#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

namespace Upp {

class WebWord : public TopWindow {
public:
	typedef WebWord CLASSNAME;
	WebWord();
	void ShowInfo();
	
	RichEditWithToolBar  editor;
	StatusBar            statusbar;
};

}
#endif
