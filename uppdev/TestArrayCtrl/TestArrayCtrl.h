#ifndef _TESTARRAYCTRL_H_
#define _TESTARRAYCTRL_H_

#include <CtrlLib/CtrlLib.h>
#include "Test.h"

using namespace Upp;

template <class T>
class _TestArrayCtrl : public T{

public:
	EditString _N[10];

	_TestArrayCtrl();

	typedef _TestArrayCtrl CLASSNAME;
};

template <class T>
_TestArrayCtrl<T>::_TestArrayCtrl(){

	_array.AutoHideSb(true);
	_array.HeaderObject().Absolute();
	
	// Add 10 Columns
	_array.AddColumn(  "N1", 100 ).Edit( _N[0] );
	_array.AddColumn(  "N2", 100 ).Edit( _N[1] );
	_array.AddColumn(  "N3", 100 ).Edit( _N[2] );
	_array.AddColumn(  "N4", 100 ).Edit( _N[3] );
	_array.AddColumn(  "N5", 100 ).Edit( _N[4] );
	_array.AddColumn(  "N6", 100 ).Edit( _N[5] );
	_array.AddColumn(  "N7", 100 ).Edit( _N[6] );
	_array.AddColumn(  "N9", 100 ).Edit( _N[8] );
	_array.AddColumn(  "N10", 100 ).Edit( _N[9] );

	// Add 30 Rows
	_array.SetCount( 30 );
}

#endif
