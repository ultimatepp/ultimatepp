#include <Core/Core.h>

using namespace Upp;

#define TEST(fn, s) { \
	String r = AsString(fn); \
	LOG("TEST(" << #fn << ", " << AsCString(r) << ");"); \
	if(s) ASSERT((char *)s == r); \
}

#define TEST2(fn, s) { \
	a = b = c = Null; \
	String r; r << (fn ? "true" : "false") << ":" << a << ":" << b << ":" << c; \
	LOG("TEST2(" << #fn << ", " << AsCString(r) << ");"); \
	if(s) ASSERT((char *)s == r); \
}

int CharFilterComma(int c)
{
	return c == ',' ? c : 0;
}

const char *DelimTextFn(const char *s)
{
	bool found = false;
	while(*s == ' ' || *s == ';') {
		if(*s == ';')
			found = true;
		s++;
	}
	return found ? s : NULL;
}

const wchar *DelimTextFnW(const wchar *s)
{
	bool found = false;
	while(*s == ' ' || *s == ';') {
		if(*s == ';')
			found = true;
		s++;
	}
	return found ? s : NULL;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	const char *s = "alfa,beta,,gamma,delta";

	TEST(Split(s, ','), "[alfa, beta, gamma, delta]");
	TEST(Split(3, s, ',', true), "[alfa, beta, gamma]");
	TEST(Split(s, ',', false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, s, ',', false), "[alfa, beta, ]");
	TEST(Split(s, CharFilterComma), "[alfa, beta, gamma, delta]");
	TEST(Split(3, s, CharFilterComma, true), "[alfa, beta, gamma]");
	TEST(Split(s, CharFilterComma, false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, s, CharFilterComma, false), "[alfa, beta, ]");
	TEST(Split(s, ","), "[alfa, beta, gamma, delta]");
	TEST(Split(3, s, ",", true), "[alfa, beta, gamma]");
	TEST(Split(s, ",", false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, s, ",", false), "[alfa, beta, ]");
	TEST(Split("1 ; 2;3 ;4 ;5 6", DelimTextFn), "[1, 2, 3, 4, 5 6]");
	
	WString ws = s;
	TEST(Split(ws, ','), "[alfa, beta, gamma, delta]");
	TEST(Split(3, ws, ',', true), "[alfa, beta, gamma]");
	TEST(Split(ws, ',', false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, ws, ',', false), "[alfa, beta, ]");
	TEST(Split(ws, CharFilterComma), "[alfa, beta, gamma, delta]");
	TEST(Split(3, ws, CharFilterComma, true), "[alfa, beta, gamma]");
	TEST(Split(ws, CharFilterComma, false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, ws, CharFilterComma, false), "[alfa, beta, ]");
	WString dl = ",";
	TEST(Split(ws, dl), "[alfa, beta, gamma, delta]");
	TEST(Split(3, ws, dl, true), "[alfa, beta, gamma]");
	TEST(Split(ws, dl, false), "[alfa, beta, , gamma, delta]");
	TEST(Split(3, ws, dl, false), "[alfa, beta, ]");
	TEST(Split(~WString("1 ; 2;3 ;4 ;5 6"), DelimTextFnW), "[1, 2, 3, 4, 5 6]");

	Vector<String> w = Split(s, ',', false);
	TEST(Join(w, ";"), "alfa;beta;;gamma;delta");
	TEST(Join(w, ".", true), "alfa.beta.gamma.delta");
	
	Vector<String> v = Split(";;;hello;;", ';', false);
	TEST(Join(v, ";"), ";;;hello;;");
	TEST(Join(v, ";", true), "hello");

	Vector<WString> ww = Split(ws, ',', false);
	TEST(Join(ww, ";"), "alfa;beta;;gamma;delta");
	TEST(Join(ww, ".", true), "alfa.beta.gamma.delta");

	TEST(Merge(";", "one"), "one");
	TEST(Merge(";", ""), "");
	TEST(Merge(";", "one", "two", "three"), "one;two;three");
	TEST(Merge(";", "one", "", "three"), "one;three");
	TEST(Merge(";", "", "", "three"), "three");
	TEST(Merge(";", "", "two", "three"), "two;three");
	TEST(Merge(";", "", "", ""), "");
	
	String h;
	MergeWith(h, ";", "");
	TEST(h, 0);
	MergeWith(h, ";", "ahoj");
	TEST(h, 0);
	MergeWith(h, ";", "");
	TEST(h, 0);
	MergeWith(h, ";", "2", "");
	TEST(h, 0);
	MergeWith(h, ";", "", "3", "", "5", "6", "end");
	TEST(h, 0);

	String a, b, c;

	TEST2(SplitTo("one", ',', a, b, c), "false:::");
	TEST2(SplitTo("one,two,,three", ',', a, b, c), "true:one:two:three");
	TEST2(SplitTo("one,two,,three", ',', false, a, b, c), "true:one:two:");

	a = b = c = Null;
	TEST2(SplitTo("one", ",", a, b, c), "false:::");
	TEST2(SplitTo("one,two,,three", ",", a, b, c), "true:one:two:three");
	TEST2(SplitTo("one,two,,three", ",", false, a, b, c), "true:one:two:");

	a = b = c = Null;
	TEST2(SplitTo("one", CharFilterComma, a, b, c), "false:::");
	TEST2(SplitTo("one,two,,three", CharFilterComma, a, b, c), "true:one:two:three");
	TEST2(SplitTo("one,two,,three", CharFilterComma, false, a, b, c), "true:one:two:");

	LOG("===================== OK");	
}
