#include <Esc/Esc.h>

using namespace Upp;

template <class X, class R>
void Check(const char *op, X a, R r)
{
	ArrayMap<String, EscValue> global;
	StdLib(global);
	global.Add("a", a);
	String exp = AsString(op) + "a";
	EscValue v = Evaluate(exp, global);
	LOG("a = " << a << "; " << exp << " = " << v << " (" << r << ")");
}

#define UCHK(op, a) Check(#op, a, op a)

template <class X, class Y, class R>
void Check(X a, const char *op, Y b, R r)
{
	ArrayMap<String, EscValue> global;
	StdLib(global);
	global.Add("a", a);
	global.Add("b", b);
	String exp = "a" + AsString(op) + "b";
	EscValue v = Evaluate(exp, global);
	LOG("a = " << a << ", b = " << b << "; " << exp << " = " << v << " (" << r << ")");
	if(v.IsInt64())
		ASSERT(r == (R)v.GetInt64());
	else
		ASSERT(r == (R)v.GetNumber());
}

#define CHK(a, op, b) Check(a, #op, b, a op b)

#define CHKDBL(a, b) \
LOG("============="); \
UCHK(+, a); \
UCHK(-, a); \
UCHK(!, a); \
CHK(a, +, b); \
CHK(a, -, b); \
CHK(a, *, b); \
CHK(a, /, b); \
CHK(a, <, b); \
CHK(a, >, b); \
CHK(a, <=, b); \
CHK(a, >=, b); \
CHK(a, ==, b); \
CHK(a, &&, b); \
CHK(a, ||, b); \


#define CHKINT(a, b) \
UCHK(~, a); \
CHKDBL(a, b); \
CHK(a, %, b); \
CHK(a, <<, b); \
CHK(a, >>, b); \
CHK(a, &, b); \
CHK(a, ^, b); \
CHK(a, |, b); \


#define CHKV(val, ref) \
{ Value v = val; \
LOG(#val << " = " << v << ", ref: " << ref); \
ASSERT(v == ref); }

bool DblMatch(double x, double y)
{
	return abs(x - y) < min(abs(x), abs(y)) / 1e10;
}

#define CHKS(exp) \
{ double x = Evaluate(#exp, global).GetNumber(); LOG(#exp << " = " << x << ", " << exp); ASSERT(DblMatch(x, exp)); }

void SIC_Print(EscEscape& e)
{
	if(e[0].IsArray())
		LOG((String) e[0]);
	else
	if(e[0].IsNumber())
		LOG(e[0].GetNumber());
	else
	if(!e[0].IsVoid())
		e.ThrowError("invalid argument to 'Print'");
}

void Zero(EscEscape& e)
{
	e = 0;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	CHKDBL(3.1, 2.1);
	CHKDBL(3.0, 2.0);
	CHKDBL(3.0, (int64)2);
	CHKDBL((int64)3, 2.0);
	CHKINT((int64)3, (int64)2);
	LOG("=============================");
	ArrayMap<String, EscValue> global;
	StdLib(global);
	Escape(global, "Zero()", Zero);
	global.Add("a", (int64)2);
	CHKV(Evaluate("a++", global).GetInt64(), 2);
	CHKV(global.Get("a").GetInt64(), 3);
	CHKV(Evaluate("++a", global).GetInt64(), 4);
	CHKV(global.Get("a").GetInt64(), 4);
	CHKV(Evaluate("a--", global).GetInt64(), 4);
	CHKV(global.Get("a").GetInt64(), 3);
	CHKV(Evaluate("--a", global).GetInt64(), 2);
	CHKV(global.Get("a").GetInt64(), 2);
	LOG("----");
	global.GetAdd("a", (double)2);
	CHKV(Evaluate("a++", global).GetNumber(), 2);
	CHKV(global.Get("a").GetNumber(), 3);
	CHKV(Evaluate("++a", global).GetNumber(), 4);
	CHKV(global.Get("a").GetNumber(), 4);
	CHKV(Evaluate("a--", global).GetNumber(), 4);
	CHKV(global.Get("a").GetNumber(), 3);
	CHKV(Evaluate("--a", global).GetNumber(), 2);
	CHKV(global.Get("a").GetNumber(), 2);
	CHKV(Evaluate("a = 0", global).GetNumber(), 0);
	CHKV(Evaluate("a == 0", global).GetNumber(), 1);
	CHKV(Evaluate("Zero() == 0", global).GetNumber(), 1);
	CHKV(Evaluate("Zero() == 1", global).GetNumber(), 0);

	LOG("=============================");
	
	CHKS(sin(0.5))
	CHKS(cos(0.5))
	CHKS(tan(0.5))
	CHKS(asin(0.5))
	CHKS(acos(0.5))
	CHKS(atan(0.5))
	CHKS(atan2(0.5, 0.5))
	CHKS(sinh(0.5))
	CHKS(cosh(0.5))
	CHKS(tanh(0.5))
	CHKS(asinh(0.5))
	CHKS(acosh(3.762196))
	CHKS(atanh(0.5))

	CHKS(exp(0.5))
	CHKS(log(0.5))
	CHKS(log10(0.5))
	CHKS(exp2(8))
	CHKS(log2(256))
	CHKS(sqrt(0.5))
	CHKS(cbrt(0.5))

	CHKS(pow(10, 2))

	LOG("=============================");

	global.GetAdd("s", "Test");
	CHKV((WString)Evaluate("s", global), "Test");
	
	LOG("----");
	Escape(global, "Print(x)", SIC_Print);
	try {
		Scan(global, LoadFile(GetDataFile("script.esc")));
		Execute(global, "main", INT_MAX);
	}
	catch(CParser::Error e) {
		DDUMP(e);
		Cout() << "ERROR: " << e << "\n";
	}
	
	DDUMP(global);
	DDUMP(global.Get("out"));
	DDUMP(global.Get("out2"));
	DDUMP(global.Get("out3"));
	DDUMP(global.Get("out4"));
	DDUMP(global.Get("out5"));

	ASSERT(AsString(global.Get("out")) == "{ \"a\":123, \"b\":\"Hello!\" }");
	ASSERT(AsString(global.Get("out2")) == "\"test\"");
	ASSERT(AsString(global.Get("out3")) == "[ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ]");
	ASSERT(AsString(global.Get("out4")) == "1");
	ASSERT(AsString(global.Get("out5")) == "5");
	
	LOG("=========================== OK");
}
