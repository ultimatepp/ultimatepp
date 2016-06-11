#include <CtrlLib/CtrlLib.h>
#include <Sql/Sql.h>
#include <vector>
#include <map>
#include <TabBar/TabBar.h>

using namespace std;

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

void Autocomplete()
{
	Upp::TrimBoth().
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
{ // Check that this displays One autocomplete (not String)
	One<String>().
}

void Autocomplete()
{ // Displays String, it is OK as Single is function
	Single<String>().
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
	Vector<String>::
}

void Autocomplete()
{
	for(int i = 0; i < Font::
}

void Autocomplete()
{
	Point p;
	p.
}

struct FooBar {
	int x;
};

void Autocomplete()
{
	FooBar b;
	b.
}

void Autocomplete()
{
	::FooBar b;
	b.
}

Index<String>& Tags();

void Autocomplete() // TODO
{
	::Tags().
}

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
	type // check that there are not wrong items in the list for 'type'
}

Value Select();

void Autocomplete()
{
	Select().
}

void Autocomplete()
{
	Select().From().LeftJoinRef().Where().OrderBy().Lim
}

struct Ide { 
	struct ErrorInfo {
		String file;
	};
}

void Ide::AutoComplete()
{
	Value v;
	ValueTo<ErrorInfo>().
}

void Ide::AutoComplete()
{
	Value v;
	v.To<ErrorInfo>().
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
	popup.Ctrl::PopUp
}

void Autocomplete()
{
	String txt[2];
	txt[1].
}

void Autocomplete()
{
	VectorMap<String, String>()[i].
}

void Autocomplete()
{
	Array<String> v;
	v.begin()->
}

void Autocomplete()
{
	Vector<String> v;
	auto x = v[0];
	x.
}

void Autocomplete()
{
	Array<String> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	Vector<String> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	InVector<String> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	InArray<String> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	Index<String> h;
	for(const auto& i : h)
		i.
}

void Autocomplete()
{
	ArrayIndex<String> h;
	for(const auto& i : h)
		i.
}

void Autocomplete()
{
	VectorMap<int, String> m;
	for(auto i : m)
		i.
}

void Autocomplete()
{
	VectorMap<int, String> m;
	auto i = m.Begin();
	i.
}

void Autocomplete()
{
	VectorMap<Point, String> m;
	auto i = m.KeyBegin();
	i->
}

void Autocomplete()
{
	vector<string> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	vector<string> v;
	auto q = v.end();
	q.
}

void Autocomplete()
{
	map<int, string> m;
	for(const auto& i : m)
		i.second.
}

void Autocomplete()
{
	map<string, Point> m;
	auto q = m.begin();
	q.first.  ;
	q.second.
}

void Autocomplete()
{
	std::vector<std::string> v;
	for(const auto& i : v)
		i.
}

void Autocomplete()
{
	Vector<VectorMap<String, String>> lvs;
	lvs[0].
}

class ResourceCache {
public:
	ValueMap GetAttrs();

	static ResourceCache& The();
};

void Autocomplate()
{
	ResourceCache::The().
}

void JumpTo()
{
	ResourceCache::The().GetAttrs();
}

struct ParamInInitializer {
	Rect rect;
	ParamInInitializer(Rect r);
};

ParamInInitializer::ParamInInitializer(Rect r)
:	rect(r. // test that parameter is known here
)
{}

struct DefParam {
	void Bar(bool b = true);
};

void DefParam::Bar(bool b) {}

void Autocomplete()
{
	DefParam x;
	x. // should show variant with bool b = true (also in navigator)
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

struct JumpBase {
	void Foo();
};

struct JumpDerived {
	void Foo() {
		JumpBase::Foo(); // should jump to JumpBase
	}
};

void AddContact();

struct MYTabs : FileTabs {
	void	RenameFile(const WString &from, const WString &to, Image icon = Null);
	void    AddContact();
	int     local;
	
	MYTabs(); // Test Alt+I
	~MYTabs(); // Test Alt+I
};

MYTabs::MYTabs()
{
}

MYTabs::~MYTabs()
{
}

MYTabs::~ // check destructor match

void MYTabs::RenameFile(const WString &from, const WString &to, Image icon)
{
	local; // jump to member
	int local;
	FileTabs::RenameFile(from, to, icon); // should jump to FileTabs, not here
	Upp::FileTabs::RenameFile(from, to, icon); // should jump to FileTabs, not here
	::Upp::FileTabs::RenameFile(from, to, icon); // should jump to FileTabs, not here
	RenameFile(from, to, icon); // to MyTabs
	MYTabs::RenameFile(from, to, icon); // to MyTabs
	::MYTabs::RenameFile(from, to, icon); // to MyTabs
	local; // jump to local variable
	this->local; // jump to member
	MYTabs::local; // jump to member
}

void MYTabs::AddContact()
{
	::AddContact(); // should jump to global namespace 'add contact'
}

// ----------------------------------------------------

struct StaticVariableTest {
	static int foo; // Alt+I here
};

int StaticVariableTest::foo; // Alt+I here


// ----------------------------------------------------

enum { SM_NONE, SM_SINGLE, SM_DOUBLE } enum_var1;

String string;

struct EnumTest {
	enum { SM_NONE, SM_SINGLE, SM_DOUBLE } enum_var2;
	
	void Foo() {
		enum_var1; // Test jump
		enum_var2; // Test jump
	}
};

// ----------------------------------------------------

struct MyClass {
	
};

void MyClass::NewMethod() {} // Alt+I should go to MyClass

// ----------------------------------------------------

void Foo() {
	String x;
	x.Find(1, '\',', 0); // Check parameter positions
	x.Find(1, ",", 0); // Check parameter positions
	x.Find(1, 1, 0); // Check parameter positions
}

// ----------------------------------------------------

void Autocomplete() {
	DBG; {
		Vector<Ctrl *> h = Ctrl::GetTopCtrls();
		for(int i = 0; i < h.GetCount(); i++)
			h[i]->
	}
}

// ----------------------------------------------------

void JumpToLayout()
{
	WithPrinterLayout<TopWindow> h;
	
	SetLayout_PrinterLayout(h);
}

// ----------------------------------------------------

struct Foo {
	typedef Foo CLASSNAME;
	
	void Bar(); // Check Alt+C here
};

#endif

GUI_APP_MAIN
{
}
