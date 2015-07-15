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

static Array<CppItem> sEmpty;

const Array<CppItem>& GetTypeItems(const String& type)
{
	int q = CodeBase().Find(type);
	if(q < 0)
		return sEmpty;
	return CodeBase()[q];
}

Vector<String> GetTypeBases(const String& type)
{
	const Array<CppItem>& n = GetTypeItems(type);
	String bases;
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& im = n[i];
		if(im.IsType())
			bases << im.qptype << ';';
	}
	Index<String> r;
	Vector<String> h = Split(bases, ';');
	for(int i = 0; i < h.GetCount(); i++)
		r.FindAdd(h[i]);
	return r.PickKeys();
}

String ParseTemplatedType(const String& type, Vector<String>& tparam)
{
	const char *s = type;
	String r;
	while(*s) {
		if(*s == '<') {
			s++;
			int lvl = 0;
			String t;
			while(*s) {
				int c = *s++;
				if(c == ',' && lvl == 0) {
					tparam.Add(t);
					t.Clear();
				}
				else {
					if(c == '>') {
						if(lvl == 0)
							break;
						lvl--;
					}
					if(c == '<')
						lvl++;
					t.Cat(c);
				}
			}
			tparam.Add(t);
			break;
		}
		else
			r.Cat(*s++);
	}
	LLOG("ParseTemplatedType " << type << " -> " << r);
	LDUMPC(tparam);
	return r;
}

String ResolveTParam(const String& type, const Vector<String>& tparam)
{
	LLOG("ResolveTParam " << type << ' ' << tparam);
	String r;
	const char *s = type;
	while(*s) {
		if(IsDigit(*s)) {
			int i = *s++ - '0';
			if(i >= 0 && i < tparam.GetCount())
				r.Cat(tparam[i]);
		}
		else
		if(iscib(*s))
			while(iscid(*s))
				r.Cat(*s++);
		else
			r.Cat(*s++);
	}
	LLOG("Resolved " << type << " -> " << r);
	const Array<CppItem>& x = GetTypeItems(r);
	if(x.GetCount() && x[0].kind == TYPEDEF) {
		LLOG("Is typedef " << x[0].qtype << ';' << x[0].type << ';' << x[0].natural);
		String h = x[0].qtype;
		if(h != type && h != r)
			return ResolveTParam(h, tparam);
		return h;
	}
	return r;
}

