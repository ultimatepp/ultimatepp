#include "CppBase.h"

namespace Upp {

String GetStdDefs()
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
	
	String defs;
	Vector<String> h = Split(ns, ';');
	for(int i = 0; i < h.GetCount(); i++) {
		String id, n;
		SplitTo(h[i], ':', id, n);
		ValueMap m;
		m.Add(id, n);
		defs << "#define " << id << " namespace " << n << " {\n";
	}
	h = Split(endns, ';');
	for(int i = 0; i < h.GetCount(); i++)
		defs << "#define " << h[i] << " };\r\n";
	h = Split(ignore, ';');
	for(int i = 0; i < h.GetCount(); i++)
		defs << "#define " << h[i] << "\n";
	defs << "#define NULL NULL";
	return defs;
}

};