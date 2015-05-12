#include "CppBase.h"

namespace Upp {

Value StringArray(const char *s)
{
	Vector<String> h = Split(s, ';');
	ValueArray va;
	for(int i = 0; i < h.GetCount(); i++)
		va.Add(h[i]);
	return va;
}

String GetStdConfig()
{
	static const char ns[] = "_STD_BEGIN:std;_C_STD_BEGIN:std;_STDEXT_BEGIN:stdext;NAMESPACE_UPP:Upp";
	static const char endns[] = "_STD_END;_STDEXT_END;_C_STD_END;END_UPP_NAMESPACE";
	static const char ignore[] = "__declspec(...);__cdecl;__attribute__(...);__stdcall;__forceinline;"
                                 "__out;__in;__inout;__deref_in;__deref_inout;__deref_out;"
                                 "__pragma(...);__pascal;_far;_pascal;_cdecl;"
                                 "__AuToQuOtE;__xin;__xout;__export;"
                                 "__clrcall;__alignof(...);"
                                 
                                 "__asm__(...);__asm(...);__restrict;__inline;__typeof;"
	;
	ValueMap json;
	ValueArray va;
	Vector<String> h = Split(ns, ';');
	for(int i = 0; i < h.GetCount(); i++) {
		String id, n;
		SplitTo(h[i], ':', id, n);
		ValueMap m;
		m.Add(id, n);
		va.Add(m);
	}
	json.Add("namespace", va);
	json.Add("end_namespace", StringArray(endns));
	json.Add("ignore", StringArray(ignore));
	return AsJSON(json);
}

static VectorMap<String, String> s_namespace_macro;
static Index<String> s_namespace_end_macro;
static Vector<String> s_ignorelist;

void LoadPPConfig(const String& json)
{
	Value m = ParseJSON(json);

	try {
		s_namespace_macro.Clear();
		ValueArray va = m["namespace"];
		for(int i = 0; i < va.GetCount(); i++) {
			ValueMap m = va[i];
			if(m.GetCount())
				s_namespace_macro.Add(m.GetKey(0), m.GetValue(0));
		}
	
		s_namespace_end_macro.Clear();
		va = m["end_namespace"];
		for(int i = 0; i < va.GetCount(); i++)
			s_namespace_end_macro.Add(va[i]);
	
		s_ignorelist.Clear();
		va = m["ignore"];
		for(int i = 0; i < va.GetCount(); i++)
			s_ignorelist.Add(va[i]);
	}
	catch(ValueTypeError) {}
}

const VectorMap<String, String>& GetNamespaceMacros()
{
	return s_namespace_macro;
}

const Index<String>& GetNamespaceEndMacros()
{
	return s_namespace_end_macro;
}

const Vector<String>& GetIgnoreList()
{
	return s_ignorelist;
}

};