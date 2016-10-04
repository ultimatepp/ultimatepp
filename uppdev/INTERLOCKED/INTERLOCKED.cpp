#include <Core/Core.h>

using namespace Upp;

struct ILock {
	operator bool() const { return true; }

	ILock(int) { RLOG("Lock!"); }
	~ILock()   { RLOG("UnLock!"); }
};

CONSOLE_APP_MAIN
{
	__BREAK__;
	INTERLOCKED {}

	Parallel x;
	for(int i = 0; i < img.GetHeight(); i++)
		x(callback1(DoLine, i));
	x.Execute();


}


	Pipeline y;
	y.Start(callback(...));




void Loader()
{

}