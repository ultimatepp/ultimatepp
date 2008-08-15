#ifndef _XmlRpc_XmlRpcSysMethods_h_
#define _XmlRpc_XmlRpcSysMethods_h_
#include "XmlRpc.h"

/**
* system method listing all methods. this system method lists all methods the server knows
*/
class XmlRpcSysListMethods : public XmlRpcServer::XmlRpcSysMethod {
	public:
		XmlRpcSysListMethods(String s,Vector<XmlRpcMethod *>* _methods) : XmlRpcSysMethod(s,_methods) {}
		virtual void Execute(Value& result, ValueArray params, dword ipadd);
		/*
		* returning help for the method.returning help for the method
		*/
		virtual void Help(String& help)							{help="lists all known methods";}
		virtual void Signature(ValueArray& sig);
};
/**
* system method returning signature of a method. this system method returns all signatures of a
* methods using the Signature() function
*/
class XmlRpcSysMethodSignature : public XmlRpcServer::XmlRpcSysMethod {
	public:
		XmlRpcSysMethodSignature(String s,Vector<XmlRpcMethod *>* _methods) : XmlRpcSysMethod(s,_methods) {}
		virtual void Execute(Value& result, ValueArray params, dword ipadd);
		/*
		* returning help for the method.returning help for the method
		*/
		virtual void Help(String& help)							{help="returns the signature of a given method";}
		virtual void Signature(ValueArray& sig);
};
/**
* system method returning the help for a method. this system method returns the help a method
* provides using the Help() function
*/
class XmlRpcSysMethodHelp : public XmlRpcServer::XmlRpcSysMethod {
	public:
		XmlRpcSysMethodHelp(String s,Vector<XmlRpcMethod *>* _methods) : XmlRpcSysMethod(s,_methods) {}
		virtual void Execute(Value& result, ValueArray params, dword ipadd);
		/*
		* returning help for the method.returning help for the method
		*/
		virtual void Help(String& help)							{help="returns the help of a given method";}
		virtual void Signature(ValueArray& sig);
};
#endif
