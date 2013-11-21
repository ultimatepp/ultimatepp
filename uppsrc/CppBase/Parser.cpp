#include "CppBase.h"

NAMESPACE_UPP

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif


#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // TIMING(x)

inline const char *bew(const char *s, const char *t)
{
	while(*s)
		if(*s++ != *t++)
			return NULL;
	return t;
}

static String s_operator("operator");
static String s_virtual("virtual");
static String s_inline("inline");
static String s_static("static");

static inline bool sSpaces(String& res, const char *& s)
{
	if((byte)*s <= ' ') {
		char c = *s++;
		if(c != '\2') {
			res.Cat(' ');
			while((byte)*s <= ' ' && *s)
				s++;
		}
		return true;
	}
	return false;
}

String FnItem(const char *s, const char *pname, const char *qname, const String& name) {
	String res;
	while(*s && (byte)*s <= ' ') s++;
	while(*s) {
		while(*s && !iscid(*s) && *s != '~')
			s++;
		while(iscid(*s) || *s == '~')
			res.Cat(*s++);
		if(res == s_operator) {
			while(*s && *s != '(') {
				if((byte)*s >= ' ')
					res.Cat(*s);
				s++;
			}
			break;
		}
		while(*s && (byte)*s <= ' ')
			s++;
		if(*s == '(' && (res == name || res == '~' + name))
			break;
		res.Clear();
	}
	while(*s) {
		const char *w = bew(qname, s);
		byte c = *s;
		if(w && !iscid(*w)) {
			if(iscid(*res.Last()))
				res.Cat(' ');
			res.Cat(name);
			s = w;
		}
		else
		if(iscid(c)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			String q(b, s);
			if(q != s_virtual && q != s_inline && q != s_static && !InScList(q, pname)) {
				if(iscid(*res.Last()))
					res.Cat(' ');
				res.Cat(q);
			}
			else
				while((byte)*s <= ' ' && *s) s++;
		}
		else
		if(c == '=') {
			s++;
			int l = 0;
			while(*s && !(l == 0 && (*s == ',' || *s == ')'))) {
				if(*s == '(')
					l++;
				else
				if(*s == ')')
					l--;
				s++;
			}
		}
		else
		if((byte)*s <= ' ') {
			s++;
			while((byte)*s <= ' ' && *s)
				s++;
		}
		else {
			res.Cat(c);
			s++;
		}
	}
	return res;
}

String Purify(const char *s, const char *qname, const String& name) {
	String res;
	while(*s && (byte)*s <= ' ') s++;
	while(*s) {
		const char *w = bew(qname, s);
		if(w && w > s) {
			res.Cat(name);
			s = w;
		}
		else
		if(iscid(*s)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			String q(b, s);
			if(q != s_virtual && q != s_inline && q != s_static)
				res.Cat(q);
			else
				while((byte)*s <= ' ' && *s) s++;
		}
		else
		if(!sSpaces(res, s))
			res.Cat(*s++);
	}
	return TrimRight(res);
}

String Purify(const char *s) {
	String res;
	while(*s && (byte)*s <= ' ') s++;
	while(*s) {
		if(iscid(*s)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			String q(b, s);
			if(q != s_virtual && q != s_inline)
				res.Cat(q);
			else
				while((byte)*s <= ' ' && *s) s++;
		}
		else
		if(!sSpaces(res, s))
			res.Cat(*s++);
	}
	return TrimRight(res);
}

String Gpurify(const char *s)
{
	String res;
	while(*s)
		if(!sSpaces(res, s))
			res.Cat(*s++);
	return res;
}

void ScAdd(String& s, const String& a)
{
	if(a.IsEmpty()) return;
	if(!s.IsEmpty())
		s << ';';
	s << a;
}

String Parser::Context::Dump() const
{
	return "Scopeing: " + scope;
}

Parser::FunctionStat::FunctionStat(const String & scope,
                                   const CppItem & cppItem,
		                           const LexSymbolStat &symbolStat,
		                           int maxScopeDepth) :
	scope(scope), cppItem(cppItem),
	symbolStat(symbolStat), maxScopeDepth(maxScopeDepth)
{
}

static String s_dblcln("::");

inline void ScopeCat(String& scope, const String& s)
{
	if(scope.GetCount())
		scope << s_dblcln;
	scope << s;
}

void Parser::Context::operator<<=(const Context& t)
{
	scope = t.scope;
	typenames <<= t.typenames;
	tparam <<= t.tparam;
	access = t.access;
	ctname = t.ctname;
}

Parser::Decla::Decla()
{
	function = type_def = false;
	s_static = s_auto = s_register = s_extern = s_mutable = s_explicit = s_virtual = false;
	isfriend = istemplate = istructor = isptr = nofn = false;
	castoper = false;
}


bool Parser::Key(int code)
{
	if(lex == code) {
		++lex;
		return true;
	}
	return false;
}

