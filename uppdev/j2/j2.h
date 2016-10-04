#ifndef _j2_j2_h
#define _j2_j2_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <j2/j2.lay>
#include <CtrlCore/lay.h>

class j2 : public Withj2Layout<TopWindow> {
public:
	typedef j2 CLASSNAME;
	void Quit();
	void DragEnter();	
	void GotFocus();
	j2();
};

#endif

