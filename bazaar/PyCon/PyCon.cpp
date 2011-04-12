#include "PyCon.h"

PyObject* PyCon::ToStdOut(PyObject* self, PyObject* pArgs)
{
	char* ps = NULL;
	if(!PyArg_ParseTuple(pArgs, "s", &ps))
		return NULL;

	String s(ps);
	GetStream().Put(ps);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* PyCon::ToStdErr(PyObject* self, PyObject* pArgs)
{
	char* ps = NULL;
	if(!PyArg_ParseTuple(pArgs, "s", &ps))
		return NULL;

	String s(ps);
	GetStream().Put(ps);

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef PyCon::m[] =
{
	{ "ToStdOut", ToStdOut, METH_VARARGS, "writes to stdout" },
	{ "ToStdErr", ToStdErr, METH_VARARGS, "writes to stderr" },
	{ NULL, NULL, 0, NULL }
};
Callback PyCon::AtExit;

bool PyCon::enabled = false;
Stream* PyCon::ps = NULL;

void PyCon::Init()
{
ONCELOCK
{
	Py_Initialize(); //make sure, since we are static
	Py_InspectFlag = 1; //need to process exit ourselves

	Py_InitModule3("log", PyCon::m, "Logs stdout and stdin");
	PyRun_SimpleString (
		"import sys\n"
		"import log\n"
		"class StdOutIncept:\n"
		"\tdef write(self, str):\n"
		"\t\tlog.ToStdOut(str)\n"
		"class StdErrIncept:\n"
		"\tdef write(self, str):\n"
		"\t\tlog.ToStdErr(str)\n"
		);
}
	Enable();
}

void PyCon::Enable(bool b)
{
	if(enabled = b)
	PyRun_SimpleString (
		"sys.stdout = StdOutIncept()\n"
		"sys.stderr = StdErrIncept()\n"
	);
	else
	PyRun_SimpleString (
		"sys.stdout = sys.__stdout__\n"
		"sys.stderr = sys.__stderr__\n"
	);
}

//for having an echo print of return PyObject, is simply a copy of PyRun_SimpleStringFlags, with Py_single_input
int PyCon::MyPyRun_SimpleStringFlags(const char *command, PyCompilerFlags *flags, int start)
{
	PyObject *m, *d, *v;
	m = PyImport_AddModule("__main__");
	if (m == NULL)
		return -1;
	d = PyModule_GetDict(m);
	v = PyRun_StringFlags(command, start, d, d, flags);
	if (v == NULL) {
		bool b = (PyErr_Occurred() && PyErr_ExceptionMatches(PyExc_SystemExit));
		PyErr_Print(); //clears error
		if(b) AtExit();
		return -1;
	}
	Py_DECREF(v);
	if (Py_FlushLine())
		PyErr_Clear();
	return 0;
}
