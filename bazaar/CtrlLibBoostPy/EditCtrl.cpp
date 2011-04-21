#include "EditCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_EditCtrl()
{
ONCELOCK
{
	class_<EditField, bases<Ctrl>, EditField, boost::noncopyable>("EditField", "Upp EditField", no_init)
	;

	class_<EditString, bases<EditField>, EditString, boost::noncopyable>("EditString", "Upp EditString", no_init)
	;
	class_<EditInt, bases<EditField>, EditInt, boost::noncopyable>("EditInt", "Upp EditInt", no_init)
	;
	class_<EditInt64, bases<EditField>, EditInt64, boost::noncopyable>("EditInt64", "Upp EditInt64", no_init)
	;
	class_<EditDouble, bases<EditField>, EditDouble, boost::noncopyable>("EditDouble", "Upp EditDouble", no_init)
	;
	class_<EditStringNotNull, bases<EditString>, EditStringNotNull, boost::noncopyable>("EditStringNotNull", "Upp EditStringNotNull", no_init)
	;
	class_<EditIntNotNull, bases<EditInt>, EditIntNotNull, boost::noncopyable>("EditIntNotNull", "Upp EditIntNotNull", no_init)
	;
	class_<EditInt64NotNull, bases<EditInt64>, EditInt64NotNull, boost::noncopyable>("EditInt64NotNull", "Upp EditInt64NotNull", no_init)
	;
	class_<EditDoubleNotNull, bases<EditDouble>, EditDoubleNotNull, boost::noncopyable>("EditDoubleNotNull", "Upp EditDoubleNotNull", no_init)
	;

	class_<EditIntSpin, bases<EditInt>, EditIntSpin, boost::noncopyable>("EditIntSpin", "Upp EditIntSpin", no_init)
	;
	class_<EditInt64Spin, bases<EditInt64>, EditInt64Spin, boost::noncopyable>("EditInt64Spin", "Upp EditInt64Spin", no_init)
	;
	class_<EditDoubleSpin, bases<EditDouble>, EditDoubleSpin, boost::noncopyable>("EditDoubleSpin", "Upp EditDoubleSpin", no_init)
	;
	class_<EditIntNotNullSpin, bases<EditIntSpin>, EditIntNotNullSpin, boost::noncopyable>("EditIntNotNullSpin", "Upp EditIntNotNullSpin", no_init)
	;
	class_<EditInt64NotNullSpin, bases<EditInt64Spin>, EditInt64NotNullSpin, boost::noncopyable>("EditInt64NotNullSpin", "Upp EditInt64NotNullSpin", no_init)
	;
	class_<EditDoubleNotNullSpin, bases<EditDoubleSpin>, EditDoubleNotNullSpin, boost::noncopyable>("EditDoubleNotNullSpin", "Upp EditDoubleNotNullSpin", no_init)
	;
}
}

END_UPP_NAMESPACE
