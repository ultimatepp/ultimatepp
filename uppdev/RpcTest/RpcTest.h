#ifndef _RpcTest_RpcTest_h
#define _RpcTest_RpcTest_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <RpcTest/RpcTest.lay>
#include <CtrlCore/lay.h>

#include <ValueMap/ValueMap.h>
#include <XmlRpc/XmlRpc.h>

class RpcTestAddParam : public WithRpcTestParameterLayout<TopWindow> {
	Value v;
public:
	typedef RpcTestAddParam CLASSNAME;
	RpcTestAddParam();
	Value getVal();
};

class RpcTestHelp : public WithRpcTestMethodHelpLayout<TopWindow> {
public:
	RpcTestHelp();
};

class RpcTest : public WithRpcTestLayout<TopWindow> {
	XmlRpcClient xmlClient;
	Socket s;
public:
	typedef RpcTest CLASSNAME;
	RpcTest();
	void GetHelp();
	void CheckForMethods();
	void AddParam();
	void Execute();
	void Connect();
};

#endif
