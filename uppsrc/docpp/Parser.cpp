#include "docpp.h"

String Purify(const char *s) {
	String res;
	while(*s) {
		if(*s >= 'a' && *s <= 'z') {
			String q;
			while(*s >= 'a' && *s <= 'z')
				q.Cat(*s++);
			if(q != "virtual" && q != "inline")
				res.Cat(q);
			else
				while((byte)*s <= ' ' && *s) s++;
		}
		else
		if((byte)*s <= ' ') {
			res.Cat(' ');
			while(*s <= ' ' && *s)
				s++;
		}
		else
			res.Cat(*s++);
	}
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
	return "Namespace: " + nameing + " Struct: " + nesting;
}

void Parser::Context::operator<<=(const Context& t)
{
	nameing = t.nameing;
	nesting = t.nesting;
	typenames <<= t.typenames;
	access = t.access;
}


Parser::Decla::Decla()
{
	function = type_def = false;
	s_static = s_auto = s_register = s_extern = s_mutable = s_explicit = s_virtual = false;
	isfriend = istemplate = istructor = isptr = nofn = false;
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
	int pos = p - ~file.text;
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
	int pos = lex.Pos() - ~file.text;
	while(line + 1 < file.linepos.GetCount() && file.linepos[line + 1] <= pos)
		line++;
}