int  Parser::GetLine(const char *p)
{
	int pos = int(p - ~file.text);
	int l = 0;
	int h = file.linepos.GetCount();
	while(l < h) {
		int q = (l + h) / 2;
		if(file.linepos[q] < pos)
			l = q + 1;
		else
			h = q;
	}
	return l;
}

void Parser::Line()
{
	int pos = int(lex.Pos() - ~file.text);
	while(line + 1 < file.linepos.GetCount() && file.linepos[line + 1] <= pos)
		line++;
}

void Parser::ThrowError(const String& e)
{
	err(GetLine(lex.Pos()), e);
#ifdef _DEBUG
	int i = 0;
	while(i < 40 && lex[i] != t_eof)
		i++;
	LLOG("ERROR: (" << GetLine(lex.Pos()) << ") " << e << ", scope: " << current_scope <<
	     "  " << AsCString(String(lex.Pos(), lex.Pos(i))));
#endif
	throw Error();
}

void Parser::Check(bool b, const char *err)
{
	if(!b) ThrowError(err);
}

void Parser::CheckKey(int c)
{
	if(!Key(c)) ThrowError(Format("Missing %c", c));
}

String Parser::TemplateParams(String& param)
{
	const char *pos = lex.Pos();
	CheckKey('<');
	int level = 1;
	String id;
	bool gp = true;
	while(lex != t_eof) {
		if(lex.IsId() && gp)
			id = lex.GetId();
		else
		if(Key(',')) {
			ScAdd(param, id);
			id.Clear();
			gp = true;
		}
		else
		if(Key('=')) {
			if(!id.IsEmpty()) {
				ScAdd(param, id);
				id.Clear();
			}
			gp = false;
		}
		else
		if(Key('>')) {
			level--;
			if(level <= 0) {
				ScAdd(param, id);
				break;
			}
		}
		else
		if(Key(t_shr) && level >= 2) {
			level -= 2;
			if(level <= 0) {
				ScAdd(param, id);
				break;
			}
		}
		else
		if(Key('<'))
			level++;
		else
			++lex;
	}
	return String(pos, lex.Pos());
}

String Parser::TemplateParams()
{
	String dummy;
	return TemplateParams(dummy);
}

String Parser::TemplatePnames()
{
	String pnames;
	TemplateParams(pnames);
	return pnames;
}

String Parser::ReadOper(bool& castoper) {
	const char *p = lex.Pos();
	Key(tk_operator);
	int level = 0;
	if(Key('('))
		level++;
	for(;;) {
		if(lex == t_eof || level <= 0 && lex == '(') break;
		if(Key('(') || Key('[')) level++;
		else
		if(Key(')')  || Key(']')) level--;
		else
			++lex;
	}
	StringBuffer r;
	bool spc = false;
	while(p < lex.Pos()) {
		if((byte)*p > ' ') {
			if(spc && iscid(*p)) {
				castoper = true;
				r.Cat(' ');
			}
			r.Cat(*p);
			spc = false;
		}
		else
			spc = true;
		p++;
	}
	return r;
}

String Parser::Name(String& name, bool& castoper)
{
	String s;
	if(Key(t_dblcolon)) {
		s = "::";
		name = s;
	}
	Check(lex.IsId() || lex == tk_operator, "Name expected");
	for(;;) {
		if(lex.IsId()) {
			name << lex.Id();
			s << lex.GetId();
		}
		else {
			String h = ReadOper(castoper);
			name << h;
			s << h;
			break;
		}
		if(lex == '<')
			s << TemplateParams();
		if(Key(t_dblcolon)) {
			s << "::";
			name << "::";
			if(Key('~')) {
				s << "~";
				name << "~";
			}
		}
		else
			break;
	}
	return s;
}

String Parser::Name(bool& castoper)
{
	String h;
	return Name(h, castoper);
}

String Parser::Constant()
{
	const char *p = lex.Pos();
	const char *p1 = p;
	int level = 0;
	for(;;) {
		p1 = lex.Pos();
		if(lex == t_eof) break;
		if(level <= 0 && (lex == ',' || lex == ';' || lex == ')' || lex == '}' || lex == ']'))
			break;
		if(Key('(') || Key('[') || Key('{')) level++;
		else
		if(Key(')') || Key(']') || Key('}')) level--;
		else
			++lex;
	}
	return String(p, p1);
}

String Parser::TType()
{
	int q = FindIndex(context.tparam, lex[0]);
	if(q >= 0) return AsString(q);
	return lex.Id();
}

