#include <ide/Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#if 0
	String s = CleanupId("int (*test)(int (*test)())");
	DDUMP(s);
	return;
#endif

	for(String s : {
		"Foo1::operator<<(int)",
		"Foo1::operator<(int)",
		"Upp::Index<Upp::String>::Find(const Upp::String &k) const",
		"clang_PrintingPolicy_setProperty(CXPrintingPolicy Policy, enum CXPrintingPolicyProperty Property, unsigned int Value) __attribute__((dllimport))",
		"Foo1::Method()",
		"Foo3<int>::Foo3() noexcept",
		"Foo1::operator*(int)",
		"main(int argc, const char *argv[])",
		"Ns::EditText(int &s, const char *title, const char *label, int (*f)(int), int maxlen, bool notnull)",
		"template <class C> auto SubRange(C&& c, int pos, int count) -> decltype(SubRange(c.begin() + pos, count))",
		"inline void Foo()",
		"int (*test)(int (*test)())"
	}) {
		DLOG("======");
		DLOG(s);
		DLOG("id: " << CleanupId(s));
		DLOG("pretty: " << CleanupPretty(s));
	}
	
	CheckLogEtalon();
}
