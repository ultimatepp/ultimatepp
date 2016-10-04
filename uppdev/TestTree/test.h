#ifndef _test_test_h
#define _test_test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TreeTest: public TreeCtrl
{
public:
    typedef TreeTest CLASSNAME;

    TreeTest();
	void addElement();    

	int _number;       
private:    

};



class TEST: public TopWindow
{
	public:
	typedef TEST CLASSNAME;
	TEST();
	
	void OnChange();

	TreeTest 	_test;
	
};
#endif

