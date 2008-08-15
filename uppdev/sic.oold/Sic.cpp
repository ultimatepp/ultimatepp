#include <Sic/Sic.h>

void LambdaArgs(CParser& p, SLambda& l)
{
	p.PassChar('(');
	if(!p.Char(')'))
		for(;;) {
			l.ref.Add(p.Char('&'));
			l.arg.Add(p.ReadId());
			if(p.Char(')'))
				break;
			p.PassChar(',');
		}
	l.ref.Shrink();
	l.arg.Shrink();
}

String Sic::ReadName()
{
	if(Id("operator")) {
		if(Char2('+', '='))
			return "operator+=";
		else
		if(Char2('-', '='))
			return "operator-=";
		else
		if(Char2('*', '='))
			return "operator*=";
		else
		if(Char2('/', '='))
			return "operator/=";
		else
		if(Char2('%', '='))
			return "operator%=";
		else
		if(Char2('>', '<'))
			return "operator><";
		else
		if(Char2('=', '='))
			return "operator==";
		else
		if(Char2('<', '<'))
			return "operator<<";
		else
			ThrowError("invalid operator");
	}
	return ReadId();
}

SVal Sic::ReadLambda()
{
	SVal lambda;
	SLambda& l = lambda.CreateLambda();
	LambdaArgs(*this, l);
	if(!IsChar('{'))
		ThrowError("missing '{'");
	const char *t = GetPtr();
	SkipStatement();
	l.code = String(t, GetPtr());
	return lambda;
}

SVal Sic::ExecuteLambda(const String& id, SVal lambda, SVal *self, Vector<SRVal>& arg)
{
	if(!lambda.IsLambda())
		ThrowError("lambda required");
	const SLambda& l = lambda.GetLambda();
	if(l.arg.GetCount() != arg.GetCount())
		ThrowError("invalid number of arguments in call to '" + id + "'");
	if(l.escape) {
		SVal ret_val = (*l.escape)(*this, arg);
		return ret_val;
	}
	if(stack_level > 40)
		ThrowError("Stack overflow");
	Sic sub(global, l.code);
	sub.self = self;
	sub.stack_level = stack_level + 1;
	for(int i = 0; i < l.arg.GetCount(); i++) {
		Var& v = sub.var.GetAdd(l.arg[i]);
		if(l.ref[i]) {
			if(!arg[i].lvalue)
				ThrowError("l-value required for " + l.arg[i] +
				           " argument in call to  '" + id + "'");
			v.ref = arg[i].lvalue;
		}
		else
			v.var = arg[i];
	}
	sub.Run();
	return sub.return_value;
}

Sic::SRVal Sic::Term()
{
	if(IsInt())
		return ReadInt();
	if(IsString())
		return ReadString();
	if(Char('@'))
		return ReadLambda();
	SRVal v;
	if(Id("void"))
		return v;
	if(Char('{')) {
		SVal map;
		map.SetMap();
		if(!Char('}'))
			for(;;) {
				SVal v = Exp();
				if(Char(':'))
					map.Set(v) = Exp();
				else
					map.Add() = v;
				if(Char('}'))
					break;
				PassChar(',');
			}
		return map;
	}
	if(Char('(')) {
		SRVal v = Exp();
		PassChar(')');
		return v;
	}
	SVal *_self = NULL;
	bool  _global = false;
	if(Char('.')) {
		if(!self)
			ThrowError("member-access in non-member lambda");
		_self = self;
	}
	else
	if(Char(':'))
		_global = true;
	if(IsId()) {
		String id = ReadName();
		int ii;
		SVal *self = _self;
		if(!_self && !_global && var.Find(id) < 0 &&
		   IsChar('(') && (ii = global.Find(id)) >= 0 &&
		   global[ii].IsLambda())
			v = global[ii];
		else {
			if(_self)
				v.lvalue = &_self->Set(id);
			else
			if(_global)
				v.lvalue = &global.GetAdd(id);
			else {
				Var& q = var.GetAdd(id);
				if(q.ref)
					v.lvalue = q.ref;
				else
					v.lvalue = &q.var;
			}
			for(;;)
				if(Char('[')) {
					self = v.lvalue;
					if(Char(']'))
						v.lvalue = &v.lvalue->Add();
					else {
						SVal x = Exp();
						v.lvalue = &v.lvalue->Set(x);
						PassChar(']');
					}
				}
				else
				if(Char('.')) {
					self = v.lvalue;
					id = ReadId();
					v.lvalue = &v.lvalue->Set(id);
				}
				else
					break;
		}
		if(Char('(')) {
			Vector<SRVal> arg;
			if(!Char(')'))
				for(;;) {
					arg.Add(Exp());
					if(Char(')')) break;
					PassChar(',');
				}
			v = ExecuteLambda(id, v, self, arg);
		}
	}
	else
		ThrowError("invalid expression");
	return v;
}

