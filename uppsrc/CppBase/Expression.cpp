#include "CppBase.h"

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

namespace Upp {

const Array<CppItem>& GetTypeItems(const CppBase& codebase, const String& type)
{
	static Array<CppItem> sEmpty;
	int q = codebase.Find(type);
	if(q < 0)
		return sEmpty;
	return codebase[q];
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

String ResolveTParam(const CppBase& codebase, const String& type, const Vector<String>& tparam)
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
	const Array<CppItem>& x = GetTypeItems(codebase, r);
	if(x.GetCount() && x[0].kind == TYPEDEF) {
		LLOG("Is typedef " << x[0].qtype << ';' << x[0].type << ';' << x[0].natural);
		String h = x[0].qtype;
		if(h != type && h != r)
			return ResolveTParam(codebase, h, tparam);
		return h;
	}
	return r;
}

void ResolveTParam(const CppBase& codebase, Vector<String>& type, const Vector<String>& tparam)
{
	for(int i = 0; i < type.GetCount(); i++)
		type[i] = ResolveTParam(codebase, type[i], tparam);
}

struct ExpressionTyper {
	const CppBase&        codebase;
	int                   scan_counter; // limit permutations
	String                context_type;
	String                usings;
	const Vector<String>& xp;
	Index<String>         typeset;
	const Parser&         parser;

	Vector<String>        GetTypeBases(const String& type);
	String                ResolveReturnType(const CppItem& m, const Vector<String>& tparam);

	void                  ExpressionType(const String& ttype, int ii,
	                                     bool variable, bool can_shortcut_operator,
	                                     Index<String>& visited_bases, int lvl);
	void                  ExpressionType(const String& ttype, int ii, bool variable, int lvl);

	Index<String>         ExpressionType();

	ExpressionTyper(const CppBase& codebase, const Parser& parser, const Vector<String>& xp);	                
};

ExpressionTyper::ExpressionTyper(const CppBase& codebase, const Parser& parser, const Vector<String>& xp)
:	codebase(codebase), parser(parser), xp(xp)
{
	scan_counter = 0;
	context_type = parser.current_scope;
	usings = parser.context.namespace_using;
}

Vector<String> ExpressionTyper::GetTypeBases(const String& type)
{
	const Array<CppItem>& n = GetTypeItems(codebase, type);
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

String ExpressionTyper::ResolveReturnType(const CppItem& m, const Vector<String>& tparam)
{
	if(m.tparam.GetCount()) {
		int q = InScListIndex(m.qtype, m.tname);
		if(q >= 0 && q < tparam.GetCount())
			return tparam[q];
	}
	return m.qtype;
}

void ExpressionTyper::ExpressionType(const String& ttype, int ii,
                                     bool variable, bool can_shortcut_operator,
                                     Index<String>& visited_bases, int lvl)
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
	const Array<CppItem>& n = GetTypeItems(codebase, type);
	LDUMP(type);
	LDUMP(tparam);
	if(codebase.namespaces.Find(ttype) < 0 && ttype.GetCount()) // do not search for namespace typedefs
		for(int i = 0; i < n.GetCount(); i++)
			if(n[i].kind == TYPEDEF) {
				LLOG("typedef -> " << n[i].qtype);
				ExpressionType(n[i].qtype, ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
				return;
			}
	String id = xp[ii];
	int q = id.ReverseFind(':');
	if(q > 0 && id[q - 1] == ':') {
		type = ResolveTParam(codebase, Qualify(codebase, ttype, id.Mid(0, q - 1), usings), tparam);
		id = id.Mid(q + 1);
	}
	if(id.Find('<') >= 0) // as in Single<Display>
		id = ParseTemplatedType(id, tparam);
	LLOG("ExpressionType " << type << " ii: " << ii << " id:" << id << " variable:" << variable);
	
	for(int i = 0; i < tparam.GetCount(); i++) // need to qualify template parameters
		tparam[i] = Qualify(codebase, context_type, tparam[i], usings);

	bool shortcut_oper = false;
	if(!iscid(*id) && *id != '.') {
		shortcut_oper = can_shortcut_operator;
		id = "operator" + id;
		LLOG("id as: " << id);
	}
	if(*id == '.' || (!variable && !iscid(*id))) {
		LLOG(". " << ttype);
		ExpressionType(ttype, ii + 1, false, lvl + 1);
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
				ExpressionType(ResolveTParam(codebase, t, tparam), ii + skipfnpars + 1,
				               m.IsData() && !m.isptr, lvl + 1);
			}
		}
	}
	
