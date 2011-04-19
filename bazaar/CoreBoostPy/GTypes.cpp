#include "GTypes.h"
using namespace boost::python;

NAMESPACE_UPP

//Color

struct Color_to_python
{
	static PyObject* convert(const Color& c)
	{
		list l;
		l.append(object(c.GetR()));
		l.append(object(c.GetG()));
		l.append(object(c.GetB()));
		return incref(l.ptr()); //need to create a list
	}
};

struct Color_from_python
{
	Color_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Color>());
	}

	static void* convertible(PyObject* po)
	{
		if(PyList_Check(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Color>*)data)->storage.bytes;
		
		list l(handle<>(borrowed(po)));
		assert(len(l)==3);
		int r,g,b;
		r = extract<int>(l[0]);
		g = extract<int>(l[1]);
		b = extract<int>(l[2]);
		new(d) Color(r,g,b);

		data->convertible = d;
	}
};

void export_Color()
{
ONCELOCK
{
	to_python_converter<Color, Color_to_python>();
	Color_from_python();
}
}

//

void export_GTypes()
{
ONCELOCK
{
	export_Color();
}
}

END_UPP_NAMESPACE