String Parser::SimpleType(Decla& d)
{
	Key(tk_typename) || Key(tk_struct) || Key(tk_class) || Key(tk_union) || Key(tk_enum);
	if(Key(tk_bool) || Key(tk_float) || Key(tk_double) || Key(tk_void))
		return Null;
	bool sgn = Key(tk_signed) || Key(tk_unsigned);
	if(Key(tk_long)) {
		Key(tk_long);
		Key(tk_int) || Key(tk_double);
		return Null;
	}
	if(Key(tk_short)) {
		Key(tk_unsigned);
		Key(tk_int);
		return Null;
	}
	if(Key(tk_int) || Key(tk_char) ||
	   Key(tk___int8) || Key(tk___int16) || Key(tk___int32) || Key(tk___int64)) return Null;
	if(sgn) return Null;
	const char *p = lex.Pos();
	bool cs = false;
	Index<int> cix;
	if(Key(t_dblcolon))
		d.type << "::";
	Check(lex.IsId(), "Name expected");
	while(lex.IsId()) {
		d.type << TType();
		if(cix.Find(lex) >= 0)
			cs = true;
		else
			cix.Add(lex);
		++lex;
		if(lex == '<')
			d.type << TemplateParams();
		if(Key(t_dblcolon)) {
			d.type << "::";
			if(Key('~'))
				cs = true;
		}
		else
			break;
	}
	return cs ? String(p, lex.Pos()) : String();
}

void Parser::Qualifier()
{
	Key(tk_const);
	Key(tk_volatile);
	if(Key(tk_throw)) {
		while(lex != t_eof && !Key(')'))
			++lex;
	}
}

void Parser::Elipsis(Decl& d)
{
	Decl& q = d.param.Add();
	q.name = "...";
	CheckKey(')');
}

void Parser::ParamList(Decl& d) {
	if(!Key(')'))
		for(;;) {
			if(Key(t_elipsis)) {
				Elipsis(d);
				break;
			}
			else
				d.param.Add() = Declaration().Top();
			if(Key(t_elipsis)) {
				Elipsis(d);
				break;
			}
			if(Key(')')) break;
			CheckKey(',');
		}
}

int Parser::RPtr()
{
	int n = 0;
	int tlevel = 0;
	for(;;) {
		int t = lex[n];
		if(t == '*') return n + 1;
		if(t == '<') {
			tlevel++;
			n++;
		}
		else
		if(t == '>') {
			tlevel--;
			n++;
		}
		else
		if(t == t_shr && tlevel >= 2) {
			tlevel -= 2;
			n++;
		}
		else
		if(t == t_dblcolon || lex.IsId(n) || t == ',' && tlevel > 0)
			n++;
		else
			return 0;
	}
}

void Parser::EatInitializers()
{
	if(Key(':'))
		while(lex != '{' && lex != t_eof)
			++lex;
}

void Parser::Declarator(Decl& d, const char *p)
{
	int n = RPtr();
	if(n) {
		while(n--) lex.Get();
		Declarator(d, p);
		d.isptr = true;
		return;
	}
	if(Key('&')) {
		Declarator(d, p);
		return;
	}
	if(Key(tk_const)) {
		Declarator(d, p);
		return;
	}
	if(Key('(')) {
		Declarator(d, p);
		if(d.isptr)
			d.nofn = true;
		CheckKey(')');
	}
//	if(lex == tk_operator)
//		d.name = ReadOper();
//	else
	if(lex.IsId() || lex == t_dblcolon || lex == tk_operator) {
		d.name = Name(d.castoper);
		bool dummy;
		if(Key(':'))
			if(!Key(t_integer))
				Name(dummy);
	}
	if(Key('(')) {
		if(inbody || (lex < 256 || lex == tk_true || lex == tk_false)
		   && lex != ')' && lex != t_dblcolon) {
			int level = 0;
			for(;;) {
				if(lex == t_eof) break;
				if(Key('(')) level++;
				else
				if(Key(')')) {
					if(--level < 0) break;
				}
				else
					++lex;
			}
			return;
		}
		else {
			d.function = !d.nofn;
			ParamList(d);
			p = lex.Pos();
			Qualifier();
		}
	}
	EatInitializers();
	while(Key('[')) {
		d.isptr = true;
		int level = 1;
		while(level && lex != t_eof) {
			if(Key('[')) level++;
			else
			if(Key(']')) level--;
			else
				++lex;
		}
	}
	if(Key('=') || (inbody && lex == '(')) {
		int level = 0;
		for(;;) {
			if(lex == t_eof || level == 0 && (lex == ',' || lex == ')' || lex == ';'))
				break;
			if(Key('(') || Key('{'))
				level++;
			else
			if(Key(')') || Key('}'))
				level--;
			else
				++lex;
		}
	}
}

Parser::Decl& Parser::Finish(Decl& d, const char *s)
{
	d.natural = String(s, lex.Pos());
	return d;
}

Parser::Decl Parser::Type() {
	Decl d;
	const char *p = lex.Pos();
	Qualifier();
	SimpleType(d);
	Declarator(d, p);
	return Finish(d, p);
}

bool Parser::IsParamList(int q)
{
	return true;
}

