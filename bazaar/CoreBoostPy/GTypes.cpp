#include "GTypes.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Color()
{
ONCELOCK
{
	class_<Color>("Color", "Upp Color")
		.def(init<int, int, int>())
		.def(init<int, int>())
		.add_property("raw", &Color::GetRaw)
		.add_property("r", &Color::GetR)
		.add_property("g", &Color::GetG)
		.add_property("b", &Color::GetB)
		.def("setnull", &Color::SetNull)
		.def("isnull", &IsNull<Color>)
		.def("__str__", (String (*)(const Color&))&AsString<Color>)
	;
}
}

void export_Size()
{
ONCELOCK
{
	class_<Size>("Size", "Upp Size")
		.def(init<int, int>())
		.def_readwrite("cx", &Size::cx)
		.def_readwrite("cy", &Size::cy)
		.def("clear", &Size::Clear)
		.def("setnull", &Size::SetNull)
		.def("isnull", &IsNull<Size>)
		.def("__str__", (String (*)(const Size&))&AsString<Size>)
	;
}
}

void export_Point()
{
ONCELOCK
{
	class_<Point>("Point", "Upp Point")
		.def(init<int, int>())
		.def_readwrite("x", &Point::x)
		.def_readwrite("y", &Point::y)
		.def("clear", &Point::Clear)
		.def("setnull", &Point::SetNull)
		.def("isnull", &IsNull<Point>)
		.def("__str__", (String (*)(const Point&))&AsString<Point>)
	;
}
}

void export_Rect()
{
ONCELOCK
{
	class_<Rect>("Rect", "Upp Rect")
		.def(init<int, int, int, int>())
		.def(init<Point, Point>())
		.def(init<Point, Size>())
		.def_readwrite("left", &Rect::left)
		.def_readwrite("top", &Rect::top)
		.def_readwrite("right", &Rect::right)
		.def_readwrite("bottom", &Rect::bottom)
		.def("clear", &Rect::Clear)
		.def("setnull", &Rect::SetNull)
		.def("isnull", &IsNull<Rect>)
		.def("__str__", (String (*)(const Rect&))&AsString<Rect>)

		.add_property("topleft", &Rect::TopLeft)
		.add_property("topcenter", &Rect::TopCenter)
		.add_property("topright", &Rect::TopRight)

		.add_property("centerleft", &Rect::CenterLeft)
		.add_property("center", &Rect::CenterPoint)
		.add_property("centerright", &Rect::CenterRight)

		.add_property("bottomleft", &Rect::BottomLeft)
		.add_property("bottomcenter", &Rect::BottomCenter)
		.add_property("bottomright", &Rect::BottomRight)

		.add_property("width", &Rect::Width)
		.add_property("height", &Rect::Height)
		.add_property("size", &Rect::Size)

		.add_property("centerpos", (Point (Rect::*)(int, int) const)&Rect::CenterPos)
		.add_property("centerrect", (Rect (Rect::*)(int, int) const)&Rect::CenterRect)
	;
}
}

//

void export_GTypes()
{
ONCELOCK
{
	export_Color();
	export_Point();
	export_Size();
	export_Rect();
}
}

END_UPP_NAMESPACE
