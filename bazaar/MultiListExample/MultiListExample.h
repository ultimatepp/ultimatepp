#ifndef _MultiListExample_MultiListExample_h
#define _MultiListExample_MultiListExample_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <MultiListExample/MultiListExample.lay>
#include <CtrlCore/lay.h>

class MultiListExample : public WithMultiListExampleLayout<TopWindow> {
public:
	typedef MultiListExample CLASSNAME;
	MultiListExample();
};

#endif

