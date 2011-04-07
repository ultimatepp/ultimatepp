#ifndef _PyShell_PyShell_h
#define _PyShell_PyShell_h

#include <Py/Py.h>

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class PyShell
{
public:
	typedef PyShell CLASSNAME;
	PyShell() : a(0) {}

	void Start()
	{
		if(IsRunning()) return;
		Wait();
		if(!t.IsOpen()) t.Run(THISBACK(Proc));
	}
	void Wait() { t.Wait(); }
	bool IsRunning() const { return AtomicRead(a>0); }

	void Proc();

protected:
	Atomic a;
	Thread t;
};

#endif
