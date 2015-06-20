#include <CtrlLib/CtrlLib.h>

#if 0 // Do not compile these tests in nightly job

// Each test in separate function to activate grounding

void Autocomplete()
{
	String x;
	// Should not show anything here - qualification missing
	x.
}

void Autocomplete()
{
	Upp::String x;
	x.
}

void Autocomplete()
{
	Upp::MD5String
}

using namespace Upp;

void Autocomplete()
{
	MD5String
}

void Autocomplete()
{
	String h;
	h.
}

void Autocomplete()
{ // Check that this displays String autocomplete (not One)
	One<String>()->
}

void Autocomplete()
{
	Single<String>()->
}

void Autocomplete()
{
	EditDouble x; // Also try jump to HSizePos
	x.HSizePos().
}

void Autocomplete()
{
	EditDouble x;
	x.SetFilter(Char
}

void JumpTo()
{
	Point p;
	int x;
	p.x // should jump to Point::x, not local variable
}

void JumpTo()
{
	int local_var;
	Point foo;	
	local_var // should jump to local variable declaration two lines above
}

struct JumpToTest {
	int x; // test that this does not jump anywhere...
}

#endif

GUI_APP_MAIN
{
}
