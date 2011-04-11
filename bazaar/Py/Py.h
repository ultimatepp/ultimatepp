#ifndef _Python_Python_h
#define _Python_Python_h

//this drops any auto linking/inclusion of python/python_d libs
//use this when we use those boost python prelinked dlls
//which link to python27.dll even in debug mode

#define MS_NO_COREDLL 1
#define Py_ENABLE_SHARED 1

#ifdef flagBOOSTPY
//	#ifdef _DEBUG
//	#define BOOST_DEBUG_PYTHON 1
//	#endif
	#include <boost/python.hpp>
#else
	#include <Python.h>
#endif

#include <Core/Core.h>
using namespace Upp;

#endif
