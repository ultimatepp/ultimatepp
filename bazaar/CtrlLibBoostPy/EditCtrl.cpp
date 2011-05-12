#include "EditCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_EditCtrl()
{
ONCELOCK
{
	class_<EditField, bases<Ctrl>, boost::noncopyable>("EditField", "Upp EditField")
	;

	class_<EditString, bases<EditField>, boost::noncopyable>("EditString", "Upp EditString")
	;
	class_<EditInt, bases<EditField>, boost::noncopyable>("EditInt", "Upp EditInt")
	;
	class_<EditInt64, bases<EditField>, boost::noncopyable>("EditInt64", "Upp EditInt64")
	;
	class_<EditDouble, bases<EditField>, boost::noncopyable>("EditDouble", "Upp EditDouble")
	;
	class_<EditStringNotNull, bases<EditString>, boost::noncopyable>("EditStringNotNull", "Upp EditStringNotNull")
	;
	class_<EditIntNotNull, bases<EditInt>, boost::noncopyable>("EditIntNotNull", "Upp EditIntNotNull")
	;
	class_<EditInt64NotNull, bases<EditInt64>, boost::noncopyable>("EditInt64NotNull", "Upp EditInt64NotNull")
	;
	class_<EditDoubleNotNull, bases<EditDouble>, boost::noncopyable>("EditDoubleNotNull", "Upp EditDoubleNotNull")
	;

	class_<EditIntSpin, bases<EditInt>, boost::noncopyable>("EditIntSpin", "Upp EditIntSpin")
	;
	class_<EditInt64Spin, bases<EditInt64>, boost::noncopyable>("EditInt64Spin", "Upp EditInt64Spin")
	;
	class_<EditDoubleSpin, bases<EditDouble>, boost::noncopyable>("EditDoubleSpin", "Upp EditDoubleSpin")
	;
	class_<EditIntNotNullSpin, bases<EditIntSpin>, boost::noncopyable>("EditIntNotNullSpin", "Upp EditIntNotNullSpin")
	;
	class_<EditInt64NotNullSpin, bases<EditInt64Spin>, boost::noncopyable>("EditInt64NotNullSpin", "Upp EditInt64NotNullSpin")
	;
	class_<EditDoubleNotNullSpin, bases<EditDoubleSpin>, boost::noncopyable>("EditDoubleNotNullSpin", "Upp EditDoubleNotNullSpin")
	;
}
}

END_UPP_NAMESPACE
