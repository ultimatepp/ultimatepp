#include "smallserver.h"

void Hello::Help(String& s)  {
	s<<"just says hello";
}

void Hello::Signature(ValueArray& sig)  {
	LOG("i get asked");
	ValueArray va;
	va.Add(String());
	sig.Add(va);
}

void Hello::Execute(Value& res, ValueArray va)  {
	res = "Hello out there!";
}

CONSOLE_APP_MAIN {
	XmlRpcServer server;
	Hello h;
	Value v;
	h.Execute(v,ValueArray());
	LOG(v.ToString());
	server.SysMethods();
	server.Add(&h);
	if(server.Open(8080)) {
		server.Run();
	}
}
