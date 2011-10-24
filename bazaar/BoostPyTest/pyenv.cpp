#include "BoostPyTest.h"
using namespace boost::python;

//CAUTION special module definitions to reduce compile time
#include "modules.cppi"

String GetDataDir()
{
	String s = GetEnv("UPP_MAIN__");
	return s.GetCount() ? s : GetFileFolder(GetExeFilePath());
}

void BoostPyTest::CBi(int i)
{
	RLOG(i);	
}

dict swap_object_dict(object target, dict d)
{
	dict result = extract<dict>(target.attr("__dict__"));
	target.attr("__dict__") = d;
	return result;
}

void BoostPyTest::EvalCB()
{
	String s = ev.GetData();

	Value v;
	try
	{
		object o = eval(s.Begin(), main_namespace, main_namespace);
		v = extract<Value>(o);
	}
	catch(boost::python::error_already_set const &)
	{
		// Parse and output the exception
		std::string perror_str = parse_py_exception();
		String es = perror_str;
		v = ErrorValue(es);
	}
	
	evr.SetData(v);	
}

void BoostPyTest::EvalPyCv()
{
	Value v = invc.GetData();
	pycv.expr = evcv.GetData();
	Value w = pycv.Format(v);
	outvc.SetData(w);
}

void BoostPyTest::InitPyEnv()
{
	//bool b = Py_IsInitialized();

	int ret;
	ret = PyImport_AppendInittab( "hello", &inithello );
	ret = PyImport_AppendInittab( "upp", &initupp );

	//Py_Initialize(); //should be done *after* PyImport_AppendInittab, but it still works :), leaving INITBLOCK from Py
	//PyCon::Init();

	PyCon::AtExit = THISBACK(ExitHandler);

	try {
		object main_module(handle<>(borrowed(PyImport_AddModule("__main__"))));
		main_namespace = main_module.attr("__dict__");

		//add path
		String s = GetDataDir();
		object sys = import("sys");
		object path = sys.attr("path");
		path.attr("append")(s.Begin());

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

#if PUREVIRTEST
#else
		w.set("Welcom on earth");
		scope(hello_module).attr("earth") = ptr(&w);
#endif

		object upp_module = import("upp");
		main_namespace["upp"] = upp_module;

		Value v = "Teststring";
		//object tstr(v);
		main_namespace["teststr"] = object(v);

if(0) {
		object locals;
		locals = dict();
		locals["arg"] = object(v);
		main_namespace["loc"] = locals;
}
		scope(upp_module).attr("app") = ptr((TopWindow*)this);

		scope(upp_module).attr("con") = ptr(&con);

		sl.SetData(50);
		scope(upp_module).attr("sl") = ptr(&sl);

		vc.SetData(123);
		scope(upp_module).attr("vc") = ptr(&vc);

		es.SetData("Hello from Upp");
		scope(upp_module).attr("es") = ptr(&es);

		scope(upp_module).attr("pr") = ptr(&pr);

		cbi = THISBACK(CBi);
		scope(upp_module).attr("cbi") = ptr(&cbi);

		object o = eval("100 + upp.sl.data", main_namespace, main_namespace);
		int data = extract<int>(o);

		ev.WhenEnter = THISBACK(EvalCB);
		ex <<= THISBACK(EvalCB);
		evr.SetReadOnly();
		ev.SetData("100 + upp.sl.getdata()");
		EvalCB();

		pycv.globals = main_namespace;
		pycv.locals = dict();
		excv <<= THISBACK(EvalPyCv);
		evcv.SetData("arg + ' ' + upp.app.title");
		invc.SetData("Please try");
		outvc.SetReadOnly();

		String sc = 

		"upp.app.title = 'My Big BoostPy Test Environment'\n"

#if PUREVIRTEST
#else
		"p = hello.World()\n"
		"p.set('Some Greet Text')\n"
		"print p.get()\n"
#endif

		"upp.sl.data = 75\n"
		"print upp.sl.data\n"

		"upp.vc.data = range(10)\n"
		"print upp.vc.data\n"

		"print upp.es.data\n"
		"upp.es.data = 'Another Text from Upp'\n"
		"print upp.es.data\n"

		"c = upp.Color(123,124,125)\n"
		"print c\n"

#if PUREVIRTEST
#else
		"pp = p\n"
		"p.set('Hi')\n"
		"print p\n"
		"print pp\n"
#endif

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

		"print 'USE DEBUG TO SEE THE INVOKATIONS in WrapWorld'\n"
#if PUREVIRTEST
		"class MyPureWorld(hello.World):\n"
		"    def vir(self, a):\n"
		"        return 'MyPurePyWorld'\n"
		"mw = MyPureWorld()\n"
		"print mw.vir(8)\n"
		"print hello.invworld(mw,7)\n"
#else
		"w = hello.World()\n"
		"print hello.invworld(w,7)\n"
		"print w.vir(7)\n"

		"class MyWorld(hello.World):\n"
		"    def vir(self, a):\n"
		"        return 'MyPyWorld'\n"
		"mw = MyWorld()\n"
		"print mw.vir(8)\n"
		"print hello.invworld(mw,7)\n"
#endif

		"class MyUWorld(hello.Universe):\n"
		"    def vir(self, a):\n"
		"        return 'MyPyUWorld'\n"
		"mu = MyUWorld()\n"
		"print mu.vir(8)\n"
		"print hello.invworld(mu,7)\n"

		"import fibo\n"
		"print fibo.fib(200)\n"

		"import myupp\n"
		"myupp.chtitle()\n"

		;
		con.cmd.SetData(sc);

	} catch( error_already_set ) {
		PyErr_Print();
	}
}
