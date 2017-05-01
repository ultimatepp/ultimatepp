#ifndef _Spin_Spin_h
#define _Spin_Spin_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Spin/Spin.lay>
#include <CtrlCore/lay.h>

class Spin : public WithSpinLayout<TopWindow> {
public:
	typedef Spin CLASSNAME;
	Spin();
};

#endif
