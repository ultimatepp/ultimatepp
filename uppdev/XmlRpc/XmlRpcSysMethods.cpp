#include "XmlRpcSysMethods.h"
/**
* executing listMethods. executing system.listMethods
*/
void XmlRpcSysListMethods::Execute(Value& result, ValueArray params, dword ipadd) {
	ValueArray va;

	for(int i=0;i<methods->GetCount();i++) {
		String name;
		name = methods->At(i)->GetName();
		va.Add(name);
	}
	result=Value(va);
}
/**
* returns signature. returns signature
*/
void XmlRpcSysListMethods::Signature(ValueArray& sig) {
	ValueArray va;
	va.Add(ValueArray());

	sig.Add(va);
};
/**
* executing methodSignature. executing system.methodSignature
*/
void XmlRpcSysMethodSignature::Execute(Value& result, ValueArray params, dword ipadd) {
	result = ErrorValue("internal error");
	ASSERT(params.GetCount()==1);
	ValueArray sig,temp;

	if(params[0].GetType()!=STRING_V)
		return;
	for(int i=0;i<methods->GetCount();i++) {
		if(methods->At(i)->GetName() == (params[0].ToString())) {
			methods->At(i)->Signature(temp);
		}
	}
	if(temp.GetCount()<1)
		return;
	for(int i=0;i<temp.GetCount();i++) {
		if(temp[i].GetType() != VALUEARRAY_V)
			return;
		ValueArray va(temp[0]), vaa;
		for(int j=0;j<va.GetCount();j++) {
			switch(va[j].GetType()) {
				case INT_V:
					vaa.Add("int");
				case STRING_V:
					vaa.Add("string");
					break;
				case DOUBLE_V:
					vaa.Add("double");
					break;
				case DATE_V:
				case TIME_V:
					vaa.Add("dateTime.iso8601");
					break;
				case VALUEARRAY_V:
					vaa.Add("array");
					break;
				case VALUEMAP_V:
					vaa.Add("struct");
					break;
				default:
					vaa.Add("base64");
					return;
			}
		}
		sig.Add(vaa);
	}
	result=sig;
}
/**
* returns signature. returns signature
*/
void XmlRpcSysMethodSignature::Signature(ValueArray& sig) {
	ValueArray va;
	va.Add(ValueArray());
	va.Add(String());

	sig.Add(va);
}
/**
* executing methodHelp. executing system.methodHelp
*/
void XmlRpcSysMethodHelp::Execute(Value& result, ValueArray params, dword ipadd) {
	result = ErrorValue("internal error");
	ASSERT(params.GetCount()==1);
	if(params[0].GetType()!=STRING_V)
		return;
	for(int i=0;i<methods->GetCount();i++) {
		if(methods->At(i)->GetName() == (params[0].ToString())) {
			String help;
			methods->At(i)->Help(help);
			result=help;
			return;
		}
	}
}
/**
* returns signature. returns signature
*/
void XmlRpcSysMethodHelp::Signature(ValueArray& sig) {
	ValueArray va;
	va.Add(String());
	va.Add(String());

	sig.Add(va);
};
