#ifndef _SmartText_SmartText_h
#define _SmartText_SmartText_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <SmartText/SmartText.lay>
#include <CtrlCore/lay.h>



class SmartText : public TopWindow {
public:
	virtual void Paint(Draw& w);

	Button button;

	typedef SmartText CLASSNAME;
	SmartText();
};

#endif
