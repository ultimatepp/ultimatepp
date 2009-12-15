#include "XmlRpc.h"

using namespace Upp;

/*
void Method(XmlRpcIO& rpc)
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
*/

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
	String result;
	String r = LoadFile(GetDataFile("request.txt"));
	XmlParser p(r);
	try {
		p.ReadPI();
		p.PassTag("methodCall");
			p.PassTag("methodName");
			LOG("method name: " << p.ReadText());
			p.PassEnd();
			ValueArray param;
			if(p.Tag("params")) {
				while(!p.End()) {
					param.Add(ParseXmlRpcParam(p));
				}
			}
			DUMP(param);
		p.PassEnd();
	}
	catch(XmlError e) {
		LOG("XmlError " << e << ": " << p.GetPtr());
	}
	LOG("Result: " << result);
}
