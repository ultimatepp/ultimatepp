#include <CtrlLib/CtrlLib.h>
#include <Sql/Sql.h>

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

void Autocomplete()
{
	Upp::String
}

using namespace Upp;

void Autocomplete()
{
	MD5String
}

void Autocomplete()
{
	String
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

void Autocomplete()
{
	Point p;
	p.
}

Index<String>& Tags();

void Autocomplete()
{
	Tags().
}

void Autocomplete()
{
	Tags()[i].
}

void Autocomplete()
{
	Index<String> x;
	x[i].
}

VectorMap<String, String>& Map();

void Autocomplete()
{
	Map().
}

void Autocomplete()
{
	Map()[i].
}

void Autocomplete()
{
	VectorMap<String, String> map;
	map[i].
}

void Autocomplete()
{
	type // check that there are not wrong item is the list for 'type'
}

void Autocomplete()
{
	Select().From().LeftJoinRef().Where().OrderBy().Lim
}

enum {
	TEST_ONE,
	TEST_TWO
};

void Autocomplete()
{
	b ? TEST_ONE : TES
}

void Autocomplete()
{
	Splitter popup;
	popup.Ctrl::PopUp  // Should show PopUp method (does not work yet)
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
