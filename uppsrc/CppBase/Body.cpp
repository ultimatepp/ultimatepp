#include "CppBase.h"

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Parser::Locals(const String& type)
{
	Line();
	Array<Parser::Decl> d = Declaration(true, true, Null, Null);
	for(int i = 0; i < d.GetCount(); i++) {
		Local& l = local.Add(d[i].name);
		l.type = *type == '*' ? d[i].type : type;
		l.isptr = d[i].isptr;
		l.line = line + 1;
		LLOG("== Local variable " << d[i].name << ": " << l.type);
	}
}

bool Parser::TryDecl()
{ // attempt to interpret code as local variable declaration
	for(;;) {
		if(lex[0] == tk_static || lex[0] == tk_const ||
	       lex[0] == tk_register || lex[0] == tk_volatile)
	    	++lex;
		else
		if(!VCAttribute())
			break;
	}
	int t = lex[0];
	int q = 0;
	if(t == tk_int || t == tk_bool || t == tk_float || t == tk_double || t == tk_void ||
	   t == tk_long || t == tk_signed || t == tk_unsigned || t == tk_short ||
	   t == tk_char || t == tk___int8 || t == tk___int16 || t == tk___int32 || t == tk___int64 ||
	   t == tk_auto) {
	    q++;
		while(lex[q] == '*' || lex[q] == '&' || lex[q] == t_and || lex[q] == tk_const) // t_and is r-value here
			q++;
		if(!lex.IsId(q))
			return false;
		static String aut("*"), empty;
		Locals(t == tk_auto ? aut : empty);
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
	while(lex[q] == '*' || lex[q] == '&' || lex[q] == t_and) // t_and is r-value here
		q++;
	if(!lex.IsId(q))
		return false;
	type = Qualify(*base, current_scope, type, context.namespace_using);
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
		Context cc;
		cc <<= context;
		int l = local.GetCount();
		while(!Key('}')) {
			if(lex == t_eof)
				ThrowError("eof");
			Statement();
		}
		context <<= cc;
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
	if(UsingNamespace())
		;
	else
	if(TryDecl())
		Key(';');
	else
		for(;;) {
			if(lex == t_eof)
				ThrowError("");
			TryLambda();
			if(Key(';') || lex == '{' || lex == '}' || lex >= tk_if && lex <= tk_do)
				break;
			++lex;
		}
}

bool Parser::Skipars(int& q)
{
	int par = lex[q];
	q++;
	int lvl = 1;
	while(lex[q] != t_eof) {
		int c = lex[q++];
		if(c == '(' || c == '[' || c == '{')
			lvl++;
		else
		if(c == ')' || c == ']' || c == '}') {
			lvl--;
			if(lvl == 0)
				return c == decode(par, '(', ')', '[', ']', '{', '}', 0);
		}
	}
	return false;
}

void Parser::TryLambda()
{
	if(lex != '[' || !dobody)
		return;
	int q = 0;
	if(!Skipars(q))
		return;
	int params = q;
	if(lex[q] == '(')
		if(!Skipars(q))
			return;
	if(lex[q] == tk_mutable)
		q++;
	if(lex[q] == t_arrow) {
		q++; // TODO: auto declaration could assign a type here
		for(;;) {
			if(lex[q] == ';' || lex[q] == t_eof)
				return;
			if(lex[q] == '{')
				break;
			q++;
		}
	}
	if(lex[q] == '{') {
		lex.Get(params);
		if(Key('(')) {
			Decl d;
			Line();
			ParamList(d);
			for(int i = 0; i < d.param.GetCount(); i++) {
				const Decla& p = d.param[i];
				if(dobody) {
					Local& l = local.Add(p.name);
					l.type = p.type;
					l.isptr = p.isptr;
					l.line = line + 1;
				}
			}
		}
		while(lex != '{' && lex != t_eof)
			++lex;
		Statement();
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

String Parser::ResolveAutoType()
{
	Vector<String> xp = MakeXP(lex.Pos());
	if(lex == ':') // resolve for declaration, like 'for(auto i: vector)'
		xp << "." << "begin" << "()" << "->"; // incorrect, should rather use operator*(), but sufficient for now
	Index<String> s = GetExpressionType(*base, *this, xp);
	int i = FindMax(s); // Ugly hack: we are not resolving overloading at all, so just choose stable type if there are more, not Null
	return i < 0 ? String() : s[i];
}
	
END_UPP_NAMESPACE
