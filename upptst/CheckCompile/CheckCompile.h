#ifndef _CheckCompile_CheckCompile_h
#define _CheckCompile_CheckCompile_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CheckCompile/CheckCompile.lay>
#include <CtrlCore/lay.h>

class CheckCompile : public WithCheckCompileLayout<TopWindow> {
public:
	typedef CheckCompile CLASSNAME;
	CheckCompile();
};

#endif