void Escape(ArrayMap<String, SVal>& global,
            const char *function,
            SVal (*fn)(Sic& sic, Vector<Sic::SRVal>& var))
{
	CParser p(function);
	SVal& v = global.GetAdd(p.ReadId());
	SLambda& l = v.CreateLambda();
	l.escape = fn;
	LambdaArgs(p, l);
}

Sic::SRVal Sic::Unary()
{
	SRVal a;
	if(Char2('+', '+')) {
		a = Unary();
		if(!a->IsNumber())
			ThrowError("invalid type for prefix '++' operator");
		if(!a.lvalue)
			ThrowError("l-value required for prefix '++' operator");
		*a.lvalue = a->GetNumber() + 1;
	}
	else
	if(Char2('-', '-')) {
		a = Unary();
		if(!a->IsNumber())
			ThrowError("invalid type for prefix '--' operator");
		if(!a.lvalue)
			ThrowError("l-value required for prefix '--' operator");
		*a.lvalue = a->GetNumber() - 1;
	}
	else
	if(Char('-')) {
		a = Unary();
		if(a->IsNumber())
			a = -a->GetNumber();
		else
			ThrowError("incompatible types for unary '-' operator");
	}
	else
	if(Char('+')) {
		a = Unary();
		if(a->IsNumber())
			a = -a->GetNumber();
		else
			ThrowError("incompatible types for unary '+' operator");
	}
	else
	if(Char('!'))
		a = !IsTrue(Unary());
	else
	if(Char('~')) {
		a = Unary();
		if(a->IsNumber())
			a = ~(int)a->GetNumber();
		else
			ThrowError("incompatible types for unary '~' operator");
	}
	else
		a = Term();

	if(Char2('+', '+')) {
		if(!a->IsNumber())
			ThrowError("invalid type for postfix '++' operator");
		if(!a.lvalue)
			ThrowError("l-value required for postfix '++' operator");
		SRVal x = a->GetNumber();
		*a.lvalue = a->GetNumber() + 1;
		return x;
	}
	if(Char2('-', '-')) {
		if(!a->IsNumber())
			ThrowError("invalid type for postfix '--' operator");
		if(!a.lvalue)
			ThrowError("l-value required for postfix '--' operator");
		SRVal x = a->GetNumber();
		*a.lvalue = a->GetNumber() - 1;
		return x;
	}
	return a;
}

Sic::SRVal Sic::Operator(Sic::SRVal a, Sic::SRVal b, const char *oper, const char *op, bool sym)
{
	if(a->IsMap()) {
		const ArrayMap<SVal, SVal>& m = a->GetMap();
		int q = m.Find(String(oper));
		if(q >= 0 && m[q].IsLambda()) {
			Vector<SRVal> arg;
			arg.SetCount(1);
			arg[0] = b;
			ExecuteLambda(oper, m[q], a.lvalue ? a.lvalue : &a.rvalue, arg);
			return a;
		}
	}
	if(sym)
		return Operator(b, a, oper, op);
	ThrowError(String() << "incompatible types for '" << op << "' operator");
	return a;
}

Sic::SRVal Sic::Mul()
{
	SRVal a = Unary();
	for(;;)
		if(!IsChar2('*', '=') && Char('*')) {
			SRVal b = Unary();
			if(a->IsNumber() && b->IsNumber())
				a = a->GetNumber() * b->GetNumber();
			else
				a = Operator(~a, ~b, "operator*=", "*", true);
		}
		else
		if(!IsChar2('/', '=') && Char('/')) {
			SRVal b = Unary();
			if(a->IsNumber() && b->IsNumber())
				a = a->GetNumber() / b->GetNumber();
			else
				a = Operator(~a, b, "operator/=", "/");
		}
		else
		if(!IsChar2('%', '=') && Char('%')) {
			SRVal b = Unary();
			if(a->IsNumber() && b->IsNumber())
				a = (int)a->GetNumber() % (int)b->GetNumber();
			else
				a = Operator(~a, b, "operator%=", "%");
		}
		else
			return a;
}

