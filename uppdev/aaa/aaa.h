#ifndef _aaa_aaa_h
#define _aaa_aaa_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <aaa/aaa.lay>
#include <CtrlCore/lay.h>

class aaa : public WithaaaLayout<TopWindow> 
{
public:
	typedef aaa CLASSNAME;
	aaa();
};

#endif

