#include "BoostPyTest.h"

//CAUTION special module definitions to reduce compile time
#include "modules.cppi"

BoostPyTest::BoostPyTest()
{
	CtrlLayout(*this, "Boost Test");
	Sizeable().Zoomable();

	//bool b = Py_IsInitialized();

	int ret;
	ret = PyImport_AppendInittab( "hello", &inithello );
	ret = PyImport_AppendInittab( "upp", &initupp );

	//Py_Initialize(); //should be done *after* PyImport_AppendInittab, but it still works :), leaving INITBLOCK from Py
	//PyCon::Init();

	PyCon::AtExit = THISBACK(ExitHandler);

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
					//is only caused on boost python module declarations
		main_namespace["hello"] = hello_module;

		w.set("Welcom on earth");
		scope(hello_module).attr("earth") = ptr(&w);


		object upp_module = import("upp");
		main_namespace["upp"] = upp_module;

		sl.SetData(50);
		scope(upp_module).attr("sl") = ptr(&sl);

		vc.SetData(123);
		scope(upp_module).attr("vc") = ptr(&vc);

		//the additional import is needless
		String sc = 
		"p = hello.World()\n"
		"p.set('Some Greet Text')\n"
		"print p.greet()\n"
		
		"upp.sl.setdata(75)\n"
		"print upp.sl.getdata()\n"

		"upp.vc.setdata(range(10))\n"
		"print upp.vc.getdata()\n"

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

void BoostPyTest::ExitHandler()
{
	SetTimeCallback(1000, Breaker());
}

GUI_APP_MAIN
{
	BoostPyTest().Run();
}

