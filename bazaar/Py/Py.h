#ifndef _Python_Python_h
#define _Python_Python_h

#ifdef flagBOOSTPY
	#ifdef _DEBUG
	#define BOOST_DEBUG_PYTHON 1
	#endif
	#include <boost/python.hpp>
#else
	#include <Python.h>
#endif

#include <Core/Core.h>
using namespace Upp;

#endif
