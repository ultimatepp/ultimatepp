#ifndef _Layout_Layout_h
#define _Layout_Layout_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Layout/Layout.lay>
#include <CtrlCore/lay.h>

class Layout : public WithLayoutLayout<TopWindow> {
public:
	typedef Layout CLASSNAME;
	Layout();
};

#endif