void ResolveTParam(Vector<String>& type, const Vector<String>& tparam)
{
	for(int i = 0; i < type.GetCount(); i++)
		type[i] = ResolveTParam(type[i], tparam);
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


Vector<String> GetNamespaces(const Parser& parser)
{
	Vector<String> ns;
	Vector<String> h = Split(parser.current_scope, ':');
	while(h.GetCount()) {
		ns.Add(Join(h, "::"));
		h.Drop();
	}
	ns.Append(Split(parser.context.namespace_using, ';'));
	ns.Add(""); // Add global namespace too
	return ns;
}

String Qualify(const String& scope, const String& type, const String& usings)
{
	return Qualify(CodeBase(), scope, type, usings);
}

String ResolveReturnType(const CppItem& m, const Vector<String>& tparam)
{
	if(m.tparam.GetCount()) {
		int q = InScListIndex(m.qtype, m.tname);
		if(q >= 0 && q < tparam.GetCount())
			return tparam[q];
	}
	return m.qtype;
}

void AssistEditor::ExpressionType(const String& ttype,
                                  const String& context_type,
                                  const String& usings,
                                  const Vector<String>& xp, int ii,
                                  Index<String>& typeset, bool variable,
                                  bool can_shortcut_operator, Index<String>& visited_bases,
                                  int lvl)
{
	LLOG("--- ExpressionType " << scan_counter << ", lvl " << lvl << ", ttype " << ttype);
	if(++scan_counter > 1000 || lvl > 100) // sort of ugly limitation of parsing permutations
		return;
	if(ii >= xp.GetCount()) {
		LLOG("--- Final type: " << ttype);
		typeset.FindAdd(ttype);
		return;
	}
	LDUMP(ii);
	LDUMP(xp[ii]);
	LDUMP(can_shortcut_operator);
	Vector<String> tparam;
	String type = ParseTemplatedType(ttype, tparam);
	int c0 = typeset.GetCount();
	const Array<CppItem>& n = GetTypeItems(type);
	LDUMP(type);
	LDUMP(tparam);
	if(CodeBase().namespaces.Find(ttype) < 0 && ttype.GetCount()) // do not search for namespace typedefs
		for(int i = 0; i < n.GetCount(); i++)
			if(n[i].kind == TYPEDEF) {
				LLOG("typedef -> " << n[i].qtype);
				ExpressionType(n[i].qtype, context_type, usings, xp, ii, typeset, variable, can_shortcut_operator, visited_bases, lvl + 1);
				return;
			}
	String id = xp[ii];
	int q = id.ReverseFind(':');
	if(q > 0 && id[q - 1] == ':') {
		type = ResolveTParam(Qualify(ttype, id.Mid(0, q - 1), usings), tparam);
		id = id.Mid(q + 1);
	}
	if(id.Find('<') >= 0) // as in Single<Display>
		id = ParseTemplatedType(id, tparam);
	LLOG("ExpressionType " << type << " ii: " << ii << " id:" << id << " variable:" << variable);
	
	for(int i = 0; i < tparam.GetCount(); i++) // need to qualify template parameters
		tparam[i] = Qualify(context_type, tparam[i], usings);
/*	
	bool tryop = id == "->";
	if(tryop)
		id = "operator->";
	else
	if(id == "[]") {
		tryop = true;
		id = "operator[]";
	}
*/
	bool shortcut_oper = false;
	if(!iscid(*id) && *id != '.') {
		shortcut_oper = can_shortcut_operator;
		id = "operator" + id;
		LLOG("id as: " << id);
	}
	if(*id == '.' || (!variable && !iscid(*id))) {
		LLOG(". " << ttype);
		ExpressionType(ttype, context_type, usings, xp, ii + 1, typeset, false, lvl + 1);
		return;
	}
	LDUMP(id);
	Index<String> done;
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		if(m.name == id) {
			LLOG("Member " << m.qtype << " " << m.name);
			String t = ResolveReturnType(m, tparam);
			if(done.Find(t) < 0) {
				bool skipfnpars = m.IsCode() && ii + 1 < xp.GetCount() && xp[ii + 1] == "()";
				ExpressionType(ResolveTParam(t, tparam), context_type, usings, xp, ii + skipfnpars + 1,
				               typeset, m.IsData() && !m.isptr, lvl + 1);
			}
		}
	}
	
	if(typeset.GetCount() != c0 || IsNull(type))
		return;
	Vector<String> base = GetTypeBases(type);
	LDUMPC(base);
	ResolveTParam(base, tparam);
	LDUMPC(base);
	for(int i = 0; i < base.GetCount(); i++)
		if(visited_bases.Find(base[i]) < 0) {
			visited_bases.Add(base[i]);
			ExpressionType(base[i], context_type, usings, xp, ii, typeset, variable, false, visited_bases, lvl + 1);
			if(typeset.GetCount() != c0)
				return;
		}

	if(shortcut_oper) {
		LLOG("Shortcut " << xp[ii] << ", ttype " << ttype);
		ExpressionType(ttype, context_type, usings, xp, ii + 1, typeset, false, lvl + 1);
	}
}

void AssistEditor::ExpressionType(const String& type, const String& context_type,
                                  const String& usings, const Vector<String>& xp, int ii,
                                  Index<String>& typeset, bool variable, int lvl)
{
	Index<String> visited_bases;
	ExpressionType(type, context_type, usings, xp, ii, typeset, variable, true, visited_bases, lvl);
}

Index<String> AssistEditor::ExpressionType(const Parser& parser, const Vector<String>& xp)
{
	LLOG("**** ExpressionType " << xp);
	String type;
	Index<String> typeset;
	if(xp.GetCount() == 0)
		return typeset;
	if(xp[0] == "this") {
		LLOG("this: " << type);
		ExpressionType(parser.current_scope, parser.current_scope, parser.context.namespace_using, xp, 1, typeset, false, 0);
		return typeset;
	}
	int q = parser.local.FindLast(xp[0]);
	if(q >= 0) {
		String type = Qualify(parser.current_scope, parser.local[q].type, parser.context.namespace_using);
		LLOG("Found type local: " << type << " in scope: " << parser.current_scope);
		ExpressionType(type, parser.current_scope, parser.context.namespace_using, xp, 1, typeset, !parser.local[q].isptr, 0);
		return typeset;
	}
	ExpressionType(parser.current_scope, parser.current_scope, parser.context.namespace_using, xp, 0, typeset, false, 0);
	if(typeset.GetCount())
		return typeset;
	if(xp.GetCount() >= 2 && xp[1] == "()") {
		String qtype = Qualify(parser.current_scope, xp[0], parser.context.namespace_using);
		Vector<String> tparam;
		if(CodeBase().Find(ParseTemplatedType(qtype, tparam)) >= 0) {
			LLOG("Is constructor " << qtype);
			ExpressionType(qtype, parser.current_scope, parser.context.namespace_using, xp, 2, typeset, false, 0);
			return typeset;
		}
	}
	Vector<String> ns = GetNamespaces(parser);
	for(int i = 0; i < ns.GetCount(); i++)
		ExpressionType(ns[i], parser.current_scope, parser.context.namespace_using, xp, 0, typeset, false, 0);
	return typeset;
}

Index<String> AssistEditor::EvaluateExpressionType(const Parser& parser, const Vector<String>& xp)
{
	scan_counter = 0;
	return ExpressionType(parser, xp);
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
