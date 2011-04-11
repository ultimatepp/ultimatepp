#include "BoostPyTest.h"

BoostPyTest::BoostPyTest()
	: slpy(sl)
{
	CtrlLayout(*this, "Boost Test");

	//bool b = Py_IsInitialized();

	int ret;
	ret = PyImport_AppendInittab( "hello", &inithello );
	ret = PyImport_AppendInittab( "UppCtrl", &initUppCtrl );

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

		w.set("Welcom on earth");
		scope(hello_module).attr("earth") = ptr(&w);

		slpy.Set(50);
		object ctrl_module(handle<>(PyImport_ImportModule("UppCtrl"))); 
		main_namespace["UppCtrl"] = ctrl_module;
		scope(ctrl_module).attr("sl") = ptr(&slpy);

		//the additional import is needless
		String sc = 
		"import hello\n"
		"p = hello.World()\n"
		"p.set('Some Greet Text')\n"
		"print p.greet()\n"
		
		"print UppCtrl.sl.get()\n"
		"UppCtrl.sl.set(23)\n"
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