Sic::SRVal Sic::Add()
{
	SRVal a = Sic::Mul();
	for(;;)
		if(!IsChar2('+', '=') && Char('+')) {
			SRVal b = Mul();
			if(a->IsNumber() && b->IsNumber())
				a = a->GetNumber() + b->GetNumber();
			else
			if(a->IsString() && b->IsString())
				a = a->GetString() + b->GetString();
			else
				a = Operator(~a, ~b, "operator+=", "+", true);
		}
		else
		if(!IsChar2('-', '=') && Char('-')) {
			SRVal b = Mul();
			if(a->IsNumber() && b->IsNumber())
				a = a->GetNumber() - b->GetNumber();
			else
				a = Operator(~a, b, "operator-=", "-");
		}
		else
			return a;
}

Sic::SRVal Sic::Shift()
{
	SRVal a = Add();
	for(;;)
		if(Char2('<', '<')) {
			SRVal b = Add();
			if(a->IsNumber() && b->IsNumber())
				a = (int)a->GetNumber() << (int)b->GetNumber();
			else {
				if(a->IsVoid())
					if(a.lvalue)
						*a.lvalue = String();
					else
						a.rvalue = String();
				if(a->IsString()) {
					if(b->IsString())
						a.lvalue->GetString() << b->GetString();
					else
					if(b->IsNumber())
						a.lvalue->GetString() << b->GetNumber();
					else
					if(!b->IsVoid())
						ThrowError("Not implemented");
				}
				else
					a = Operator(a, b, "operator<<", "<<", true);
			}
		}
		else
		if(Char2('>', '>')) {
			SRVal b = Add();
			if(a->IsNumber() && b->IsNumber())
				a = (int)a->GetNumber() >> (int)b->GetNumber();
			else
				ThrowError("incompatible types for '>>' operator");
		}
		else
			return a;

}

int Sic::DoCompare(const SVal& a)
{
	SVal b = Shift();
	if(a.IsNumber() && b.IsNumber())
		return a.GetNumber() - b.GetNumber();
	if(a.IsString() && b.IsString())
		return a.GetString().Compare(b.GetString());
	ThrowError("incompatible types for comparison operator");
	return 0;
}

Sic::SRVal Sic::Compare()
{
	SRVal a = Shift();
	for(;;)
		if(Char2('>', '='))
			a = DoCompare(a) >= 0;
		else
		if(Char2('<', '='))
			a = DoCompare(a) <= 0;
		else
		if(Char('>'))
			a = DoCompare(a) > 0;
		else
		if(Char('<'))
			a = DoCompare(a) < 0;
		else
			return a;
}

Sic::SRVal Sic::Equal()
{
	SRVal a = Compare();
	for(;;)
		if(Char2('=', '=')) {
			SRVal b = Compare();
			a = int(~a == ~b);
		}
		else
		if(Char2('!', '=')) {
			SRVal b = Compare();
			a = int(~a != ~b);
		}
		else
			return a;
}

Sic::SRVal Sic::BinAnd()
{
	SRVal a = Equal();
	while(!IsChar2('&', '&') && Char('&')) {
		SRVal b = Equal();
		if(a->IsNumber() && b->IsNumber())
			a = (int)a->GetNumber() & (int)b->GetNumber();
		else
			ThrowError("incompatible types for '&' operator");
	}
	return a;
}

Sic::SRVal Sic::BinXor()
{
	SRVal a = BinAnd();
	while(Char('^')) {
		SRVal b = BinAnd();
		if(a->IsNumber() && b->IsNumber())
			a = (int)a->GetNumber() ^ (int)b->GetNumber();
		else
			ThrowError("incompatible types for '^' operator");
	}
	return a;
}

Sic::SRVal Sic::BinOr()
{
	SRVal a = BinXor();
	while(!IsChar2('|', '|') && Char('|')) {
		SRVal b = BinXor();
		if(a->IsNumber() && b->IsNumber())
			a = (int)a->GetNumber() | (int)b->GetNumber();
		else
			ThrowError("incompatible types for '|' operator");
	}
	return a;
}

Sic::SRVal Sic::And()
{
	SRVal a = BinOr();
	while(Char2('&', '&')) {
		SRVal b = BinOr();
		a = IsTrue(a) && IsTrue(b);
	}
	return a;
}

