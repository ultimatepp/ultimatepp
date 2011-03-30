#include "PyShellTest.h"

// When PyShell is opened write: import newmodule   newmodule.test("is it working?")
PyObject* newmodule_test(PyObject* pSelf, PyObject* pArgs)
{
	char* str = NULL;
	if (!PyArg_ParseTuple(pArgs, "s", &str)) return NULL;
	PromptOK("exported function");
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef newmoduleMethods[] = {
 {"test", newmodule_test, METH_VARARGS, "Exported function testing."},
 {NULL, NULL, 0, NULL}
};

void PyShellTest::StartNew()
{
	ps.Start();

	// You could do whatever you want, usually your message loop will be here.
	PromptOK("I am blocking, pyshell is working from the other thread");
}

PyShellTest::PyShellTest()
{
	CtrlLayout(*this, "PyShell Test");
	b <<= THISBACK(StartNew);
}

PyShellTest::~PyShellTest()
{
	ps.Wait();
}

GUI_APP_MAIN
{
	char *argv[] = {"", NULL};
	PySys_SetArgv(1, argv);
	PyRun_SimpleString("import sys; sys.path.pop(0)\n");

	PyObject* pyRet = PyString_FromString(">>> ");
	PySys_SetObject("ps1", pyRet);
	Py_XDECREF(pyRet);
	// ps2 is not used in PyShell, to be honset it's more comfortable, think of it
	// that you want to copy/paste the code and you have "..." in every line then you have to
	// remove it manually.

#if 0
	// Init our newmodule.
	Py_InitModule3("newmodule", newmoduleMethods, "This is a new module");
#endif

	PyShellTest().Run();
}

