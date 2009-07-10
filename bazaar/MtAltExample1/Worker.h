#ifndef _MtAltExample1_OtherThreadClasses_h_
#define _MtAltExample1_OtherThreadClasses_h_

#include <MtAlt/MtAlt.h>
using namespace Upp;

//new thread 1
class Worker : public CallbackThread
{
public: //thread GUI
	class Notify { public: virtual void OnWork2Result(int x1, int x2, String s) = 0; };
	
	Worker(Notify *n) :notify(n) {}
	
public: //thread 1
	void DoWork1() 
	{
		Sleep(300); //do something
	}
	void DoWork2()
	{
		Sleep(400); //do something
		Time t = GetSysTime();
		notify->OnWork2Result((int) Random(10), (int) Random(20), Format(t)); //notify result
	}

private:
	Notify *notify;
};

#endif
