#ifndef _SWR_SWR_h
#define _SWR_SWR_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SWR/SWR.lay>
#include <CtrlCore/lay.h>



class SWR : public WithSWRLayout<TopWindow> {
public:
	typedef SWR CLASSNAME;
	SWR();
};

#endif

