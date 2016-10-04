#ifndef _editwithkey_editwithkey_h
#define _editwithkey_editwithkey_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

//#define LAYOUTFILE <editwithkey/editwithkey.lay>
//#include <CtrlCore/lay.h>

class element: public TopWindow
{
public:
 	typedef element CLASSNAME;
 	element();
 	
 	void Increment();
 	virtual bool Key(dword key, int count);
 	
private:
 	Button		_test;
 	EditField	_edit;
 	EditField	_edit2;
 	int			_count;
 	
};


//class editwithkey : public WitheditwithkeyLayout<TopWindow> {
class editwithkey : public TopWindow
{
public:
	typedef editwithkey CLASSNAME;
	editwithkey();
	
private:
	element		_test;
	element		_test2;
};

#endif

