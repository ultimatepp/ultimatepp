#include "ide.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

#define LTIMING(x) // DTIMING(x)

String ResolveTParam(const String& type, const Vector<String>& tparam)
{
	return ResolveTParam(CodeBase(), type, tparam);
}

void ResolveTParam(Vector<String>& type, const Vector<String>& tparam)
{
	return ResolveTParam(CodeBase(), type, tparam);
}

String Qualify(const String& scope, const String& type, const String& usings)
{
	return Qualify(CodeBase(), scope, type, usings);
}

void AssistScanError(int line, const String& text)
{
#ifdef _DEBUG
	PutVerbose(String().Cat() << "(" << line << "): " << text);
#endif
}

void AssistEditor::Context(Parser& parser, int pos)
{
	LTIMING("Context");
	
	
	theide->ScanFile();
	String s = Get(0, pos);
	StringStream ss(s);

	String path = NormalizeSourcePath(theide->editfile);
	LLOG("---------- Context " << path);

	Cpp cpp;
	cpp.Preprocess(path, ss, GetMasterFile(path));

	parser.dobody = true;
	StringStream pin(cpp.output);
	parser.Do(pin, CodeBase(), Null, Null, GetFileTitle(theide->editfile), callback(AssistScanError),
	          Vector<String>(), cpp.namespace_stack, cpp.namespace_using); // needs CodeBase to identify type names

//	QualifyTypes(CodeBase(), parser.current_scope, parser.current);
	inbody = parser.IsInBody();
#ifdef _DEBUG
	PutVerbose("body: " + AsString(inbody));
	PutVerbose("scope: " + AsString(parser.current_scope));
	PutVerbose("using: " + AsString(parser.context.namespace_using));
	for(int i = 0; i < parser.local.GetCount(); i++)
		PutVerbose(parser.local.GetKey(i) + ": " + parser.local[i].type);
#endif
}

Index<String> AssistEditor::EvaluateExpressionType(const Parser& parser, const Vector<String>& xp)
{
	return GetExpressionType(CodeBase(), parser, xp);
}

void AssistEditor::AssistItemAdd(const String& scope, const CppItem& m, int typei)
{
	if(!iscib(*m.name) || m.name.GetCount() == 0)
		return;
	CppItemInfo& f = assist_item.Add();
	f.typei = typei;
	f.scope = scope;
	(CppItem&)f = m;
}

void AssistEditor::GatherItems(const String& type, bool only_public, Index<String>& in_types, bool types)
{
	LTIMING("GatherItems");
	LLOG("---- GatherItems " << type);
	if(in_types.Find(type) >= 0) {
		LLOG("-> recursion, exiting");
		return;
	}
	in_types.Add(type);
	Vector<String> tparam;
	String ntp = ParseTemplatedType(ResolveTParam(type, tparam), tparam);
	int q = CodeBase().Find(ntp);
	if(q >= 0) {
		if(types) {
			if(ntp.GetCount())
				ntp << "::";
			int typei = assist_type.FindAdd("<types>");
			for(int i = 0; i < CodeBase().GetCount(); i++) {
				String nest = CodeBase().GetKey(i);
				if(nest.GetLength() > ntp.GetLength() &&        // Subscope of scope
				   memcmp(~ntp, ~nest, ntp.GetLength()) == 0 && // e.g. Upp:: -> Upp::String
				   nest.Find("::", ntp.GetLength()) < 0) {      // but not Upp::String::Buffer
					Array<CppItem>& n = CodeBase()[i];
					for(int i = 0; i < n.GetCount(); i++) {
						const CppItem& m = n[i];
						if(m.IsType())
							AssistItemAdd(nest, m, typei);
					}
				}
			}
		}
		const Array<CppItem>& n = CodeBase()[q];
		String base;
		int typei = assist_type.FindAdd(ntp);
		bool op = only_public;
		for(int i = 0; i < n.GetCount(); i++)
			if(n[i].kind == FRIENDCLASS)
				op = false;
		for(int i = 0; i < n.GetCount(); i++) {
			const CppItem& im = n[i];
			if(im.kind == STRUCT || im.kind == STRUCTTEMPLATE)
				base << im.qptype << ';';
			if((im.IsCode() || !thisback && (im.IsData() || im.IsMacro() && IsNull(type)))
			   && (!op || im.access == PUBLIC)) {
				AssistItemAdd(ntp, im, typei);
			}
		}
		if(!thisback) {
			Vector<String> b = Split(base, ';');
			Index<String> h;
			for(int i = 0; i < b.GetCount(); i++)
				h.FindAdd(b[i]);
			b = h.PickKeys();
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.GetCount(); i++)
				if(b[i].GetCount())
					GatherItems(b[i], only_public, in_types, types);
		}
	}
	in_types.Drop();
}

bool OrderAssistItems(const CppItemInfo& a, const CppItemInfo& b)
{
	return CombineCompare(a.uname, b.uname)(a.typei, b.typei)(a.qitem, b.qitem)(b.filetype, a.filetype) < 0;
}

void AssistEditor::RemoveDuplicates()
{
	LTIMING("RemoveDuplicates");
	{ LTIMING("Sort");
	Upp::Sort(assist_item, OrderAssistItems);
	}
	Vector<int> remove;
	{
	LTIMING("Find duplicates");
	int i = 0;
	while(i < assist_item.GetCount()) { // Remove identical items
		int ii = i;
		i++;
		while(i < assist_item.GetCount()
			  && assist_item[ii].typei == assist_item[i].typei
		      && assist_item[ii].qitem == assist_item[i].qitem
		      && assist_item[ii].scope == assist_item[i].scope)
			remove.Add(i++);
	}
	}
	LTIMING("Final remove");
	assist_item.Remove(remove);
}
