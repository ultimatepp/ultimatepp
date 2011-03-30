#include "PyTest.h"

void EmbedPython()
{
	Cout() << "Embedding Python:"<< EOL;

//
	if(!FileExists("pytest.py"))
	{
		String s = 
		"'''py_function.py - Python source designed to '''\n"
		"'''demonstrate the use of python embedding'''\n"
		"\n"
		"def multiply():\n"
		"	c = 12345*6789\n"
		"	print 'The result of 12345 x 6789 :', c\n"
		"	return c\n"
		;
		SaveFile("pytest.py", s);
	}
//

	PyObject *pName, *pModule, *pDict, *pFunc, *pValue;

	// Build the name object
	pName = PyString_FromString("pytest");
	
	// Load the module object
	pModule = PyImport_Import(pName);
	
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, "multiply");
	
	if (PyCallable_Check(pFunc)) 
	{
		PyObject_CallObject(pFunc, NULL);
	} else 
	{
		PyErr_Print();
	}

	Cout() << "Clean up" << EOL;	
	Py_DECREF(pModule);
	Py_DECREF(pName);
}
