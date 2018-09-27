#ifndef _LabelBox_LabelBox_h
#define _LabelBox_LabelBox_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <LabelBox/LabelBox.lay>
#include <CtrlCore/lay.h>

class MyApp : public WithLabelBoxLayout<TopWindow> {
public:
	typedef MyApp CLASSNAME;
	MyApp();
};

#endif
