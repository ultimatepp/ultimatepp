#include "test.h"

#define DEBUG3	1

#if DEBUG3 != 0
	#define D3(x) RLOG("[" << GetSysTime() << "." << int(GetTickCount() % 10000u) << "] " << x)
#else
	#define D3(x)
#endif

TreeTest::TreeTest()
{
	_number = 0;
	
    NoRoot();
    Add(0, Image(),"field1", true);
    Add(0, Image(),"field2", true);
    Add(0, Image(),"field3", true);
    MouseMoveCursor();
	OpenDeep(0);

	//SetTimeCallback(-2000,callback(this,&TreeTest::addElement));
}

void TreeTest::addElement()
{
	int amount = 8;
	if(amount > _number)
	{
		Add(1,Image(),Sprintf("fieldn%d",_number), true);
		D3(Sprintf("Output %2d",_number));
		Refresh();
		_number++;
	}
	else if( (amount+1) > _number)
	{	
		Add(1,Image(),Sprintf("fieldn%d-last",_number), true);
		D3(Sprintf("Output %2d",_number));
		Refresh();
		_number++;
	}
}

TEST::TEST()
{
	SetRect(0, 0, 300, 400);
	CenterScreen();

	Add(_test.SizePos());
	
	PostCallback(callback(this,&TEST::OnChange));	
}

void TEST::OnChange()
{
	int max=1073741824, x=0;
		_test.addElement();
}

GUI_APP_MAIN
{
	TEST().Run();
}

