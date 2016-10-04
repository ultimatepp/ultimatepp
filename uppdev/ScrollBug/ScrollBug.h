#ifndef _ScrollBug_ScrollBug_h
#define _ScrollBug_ScrollBug_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ScrollBug/ScrollBug.lay>
#include <CtrlCore/lay.h>

class ScrollBug : public WithScrollBugLayout<TopWindow> {
public:
	typedef ScrollBug CLASSNAME;
	ScrollBug();

	LineEdit edit;
};

#endif