void Parser::ThrowError(const String& e)
{
	throw Error(Format("(%d) : %s", GetLine(lex.Pos()), ~e));
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
	for(;;) {
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

String Parser::Name()
{
	String s;
	if(Key(t_dblcolon))
		s << "::";
	Check(lex.IsId(), "Name expected");
	while(lex.IsId()) {
		s << lex.GetId();
		if(lex == '<')
			s << TemplateParams();
		if(Key(t_dblcolon)) {
			s << "::";
			if(Key('~'))
				s << "~";
		}
		else
			break;
	}
	return s;
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

String Parser::SimpleType()
{
	if(Key(tk_struct) || Key(tk_class) || Key(tk_union) || Key(tk_enum) || Key(tk_typename)) {
		if(lex.IsId() || lex == t_dblcolon) Name();
		if(lex == '{') EatBody();
		return Null;
	}
	if(Key(tk_bool) || Key(tk_float) || Key(tk_double) || Key(tk_void))
		return Null;
	bool sgn = Key(tk_signed) || Key(tk_unsigned);
	if(Key(tk_long)) {
		Key(tk_int);
		return Null;
	}
	if(Key(tk_short)) {
		Key(tk_int);
		return Null;
	}
	if(Key(tk_int) || Key(tk_char) ||
	   Key(tk___int8) || Key(tk___int16) || Key(tk___int32) || Key(tk___int64)) return Null;
	if(sgn) return Null;
	const char *p = lex.Pos();
	int cs = 0;
	Index<int> cix;
	Key(t_dblcolon);
	Check(lex.IsId(), "Name expected");
	while(lex.IsId()) {
		if(cix.Find(lex) >= 0)
			cs++;
		else
			cix.Add(lex);
		++lex;
		if(lex == '<')
			TemplateParams();
		if(Key(t_dblcolon))
			Key('~');
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
	int q = 0;
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
		if(t == t_dblcolon || lex.IsId(n) || t == ',' && tlevel > 0)
			n++;
		else
			return 0;
	}
}

void Parser::EatInitializers()
{
	if(Key(':')) {
		while(lex != '{' && lex != t_eof) ++lex;
	}
}

String Parser::ReadOper() {
	const char *p = lex.Pos();
	const char *p1 = p;
	Key(tk_operator);
	int level = 0;
	if(Key('('))
		level++;
	for(;;) {
		p1 = lex.Pos();
		if(lex == t_eof) break;
		if(level <= 0 && lex == '(') break;
		if(Key('(') || Key('[')) level++;
		else
		if(Key(')')  || Key(']')) level--;
		else
			++lex;
	}
	return TrimRight(Purify(String(p, p1)));
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
	if(lex == tk_operator)
		d.name = ReadOper();
	else
	if(lex.IsId() || lex == t_dblcolon) {
		d.name = Name();
		if(Key(':'))
			if(!Key(t_integer))
				Name();
	}
	if(Key('(')) {
		if((lex < 256 || lex == tk_true || lex == tk_false) && lex != ')') {
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
			d.header = String(p, lex.Pos() - 1);
			d.function = !d.nofn;
			ParamList(d);
			p = lex.Pos();
			Qualifier();
			d.ender = String(p, lex.Pos());
		}
	}
	EatInitializers();
	while(Key('[')) {
		const char *p = lex.Pos();
		int level = 1;
		while(level && lex != t_eof) {
			if(Key('[')) level++;
			else
			if(Key(']')) level--;
			else
				++lex;
		}
	}
	if(Key('=')) {
		Constant();
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
	SimpleType();
	Declarator(d, p);
	return Finish(d, p);
}

Array<Parser::Decl> Parser::Declaration(bool l0, bool more)
{
	Array<Decl> r;
	Decla d;
	const char *p = lex.Pos();
	if(Key(tk_typedef)) {
		r = Declaration();
		r.Top().type_def = true;
		r.Top().natural = String(p, lex.Pos());
		return r;
	}
	if(Key(tk_friend))
		d.isfriend = true;
//	if(Key(tk_template)) {
//		d.istemplate = true;
//		d.template_params = TemplateParams(d.tnames);
//	}
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
		if(!Key(tk_inline))
			break;
	}
	Qualifier();
	bool isdestructor = Key('~');
	if(l0 && context.typenames.Find(lex) >= 0 && lex[1] == '(') {
		Decl& a = r.Add();
		a.name = lex.GetId();
		a.isdestructor = isdestructor;
		a.function = true;
		a.istructor = true;
		a.header = String(p, lex.Pos());
		++lex;
		ParamList(a);
		const char *p1 = lex.Pos();
		Qualifier();
		a.ender = String(p1, lex.Pos());
		a.natural = String(p, lex.Pos());
		EatInitializers();
		return r;
	}
	if(lex == tk_operator) {
		Decl& a = r.Add();
		(Decla&)a = d;
		a.name = ReadOper();
		a.header = String(p, lex.Pos());
		Key('(');
		ParamList(a);
		const char *p1 = lex.Pos();
		Qualifier();
		a.ender = String(p1, lex.Pos());
		a.function = true;
		a.natural = String(p, lex.Pos());
		return r;
	}
	String st = SimpleType();
	if(!st.IsEmpty()) {
		Decl& a = r.Add();
		a.name = st;
		a.isdestructor = st.Find('~') > 0;
		a.function = true;
		a.istructor = true;
		a.header = String(p, lex.Pos());
		if(Key('('))
			ParamList(a);
		const char *p1 = lex.Pos();
		Qualifier();
		a.ender = String(p1, lex.Pos());
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
		a.natural = natural1 + String(p1, lex.Pos());
		p = lex.Pos();
	}
	while(more && Key(','));
	return r;
}

bool Parser::EatBody()
{
	if(!Key('{')) return false;
	int level = 1;
	while(level && lex != t_eof) {
		if(Key('{')) level++;
		else
		if(Key('}')) level--;
		else
			++lex;
	}
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
			while(*s == ' ')
				s++;
		}
		else
			r.Cat(*s++);
	}
	return r;
}

bool Parser::Nest(const String& tp, const String& tn) {
	if(Key(tk_namespace)) {
		Check(lex.IsId(), "Expected name of namespace");
		String name = lex.GetId();
		if(Key('{')) {
			Context cc;
			cc <<= context;
			context.nameing << name << "::";
			Do();
			context <<= cc;
			Key('}');
		}
		Key(';');
		return true;
	}
	if((lex == tk_class || lex == tk_struct || lex == tk_union) &&
	   lex.IsId(1) &&
	   (lex[2] == ':' || lex[2] == ';' || lex[2] == '{')) {
		int t = lex.GetCode();
		context.typenames.FindAdd(lex);
		String name = lex.GetId();
		Context cc;
		cc <<= context;
		if(context.nesting.IsEmpty())
			context.nesting = name + TrimRight(tp);
		else
			context.nesting = context.nesting + "::" + name + TrimRight(tp);
		context.access = t == tk_class ? PRIVATE : PUBLIC;
		String nn;
		if(!tp.IsEmpty())
			nn = "template " + tp + " ";
		nn << (t == tk_class ? "class " : t == tk_union ? "union " : "struct ") + name;
		CppItem& im = Item(context.nameing, context.nesting, nn);
		im.kind = tp.IsEmpty() ? STRUCT : STRUCTTEMPLATE;
		im.name = name;
		im.access = cc.access;
		im.tname = tn;
		im.tparam = CleanTp(tp);
		if(Key(':')) {
			do {
				String access = t == tk_class ? "private" : "public";
				if(Key(tk_public)) access = "public";
				else
				if(Key(tk_protected)) access = "protected";
				else
				if(Key(tk_private)) access = "private";
				if(Key(tk_virtual)) access << " virtual";
				String n = Name();
				ScAdd(im.pname, n);
				ScAdd(im.param, access + ' ' + n);
			}
			while(Key(','));
		}
		if(Key('{')) {
			Do();
			CheckKey('}');
		}
		CheckKey(';');
		context = cc;
		return true;
	}
	return false;
}

CppItem& Parser::Item(const String& nameing, const String& nesting, const String& item)
{
	CppItem& im = base->GetAdd(nameing).GetAdd(nesting).GetAdd(TrimRight(Purify(item)));
	im.pname.Clear();
	im.param.Clear();
	im.package = package;
	im.file = filename;
	im.line = line + 1;
	im.name.Clear();
	return im;
}

void Parser::Do()
{
	while(lex != t_eof && lex != '}') {
		Line();
		if(Key(tk_using)) {
			while(!Key(';'))
				++lex;
		}
		else
		if(Key(tk_extern) && lex == t_string) {
			++lex;
			++lex;
			if(Key('{')) {
				Do();
				Key('}');
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
				if(!Nest(tparam, tnames)) {
					Array<Decl> r = Declaration(true, true);
					for(int i = 0; i < r.GetCount(); i++) {
						Decl& d = r[i];
						d.natural = "template" + tparam + ' ' + d.natural;
						if(context.access != PRIVATE && !d.isfriend && d.function) {
							CppItem& im = Item(context.nameing, context.nesting, d.natural);
							im.name = d.name;
							im.access = context.access;
							im.header = Purify(d.header);
							im.ender = Purify(d.ender);
							im.kind = context.nesting.IsEmpty() ? FUNCTIONTEMPLATE :
									  d.s_static ? CLASSFUNCTIONTEMPLATE
												 : INSTANCEFUNCTIONTEMPLATE;
							for(int i = 0; i < d.param.GetCount(); i++) {
								ScAdd(im.param, d.param[i].natural);
								ScAdd(im.pname, d.param[i].name);
							}
							im.tname = tnames;
							im.tparam = CleanTp(tparam);
						}
					}
					EatBody();
					Key(';');
				}
			}
		}
		else
		if(lex == tk_enum && (lex[1] == '{' || lex[2] == '{')) {
			++lex;
			String name;
			if(lex.IsId())
				name = lex.GetId();
			String param;
			String pname;
			String n = "enum " + name + " { ";
			Key('{');
			for(;;) {
				String val;
				Check(lex.IsId(), "Expected identifier");
				String nm = lex.GetId();
				if(Key('='))
					val = Constant();
				if(!param.IsEmpty())
					n << ", ";
				n << nm;
				ScAdd(param, nm + " = " + val);
				ScAdd(pname, nm);
				Key(',');
				if(Key('}')) break;
			}
			n << " }";
			CppItem& im = Item(context.nameing, context.nesting, n);
			im.kind = ENUM;
			im.name = name;
			im.access = context.access;
			im.param = param;
			im.pname = pname;
			if(lex.IsId())
				im.name = lex.GetId();
			CheckKey(';');
		}
		else
		if(Key('#')) {
			String n = lex.GetText();
			CppItem& im = Item(context.nameing, context.nesting, n);
			im.kind = MACRO;
			im.name.Clear();
			const char *s = n;
			while(*s && iscid(*s))
				im.name.Cat(*s++);
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
		else
		if(!Nest(String(), String())) {
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
				const char *p = lex.Pos();
				Array<Decl> r = Declaration(true, true);
				bool body = EatBody();
				for(int i = 0; i < r.GetCount(); i++) {
					Decl& d = r[i];
					if(context.access != PRIVATE && !d.isfriend || d.isfriend && body) {
						CppItem& im = Item(context.nameing, context.nesting, d.natural);
						im.name = d.name;
						im.header = Purify(d.header);
						im.ender = Purify(d.ender);
						im.access = context.access;
						if(d.function) {
							im.kind = d.istructor ? (d.isdestructor ? DESTRUCTOR : CONSTRUCTOR) :
							          d.isfriend ? INLINEFRIEND :
							          context.nesting.IsEmpty() ? FUNCTION :
							          d.s_static ? CLASSFUNCTION : INSTANCEFUNCTION;
							for(int i = 0; i < d.param.GetCount(); i++) {
								ScAdd(im.param, d.param[i].natural);
								ScAdd(im.pname, d.param[i].name);
							}
						}
						else
							im.kind = d.type_def ? TYPEDEF :
							          context.nesting.IsEmpty() ? VARIABLE :
							          d.s_static ? CLASSVARIABLE : INSTANCEVARIABLE;
					}
				}
				EatBody();
				Key(';');
			}
		}
	}
}

void Parser::Do(Stream& in, const Vector<String>& ignore, CppBase& _base,
                const String& p, const String& f) throw(Parser::Error)
{
	base = &_base;
	file = PreProcess(in);
	lex = ~file.text;
	lex.Ignore(ignore);
	context.nesting.Clear();
	context.nameing = "::";
	context.access = PUBLIC;
	context.typenames.Clear();
	package = p;
	filename = f;
	lpos = 0;
	line = 0;
	Do();
}

void Parse(Stream& s, const Vector<String>& ignore, CppBase& base,
           const String& package, const String& file) throw(Parser::Error)
{
	Parser p;
	p.Do(s, ignore, base, package, file);
}
