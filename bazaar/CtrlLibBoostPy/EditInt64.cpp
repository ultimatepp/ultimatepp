#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void EditInt64SetMin(EditInt64& c, int m) { c.MinMax(m, c.GetMax()); }
void EditInt64SetMax(EditInt64& c, int m) { c.MinMax(c.GetMin(), m); }
void EditInt64SetNotNull(EditInt64& c, bool b) { c.NotNull(b); }

void EditInt64SpinSetInc(EditInt64Spin& c, int m) { c.SetInc(m); }
void EditInt64SpinShowSpin(EditInt64Spin& c, bool b) { c.ShowSpin(b); }

void export_EditInt64()
{
ONCELOCK
{
	class_<EditInt64, bases<EditField>, boost::noncopyable>("EditInt64", "Upp EditInt64")
		.add_property("max", &EditInt64::GetMax, &EditInt64SetMax)
		.add_property("min", &EditInt64::GetMin, &EditInt64SetMin)
		.add_property("notnull", &EditInt64::IsNotNull, &EditInt64SetNotNull)
	;
	class_<EditInt64NotNull, bases<EditInt64>, boost::noncopyable>("EditInt64NotNull", "Upp EditInt64NotNull")
	;
	class_<EditInt64Spin, bases<EditInt64>, boost::noncopyable>("EditInt64Spin", "Upp EditInt64Spin")
		.add_property("inc", &EditInt64Spin::GetInc, &EditInt64SpinSetInc)
		.add_property("showspin", &EditInt64Spin::IsSpinVisible, &EditInt64SpinShowSpin)
	;
	class_<EditInt64NotNullSpin, bases<EditInt64Spin>, boost::noncopyable>("EditInt64NotNullSpin", "Upp EditInt64NotNullSpin")
	;
}
}

END_UPP_NAMESPACE
