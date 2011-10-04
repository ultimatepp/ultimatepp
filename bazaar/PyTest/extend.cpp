#include "PyTest.h"

using namespace std;

struct FuncID
{
	short id; // An index number of the function.
	PyObject* cbf; // Pointer to the callback function.
};

Array<FuncID> FunctionsList;

PyObject* newmodule_RegisterFunction ( PyObject* pSelf, PyObject* pArgs )
{
	short index = 0;
	PyObject* CallbackFunction = NULL;

	if ( !PyArg_ParseTuple ( pArgs, "lO", &index, &CallbackFunction ) )
		return NULL;

	if ( !PyCallable_Check ( CallbackFunction ) )
	{
		PyErr_SetString ( PyExc_StandardError, "The object should be callable!" );
		return NULL;
	}

	// Make sure the index isn't in the list already.

	for (  int i = 0; FunctionsList.GetCount(); i++ )
	{
		if ( FunctionsList[i].id == index )
		{
			// Sorry mates.
			return Py_BuildValue ( "i", 0 );
		}
	}

	// And most important, add a reference to it!
	Py_INCREF ( CallbackFunction );

	// Try to insert it to the list.
	FuncID info = {index, CallbackFunction};

	try
	{
		FunctionsList.Add(info);
	}

	catch ( ... )
	{
		// Oh uh!
		Py_DECREF ( CallbackFunction );
		PyErr_SetString ( PyExc_StandardError, "An error occurred while inserting the given function into the DB!" );
		return NULL;
	}

	return Py_BuildValue ( "i", 1 );
}

PyObject* newmodule_CallFunctionByID ( PyObject* pSelf, PyObject* pArgs )
{
	short index = 0;

	if ( !PyArg_ParseTuple ( pArgs, "l", &index ) )
		return NULL;

	// Find the ID in the list, and call it's appropriate function.
	for ( int i = 0; FunctionsList.GetCount(); i++  )
	{
		if ( FunctionsList[i].id == index )
		{
			// We pass no args, I leave it for you :)
			// Py_XDECREF checks if the object isn't NULL first.
			PyObject* ret = PyEval_CallObject ( FunctionsList[i].cbf, NULL );
			Py_XDECREF ( ret );
			Py_INCREF ( Py_None );
			return Py_None;
		}
	}

	// Damn function wasn't found.
	return Py_BuildValue ( "i", 0 );
}

static PyMethodDef newmoduleMethods[] =
{
	{"RegisterFunction", newmodule_RegisterFunction, METH_VARARGS, "Register a function to a given ID."},
	{"CallFunctionByID", newmodule_CallFunctionByID, METH_VARARGS, "Call a registered function by its ID."},
	{NULL, NULL, 0, NULL}
};

void InitNewModule()
{
	// When you'll run in Python: newmodule.__doc__, you'll get the last string. :)
	Py_InitModule3 ( "newmodule", newmoduleMethods, "This is a new module" );
	PyModule_AddIntConstant ( PyImport_AddModule ( "newmodule" ), "INT1", 2003 );
	PyModule_AddStringConstant ( PyImport_AddModule ( "newmodule" ), "STR1", "http://ragestorm.net" );

	FunctionsList.Clear();
}

void DestroyNewModule()
{
	// Clean up the list.
	for ( int i = 0; FunctionsList.GetCount(); i++ )
	{
		Py_DECREF ( FunctionsList[i].cbf );
	}

	FunctionsList.Clear();

	// And free the newmodule.
}

//

void ExtendPython()
{
	Cout() << "Extending Python:"<< EOL;
	InitNewModule();
}
