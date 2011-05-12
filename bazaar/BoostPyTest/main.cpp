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

		scope(upp_module).attr("app") = ptr((TopWindow*)this);

		sl.SetData(50);
		scope(upp_module).attr("sl") = ptr(&sl);

		vc.SetData(123);
		scope(upp_module).attr("vc") = ptr(&vc);

		es.SetData("Hello from Upp");
		scope(upp_module).attr("es") = ptr(&es);

		scope(upp_module).attr("pr") = ptr(&pr);


		//the additional import is needless
		String sc = 

		"p = hello.World()\n"
		"p.set('Some Greet Text')\n"
		"print p.get()\n"

		"upp.sl.data = 75\n"
		"print upp.sl.data\n"

		"upp.vc.data = range(10)\n"
		"print upp.vc.data\n"

		"print upp.es.data\n"
		"upp.es.data = 'Another Text from Upp'\n"
		"print upp.es.data\n"

		"c = upp.Color(123,124,125)\n"
		"print c\n"

		"pp = p\n"
		"p.set('Hi')\n"
		"print p\n"
		"print pp\n"

		//"upp.vc.data = c\n"

		"import time\n"
		"upp.pr.reset()\n"
		"upp.pr.create()\n"
		"upp.pr.canceled()\n"
		"i = 0\n"
		"while i < 3:\n"
		"    print i\n"
		"    upp.pr.text = str(i)\n"
		"    i = i+1\n"
		"    upp.pr.step(1)\n"
		"    time.sleep(1)\n"
		"    if upp.pr.canceled():\n"
		"        break\n"
		"upp.pr.close()\n"

		"def ff():\n"
		"    print 'MyCallback, would could work with p.greet or lambda: as well'\n"
		"upp.sl.whenaction = ff\n"

		"sl2 = upp.SliderCtrl()\n"
		"sl2.rightpos(0,200)\n"
		"sl2.bottompos(0,40)\n"
		"sl2.whenaction = ff\n"
		"sl2.data = 12\n"
		"upp.app.add(sl2)\n"
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

