#ifndef _VerticalLabel_VerticalLabel_h
#define _VerticalLabel_VerticalLabel_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <VerticalLabel/VerticalLabel.lay>
#include <CtrlCore/lay.h>

class VerticalLabel : public WithVerticalLabelLayout<TopWindow> {
public:
	VerticalLabel();
};

#endif
