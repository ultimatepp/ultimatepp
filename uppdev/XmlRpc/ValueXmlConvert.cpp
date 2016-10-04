#include "XmlRpc.h"

/**
* Conversion of Primitives to Xml. This converts primitives such as String, Boolean, Integer,
* Double, Date, Time and Base64 to their corresponding Xml representation.
* @param xml the XmlNode that will hold the xml after conversion
* @param v the Primitive-Value that is to be converted
* @return nothing
*/
void ValueXmlConvert::PrimitiveToXml(XmlNode& xml,const Value& v) const {
	if(v.GetType() == DOUBLE_V)
		xml("double").Add().CreateText(v.ToString());
	if(v.GetType() == INT_V)
		xml("int").Add().CreateText(v.ToString());
	if(v.GetType() == STRING_V || v.GetType() == WSTRING_V )
		xml("string").Add().CreateText(v.ToString());
	//if(v.GetType() == BOOL_V)
	//	xml("boolean").Add().CreateText(v.ToString());
	if(v.GetType() == DATE_V || v.GetType() == TIME_V) {
		String text;
		Time t=v;
		text << t.year << (t.month<10?"0":"") << (int)t.month << (t.day<10?"0":"") << (int)t.day <<'T'<<(t.hour<10?"0":"") << (int)t.hour;
		text << ':' << (t.minute<10?"0":"");
		text << (int)t.minute << ":"<<(t.second<10?"0":"") << (int)t.second;
		xml("dateTime.iso8601").Add().CreateText(text);
	}
}

/**
* Conversion of all Values to Xml. This converts Values to their xml representation. Primitives
* such as String, Boolean, Integer, Double, Date, Time and Base64 are passed to PrimitiveToXml().
* Unknown Types are converted to Base64.
* @param xml the XmlNode that will hold the xml after conversion
* @param v the Value that is to be converted
* @return nothing
*/
void ValueXmlConvert::ValueToXml(XmlNode& xml, Value& v) const {
	ValueArray va;
	ValueMap vm;
	switch(v.GetType()) {
		case DOUBLE_V:
		case INT_V:
		case STRING_V:
		case WSTRING_V:
		case DATE_V:
		case TIME_V:
		//case BOOL_V:
			PrimitiveToXml(xml,v);
			break;
		case VALUEARRAY_V:
			va=v;
			for(int j=0;j<va.GetCount();j++) {
				XmlNode& value=xml("array")("data").Add("value");
				Value vv(va.Get(j));
				ValueToXml(value,vv);
			};
			break;
		case VALUEMAP_V:
			vm=v;
			for(int j=0;j<vm.GetCount();j++) {
				XmlNode& member=xml("struct").Add("member");
				member("name").Add().CreateText(vm.GetKey(j));
				Value vv(vm.Get(j));
				ValueToXml(member("value"),vv);
			}
			break;
		default:
			xml("base64").Add().CreateText(StoreAsString(v));
			break;
	}
}
