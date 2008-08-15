#include "cpp.h"

/*
#define TEST(x, y) %x%y%
#define TESTS(x)   #x
#define TEST2(x, y)   x ## y
#define TEST4 alfa x ## y.r #hahaha
#define TESTX x\
//h

#define TESTY haha // note\
ble

#define TESTZ haha \
ble

//*
TEST('\1', "\2")
TEST(  1     +1 , 3    )
TEST(  "1" "2" ",", ',')
TESTS(a)
TESTS(a x y)
TESTS("A")
TEST2(a, 3)
TEST2(3, a)
TEST2(a, #)
TEST2(a, "X")
TEST2("Y", "X")
TESTX
TESTY
TESTZ
TEST4
//*/

CONSOLE_APP_MAIN
{
	{
		CppMacro m;
		m.body = "a + b * 123 - alfa";
		m.param.Add("a");
		m.param.Add("b");
		Vector<String> x;
		x.Add("one");
		x.Add("two");
		DUMP(m.Expand(x));
	}
/*
	Cpp x;
	x.Define("TEST(a, b) |a|b|");
	DUMP(x.Expand("This is: TEST(('\\1'   , \"2\"    \"4\" '4' ',' \",\"), 3)!"));
	DUMP(x.Expand("This is: TEST(1, 2)!"));
	DUMP(x.Expand("This is: TEST(1   +   a, 2)!"));
	DUMP(x.Expand("This is: TEST((1   , 2), 3)!"));
	DUMP(x.Expand("This is: TEST(  \"1\" \"2\" \",\", ',')!"));
*/
	Cpp x;
	FileIn in(GetDataFile("test.h"));
	while(!in.IsEof()) {
		String l = in.GetLine();
		if(l.StartsWith("#define"))
			x.Define(~l + strlen("#define"));
		else
			LOG(/*l << " -> " << */x.Expand(l));
	}
}

// TEST:a,b:|a|b|\n
