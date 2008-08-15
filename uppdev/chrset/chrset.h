#ifndef _chrset_chrset_h
#define _chrset_chrset_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <chrset/chrset.lay>
#include <CtrlCore/lay.h>



class chrset : public WithchrsetLayout<TopWindow> {
public:
	typedef chrset CLASSNAME;
	chrset();
};

#endif
