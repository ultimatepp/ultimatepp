#ifndef _GUISample1_GUISample1_h
#define _GUISample1_GUISample1_h


#include <CtrlLib/CtrlLib.h>



using namespace Upp;

#define LAYOUTFILE <GUISample1/GUISample1.lay>
#include <CtrlCore/lay.h>




class GUISample1 : public WithGUISample1Layout<TopWindow> {
public:
	typedef GUISample1 CLASSNAME;
	GUISample1();
	void ToolbarClicked();
	void InitToolbar(Bar& bar);
};

#endif

