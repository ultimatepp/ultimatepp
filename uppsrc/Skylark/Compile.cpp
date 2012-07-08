#include "Skylark.h"

#define LLOG(x)  // DLOG(x)

namespace Upp {

Value Raw(const String& s)
{
	RawHtmlText r;
	r.text = s;
	return RawToValue(r);
}

VectorMap<String, Value (*)(const Vector<Value>&, const Renderer *)>& Compiler::functions()
{
	static VectorMap<String, Value (*)(const Vector<Value>&, const Renderer *)> x;
	return x;
}

void Compiler::Register(const String& id, Value (*fn)(const Vector<Value>&, const Renderer *))
{
	functions().GetAdd(id) = fn;
}

int Compiler::ForVar(String id, int i)
{
	if(i + 1 < var.GetCount() && forvar[i])
		return i + 1;
	p.ThrowError(id + " is not 'for' iterator");
	return 0;
}

int CountLinkArgs(const Vector<String>& part)
{
	int args = 0;
	for(int i = 0; i < part.GetCount(); i++) {
		int p = (byte)*part[i];
		if(p >= 0 && p < 30)
			args = max(args, p + 1);
	}
	return args;
}

One<Exe> Compiler::Prim()
{
	One<Exe> result;
	if(p.Char('!'))
		result = Create<ExeNot>(Prim());
	else
	if(p.Char('-'))
		result = Create<ExeNeg>(Prim());
	else
	if(p.Char('+'))
		result = Prim();
	else
	if(p.IsId()) {
		String id = p.ReadId();
		int n = var.Find(id);
		if(p.Char('(')) {
			Value (*f)(const Vector<Value>&, const Renderer *) = functions().Get(id, NULL);
			if(!f) {
				Vector<String> *part = GetUrlViewLinkParts(id);
				if(!part)
					p.ThrowError("function nor link not found '" + id + "'");
				ExeLink& ln = result.Create<ExeLink>();
				ln.part = part;
				if(!p.Char(')')) {
					do
						ln.arg.AddPick(Exp());
					while(p.Char(','));
					p.PassChar(')');
				}
				if(CountLinkArgs(*part) > ln.arg.GetCount())
					p.ThrowError("invalid number of link arguments '" + id + "'");
			}
			else {
				ExeFn& fn = result.Create<ExeFn>();
				fn.fn = f;
				if(!p.Char(')')) {
					do
						fn.arg.AddPick(Exp());
					while(p.Char(','));
					p.PassChar(')');
				}
				while(p.Char('.')) {
					One<Exe> r;
					ExeField& f = r.Create<ExeField>();
					f.value = result;
					f.id = p.ReadId();
					result = r;
				}
			}
			return result;
		}
		if(n < 0) {
			Vector<String> *part = GetUrlViewLinkParts(id);
			ExeConst& c = result.Create<ExeConst>();
			if(!part) {
				return result;
			}
			String l = "\"/";
			for(int i = 0; i < (*part).GetCount(); i++) {
				if(i)
					l << '/';
				l << UrlEncode((*part)[i]);
			}
			l << '\"';
			c.value = Raw(l);
		}
		else
		if(p.Char('.')) {
			if(p.Id("_first"))
				result.Create<ExeFirst>().var_index = ForVar(id, n);
			else
			if(p.Id("_last"))
				result.Create<ExeLast>().var_index = ForVar(id, n);
			else
			if(p.Id("_index"))
				result.Create<ExeIndex>().var_index = ForVar(id, n);
			else
			if(p.Id("_key"))
				result.Create<ExeKey>().var_index = ForVar(id, n);
			else {
				result.Create<ExeVar>().var_index = n;
				do {
					One<Exe> r;
					ExeField& f = r.Create<ExeField>();
					f.value = result;
					f.id = p.ReadId();
					result = r;
				}
				while(p.Char('.'));
			}
		}
		else
			result.Create<ExeVar>().var_index = n;
	}
	else
	if(p.Char('{')) {
		ExeMap& m = result.Create<ExeMap>();
		do {
			m.key.AddPick(Exp());
			p.PassChar(':');
			m.value.AddPick(Exp());
		}
		while(p.Char(','));
		p.PassChar('}');
	}
	else
	if(p.Char('[')) {
		ExeArray& m = result.Create<ExeArray>();
		do {
			m.item.AddPick(Exp());
		}
		while(p.Char(','));
		p.PassChar(']');
	}
	else
	if(p.Char('(')) {
		result = Exp();
		p.PassChar(')');
	}
	else {
		ExeConst& c = result.Create<ExeConst>();
		if(p.Char2('0', 'x') || p.Char2('0', 'X'))
			c.value = (int)p.ReadNumber(16);
		else
		if(p.Char('0'))
			c.value = int(p.IsNumber() ? p.ReadNumber(8) : 0);
		else
			c.value = p.IsString() ? Value(p.ReadString()) : Value(p.ReadDouble());
	}
	return result;
}

One<Exe> Compiler::Mul()
{
	One<Exe> result = Prim();
	for(;;)
		if(p.Char('*'))
			result = Create<ExeMul>(result, Prim());
		else
		if(p.Char('/'))
			result = Create<ExeDiv>(result, Prim());
		else
		if(p.Char('%'))
			result = Create<ExeMod>(result, Prim());
		else
			return result;
}

One<Exe> Compiler::Add()
{
	One<Exe> result = Mul();
	for(;;)
		if(p.Char('+'))
			result = Create<ExeAdd>(result, Mul());
		else
		if(p.Char('-'))
			result = Create<ExeSub>(result, Mul());
		else
			return result;
}

One<Exe> Compiler::Shift()
{
	One<Exe> result = Add();
	for(;;)
		if(p.Char3('>', '>', '>'))
			result = Create<ExeSrl>(result, Add());
		else
		if(p.Char2('>', '>'))
			result = Create<ExeSra>(result, Add());
		else
		if(p.Char2('<', '<'))
			result = Create<ExeSll>(result, Add());
		else
			return result;
}

One<Exe> Compiler::Rel()
{
	One<Exe> result = Shift();
	for(;;)
		if(p.Char2('<', '='))
			result = Create<ExeLte>(result, Shift());
		else
		if(p.Char2('>', '='))
			result = Create<ExeLte>(Shift(), result);
		else
		if(p.Char('<'))
			result = Create<ExeLt>(result, Shift());
		else
		if(p.Char('>'))
			result = Create<ExeLt>(Shift(), result);
		else
			return result;
}

One<Exe> Compiler::Eq()
{
	One<Exe> result = Rel();
	for(;;)
		if(p.Char2('=', '='))
			result = Create<ExeEq>(result, Rel());
		else
		if(p.Char2('!', '='))
			result = Create<ExeNeq>(Rel(), result);
		else
			return result;
}

One<Exe> Compiler::And()
{
	One<Exe> result = Eq();
	while(!p.IsChar2('&', '&') && p.Char('&'))
		result = Create<ExeAnd>(result, Eq());
	return result;
}

One<Exe> Compiler::Xor()
{
	One<Exe> result = And();
	while(p.Char('^'))
		result = Create<ExeXor>(result, And());
	return result;
}

One<Exe> Compiler::Or()
{
	One<Exe> result = Xor();
	while(!p.IsChar2('|', '|') && p.Char('|'))
		result = Create<ExeOr>(result, Xor());
	return result;
}

One<Exe> Compiler::LogAnd()
{
	One<Exe> result = Or();
	while(p.Char2('&', '&'))
		result = Create<ExeAnl>(result, Or());
	return result;
}

One<Exe> Compiler::LogOr()
{
	One<Exe> result = LogAnd();
	while(p.Char2('|', '|'))
		result = Create<ExeOrl>(result, LogAnd());
	return result;
}

One<Exe> Compiler::Conditional()
{
	One<Exe> result = LogOr();
	if(p.Char('?')) {
		One<Exe> r;
		ExeCond& c = r.Create<ExeCond>();
		c.cond = result;
		c.ontrue = LogOr();
		p.PassChar(':');
		c.onfalse = LogOr();
		result = r;
	}
	return result;
}

One<Exe> Compiler::Exp()
{
	return Conditional();
}

void Compiler::ExeBlock::AddText(const char *b, const char *s)
{
	if(s > b) {
		RawHtmlText t;
		t.text = String(b, s);
		item.Add().Create<ExeConst>().value = RawToValue(t);
	}
}

One<Exe> Compiler::Block()
{
	One<Exe> result;
	ExeBlock& blk = result.Create<ExeBlock>();
	const char *s = p.GetSpacePtr();
	const char *b = s;
	int line = 1;
	while(*s) {
		if(*s == '$') {
			if(s[1] == '$')
				s += 2;
			else {
				blk.AddText(b, s);
				p.Set(s + 1, NULL, line);
				if(p.Id("if")) {
					ExeCond& c = blk.item.Add().Create<ExeCond>();
					p.PassChar('(');
					c.cond = Exp();
					p.PassChar(')');
					c.ontrue = Block();
					if(p.Id("else"))
						c.onfalse = Block();
					if(!p.Char('/'))
						p.PassId("endif");
				}
				else
				if(p.Id("for")) {
					ExeFor& c = blk.item.Add().Create<ExeFor>();
					p.PassChar('(');
					int q = var.GetCount();
					var.Add(p.ReadId());
					var.Add(Null); // LoopInfo placeholder
					forvar.Add(true);
					forvar.Add(true);
					p.PassId("in");
					c.value = Exp();
					p.PassChar(')');
					c.body = Block();
					var.Trim(q);
					forvar.SetCount(q);
					if(p.Id("else"))
						c.onempty = Block();
					if(!p.Char('/'))
						p.PassId("endfor");
				}
				else
				if(p.IsId("else") || p.IsId("endif") || p.IsId("endfor") || p.IsChar('/'))
					return result;
				else
					blk.item.AddPick(Prim());
				b = s = p.GetSpacePtr();
				line = p.GetLine();
			}
		}
		else
		if(*s++ == '\n')
			line++;
	}
	blk.AddText(b, s);
	p.Set(s, NULL, line);
	return result;
}

One<Exe> Compile(const char *code, const Index<String>& vars)
{
	Compiler c(code, vars);
	One<Exe> exe = c.Block();
	LLOG("Before optimization node count: " << c.GetNodeCount(exe));
	c.Optimize(exe);
	LLOG("After optimization node count: " << c.GetNodeCount(exe));
	return exe;
}

};