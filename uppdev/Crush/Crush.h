#ifndef _Crush_Crush_h
#define _Crush_Crush_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Crush/Crush.lay>
#include <CtrlCore/lay.h>


class Crush : public WithCrushLayout<TopWindow>
{
public:
	typedef Crush CLASSNAME;

	Crush();

private:
	void CrushThis();
};

#endif
