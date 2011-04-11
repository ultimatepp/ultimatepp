#include "BoostPyTest.h"

BOOST_PYTHON_MODULE(hello)
{
	class_<World>("World")
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}

BoostPyTest::BoostPyTest()
{
	CtrlLayout(*this, "Boost Test");

	int ret = PyImport_AppendInittab( "hello", &inithello );

	//Py_Initialize(); //should be done *after* PyImport_AppendInittab, but it still works :), leaving INITBLOCK from Py
	//PyCon::Init();

	try {
		object main_module(handle<>(borrowed(PyImport_AddModule("__main__"))));
		object main_namespace = main_module.attr("__dict__");

		handle<> ign(PyRun_String( "print \"Hello, World\"",
		                              Py_file_input,
		                              main_namespace.ptr(),
		                              main_namespace.ptr() ));

		object hello_module(handle<>(PyImport_ImportModule("hello"))); 
					//MEM LEAK from PyImport_ImportModule ??
					//http://mail.python.org/pipermail/python-win32/2008-August/008092.html
					//seems to be well known and not correctable
					//so better do it in python code
		main_namespace["hello"] = hello_module;

		String sc = 
		"import hello\n"
		"p = hello.World()\n"
		"p.set('Some Greet Text')\n"
		"print p.greet()\n"
		;
		con.cmd.SetData(sc);
	} catch( error_already_set ) {
		PyErr_Print();
	}
}

BoostPyTest::~BoostPyTest()
{
//	Py_Finalize();	
}

GUI_APP_MAIN
{
	BoostPyTest().Run();
}