Array<Parser::Decl> Parser::Declaration0(bool l0, bool more)
{
	Array<Decl> r;
	Decla d;
	const char *p = lex.Pos();
	if(Key(tk_friend))
		d.isfriend = true;
	for(;;) {
		if(Key(tk_static))
			d.s_static = true;
		else
		if(Key(tk_extern))
			d.s_extern = true;
		else
		if(Key(tk_auto))
			d.s_auto = true;
		else
		if(Key(tk_register))
			d.s_register = true;
		else
		if(Key(tk_mutable))
			d.s_mutable = true;
		else
		if(Key(tk_explicit))
			d.s_explicit = true;
		else
		if(Key(tk_virtual))
			d.s_virtual = true;
		else
		if(!(Key(tk_inline) || Key(tk_force_inline)))
			break;
	}
	Qualifier();
	if(l0)
		if(lex == tk_SKYLARK && lex[1] == '(' && lex.IsId(2)) {
			++lex;
			++lex;
			Decl& a = r.Add();
			a.name = lex.GetId();
			a.function = true;
			a.natural = String().Cat() << "void " << a.name << "(Http& http)";
			Decl& p = a.param.Add();
			p.name = "http";
			p.type = "Http";
			p.natural = "Http& http";
			Key(',');
			lex.GetText();
			Key(')');
			return r;
		}
		else
		if((lex == tk_RPC_METHOD || lex == tk_RPC_GMETHOD) && lex[1] == '(' && lex.IsId(2)) {
			++lex;
			++lex;
			Decl& a = r.Add();
			a.name = lex.GetId();
			a.function = true;
			a.natural = String().Cat() << "void " << a.name << "(RpcData& rpc)";
			Decl& p = a.param.Add();
			p.name = "rpc";
			p.type = "RpcData";
			p.natural = "RpcData& rpc";
			if (Key(','))
				lex.GetText();
			Key(')');
			return r;
		}
	bool isdestructor = Key('~');
	if(l0 && context.typenames.Find(lex) >= 0 && lex[1] == '(' && lex.IsId()) {
		Decl& a = r.Add();
		a.name = lex.GetId();
		a.isdestructor = isdestructor;
		a.function = true;
		a.istructor = true;
		++lex;
		ParamList(a);
		Qualifier();
		a.natural = String(p, lex.Pos());
		EatInitializers();
		return r;
	}
	if(lex == tk_operator) {
		Decl& a = r.Add();
		(Decla&)a = d;
		a.name = ReadOper(a.castoper);
		Key('(');
		ParamList(a);
		Qualifier();
		a.function = true;
		a.natural = String(p, lex.Pos());
		return r;
	}
	String st = SimpleType(d);
	if(!st.IsEmpty()) {
		Decl& a = r.Add();
		a.name = st;
		a.isdestructor = st.Find('~') > 0;
		a.function = true;
		a.istructor = true;
		if(Key('('))
			ParamList(a);
		Qualifier();
		a.natural = String(p, lex.Pos());
		EatInitializers();
		return r;
	}
	String natural1 = String(p, lex.Pos());
	do {
		const char *p1 = lex.Pos();
		Decl& a = r.Add();
		(Decla&)a = d;
		Declarator(a, p);
		if(a.castoper)
			a.name = Filter(natural1, CharFilterNotWhitespace) + a.name;
		a.natural = natural1 + String(p1, lex.Pos());
		p = lex.Pos();
	}
	while(more && Key(','));
	return r;
}

Array<Parser::Decl> Parser::Declaration(bool l0, bool more)
{
	if(Key(tk_typedef)) {
		Array<Decl> r = Declaration(false, true);
		for(int i = 0; i < r.GetCount(); i++) {
			r[i].type_def = true;
			r[i].natural = "typedef " + r[i].natural;
		}
		return r;
	}
	return Declaration0(l0, more);
}

void Parser::Locals(const String& type)
{
	Array<Parser::Decl> d = Declaration(true, true);
	for(int i = 0; i < d.GetCount(); i++) {
		Local& l = local.Add(d[i].name);
		l.type = type;
		l.isptr = d[i].isptr;
	}
}

String Parser::Tparam(int& q)
{
	if(lex[q] != '<')
		return Null;
	const char *p = lex.Pos(q);
	int level = 1;
	q++;
	while(lex[q] != t_eof && level) {
		if(lex[q] == '<')
			level++;
		if(lex[q] == '>')
			level--;
		else
		if(lex[q] == t_shr && level >= 2)
			level -= 2;
		q++;
	}
	return String(p, lex.Pos(q));
}

String NoTemplatePars(const String& s)
{
	int q = s.Find('<');
	return q >= 0 ? s.Mid(0, q) : s;
}

