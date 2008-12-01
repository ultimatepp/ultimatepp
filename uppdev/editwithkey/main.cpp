#include "editwithkey.h"
#include <sstream>

element::element()
{
	_count = 0;
	_test.SetRect(20,10,160,20);
	_test.SetLabel("increment");
	_test <<= callback(this, &element::Increment);
	Add(_test);
	
	_edit.SetRect(20,40,160,20);
	_edit.AlignRight();
	_edit.SetData(Sprintf("%d",_count));
	Add(_edit);

	_edit2.SetRect(20,70,160,20);
	_edit2.AlignRight();
	_edit2.SetData(Sprintf("%d",_count));
	Add(_edit2);
}

void element::Increment()
{
	_count++;
	_edit.SetData(Sprintf("%d",_count));
}

bool element::Key(dword key, int count)
{
    if(key == K_ENTER) {
        String tmp;
        if(_edit.HasFocus())
        {
        	tmp = _edit.GetData();
        	std::stringstream ssStream(~tmp); 
        	ssStream >> _count;
        	_edit.SetData(Sprintf("%d",_count));
        }
        if(_edit2.HasFocus())
        {
        	/*do something else*/
        }
        
        
		
        return true; // key accepted
    }
    return TopWindow::Key(key, count);
}



editwithkey::editwithkey()
{
//	CtrlLayout(*this, "Window title");
	SetRect(0,0,200,200);
	CenterScreen();
	_test.SetRect(0,0,200,100);
	_test2.SetRect(0,100,200,100);

	Add(_test);
	Add(_test2);
}

GUI_APP_MAIN
{
	editwithkey().Run();
}

