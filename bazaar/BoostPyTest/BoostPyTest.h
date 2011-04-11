#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>

using namespace boost::python;

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

struct World
{
	void set(std::string msg) { this->msg = msg; }
	std::string greet() { return msg; }
	std::string msg;
};

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();
};

#endif