bool Parser::TryDecl()
{
	int q = 0;
	while(lex[0] == tk_static || lex[0] == tk_const || lex[0] == tk_auto ||
	      lex[0] == tk_register || lex[0] == tk_volatile)
	      	++lex;
	int t = lex[q];
	if(t == tk_int || t == tk_bool || t == tk_float || t == tk_double || t == tk_void ||
	   t == tk_long || t == tk_signed || t == tk_unsigned || t == tk_short ||
	   t == tk_char || t == tk___int8 || t == tk___int16 || t == tk___int32 || t == tk___int64) {
		while(lex[q] == '*' || lex[q] == '&')
			q++;
		if(!lex.IsId(q))
			return false;
		Locals(Null);
		return true;
	}
	String type;
	if(lex[q] == t_dblcolon) {
		type << "::";
		q++;
	}
	if(lex.IsId(q)) {
		type << lex.Id(q++);
		type << Tparam(q);
	}
	else
		return false;
	while(lex[q] == t_dblcolon) {
		type << "::";
		if(lex.IsId(++q))
			type << lex.Id(q++);
		else
			return false;
		type << Tparam(q);
	}
	while(lex[q] == '*' || lex[q] == '&')
		q++;
	if(!lex.IsId(q))
		return false;
	type = Qualify(*base, current_scope, type);
	if(base->Find(NoTemplatePars(type)) >= 0) {
		Locals(type);
		return true;
	}
	return false;
}

void Parser::MatchPars()
{
	int level = 1;
	while(level && lex != t_eof) {
		if(Key('(')) level++;
		else
		if(Key(')')) level--;
		else
			++lex;
	}
}

void Parser::Statement()
{
	RecursionCounter recursionCounter(currentScopeDepth, lex == '{' ? 0 : 1);
	maxScopeDepth = max(maxScopeDepth, currentScopeDepth);

	if(Key(tk_case)) {
		if(lex.IsId())
			++lex;
		Key(':');
	}
	if(Key(tk_default))
		Key(':');
	if(lex.IsId() && lex[1] == ':') {
		++lex;
		++lex;
	}
	if(Key('{')) {
		int l = local.GetCount();
		while(!Key('}')) {
			if(lex == t_eof)
				ThrowError("");
			Statement();
		}
		local.Trim(l);
	}
	else
	if(Key(tk_if)) {
		int l = local.GetCount();
		Key('(');
		TryDecl();
		MatchPars();
		Statement();
		if(Key(tk_else))
			Statement();
		local.Trim(l);
	}
	else
	if(Key(tk_for)) {
		int l = local.GetCount();
		Key('(');
		TryDecl();
		MatchPars();
		Statement();
		local.Trim(l);
	}
	else
	if(Key(tk_while)) {
		int l = local.GetCount();
		Key('(');
		TryDecl();
		MatchPars();
		Statement();
		local.Trim(l);
	}
	else
	if(Key(tk_try))
		Statement();
	else
	if(Key(tk_catch)) {
		Key('(');
		MatchPars();
		Statement();
	}
	else
	if(Key(tk_do)) {
		Statement();
		Key(tk_while);
		Key('(');
		MatchPars();
	}
	else
	if(Key(tk_switch)) {
		int l = local.GetCount();
		Key('(');
		TryDecl();
		MatchPars();
		Statement();
		local.Trim(l);
	}
	else
	if(TryDecl())
		Key(';');
	else
		for(;;) {
			if(lex == t_eof)
				ThrowError("");
			if(Key(';') || lex == '{' || lex == '}' || lex >= tk_if && lex <= tk_do)
				break;
			++lex;
		}
}

bool Parser::EatBody()
{
	if(lex != '{') {
		local.Clear();
		return false;
	}
	lex.BeginBody();
	maxScopeDepth = currentScopeDepth = dobody ? 0 : 1;
	if(dobody) {
		inbody = true;
		Statement();
		inbody = false;
		local.Clear();
	}
	else {
		Key('{');
		int level = 1;
		while(level && lex != t_eof) {
			if(Key('{')) level++;
			else
			if(Key('}')) level--;
			else
				++lex;
			maxScopeDepth = max(level, maxScopeDepth);
		}
	}
	lex.EndBody();
	return true;
}

String CleanTp(const String& tp)
{
	int q = tp.Find('<');
	int w = tp.ReverseFind('>');
	if(q < 0 || w < 0) return tp;
	String a = TrimLeft(TrimRight(tp.Mid(q + 1, w - q - 1)));
	const char *s = a;
	String r;
	while(*s) {
		if(*s == ',') {
			r.Cat(';');
			s++;
			while(*s && *s <= ' ')
				s++;
		}
		else
			r.Cat(*s++);
	}
	return r;
}

void Parser::SetScopeCurrent()
{
	current_scope = context.scope;
}

CppItem& Parser::Item(const String& scope, const String& item, const String& name, bool impl)
{
	current_scope = scope;
	if(dobody)
		current = CppItem();
	current_key = item;
	current_name = name;
	Array<CppItem>& n = base->GetAdd(current_scope);
	CppItem& im = dobody ? current : n.Add();
	im.item = item;
	im.name = name;
	im.file = filei;
	im.line = line + 1;
	im.impl = impl;
	im.filetype = filetype;
	LLOG("New item " << GetCppFile(filei) << ' ' << line + 1 << "    " << scope << "::" << item);
	return im;
}

