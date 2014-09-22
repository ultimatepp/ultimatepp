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
	
	LOG("----");
	global.GetAdd("s", "Test");
	CHKV((WString)Evaluate("s", global), "Test");
	
	LOG("----");
	Escape(global, "Print(x)", SIC_Print);
	try {
		Scan(global, LoadFile(GetDataFile("script.esc")));
		Execute(global, "main", INT_MAX);
	}
	catch(CParser::Error e) {
		Cout() << "ERROR: " << e << "\n";
	}
	
	DDUMP(global.Get("out"));
	DDUMP(global.Get("out2"));
	DDUMP(global.Get("out3"));
}
