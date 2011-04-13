#ifndef _BoostPyTest_SliderCtrlPy_h_
#define _BoostPyTest_SliderCtrlPy_h_

#include <Py/Py.h>
using namespace boost::python;

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//A wrapped instance

struct SliderCtrlPy
{
	SliderCtrlPy(SliderCtrl& o) : o(o) {}

	void Set(const int& d) { o.SetData(d); }
	int Get() const { return o.GetData(); }
	
	SliderCtrl& o;
};

//fw
void export_UppCtrl();
void export_SliderCtrl(); //relys on Value export

#endif