CppItem& Parser::Item(const String& scope, const String& item, const String& name)
{
	String h = Purify(item);
	CppItem& im = Item(scope, h, name, false);
	im.natural = h;
	return im;
}

void Parser::Resume(int bl)
{
	for(;;) {
		if(lex == t_eof || lex.GetBracesLevel() == bl && lex == ';')
			break;
		++lex;
	}
}

void Parser::ScopeBody()
{
	int bl = lex.GetBracesLevel();
	while(!Key('}')) {
		if(lex == t_eof)
			ThrowError("Unexpected end of file");
		try {
			Do();
		}
		catch(Error) {
			Resume(bl);
			Key(';');
		}
	}
}

void TpSkip(CParser& p)
{
	int lvl = 0;
	for(;;) {
		if(lvl == 0 && (p.IsChar(',') || p.IsChar('>')) || p.IsEof())
			break;
		if(p.Char('<'))
			lvl++;
		else
		if(p.Char('>'))
			lvl--;
		else
			p.SkipTerm();
	}
}

String Subst(const String& s, const Vector<String>& tpar)
{
	if(tpar.GetCount() == 0)
		return s;
	String r;
	CParser p(s);
	while(!p.IsEof()) {
		if(p.IsId()) {
			String id = p.ReadId();
			int q = FindIndex(tpar, id);
			if(q >= 0)
				r << AsString(q);
			else
				r << id;
		}
		else
			r << p.GetChar();
	}
	return r;
}

bool Parser::Scope(const String& tp, const String& tn) {
	if(Key(tk_namespace)) {
		Check(lex.IsId(), "Expected name of namespace");
		String name = lex.GetId();
		if(Key('{')) {
			Context cc;
			cc <<= context;
			ScopeBody();
			context <<= cc;
		}
		Key(';');
		SetScopeCurrent();
		return true;
	}
	if((lex == tk_class || lex == tk_struct || lex == tk_union) && lex[1] != '{') {
		int t = lex.GetCode();
		context.typenames.FindAdd(lex);
		Context cc;
		cc <<= context;
		CParser p(tp);
		Vector<String> tpar;
		if(p.Char('<')) {
			while(!p.IsEof() && !p.Char('>')) {
				if((p.Id("class") || p.Id("typename") || p.Id("struct")) && p.IsId()) {
					tpar.Add(p.ReadId());
					context.tparam.Add(lex.Id(tpar.Top()));
				}
				else
					context.tparam.Add(0);
				TpSkip(p);
				p.Char(',');
			}
		}
		if(Key(t_dblcolon))
			context.scope = Null;
		String name;
		do {
			Check(lex.IsId(), "Missing identifier");
			context.typenames.FindAdd(lex);
			name = lex.GetId();
			ScopeCat(context.scope, name);
		}
		while(Key(t_dblcolon));
		context.access = t == tk_class ? PRIVATE : PUBLIC;
		if(tn.GetCount()) {
			if(context.ctname.GetCount())
				context.ctname << ';';
			context.ctname << tn;
		}
		String nn;
		if(!tp.IsEmpty())
			nn = "template " + tp + " ";
		String key = (t == tk_class ? "class" : t == tk_union ? "union" : "struct");
		nn << key << ' ' << name;
		CppItem& im = Item(context.scope, key, name, lex != ';');
		im.kind = tp.IsEmpty() ? STRUCT : STRUCTTEMPLATE;
		im.type = name;
		im.access = cc.access;
		im.tname = tn;
		im.ctname = context.ctname;
		im.tparam = CleanTp(tp);
		im.ptype.Clear();
		im.pname.Clear();
		im.param.Clear();
		if(Key(';')) {
			context = cc;
			im.natural = Gpurify(nn);
			SetScopeCurrent();
			return true;
		}
		if(Key(':')) {
			nn << " : ";
			bool c = false;
			do {
				String access = t == tk_class ? "private" : "public";
				if(Key(tk_public)) access = "public";
				else
				if(Key(tk_protected)) access = "protected";
				else
				if(Key(tk_private)) access = "private";
				if(Key(tk_virtual)) access << " virtual";
				String h;
				bool dummy;
				String n = Name(h, dummy);
				ScAdd(im.pname, h);
				if(c)
					im.ptype << ';';
				im.ptype << Subst(n, tpar);
				ScAdd(im.param, access + ' ' + n);
				if(c)
					nn << ", ";
				nn << access + ' ' + n;
				c = true;
			}
			while(Key(','));
		}
		if(Key('{')) {
			ScopeBody();
			im.natural = Gpurify(nn);
			im.decla = true;
		}
		else
			if(IsNull(im.natural))
				im.natural = Gpurify(nn);
		context = cc;
		CheckKey(';');
		SetScopeCurrent();
		return true;
	}
	return false;
}

String DeTemp(const char *s)
{
	String r;
	return r;
}

