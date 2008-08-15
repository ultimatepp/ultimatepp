#ifndef _smallrpcserver_smallserver_h_
#define _smallrpcserver_smallserver_h_
#include <Core/Core.h>
#include <ValueMap/ValueMap.h>
#include <XmlRpc/XmlRpc.h>


class Hello : public XmlRpcMethod {
	public:
		Hello() : XmlRpcMethod("hello") {}
		void Help(String& h);
		void Signature(ValueArray& va);
		void Execute(Value& result,ValueArray params);
};



#endif
