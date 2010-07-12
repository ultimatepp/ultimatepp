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
	for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
		const CppItem& im = n[i];
		if(im.IsType())
			return Split(im.qptype, ';');
	}
	Vector<String> empty;
	return empty;
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
#ifdef _DEBUG_X_0
	PutVerbose(String().Cat() << "(" << line << "): " << text);
#endif
}

void AssistEditor::Context(Parser& parser, int pos)
{
	theide->ScanFile();
	String s = Get(0, pos);
	StringStream ss(s);
	parser.dobody = true;
	parser.Do(ss, IgnoreList(), CodeBase(), Null, callback(AssistScanError));
	QualifyTypes(CodeBase(), parser.current_scope, parser.current);
	inbody = parser.IsInBody();
#ifdef _DEBUG_X_0
	PutVerbose("body: " + AsString(inbody));
	PutVerbose("scope: " + AsString(parser.current_scope));
#endif
}

String Qualify(const String& scope, const String& type)
{
	return Qualify(CodeBase(), scope, type);
}

void AssistEditor::ExpressionType(const String& ttype, const Vector<String>& xp, int ii,
                                  Index<String>& typeset, bool variable,
                                  bool can_shortcut_operator, Index<String>& visited_bases)
{
	if(ii >= xp.GetCount()) {
		LLOG("--- Final type: " << ttype);
		typeset.FindAdd(ttype);
		return;
	}
	LDUMP(xp[ii]);
	Vector<String> tparam;
	String type = ParseTemplatedType(ttype, tparam);
	int c0 = typeset.GetCount();
	const Array<CppItem>& n = GetTypeItems(type);
	String id = xp[ii];
	int q = id.ReverseFind(':');
	if(q > 0 && id[q - 1] == ':') {
		type = ResolveTParam(Qualify(ttype, id.Mid(0, q - 1)), tparam);
		id = id.Mid(q + 1);
	}
	LLOG("ExpressionType " << type << " ii: " << ii << " id:" << id << " variable:" << variable);
	if(*id == '.' || (!variable && !iscid(*id))) {
		ExpressionType(ttype, xp, ii + 1, typeset, false);
		return;
	}
	bool shortcut_oper = false;
	if(!iscid(*id)) {
		shortcut_oper = can_shortcut_operator;
		id = "operator" + id;
		LLOG("id as: " << id);
	}
	for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
		const CppItem& m = n[i];
		if(m.name == id) {
			LLOG("Member " << m.qtype << "'" << m.name << "'");
			ExpressionType(ResolveTParam(m.qtype, tparam), xp, ii + 1, typeset, m.IsData() && !m.isptr);
		}
	}
	if(typeset.GetCount() != c0 || IsNull(type))
		return;
	Vector<String> base = GetTypeBases(type);
	ResolveTParam(base, tparam);
	for(int i = 0; i < base.GetCount(); i++)
		if(visited_bases.Find(base[i]) < 0) {
			visited_bases.Add(base[i]);
			ExpressionType(base[i], xp, ii, typeset, variable, false, visited_bases);
			if(typeset.GetCount() != c0)
				return;
		}
	if(shortcut_oper)
		ExpressionType(ttype, xp, ii + 1, typeset, false);
}

void AssistEditor::ExpressionType(const String& type, const Vector<String>& xp, int ii,
                                  Index<String>& typeset, bool variable)
{
	Index<String> visited_bases;
	ExpressionType(type, xp, ii, typeset, variable, true, visited_bases);
}
/*
void AssistEditor::ExpressionType(const String& type, const Vector<String>& xp, int ii,
                                  Index<String>& typeset)
{
	ExpressionType(type, xp, ii, typeset, false);
}
*/
Index<String> AssistEditor::ExpressionType(const Parser& parser, const Vector<String>& xp)
{
	String type;
	Index<String> typeset;
	if(xp.GetCount() == 0)
		return typeset;
	if(xp[0] == "this") {
		LLOG("this: " << type);
		ExpressionType(parser.current_scope, xp, 1, typeset, false);
		return typeset;
	}
	int q = parser.local.FindLast(xp[0]);
	if(q >= 0) {
		String type = Qualify(parser.current_scope, parser.local[q].type);
		LLOG("Found type local: " << type << " in scope: " << parser.current_scope);
		ExpressionType(type, xp, 1, typeset, !parser.local[q].isptr);
		return typeset;
	}
	ExpressionType(parser.current_scope, xp, 0, typeset, false);
	if(typeset.GetCount())
		return typeset;
	if(xp.GetCount() >= 2 && xp[1] == "()") {
		String qtype = Qualify(parser.current_scope, xp[0]);
		Vector<String> tparam;
		if(CodeBase().Find(ParseTemplatedType(qtype, tparam)) >= 0) {
			LLOG("Is constructor " << qtype);
			ExpressionType(qtype, xp, 2, typeset, false);
			return typeset;
		}
	}
	ExpressionType("", xp, 0, typeset, false);
	return typeset;
}

int CharFilterT(int c)
{
	return c >= '0' && c <= '9' ? "TUVWXYZMNO"[c - '0'] : c;
}

void AssistEditor::GatherItems(const String& type, bool only_public, Index<String>& in_types, bool types)
{
	LLOG("GatherItems " << type);
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
				String n = CodeBase().GetKey(i);
				if(n.GetLength() > ntp.GetLength() && memcmp(~ntp, ~n, ntp.GetLength()) == 0) {
					Array<CppItem>& n = CodeBase()[i];
					for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
						const CppItem& m = n[i];
						if(m.IsType()) {
							CppItemInfo& f = assist_item.Add(m.name);
							f.typei = typei;
							(CppItem&)f = m;
							break;
						}
					}
				}
			}
		}
		const Array<CppItem>& n = CodeBase()[q];
		String base;
		int typei = assist_type.FindAdd(ntp);
		bool op = only_public;
		for(int i = 0; i < n.GetCount(); i = FindNext(n, i))
			if(n[i].kind == FRIENDCLASS)
				op = false;
		for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
			const CppItem& im = n[i];
			if(im.kind == STRUCT || im.kind == STRUCTTEMPLATE)
				base = im.qptype;
			if((im.IsCode() || !thisback && (im.IsData() || im.IsMacro() && type == ""))
			   && (!op || im.access == PUBLIC)) {
				int q = assist_item.Find(im.name);
				while(q >= 0) {
					if(assist_item[q].typei != typei)
						assist_item[q].over = true;
					q = assist_item.FindNext(q);
				}
				CppItemInfo& f = assist_item.Add(im.name);
				f.typei = typei;
				f.scope = ntp;
				(CppItem&)f = im;
			}
		}
		if(!thisback) {
			Vector<String> b = Split(base, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.GetCount(); i++)
				if(b[i].GetCount())
					GatherItems(b[i], only_public, in_types, types);
		}
	}
	in_types.Drop();
}