CppItem& Parser::Fn(const Decl& d, const String& templ, bool body,
                    const String& tname, const String& tparam)
{
	String param;
	String pname;
	String ptype;
	for(int i = 0; i < d.param.GetCount(); i++) {
		const Decla& p = d.param[i];
		if(dobody) {
			Local& l = local.Add(p.name);
			l.type = p.type;
			l.isptr = p.isptr;
		}
		ScAdd(param, p.natural);
		if(i)
			ptype << ';';
		ptype << p.type;
		ScAdd(pname, p.name);
	}
	String nn0;
	String nm = d.name;
	int q;
	if(d.castoper) {
		q = d.name.ReverseFind(' ');
		q = q > 0 ? d.name.ReverseFind(':', q) : d.name.ReverseFind(':');
	}
	else
		q = d.name.ReverseFind(':');
	if(q >= 0) {
		nm = d.name.Mid(q + 1);
		if(q > 0)
			nn0 = d.name.Mid(0, q - 1);
	}
	String item = FnItem(d.natural, pname, d.name, nm);
	String scope = context.scope;
	String nn;
	const char *s = nn0;
	int l = 0;
	while(*s) {
		if(*s == '<')
			l++;
		else
		if(*s == '>')
			l--;
		else
		if(l == 0)
			nn.Cat(*s);
		s++;
	}
	s = nn;
	while(*s == ':') s++;
	if(*s)
		ScopeCat(scope, s);
	CppItem& im = Item(scope, item, nm, body);
	im.natural.Clear();
	if(!IsNull(templ)) {
		im.natural = TrimRight(Gpurify(templ)) + ' ';
		im.at = im.natural.GetLength();
	}
	im.natural << Purify(d.natural, d.name, nm);
	im.kind = templ.GetCount() ? IsNull(scope) ? FUNCTIONTEMPLATE
	                                           : d.s_static ? CLASSFUNCTIONTEMPLATE
	                                                        : INSTANCEFUNCTIONTEMPLATE
	                           : d.istructor ? (d.isdestructor ? DESTRUCTOR : CONSTRUCTOR)
	                                         : d.isfriend ? INLINEFRIEND
	                                                      : IsNull(scope) ? FUNCTION
	                                                                      : d.s_static ? CLASSFUNCTION
	                                                                                   : INSTANCEFUNCTION;
	im.param = param;
	im.pname = pname;
	im.ptype = ptype;
	im.access = context.access;
	im.virt = d.s_virtual;
	im.type = d.type;
	im.decla = true;
	im.tname = tname;
	im.tparam = tparam;
	im.ctname = context.ctname;
	LLOG("FnItem: " << scope << "::" << item << ", natural: " << im.natural
	                << ", ctname: " << im.ctname);
	return im;
}