	if(typeset.GetCount() != c0 || IsNull(type))
		return;
	Vector<String> base = GetTypeBases(type);
	LDUMPC(base);
	ResolveTParam(codebase, base, tparam);
	LDUMPC(base);
	for(int i = 0; i < base.GetCount(); i++)
		if(visited_bases.Find(base[i]) < 0) {
			visited_bases.Add(base[i]);
			ExpressionType(base[i], ii, variable, false, visited_bases, lvl + 1);
			if(typeset.GetCount() != c0)
				return;
		}

	if(shortcut_oper) {
		LLOG("Shortcut " << xp[ii] << ", ttype " << ttype);
		ExpressionType(ttype, ii + 1, false, lvl + 1);
	}
}

void ExpressionTyper::ExpressionType(const String& ttype, int ii, bool variable, int lvl)
{
	Index<String> bases;
	ExpressionType(ttype, ii, false, true, bases, 0);
}

Index<String> ExpressionTyper::ExpressionType()
{
	LLOG("**** ExpressionType " << xp);
	String type;
	if(xp.GetCount() == 0)
		return typeset;
	if(xp[0] == "this") {
		LLOG("this: " << type);
		ExpressionType(context_type, 1, false, 0);
		return typeset;
	}
	int q = parser.local.FindLast(xp[0]);
	if(q >= 0) {
		String type = Qualify(codebase, context_type, parser.local[q].type, parser.context.namespace_using);
		LLOG("Found type local: " << type << " in scope: " << context_type);
		ExpressionType(type, 1, !parser.local[q].isptr, 0);
		return typeset;
	}
	ExpressionType(context_type, 0, false, 0);
	if(typeset.GetCount())
		return typeset;
	if(xp.GetCount() >= 2 && xp[1] == "()") {
		String qtype = Qualify(codebase, context_type, xp[0], parser.context.namespace_using);
		Vector<String> tparam;
		if(codebase.Find(ParseTemplatedType(qtype, tparam)) >= 0) {
			LLOG("Is constructor " << qtype);
			ExpressionType(qtype, 2, false, 0);
			return typeset;
		}
	}
	Vector<String> ns = parser.GetNamespaces();
	for(int i = 0; i < ns.GetCount(); i++)
		ExpressionType(ns[i], 0, false, 0);
	return typeset;
}

Index<String> GetExpressionType(const CppBase& codebase, const Parser& parser, const Vector<String>& xp)
{ // xp is a list of meaningful parts like "foo", "." , "Fn", "()", "->", "m", "[]"
	return ExpressionTyper(codebase, parser, xp).ExpressionType();
}

void SkipPars(CParser& p)
{
	int lvl = 1;
	while(lvl && !p.IsEof())
		if(p.Char('(') || p.Char('['))
			lvl++;
		else
		if(p.Char(')') || p.Char(']'))
			lvl--;
		else
			p.SkipTerm();
}

Vector<String> MakeXP(const char *s)
{
	Vector<String> xp;
	try {
		CParser p(s);
		while(!p.IsChar2(':', ':') && !p.IsId() && !p.IsEof())
			p.SkipTerm();
		while(!p.IsEof()) {
			String id;
			if(p.IsChar2(':', ':') || p.IsId()) {
				for(;;)
					if(p.Char2(':', ':'))
						id.Cat("::");
					else
					if(p.IsId())
						id.Cat(p.ReadId());
					else
						break;
				const char *s = p.GetPtr();
				if(p.Char('<')) {
					int lvl = 1;
					while(lvl && !p.IsEof()) {
						if(p.Char('<'))
							lvl++;
						else
						if(p.Char('>'))
							lvl--;
						else
							p.SkipTerm();
					}
					while(s < p.GetPtr()) {
						if((byte)*s > ' ')
							id.Cat(*s);
						s++;
					}
				}
				xp.Add(id);
			}
			else
			if(p.Char('(')) {
				xp.Add("()");
				SkipPars(p);
			}
			else
			if(p.Char('[')) {
				xp.Add("[]");
				SkipPars(p);
			}
			else
			if(p.Char2('-', '>'))
				xp.Add("->");
			else
			if(p.Char('.'))
				xp.Add(".");
			else
				break;
		}
	}
	catch(CParser::Error) {}
	return xp;
}

Index<String> GetExpressionType(const CppBase& codebase, const Parser& parser, const char *s)
{
	return GetExpressionType(codebase, parser, MakeXP(s));
}

};