#ifndef _KissFFTTest_KissFFTTest_h
#define _KissFFTTest_KissFFTTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <KissFFTTest/KissFFTTest.lay>
#include <CtrlCore/lay.h>

#include <plugin/kissfft/kissfft.h>

class KissFFTTest : public WithKissFFTTestLayout<TopWindow> {
public:
	typedef KissFFTTest CLASSNAME;
	KissFFTTest();
};

#endif

