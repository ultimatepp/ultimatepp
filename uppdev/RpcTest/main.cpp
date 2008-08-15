#include "RpcTest.h"

RpcTestHelp::RpcTestHelp() {
	CtrlLayoutOK(*this, "XML-RPC Help");
}

RpcTestAddParam::RpcTestAddParam() {
	CtrlLayoutOKCancel(*this, "XmlRpc Client");
}

Value RpcTestAddParam::getVal() {
	LOG((~str).GetType());
	if(str.GetLength())
		return ~str;
	if(i.GetLength())
		return ~i;
	if(dou.GetLength())
		return ~dou;
	if(dat.GetLength())
		return ~dat;
	if(tim.GetLength())
		return ~tim;

}

RpcTest::RpcTest()
{
	CtrlLayout(*this, "XmlRpc Client");

	help.WantFocus(false);
	params.WantFocus(false);
	rawsent.SetEditable(false);
	rawreply.SetEditable(false);
	valToString.SetEditable(false);
	serverval.SetEditable(false);
	params.AddColumn("params");

	execute<<THISBACK(Execute);
	add<<THISBACK(AddParam);
	getmethods<<THISBACK(CheckForMethods);
	help<<THISBACK(GetHelp);
	usessl = 0;
	host=String("localhost");
	path=String("/");
	port=8080;
	CheckForMethods();
}

void RpcTest::GetHelp() {
	Value v;
	String help;
	XmlRpcCall helpCall("system.methodHelp");
	XmlRpcCall signature("system.methodSignature");

	if(method.GetLength()>0) {

		helpCall(~method);
		signature(~method);
		xmlClient.Host(host);
		xmlClient.Path(path);
		xmlClient.Port(port);
		xmlClient.UseSSl(usessl==1);

		help << "XML-RPC Help\n";
		help << "method: "<<~method<<" at "<<~host<<~path<<"\n\n";

		v = xmlClient.Execute(helpCall);
		if(IsString(v)) {
			help << "explanation: \n" << v<<"\n\n";
		}else
			help << "no explanation provided\n\n";
		v = xmlClient.Execute(signature);
		if(v.GetType() == VALUEARRAY_V) {
			ValueArray va(v);
			help << "signature: \n";
			for(int i=0; i<va.GetCount();i++) {
				ValueArray vaa(va[i]);
				help << vaa[0] <<" "<< ~method <<"(";
				for(int j=1;j<vaa.GetCount();j++) {
					if(j>1)
						help <<", ";
					help << vaa[j];
				}
				help <<")\n";
			}
		}else
			help << "no signature provided";
		RpcTestHelp h;
		h.text.Set(help);
		h.RunAppModal();
	}
}

void RpcTest::AddParam() {
	RpcTestAddParam p;
	if(p.RunAppModal() == IDOK) {
		Vector<Value> v;
		v.Add() = p.getVal();
		params.Add(v);
	}
	Refresh();
}

void RpcTest::CheckForMethods() {
	Value v;

	XmlRpcCall call("system.listMethods");
	xmlClient.Host(host);
	xmlClient.Path(path);
	xmlClient.Port(port);
	xmlClient.UseSSl(usessl==1);
	v = xmlClient.Execute(call);
	if(v.IsError())
		Exclamation(v.ToString());
	else {
		ValueArray va(v);
		method.ClearList();
		for(int i=0; i<va.GetCount();i++)
			method.AddList(va[i]);
		method.SetText(va[0].ToString());
	}
}

void RpcTest::Execute() {
	Value v,vv;
	XmlRpcParser p;

	XmlRpcCall call(method);
	for(int i=0;i<params.GetCount();i++) {
		call(params.Get(i,0));
	}

	rawsent.Set(call.ToXml());
	vv = p.ParseMethodCall(call.ToXml());

	xmlClient.Host(host);
	xmlClient.Path(path);
	xmlClient.Port(port);
	xmlClient.UseSSl(usessl==1);
	v = xmlClient.Execute(call);
	rawreply.Set(xmlClient.GetRawReply());

	valToString.Set(v.ToString());
	serverval.Set(vv.ToString());

	params.Clear();
	Refresh();
}

GUI_APP_MAIN
{
	RpcTest().Sizeable().Run();
}