void Parser::Do()
{
	LLOG("Do, scope: " << current_scope);
	Line();
	if(Key(tk_using)) {
		while(!Key(';') && lex != t_eof)
			++lex;
	}
	else
	if(Key(tk_extern) && lex == t_string) {
		++lex;
		++lex;
		if(Key('{')) {
			int bl = lex.GetBracesLevel();
			while(lex != '}')
				try {
					if(lex == t_eof)
						ThrowError("Unexpected end of file");
					Do();
				}
				catch(Error) {
					Resume(bl);
				}
		}
		Key(';');
	}
	else
	if(Key(tk_template)) {
		if(lex.IsId() || lex == tk_class && lex.IsId(1)) {
			Key(tk_class);
			for(;;) {
				if(lex.IsId())
					lex.GetId();
				else
				if(!Key(t_dblcolon))
					break;
			}
			TemplateParams();
			Key(';');
		}
		else {
			String tnames;
			String tparam = TemplateParams(tnames);
			if(!Scope(tparam, tnames)) {
				Array<Decl> r = Declaration(true, true);
				CppItem *functionItem = 0;
				bool body = lex == '{';
				for(int i = 0; i < r.GetCount(); i++) {
					Decl& d = r[i];
					if(!d.isfriend && d.function) {
						CppItem& m = Fn(d, "template " + tparam + ' ', body, tnames, tparam);
						functionItem = &m;
					}
				}
				if(body && functionItem && whenFnEnd) {
					symbolsOutsideFunctions.Merge( lex.FinishStatCollection() );
					lex.StartStatCollection(); // start collection of function symbols
				}
				EatBody();
				if(body && functionItem && whenFnEnd) {
					whenFnEnd(FunctionStat(current_scope, *functionItem,
					                       lex.FinishStatCollection(), maxScopeDepth));
					lex.StartStatCollection(); // start collection of orphan symbols
				}
				Key(';');
			}
			EatBody();
		}
	}
	else
	if(lex == tk_enum && (lex[1] == '{' || lex[2] == '{')) {
		++lex;
		String name;
		if(lex.IsId())
			name = lex.GetId();
		if(Key('{')) {
			for(;;) {
				Line();
				String val;
				Check(lex.IsId(), "Expected identifier");
				String id = lex.GetId();
				CppItem& im = Item(context.scope, id, id);
				im.natural = "enum ";
				if(!IsNull(name))
					im.natural << name << ' ';
				im.natural << id;
				if(Key('='))
					im.natural << " = " << Constant();
				im.kind = ENUM;
				im.access = context.access;
				Key(',');
				if(Key('}')) break;
			}
		}
		Key(';');
		SetScopeCurrent();
	}
	else
	if(Key('#')) {
		if(lex.Code() == t_string) {
			String n = lex.GetText();
			String name;
			const char *s = n;
			while(*s && iscid(*s))
				name.Cat(*s++);
			CppItem& im = Item(context.scope, n, name);
			im.kind = MACRO;
			s = strchr(n, '(');
			if(s) {
				s++;
				String p;
				for(;;) {
					if(iscid(*s))
						p.Cat(*s++);
					else {
						ScAdd(im.pname, p);
						p.Clear();
						if(*s == ')' || *s == '\0') break;
						s++;
					}
				}
			}
			im.access = context.access;
		}
	}
	else
	if(!Scope(String(), String())) {
		if(Key(tk_public)) {
			context.access = PUBLIC;
			Key(':');
		}
		else
		if(Key(tk_private)) {
			context.access = PRIVATE;
			Key(':');
		}
		else
		if(Key(tk_protected)) {
			context.access = PROTECTED;
			Key(':');
		}
		else {
			Array<Decl> r = Declaration(true, true);
			CppItem *functionItem = 0;
			bool body = lex == '{';
			for(int i = 0; i < r.GetCount(); i++) {
				Decl& d = r[i];
				if(d.function) {
					if(!d.isfriend) {
						CppItem &im = Fn(d, Null, body, String(), String());
						functionItem = &im;
					}
				}
				else {
					String h = d.natural;
					int q = h.Find('=');
					if(q >= 0)
						h = TrimRight(h.Mid(0, q));
					String scope = context.scope;
					if(d.type_def)
						ScopeCat(scope, d.name);
					CppItem& im = Item(scope, d.isfriend ? "friend class"
					                          : d.type_def ? "typedef"
					                          : d.name, d.name);
					im.natural = Purify(h);
					im.type = d.type;
					im.access = context.access;
					im.kind = d.isfriend ? FRIENDCLASS :
					          d.type_def ? TYPEDEF :
					          IsNull(scope) ? VARIABLE :
					          d.s_static ? CLASSVARIABLE : INSTANCEVARIABLE;
					if(im.IsData())
						im.isptr = d.isptr;
				}
			}
			if(body && functionItem && whenFnEnd) {
				symbolsOutsideFunctions.Merge( lex.FinishStatCollection() );
				lex.StartStatCollection(); // start collection of function symbols
			}
			EatBody();
			if(body && functionItem && whenFnEnd) {
				whenFnEnd(FunctionStat(current_scope, *functionItem,
				                       lex.FinishStatCollection(), maxScopeDepth));
				lex.StartStatCollection(); // start collection of orphan symbols
			}
			SetScopeCurrent();
			Key(';');
		}
	}
}

void Parser::Do(Stream& in, const Vector<String>& ignore, CppBase& _base, const String& fn,
                Callback2<int, const String&> _err, const Vector<String>& typenames)
{
	LLOG("= C++ Parser ==================================== " << fn);
	base = &_base;
	file = PreProcess(in);
	lex.Init(~file.text, ignore);
	err = _err;
	filei = GetCppFileIndex(fn);
	String ext = ToLower(GetFileExt(fn));
	if(ext == ".h")
		filetype = FILE_H;
	else
	if(ext == ".hpp")
		filetype = FILE_HPP;
	else
	if(ext == ".cpp")
		filetype = FILE_CPP;
	else
	if(ext == ".c")
		filetype = FILE_C;
	else
		filetype = FILE_OTHER;
	lpos = 0;
	line = 0;
	if(whenFnEnd)
		lex.StartStatCollection();

	while(lex != t_eof)
		try {
			try {
				current_scope.Clear();
				context.access = PUBLIC;
				context.typenames.Clear();
				context.tparam.Clear();
				context.scope.Clear();
				inbody = false;
				for(int i = 0; i < typenames.GetCount(); i++)
					context.typenames.Add(lex.Id(typenames[i]));
				Do();
			}
			catch(Error) {
				if(lex.IsBody()) {
					LLOG("---- Recovery to next ';'");
					Resume(lex.GetBracesLevel());
					Key(';');
				}
				else {
					LLOG("---- Recovery to file level");
					++lex;
					lex.SkipToGrounding();
					lex.ClearBracesLevel();
				}
			}
		}
		catch(Lex::Grounding) {
			LLOG("---- Grounding to file level");
			lex.ClearBracesLevel();
			lex.ClearBody();
		}
}

void Parse(Stream& s, const Vector<String>& ignore, CppBase& base, const String& fn,
           Callback2<int, const String&> _err)
{
	LTIMING("Parse");
	Parser p;
	p.Do(s, ignore, base, fn, _err);
}

END_UPP_NAMESPACE
