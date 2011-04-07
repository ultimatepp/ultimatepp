#include "PyTest.h"

void SimpleCall()
{
	Cout() << "Invoking a python statement:" << EOL;
	PyRun_SimpleString(	"from time import time,ctime\n"
						"print 'Today is',ctime(time())\n");
	PyRun_SimpleString(String("print 'python can calculate', 2+3"));
}

void StartConsole()
{
	PyRun_SimpleString(	"import code\n"
						"code.interact()\n");
}

CONSOLE_APP_MAIN
{
	Cout() << "Starting Python..." << EOL;

	SimpleCall();

	EmbedPython();
	ExtendPython();

	//StartConsole();
	PyCompilerFlags cf;
	cf.cf_flags = 0;

#if 1
	PyObject *m, *d;
	m = PyImport_AddModule("__main__");
	d = PyModule_GetDict(m);

	//this'd be the variant for executing code and expecting a return value
	//Py_single_input for echoing return value, Py_eval_input for evaluation only
	PyObject* po = PyRun_String("1+2", Py_single_input, d, d);
	Py_DECREF(po);
#endif
	//this runs an interactive console loop on stdin
	int ret = PyRun_InteractiveLoopFlags(stdin, "<stdin>", &cf);

	//leftover	
	DestroyNewModule();
	
	Cout() << "DONE." << EOL;
}

