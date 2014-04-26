#include "Skylark.h"

#define LLOG(x)  // DLOG(x)

namespace Upp {

String RawHtmlText::ToString() const
{
	return "RAW: " + text;
}

Value Raw(const String& s)
{
	RawHtmlText r;
	r.text = s;
	return RichToValue(r);
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

void Compiler::DoExeField(One<Exe>& result)
{
	One<Exe> r;
	ExeField& f = r.Create<ExeField>();
	f.value = pick(result);
	f.id = p.ReadId();
	result = pick(r);
}

void Compiler::DoIndicies(One<Exe>& result)
{
	for(;;) {
		if(p.Char('.'))
			DoExeField(result);
		else
		if(p.Char('[')) {
			One<Exe> r;
			ExeBracket& f = r.Create<ExeBracket>();
			f.value = pick(result);
			f.index = Exp();
			result = pick(r);
			p.PassChar(']');
		}
		else
			break;
	}	
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
	if(p.IsId() || p.IsChar('.') || p.IsChar('@')) {
		String id;
		if(p.Char('.'))
			id = ".";
		else
		if(p.Char('@'))
			id = "@";
		id << p.ReadId();
		while(p.Char(':'))
			id << ':' << p.ReadId();
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
				DoIndicies(result);
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
				DoExeField(result);
				DoIndicies(result);
			}
		}
		else {
			result.Create<ExeVar>().var_index = n;
			DoIndicies(result);
		}
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
			result = Create<ExeMul>(pick(result), Prim());
		else
		if(p.Char('/'))
			result = Create<ExeDiv>(pick(result), Prim());
		else
		if(p.Char('%'))
			result = Create<ExeMod>(pick(result), Prim());
		else
			return result;
}

One<Exe> Compiler::Add()
{
	One<Exe> result = Mul();
	for(;;)
		if(p.Char('+'))
			result = Create<ExeAdd>(pick(result), Mul());
		else
		if(p.Char('-'))
			result = Create<ExeSub>(pick(result), Mul());
		else
			return result;
}

One<Exe> Compiler::Shift()
{
	One<Exe> result = Add();
	for(;;)
		if(p.Char3('>', '>', '>'))
			result = Create<ExeSrl>(pick(result), Add());
		else
		if(p.Char2('>', '>'))
			result = Create<ExeSra>(pick(result), Add());
		else
		if(p.Char2('<', '<'))
			result = Create<ExeSll>(pick(result), Add());
		else
			return result;
}

One<Exe> Compiler::Rel()
{
	One<Exe> result = Shift();
	for(;;)
		if(p.Char2('<', '='))
			result = Create<ExeLte>(pick(result), Shift());
		else
		if(p.Char2('>', '='))
			result = Create<ExeLte>(Shift(), pick(result));
		else
		if(p.Char('<'))
			result = Create<ExeLt>(pick(result), Shift());
		else
		if(p.Char('>'))
			result = Create<ExeLt>(Shift(), pick(result));
		else
			return result;
}

One<Exe> Compiler::Eq()
{
	One<Exe> result = Rel();
	for(;;)
		if(p.Char2('=', '='))
			result = Create<ExeEq>(pick(result), Rel());
		else
		if(p.Char2('!', '='))
			result = Create<ExeNeq>(Rel(), pick(result));
		else
			return result;
}

One<Exe> Compiler::And()
{
	One<Exe> result = Eq();
	while(!p.IsChar2('&', '&') && p.Char('&'))
		result = Create<ExeAnd>(pick(result), Eq());
	return result;
}

One<Exe> Compiler::Xor()
{
	One<Exe> result = And();
	while(p.Char('^'))
		result = Create<ExeXor>(pick(result), And());
	return result;
}

One<Exe> Compiler::Or()
{
	One<Exe> result = Xor();
	while(!p.IsChar2('|', '|') && p.Char('|'))
		result = Create<ExeOr>(pick(result), Xor());
	return result;
}

One<Exe> Compiler::LogAnd()
{
	One<Exe> result = Or();
	while(p.Char2('&', '&'))
		result = Create<ExeAnl>(pick(result), Or());
	return result;
}

One<Exe> Compiler::LogOr()
{
	One<Exe> result = LogAnd();
	while(p.Char2('|', '|'))
		result = Create<ExeOrl>(pick(result), LogAnd());
	return result;
}

One<Exe> Compiler::Conditional()
{
	One<Exe> result = LogOr();
	if(p.Char('?')) {
		One<Exe> r;
		ExeCond& c = r.Create<ExeCond>();
		c.cond = pick(result);
		c.ontrue = LogOr();
		p.PassChar(':');
		c.onfalse = LogOr();
		result = pick(r);
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
		while(b < s) {
			if(*b == CParser::LINEINFO_ESC) {
				b++;
				while(b < s && *b++ != CParser::LINEINFO_ESC);
			}
			else
				t.text.Cat(*b++);
		}
		item.Add().Create<ExeConst>().value = RawToValue(t);
	}
}

One<Exe> Compiler::Block()
{
	One<Exe> result;
	ExeBlock& blk = result.Create<ExeBlock>();
	const char *b = p.GetSpacePtr();
	while(!p.IsEof()) {
		const char *s = p.GetPtr();
		if(p.Char2('$', '$')) {
			blk.AddText(b, s + 1);
			b = p.GetSpacePtr();
		}
		else
		if(p.Char('$')) {
			blk.AddText(b, s);
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
			b = p.GetSpacePtr();
		}
		else {
			// We need to advance CParser, but SkipTerm is not good, because '$' can be in quoites - "$x"
			// We also need CParser to process whitespaces to allow for correct lineinfo processing
			CParser::Pos pos = p.GetPos();
			pos.ptr++;
			const char *s = p.GetPtr();
			while((byte)*s > ' ' && *s != '$')
				s++;
			p.SetPos(pos);
		}
	}
	blk.AddText(b, p.GetPtr());
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