Sic::SRVal Sic::Or()
{
	SRVal a = And();
	while(Char2('|', '|')) {
		SRVal b = And();
		a = IsTrue(a) || IsTrue(b);
	}
	return a;
}

Sic::SRVal Sic::Cond()
{
	SRVal a = Or();
	if(Char('?')) {
		SRVal b = Or();
		PassChar(':');
		SRVal c = Or();
		return IsTrue(a) ? b : c;
	}
	return a;
}

Sic::SRVal Sic::Assign()
{
	SRVal a = Cond();
	if(Char('=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '=' operator");
		*a.lvalue = b;
	}
	else
	if(Char2('+', '=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '+=' operator");
		if(a->IsNumber() && b->IsNumber())
			*a.lvalue = a->GetNumber() + b->GetNumber();
		else
		if(a->IsString() && b->IsString())
			a.lvalue->GetString() += b->GetString();
		else
			Operator(a, b, "operator+=", "+=");
	}
	else
	if(Char2('-', '=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '-=' operator");
		if(a->IsNumber() && b->IsNumber())
			*a.lvalue = a->GetNumber() - b->GetNumber();
		else
			Operator(a, b, "operator-=", "-=");
	}
	else
	if(Char2('*', '=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '*=' operator");
		if(a->IsNumber() && b->IsNumber())
			*a.lvalue = a->GetNumber() * b->GetNumber();
		else
			Operator(a, b, "operator*=", "*=");
	}
	else
	if(Char2('/', '=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '/=' operator");
		if(a->IsNumber() && b->IsNumber())
			*a.lvalue = a->GetNumber() / b->GetNumber();
		else
			Operator(a, b, "operator/=", "/=");
	}
	else
	if(Char2('%', '=')) {
		SRVal b = Assign();
		if(!a.lvalue)
			ThrowError("l-value required on the left side of '%=' operator");
		if(a->IsNumber() && b->IsNumber())
			*a.lvalue = (int)a->GetNumber() % (int)b->GetNumber();
		else
			Operator(a, b, "operator%=", "%=");
	}
	else
	if(Char2(':', '=')) {
		if(!a.lvalue)
			ThrowError("l-value required on the left side of ':='");
		while(Char('.')) {
			String id = ReadName();
			a.lvalue->Set(id) = ReadLambda();
		}
	}
	return a;
}

Sic::SRVal Sic::Exp()
{
	return Assign();
}

void Sic::SkipTerm()
{
	if(IsEof())
		throw CParser::Error("unexpected end of file");
	CParser::SkipTerm();
}

void Sic::SkipExp()
{
	int level = 0;
	for(;;) {
		if(IsChar(';'))
			return;
		if(IsChar(')') && level == 0)
			return;
		if(Char(')'))
			level--;
		else
		if(Char('('))
			level++;
		else
			SkipTerm();
	}
}

void Sic::SkipStatement()
{
	for(;;) {
		if(Char(';')) return;
		if(Char('{'))
			break;
		SkipTerm();
	}
	int level = 1;
	while(level > 0) {
		if(Char('{')) level++;
		else
		if(Char('}')) level--;
		else
			SkipTerm();
	}
}

bool  Sic::PCond()
{
	PassChar('(');
	bool c = IsTrue(Exp());
	PassChar(')');
	return c;
}

void Sic::FinishSwitch()
{
	while(!Char('}') && no_break && no_return) {
		if(Id("case")) {
			Exp();
			PassChar(':');
		}
		else
		if(Id("default"))
			PassChar(':');
		DoStatement();
	}
}

void  Sic::DoStatement()
{
	if(Id("if"))
		if(PCond()) {
			DoStatement();
			if(Id("else"))
				SkipStatement();
		}
		else {
			SkipStatement();
			if(Id("else"))
				DoStatement();
		}
	else
	if(Id("do")) {
		loop++;
		const char *term = GetPtr();
		do {
			SetPtr(term);
			DoStatement();
			PassId("while");
		}
		while(PCond() && no_break && no_return);
		PassChar(';');
		no_break = true;
		loop--;
	}
	else
	if(Id("while")) {
		loop++;
		const char *term = GetPtr();
		for(;;) {
			SetPtr(term);
			if(!PCond() || !no_break || !no_return) {
				SkipStatement();
				break;
			}
			DoStatement();
		}
		no_break = true;
		loop--;
	}
	else
	if(Id("for")) {
		loop++;
		PassChar('(');
		if(!Char(';')) {
			Exp();
			PassChar(';');
		}
		const char *cond = NULL;
		if(!Char(';')) {
			cond = GetPtr();
			SkipExp();
			PassChar(';');
		}
		const char *after = NULL;
		if(!Char(')')) {
			after = GetPtr();
			SkipExp();
			PassChar(')');
		}
		const char *stmt = GetPtr();
		for(;;) {
			bool c = true;
			if(cond) {
				SetPtr(cond);
				c = IsTrue(Exp());
			}
			SetPtr(stmt);
			if(!c || !no_break || !no_return) {
				SkipStatement();
				break;
			}
			DoStatement();
			if(after) {
				SetPtr(after);
				Exp()->Dump();
			}
		}
		no_break = true;
		loop--;
	}
	else
	if(Id("foreach")) {
		loop++;
		PassChar('(');
		SVal *value = Exp().lvalue;
		SVal *key = NULL;
		if(Char(':')) {
			key = value;
			value = Exp().lvalue;
		}
		if(!value || !key)
			ThrowError("l-value expected in 'foreach' statement");
		PassId("in");
		SVal c = Exp();
		if(!c.IsMap())
			ThrowError("container expected in 'foreach' statement");
		PassChar(')');
		const char *stmt = GetPtr();
		const ArrayMap<SVal, SVal>& map = c.GetMap();
		for(int i = 0; i < map.GetCount() && no_break && no_return; i++) {
			if(!map.IsUnlinked(i) && !map[i].IsVoid()) {
				if(key)
					*key = map.GetKey(i);
				*value = map[i];
				SetPtr(stmt);
				DoStatement();
			}
		}
		no_break = true;
		loop--;
	}
	else
	if(Id("break")) {
		if(!loop)
			ThrowError("misplaced 'break'");
		no_break = false;
		PassChar(';');
	}
	else
	if(Id("case"))
		ThrowError("misplaced 'case'");
	else
	if(Id("default"))
		ThrowError("misplaced 'default'");
	else
	if(Id("else"))
		ThrowError("misplaced 'else'");
	else
	if(Id("return")) {
		no_return = false;
		if(!Char(';')) {
			return_value = Exp();
			PassChar(';');
		}
		else
			return_value = Null;
	}
	else
	if(Id("switch")) {
		loop++;
		PassChar('(');
		SVal a = Exp();
		PassChar(')');
		PassChar('{');
		while(!Char('{')) {
			if(Id("case")) {
				SVal b = Exp();
				PassChar(':');
				if(a == b) {
					FinishSwitch();
					break;
				}
			}
			else
			if(Id("default")) {
				PassChar(':');
				FinishSwitch();
				break;
			}
			else
				SkipStatement();
		}
		loop--;
		no_break = true;
	}
	else
	if(Char('{')) {
		while(!Char('}') && no_break && no_return)
			DoStatement();
	}
	else
	if(!Char(';')) {
		SVal v = Exp();
		Char(';');
	}
}

void  Sic::Run()
{
	no_return = no_break = true;
	loop = 0;
	while(!IsEof() && no_return && no_break)
		DoStatement();
}

void Scan(ArrayMap<String, SVal>& global, const char *file)
{
	Sic p(global, file);
	while(!p.IsEof()) {
		SVal& v = global.GetAdd(p.ReadId());
		SLambda& l = v.CreateLambda();
		LambdaArgs(p, l);
		const char *t = p.GetPtr();
		if(!p.IsChar('{'))
			p.ThrowError("missing function body");
		p.SkipStatement();
		l.code = String(t, p.GetPtr());
	}
}

SVal Execute(ArrayMap<String, SVal>& global, const char *name, const Vector<SVal>& arg)
{
	int ii = global.Find(name);
	if(ii < 0 || !global[ii].IsLambda())
		throw CParser::Error("invalid function name");
	const SLambda& l = global[ii].GetLambda();
	if(arg.GetCount() != l.arg.GetCount())
		throw CParser::Error("invalid number of arguments");
	Sic sub(global, l.code);
	for(int i = 0; i < l.arg.GetCount(); i++) {
		Sic::Var& v = sub.var.GetAdd(l.arg[i]);
		v.var = arg[i];
		if(l.ref[i])
			v.ref = &v.var;
	}
	sub.Run();
	return sub.return_value;
}

SVal Execute(ArrayMap<String, SVal>& global, const char *name)
{
	return Execute(global, name, Vector<SVal>());
}
