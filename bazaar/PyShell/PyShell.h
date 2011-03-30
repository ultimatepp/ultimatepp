#ifndef _PyShell_PyShell_h
#define _PyShell_PyShell_h

#include <Python/Python.h>

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//ONLY WORKS IN OPTIMAL, DEBUG needs TkTcl as DEBUG????

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
