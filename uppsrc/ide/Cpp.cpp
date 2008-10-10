#include "ide.h"

#define LDUMP(x)     //DDUMP(x)
#define LDUMPC(x)    //DDUMPC(x)
#define LLOG(x)      //DLOG(x)

static Array<CppItem> sEmpty;

const Array<CppItem>& GetTypeItems(const String& type)
{
	int q = BrowserBase().Find(type);
	if(q < 0)
		return sEmpty;
	return BrowserBase()[q];
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
				if(c == ',') {
					if(lvl == 0 && t.GetCount()) {
						tparam.Add(t);
						t.Clear();
					}
				}
				else {
					if(c == '>' && --lvl)
						break;
					if(c == '<')
						lvl++;
					t.Cat(c);
				}
			}
			if(t.GetCount())
				tparam.Add(t);		
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
		LLOG("Is typedef " << x[0].qtype);
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
	theide->ScanFile();
	String s = Get(0, pos);
	StringStream ss(s);
	parser.dobody = true;
	parser.Do(ss, IgnoreList(), BrowserBase(), Null, callback(AssistScanError));
	QualifyTypes(BrowserBase(), parser.current_scope, parser.current);
	inbody = parser.IsInBody();
#ifdef _DEBUG
	PutVerbose("body: " + AsString(inbody));
	PutVerbose("scope: " + AsString(parser.current_scope));
#endif
}

String Qualify(const String& scope, const String& type)
{
	return Qualify(BrowserBase(), scope, type);
}

void AssistEditor::ExpressionType(const String& type, const Vector<String>& xp, int ii,
                                  Index<String>& typeset, const Vector<String>& tparam,
                                  bool can_shortcut_operator, Index<String>& visited_bases)
{
	if(ii >= xp.GetCount()) {
		LLOG("Final type: " << type);
		typeset.FindAdd(type);
		return;
	}
	int c0 = typeset.GetCount();
	const Array<CppItem>& n = GetTypeItems(type);
	String id = xp[ii];
	int q = id.ReverseFind(':');
	if(q > 0 && id[q - 1] == ':') {
		Vector<String> tparam;
		String qtype = ParseTemplatedType(Qualify("", id.Mid(0, q - 1)), tparam);
		id = id.Mid(q + 1);
		ExpressionType(ResolveTParam(qtype, tparam), xp, ii + 1, typeset);
		return;
	}
	LLOG("ExpressionType " << type << " ii: " << ii << " id:" << id);
	if(*id == '.') {
		ExpressionType(type, xp, ii + 1, typeset, tparam);
		return;
	}
	bool shortcut_oper = false;
	if(!iscid(*id)) {
		shortcut_oper = can_shortcut_operator;
		id = "operator" + id;
	}
	for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
		const CppItem& m = n[i];
		if(m.name == id)
			ExpressionType(ResolveTParam(m.qtype, tparam), xp, ii + 1, typeset);
	}
	if(typeset.GetCount() != c0 || IsNull(type))
		return;
	Vector<String> base = GetTypeBases(type);
	ResolveTParam(base, tparam);
	for(int i = 0; i < base.GetCount(); i++)
		if(visited_bases.Find(base[i]) < 0) {
			visited_bases.Add(base[i]);
			Vector<String> tparam;
			ExpressionType(ParseTemplatedType(base[i], tparam), xp, ii, typeset,
			               tparam, false, visited_bases);
			if(typeset.GetCount() != c0)
				return;
		}
	if(shortcut_oper)
		ExpressionType(type, xp, ii + 1, typeset, tparam);
}

void AssistEditor::ExpressionType(const String& type, const Vector<String>& xp, int ii,
                                  Index<String>& typeset, const Vector<String>& tparam)
{
	Index<String> visited_bases;
	ExpressionType(type, xp, ii, typeset, tparam, true, visited_bases);
}

void AssistEditor::ExpressionType(const String& type, const Vector<String>& xp, int ii,
                                  Index<String>& typeset)
{
	Vector<String> tparam;
	ExpressionType(ParseTemplatedType(type, tparam), xp, ii, typeset, tparam);
}

Index<String> AssistEditor::ExpressionType(const Parser& parser, const Vector<String>& xp)
{
	String type;
	Index<String> typeset;
	if(xp.GetCount() == 0)
		return typeset;
	if(xp[0] == "this") {
		LLOG("this: " << type);
		ExpressionType(parser.current_scope, xp, 1, typeset);
		return typeset;
	}
	int q = parser.local.FindLast(xp[0]);
	if(q >= 0) {
		String type = Qualify(parser.current_scope, parser.local[q]);
		LLOG("Found type local: " << type);
		ExpressionType(type, xp, 1, typeset);
		return typeset;
	}
	ExpressionType(parser.current_scope, xp, 0, typeset);
	if(typeset.GetCount())
		return typeset;
	if(xp.GetCount() >= 2 && xp[1] == "()") {
		Vector<String> tparam;
		String qtype = ParseTemplatedType(Qualify(parser.current_scope, xp[0]), tparam);
		if(BrowserBase().Find(qtype) >= 0) {
			LLOG("Is constructor " << qtype);
			ExpressionType(ResolveTParam(qtype, tparam), xp, 2, typeset);
			return typeset;
		}
	}
	ExpressionType("", xp, 0, typeset);
	return typeset;
}

int CharFilterT(int c)
{
	return c >= '0' && c <= '9' ? "TUVWXYZMNO"[c - '0'] : c;
}

void AssistEditor::GatherItems(const String& type, bool nom, Index<String>& in_types, bool tp)
{
	LLOG("GatherItems " << type);
	if(in_types.Find(type) >= 0) {
		LLOG("-> recursion, exiting");
		return;
	}
	in_types.Add(type);
	Vector<String> tparam;
	String ntp = ParseTemplatedType(type, tparam);
	int q = BrowserBase().Find(ntp);
	if(q >= 0) {
		if(tp) {
			if(ntp.GetCount())
				ntp << "::";
			int typei = assist_type.FindAdd("<types>");
			for(int i = 0; i < BrowserBase().GetCount(); i++) {
				String n = BrowserBase().GetKey(i);
				if(n.GetLength() > ntp.GetLength() && memcmp(~ntp, ~n, ntp.GetLength()) == 0) {
					Array<CppItem>& n = BrowserBase()[i];
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
		const Array<CppItem>& n = BrowserBase()[q];
		String base;
		int typei = assist_type.FindAdd(ntp);
		for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
			const CppItem& im = n[i];
			if(im.IsType())
				base = im.qptype;
			if((im.IsCode() || im.IsData() || im.IsMacro() && type == "")
			   && (nom || im.access == PUBLIC)) {
				int q = assist_item.Find(im.name);
				while(q >= 0) {
					if(assist_item[q].typei != typei)
						assist_item[q].over = true;
					q = assist_item.FindNext(q);
				}
				CppItemInfo& f = assist_item.Add(im.name);
				f.typei = typei;
				(CppItem&)f = im;
			}
		}
		Vector<String> b = Split(base, ';');
		ResolveTParam(b, tparam);
		for(int i = 0; i < b.GetCount(); i++)
			if(b[i].GetCount())
				GatherItems(b[i], nom, in_types, tp);
	}
	in_types.Drop();
}
