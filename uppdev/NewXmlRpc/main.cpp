#include "XmlRpc.h"

using namespace Upp;


void Compute(XmlRpcData& rpc)
{
	int n1, n2;
	String op;
	rpc >> n1 >> op >> n2;
	DUMP(n1);
	DUMP(op);
	DUMP(n2);
	switch(*op) {
	case '+':
		rpc << n1 + n2;
		break;
	case '-':
		rpc << n1 - n2;
		break;
	}
}

struct TestMap {
	String a;
	int    b;
	
	String ToString() const { return a + AsString(b); }
	
	void Map(ValueMapper& m) {
		m("first", a)("second", b);
	}
};

CONSOLE_APP_MAIN
{
	Register("compute", callback(Compute));

	String r = LoadFile(GetDataFile("request.txt"));
	
	DDUMP(XmlRpcExecute(r));
	
	XmlRpcData d;
	d << "12" << "-" << 22;
	DDUMP(XmlRpcCall("compute", d));
	
	int res;
	d >> res;
	DDUMP(res);
	

	DDUMP(FormatXmlRpcError(101, "error"));
	
	ValueArray va;
	va.Add("Hello");
	va.Add("world");
	
	Array<String> a;
	ValueGet(a, va);
	DUMPC(a);

	Vector<String> b;
	ValueGet(b, va);
	DUMPC(b);
	
	ValueMap m;
	m.Add("first", "Number is ");
	m.Add("second", 123);
	
	TestMap tm;
	ValueGet(tm, m);
	
	DUMP(tm.a);
	DUMP(tm.b);
	
	va.Clear();
	for(int i = 0; i < 10; i++) {
		ValueMap m;
		m.Add("first", "Number is ");
		m.Add("second", i);
		va.Add(m);
	}
	
	Array<TestMap> tmm;
	ValueGet(tmm, va);
	DUMPC(tmm);

/*	
	XmlRpcRequest qq;
	qq.va.Add(123);
	qq.va.Add(va);
	
	int x;
	qq >> x >> tmm;
	DUMP(x);
	DUMPC(tmm);
*/	

	ValueArray param;

	String result;

	DUMP(param);
	
	XmlRpcData rpc;
	rpc.in = param;
	
	if(0) {
		int n1, n2;
		String text;
		Time d;
		Vector<int> vi;
		TestMap tm;
		rpc >> n1 >> text >> n2;
		DUMP(n1);
		DUMP(text);
		DUMP(n2);
		rpc >> d >> vi >> tm;
		DUMP(d);
		DUMPC(vi);
		DUMP(tm);
	}
	
	DDUMP(FormatXmlRpcParams(param));
	LOG("Result: " << result);
}
