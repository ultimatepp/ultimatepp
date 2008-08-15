#ifndef _bbb_bbb_h
#define _bbb_bbb_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <bbb/bbb.lay>
#include <CtrlCore/lay.h>



class bbb : public WithbbbLayout<TopWindow> {
public:
	typedef bbb CLASSNAME;
	bbb();
};

#endif
