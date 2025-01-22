#include <Esc/Esc.h>


namespace Upp {

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

#define LTIMING(x)  // RTIMING(x)

force_inline void Esc::Limit(int64 count)
{
	LTIMING("Limit");
	if(count > op_limit)
		ThrowError("out of operations limit");
	op_limit -= count;
}

EscValue Esc::Get(const SRVal& val)
{
	LTIMING("Get");
	if(skipexp)
		return (int64)1;
	EscValue v = val.lval ? *val.lval : val.rval;
	for(const auto& ss : val.subscript) {
		if(v.IsMap() && ss.slice == 0)
			v = v.MapGet(ss.i1);
		else
		if(v.IsArray()) {
			int count = v.GetCount();
			if(ss.slice) {
				int i = ss.i1.GetInt();
				int n = count - i;
				if(ss.slice == 1)
					n = ss.i2.IsInt() ? ss.i2.GetInt() : n;
				else {
					if(ss.i2.IsInt()) {
						n = ss.i2.GetInt();
						if(n < 0)
							n += count;
						n -= i;
					}
				}
				if(i >= 0 && n >= 0 && i + n <= count)
					v = v.ArrayGet(i, n);
				else
					ThrowError("slice out of range");
			}
			else {
				int64 i = Int(ss.i1, "index");
				if(i < 0)
					i += count;
				if(i >= 0 && i < count)
					v = v.ArrayGet((int)i);
				else
					ThrowError("index out of range");
			}
		}
		else
			ThrowError("invalid indirection");
	}
	return v;
}

void Esc::Assign(EscValue& val, const Vector<SRVal::Subscript>& subscript, int si, const EscValue& src)
{
	LTIMING("Assign with subscript");
	const auto& ss = subscript[si++];
	if(val.IsVoid())
		val.SetEmptyMap();
	if(val.IsMap()) {
		if(ss.slice)
			ThrowError("invalid indirection");
		if(si < subscript.GetCount()) {
			EscValue x = val.MapGet(ss.i1);
			val.MapSet(ss.i1, 0.0);
			Assign(x, subscript, si, src);
			val.MapSet(ss.i1, x);
		}
		else
			val.MapSet(ss.i1, src);
		return;
	}
	else
	if(val.IsArray()) {
		if(si < subscript.GetCount()) {
			if(ss.slice)
				ThrowError("slice must be the last subscript");
			int64 i = Int(ss.i1, "index");
			if(i >= 0 && i < val.GetCount()) {
				EscValue x = val.ArrayGet((int)i);
				val.ArraySet((int)i, 0.0);
				Assign(x, subscript, si, src);
				val.ArraySet((int)i, x);
				return;
			}
		}
		else {
			int count = val.GetCount();
			if(ss.slice) {
				if(!src.IsArray())
					ThrowError("only array can be assigned to the slice");
				Limit(src.GetCount());
				int i = ss.i1.IsInt() ? ss.i1.GetInt() : 0;
				int n = count - i;
				if(ss.slice == 1)
					n = ss.i2.IsInt() ? ss.i2.GetInt() : n;
				else {
					if(ss.i2.IsInt()) {
						n = ss.i2.GetInt();
						if(n < 0)
							n += count;
						n -= i;
					}
				}
				if(i >= 0 && n >= 0 && i + n <= count) {
					val.Replace(i, n, src);
					return;
				}
				else
					ThrowError("slice out of range");
			}
			else {
				int64 i = ss.i1.IsVoid() ? val.GetCount() : Int(ss.i1, "index");
				if(i < 0)
					i = count + i;
				if(i >= 0 && i < INT_MAX) {
					Limit(max(i - val.GetCount(), (int64)0));
					val.ArraySet((int)i, src);
					return;
				}
			}
		}
	}
	ThrowError("invalid indirection");
}

void Esc::Assign(const SRVal& val, const EscValue& src)
{
	if(skipexp)
		return;
	if(!val.lval)
		ThrowError("l-value required");
	if(val.subscript.GetCount())
		Assign(*val.lval, val.subscript, 0, src);
	else
		*val.lval = src;
}

EscValue Esc::ExecuteLambda(const String& id, EscValue lambda, SRVal self, Vector<SRVal>& arg)
{
	LTIMING("ExecuteLambda");
	if(!lambda.IsLambda())
		ThrowError(Format("'%s' is not a lambda", id));
	const EscLambda& l = lambda.GetLambda();
	if(!l.varargs && arg.GetCount() > l.arg.GetCount()
	   || arg.GetCount() < l.arg.GetCount() - l.def.GetCount())
		ThrowError("invalid number of arguments in call to '" + id + "'");
	Esc sub(global, l.code, op_limit, l.filename, l.line);
	sub.self = Get(self);
	for(int i = 0; i < l.arg.GetCount(); i++) {
		sub.var.GetAdd(l.arg[i]) =
			i < arg.GetCount() ? Get(arg[i])
		                       : Evaluatexl(l.def[i - (l.arg.GetCount() - l.def.GetCount())], global, op_limit);
		Limit();
	}
	EscValue retval;
	Array<EscValue> argvar;
	if(l.escape) {
		argvar = sub.var.PickValues();
		for(int i = l.arg.GetCount(); i < arg.GetCount(); i++) {
			argvar.Add(Get(arg[i]));
		}
		EscValue v = Get(self);
		EscEscape e(*this, v, argvar);
		e.id = id;
		l.escape(e);
		retval = e.ret_val;
		self = e.self;
	}
	else {
		if(l.varargs) {
			EscValue& argv = sub.var.GetAdd("argv");
			argv.SetEmptyArray();
			for(int i = l.arg.GetCount(); i < arg.GetCount(); i++)
				argv.ArrayAdd(Get(arg[i]));
		}
		sub.Run();
		retval = sub.return_value;
		argvar = sub.var.PickValues();
	}
	for(int i = 0; i < l.inout.GetCount(); i++)
		if(l.inout[i] && i < arg.GetCount() && arg[i].lval)
			Assign(arg[i], argvar[i]);
	if(self.lval)
		Assign(self, sub.self);
	return retval;
}

void Esc::Subscript(Esc::SRVal& r, Esc::SRVal _self, String id)
{
	LTIMING("Subscript");
	for(;;) {
		if(Char('[')) {
			auto& ss = r.subscript.Add();
			if(!IsChar(',') && !IsChar(':') && !IsChar(']'))
				ss.i1 = GetExp(); // otherwise void EscValue
			if(Char(',')) {
				ss.slice = 1;
				if(!IsChar(']'))
					ss.i2 = GetExp(); // otherwise void EscValue
			}
			else
			if(Char(':')) {
				ss.slice = 2;
				if(!IsChar(']'))
					ss.i2 = GetExp(); // otherwise void EscValue
			}
			PassChar(']');
		}
		else
		if(Char('.')) {
			LTIMING("ID");
			_self = r;
			r.subscript.Add().i1 = id = ReadId();
		}
		else
		if(Char('(')) {
			LTIMING("call fn");
			Vector<SRVal> arg;
			if(!Char(')'))
				for(;;) {
					LTIMING("make args");
					Exp(arg.Add());
					if(Char(')')) break;
					PassChar(',');
				}
			if(!IsChar2('!', '=') && Char('!')) {
				Term(_self);
				EscValue g = Get(_self);
				if(!_self.lval || (!g.IsVoid() && !g.IsMap()))
					ThrowError("l-value map or l-value void expected on the right side of !");
				if(g.IsVoid()) {
					EscValue v;
					v.SetEmptyMap();
					Assign(_self, v);
				}
			}
			if(!skipexp)
				try {
					r = ExecuteLambda(id, Get(r), _self, arg);
				}
				catch(Exc e) {
					throw Error(Format("%s.%s(): %s", Get(r).GetTypeName(), id, e));
				}
		}
		else
			return;
	}
}

void Esc::Subscript(Esc::SRVal& r)
{
	Subscript(r, SRVal(), String());
}

void Esc::Term(SRVal& r)
{
	r.subscript.Clear();

	if(Char2('0', 'x') || Char2('0', 'X')) {
		r = ReadNumber64(16);
		return;
	}
	if(Char2('0', 'b') || Char2('0', 'B')) {
		r = ReadNumber64(2);
		return;
	}
	if(IsChar2('0', '.')) {
		r = ReadDouble();
		return;
	}
	if(Char('0')) {
		r = IsNumber() ? ReadNumber64(8) : 0;
		return;
	}
	if(IsNumber()) {
		Pos p = GetPos();
		r = ReadInt64();
		if(Char('.')) {
			SetPos(p);
			r = ReadDouble();
		}
		return;
	}
	if(IsString()) {
		r = EscValue(ToUtf32(ReadString()));
		return;
	}
	if(IsChar('\'')) {
		WString s = ToUtf32(ReadString('\''));
		if(s.GetLength() != 1)
			ThrowError("invalid character literal");
		r = (int64)s[0];
		return;
	}
	if(Char('@')) {
		r = ReadLambda(*this);
		Subscript(r);
		return;
	}
	if(Id("void")) {
		r = EscValue();
		return;
	}
	if(Char('{')) {
		EscValue map;
		map.SetEmptyMap();
		if(!Char('}'))
			for(;;) {
				EscValue v = GetExp();
				PassChar(':');
				map.MapSet(v, GetExp());
				if(Char('}'))
					break;
				PassChar(',');
			}
		r = map;
		Subscript(r);
		return;
	}
	if(Char('[')) {
		EscValue array;
		array.SetEmptyArray();
		if(!Char(']'))
			for(;;) {
				array.ArrayAdd(GetExp());
				if(Char(']'))
					break;
				PassChar(',');
			}
		r = array;
		Subscript(r);
		return;
	}
	if(Char('(')) {
		Exp(r);
		PassChar(')');
		Subscript(r);
		return;
	}

	SRVal _self;
	bool  _global = false;
	if(Char('.')) {
		if(!self.IsMap())
			ThrowError("member-access in non-member code");
		_self.lval = &self;
	}
	else
	if(Char(':'))
		_global = true;
	if(IsId()) {
		String id = ReadId();
		EscValue method;
		int locali = var.Find(id);
		int ii;

		if(id == "self") {
			if(!self.IsMap())
				ThrowError("self in non-member code");
			_self.lval = &self;
			r = self;
		}
		else
		if(!_self.lval && !_global && locali < 0 && IsChar('(') &&
		   self.IsMap() && (method = self.MapGet(id)).IsLambda()) {
		    _self.lval = &self;
			r = method;
		}
		else
		if(!_self.lval && !_global && locali < 0 && IsChar('(') &&
		   (ii = global.Find(id)) >= 0 && global[ii].IsLambda()) {
			r = global[ii];
		}
		else
		if(_self.lval) {
			r = _self;
			r.subscript.Add().i1 = id;
		}
		else
		if(_global)
			r.lval = &global.GetPut(id);
		else
			r.lval = &var.GetPut(id);

		try {
			Subscript(r, _self, id);
		}
		catch(CParser::Error e) {
			throw CParser::Error(id + ": " + e);
		}
	}
	else
		ThrowError("invalid expression");
}

String Lims(const String& s)
{
	return s.GetLength() > 80 ? s.Mid(0, 80) : s;
}

double Esc::Number(const EscValue& a, const char *oper)
{
	if(!a.IsNumber())
		ThrowError(String().Cat() << "number expected for '" << oper << "', encountered " << Lims(a.ToString()));
	return a.GetNumber();
}

int64 Esc::Int(const EscValue& a, const char *oper)
{
	if(!a.IsNumber())
		ThrowError(String().Cat() << "integer expected for '" << oper << "', encountered " << Lims(a.ToString()));
	return a.GetInt64();
}

double Esc::Number(const Esc::SRVal& a, const char *oper)
{
	return Number(Get(a), oper);
}

int64 Esc::Int(const Esc::SRVal& a, const char *oper)
{
	return Int(Get(a), oper);
}

void Esc::Unary(Esc::SRVal& r)
{
	if(Char2('+', '+')) {
		Unary(r);
		EscValue v = Get(r);
		if(v.IsInt64())
			Assign(r, Int(v, "++") + 1);
		else
			Assign(r, Number(v, "++") + 1);
	}
	else
	if(Char2('-', '-')) {
		Unary(r);
		EscValue v = Get(r);
		if(v.IsInt64())
			Assign(r, Int(v, "--") - 1);
		else
			Assign(r, Number(v, "--") - 1);
	}
	else
	if(Char('-')) {
		Unary(r);
		EscValue v = Get(r);
		if(v.IsInt64())
			r = -Int(v, "-");
		else
			r = -Number(v, "-");
	}
	else
	if(Char('+')) {
		Unary(r);
		EscValue v = Get(r);
		if(v.IsInt64())
			r = Int(v, "+");
		else
			r = Number(v, "+");
	}
	else
	if(Char('!')) {
		Unary(r);
		r = (int64)!IsTrue(Get(r));
	}
	else
	if(Char('~')) {
		Unary(r);
		r = ~Int(Get(r), "~");
	}
	else
		Term(r);

	if(Char2('+', '+')) {
		EscValue v = Get(r);
		if(v.IsInt64())
			Assign(r, Int(v, "++") + 1);
		else
			Assign(r, Number(v, "++") + 1);
		r = v;
	}
	if(Char2('-', '-')) {
		EscValue v = Get(r);
		if(v.IsInt64())
			Assign(r, Int(v, "--") - 1);
		else
			Assign(r, Number(v, "--") - 1);
		r = v;
	}
}

EscValue Esc::MulArray(EscValue array, EscValue times)
{
	EscValue r;
	r.SetEmptyArray();
	int n = times.GetInt();
	Limit(n);
	for(; n > 0; n >>= 1) {
		if(n & 1)
			r.Append(array);
		array.Append(array);
	}
	return r;
}

void Esc::Mul(Esc::SRVal& r)
{
	Unary(r);
	for(;;)
		if(!IsChar2('*', '=') && Char('*')) {
			EscValue x = Get(r);
			SRVal w;
			Unary(w);
			EscValue y = Get(w);
			if(x.IsArray() && y.IsInt())
				r = MulArray(x, y);
			else
			if(y.IsArray() && x.IsInt())
				r = MulArray(y, x);
			else
			if(x.IsInt64() && y.IsInt64())
				r = Int(x, "*") * Int(y, "*");
			else
				r = Number(x, "*") * Number(y, "*");
		}
		else
		if(!IsChar2('/', '=') && Char('/')) {
			SRVal w;
			Unary(w);
			EscValue x = Get(r);
			EscValue y = Get(w);
			double b = Number(y, "/");
			if(b == 0)
				ThrowError("divide by zero");
			r = Number(x, "/") / b;
		}
		else
		if(!IsChar2('%', '=') && Char('%')) {
			SRVal w;
			Unary(w);
			int64 b = Int(w, "%");
			if(b == 0)
				ThrowError("divide by zero");
			r = Int(r, "%") % b;
		}
		else
			return;
}

void Esc::Add(Esc::SRVal& r)
{
	Mul(r);
	for(;;)
		if(!IsChar2('+', '=') && Char('+')) {
			EscValue v = Get(r);
			SRVal w;
			Mul(w);
			EscValue b = Get(w);
			if(v.IsArray() && b.IsArray()) {
				Limit(b.GetCount());
				v.Replace(v.GetCount(), 0, b);
				r = v;
			}
			else
			if(!(v.IsArray() && b.IsVoid())) {
				if(v.IsInt64() && b.IsInt64())
					r = Int(v, "+") + Int(b, "+");
				else
					r = Number(v, "+") + Number(b, "+");
			}
		}
		else
		if(!IsChar2('-', '=') && Char('-')) {
			SRVal w;
			Mul(w);
			EscValue v = Get(r);
			EscValue b = Get(w);
			if(v.IsInt64() && b.IsInt64())
				r = Int(v, "-") - Int(b, "-");
			else
				r = Number(v, "-") - Number(b, "-");
		}
		else
			return;
}

void Esc::Shift(Esc::SRVal& r)
{
	Add(r);
	for(;;)
		if(Char2('<', '<')) {
			EscValue v = Get(r);
			SRVal w;
			Add(w);
			EscValue b = Get(w);
			if(v.IsArray() && b.IsArray()) {
				Limit(b.GetCount());
				v.Replace(v.GetCount(), 0, b);
				Assign(r, v);
			}
			else
			if(!(v.IsArray() && b.IsVoid()))
				r = Int(v, "<<") << Int(b, "<<");
		}
		else
		if(Char2('>', '>')) {
			SRVal w;
			Add(w);
			r = Int(r, ">>") >> Int(w,  ">>");
		}
		else
			return;
}

double Esc::DoCompare(const EscValue& a, const EscValue& b, const char *op)
{
	LTIMING("DoCompare");
	if(a.IsInt64() && b.IsInt64())
		return SgnCompare(a.GetInt64(), b.GetInt64());
	if(a.IsNumber() && b.IsNumber())
		return SgnCompare(a.GetNumber(), b.GetNumber());
	if(a.IsArray() && b.IsArray()) {
		const Vector<EscValue>& x = a.GetArray();
		const Vector<EscValue>& y = b.GetArray();
		int i = 0;
		for(;;) {
			if(i >= x.GetCount())
				return i < y.GetCount() ? -1 : 0;
			if(i >= y.GetCount())
				return i < x.GetCount() ? 1 : 0;
			double q = DoCompare(x[i], y[i], op);
			if(q) return q;
			i++;
		}
	}
	if(a.IsVoid() && b.IsVoid())
		return 0;
	if(!a.IsVoid() && b.IsVoid())
		return 1;
	if(a.IsVoid() && !b.IsVoid())
		return -1;
	ThrowError("invalid values for comparison " + a.GetTypeName() + ' ' + op + ' ' + b.GetTypeName());
	return 0;
}

double Esc::DoCompare(const SRVal& a, const char *op)
{
	SRVal w;
	Shift(w);
	return DoCompare(Get(a), Get(w), op);
}

void Esc::Compare(Esc::SRVal& r)
{
	Shift(r);
	for(;;)
		if(Char2('>', '='))
			r = DoCompare(r, ">=") >= 0;
		else
		if(Char2('<', '='))
			r = DoCompare(r, "<=") <= 0;
		else
		if(Char('>'))
			r = DoCompare(r, ">") > 0;
		else
		if(Char('<'))
			r = DoCompare(r, "<") < 0;
		else
			return;
}

void Esc::Equal(Esc::SRVal& r)
{
	Compare(r);
	for(;;)
		if(Char2('=', '=')) {
			SRVal w;
			Compare(w);
			r = Get(r) == Get(w);
		}
		else
		if(Char2('!', '=')) {
			SRVal w;
			Compare(w);
			r = Get(r) != Get(w);
		}
		else
			return;
}

void Esc::BinAnd(Esc::SRVal& r)
{
	Equal(r);
	while(!IsChar2('&', '&') && Char('&')) {
		SRVal w;
		Equal(w);
		r = Int(r, "&") & Int(w, "&");
	}
}

void Esc::BinXor(Esc::SRVal& r)
{
	BinAnd(r);
	while(Char('^')) {
		SRVal w;
		BinAnd(w);
		r = Int(r, "^") ^ Int(w, "^");
	}
}

void Esc::BinOr(Esc::SRVal& r)
{
	BinXor(r);
	while(!IsChar2('|', '|') && Char('|')) {
		SRVal w;
		BinXor(w);
		r = Int(r, "|") | Int(w, "|");
	}
}

void Esc::And(Esc::SRVal& r)
{
	BinOr(r);
	if(IsChar2('&', '&')) {
		bool b = IsTrue(Get(r));
		while(Char2('&', '&')) {
			SRVal w;
			if(b) {
				BinOr(w);
				b = b && IsTrue(Get(w));
			}
			else {
				skipexp++;
				BinOr(w);
				skipexp--;
			}
		}
		r = b;
	}
}

void Esc::Or(Esc::SRVal& r)
{
	And(r);
	if(IsChar2('|', '|')) {
		bool b = IsTrue(Get(r));
		while(Char2('|', '|')) {
			SRVal w;
			if(b) {
				skipexp++;
				And(w);
				skipexp--;
			}
			else {
				And(w);
				b = b || IsTrue(Get(w));
			}
		}
		r = b;
	}
}

void Esc::Cond(Esc::SRVal& r)
{
	Or(r);
	if(Char('?')) {
		bool t = IsTrue(Get(r));
		SRVal dummy;
		if(t) {
			Cond(r);
			PassChar(':');
			skipexp++;
			Cond(dummy);
			skipexp--;
		}
		else {
			skipexp++;
			Cond(dummy);
			skipexp--;
			PassChar(':');
			Cond(r);
		}
	}
}

void Esc::Assign(Esc::SRVal& r)
{
	Cond(r);
	if(Char('=')) {
		SRVal w;
		Assign(w);
		Assign(r, Get(w));
	}
	else
	if(Char2('+', '=')) {
		EscValue v = Get(r);
		SRVal w;
		Cond(w);
		EscValue b = Get(w);
		if(v.IsArray() && b.IsArray()) {
			Limit(b.GetCount());
			v.Replace(v.GetCount(), 0, b);
			Assign(r, v);
		}
		else
		if(!(v.IsArray() && b.IsVoid())) {
			if(v.IsInt64() && b.IsInt64())
				Assign(r, Int(v, "+=") + Int(b, "+="));
			else
				Assign(r, Number(v, "+=") + Number(b, "+="));
		}
	}
	else
	if(Char2('-', '=')) {
		SRVal w;
		Cond(w);
		EscValue v = Get(r);
		EscValue b = Get(w);
		if(v.IsInt64() && b.IsInt64())
			Assign(r, Int(v, "-=") - Int(b, "-="));
		else
			Assign(r, Number(v, "-=") - Number(b, "-="));
	}
	else
	if(Char2('*', '=')) {
		SRVal w;
		Cond(w);
		EscValue x = Get(r);
		EscValue y = Get(w);
		if(x.IsInt64() && y.IsInt64())
			Assign(r, Int(x, "*=") * Int(y, "*="));
		else
			Assign(r, Number(x, "*=") * Number(y, "*="));
	}
	else
	if(Char2('/', '=')) {
		SRVal w;
		Cond(w);
		EscValue v = Get(r);
		EscValue b = Get(w);
		double q = Number(v, "/=");
		if(q == 0)
			ThrowError("divide by zero");
		Assign(r, Number(b, "/=") / q);
	}
	else
	if(Char2('%', '=')) {
		SRVal w;
		Cond(w);
		int64 a = Int(r, "%=");
		int64 b = Int(w, "%=");
		if(b == 0)
			ThrowError("divide by zero");
		Assign(r, a % b);
	}
}

int Esc::stack_level = 50;

void Esc::Exp(Esc::SRVal& r)
{
	LTIMING("Exp");
	Spaces();
	stack_level--;
	if(stack_level <= 0)
		ThrowError("stack overflow");
	Assign(r);
	stack_level++;
}

EscValue Esc::GetExp() {
	SRVal r;
	Exp(r);
	return Get(r);
}

void Esc::SkipTerm()
{
	if(IsEof())
		ThrowError("unexpected end of file");
	CParser::SkipTerm();
	Spaces();
}

void Esc::SkipExp()
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
		if(IsEof())
			ThrowError("unexpected end of file");
	}
}

void SkipBlock(CParser& p)
{
	int level = 1;
	while(level > 0 && !p.IsEof()) {
		if(p.Char('{')) level++;
		else
		if(p.Char('}')) level--;
		else
			p.SkipTerm();
	}
}

void Esc::SkipStatement()
{
	stack_level--;
	if(stack_level <= 0)
		ThrowError("stack overflow");
	if(Id("if")) {
		PassChar('(');
		SkipExp();
		PassChar(')');
		SkipStatement();
		if(Id("else"))
			SkipStatement();
	}
	else
	if(Id("for")) {
		PassChar('(');
		if(!IsChar(';'))
			SkipExp();
		PassChar(';');
		if(!IsChar(';'))
			SkipExp();
		PassChar(';');
		if(!IsChar(')'))
			SkipExp();
		PassChar(')');
		SkipStatement();
	}
	else
	if(Id("while") || Id("switch")) {
		PassChar('(');
		SkipExp();
		PassChar(')');
		SkipStatement();
	}
	else
	if(Id("do")) {
		SkipBlock(*this);
		PassId("while");
		PassChar('(');
		SkipExp();
		PassChar(')');
		PassChar(';');
	}
	else
	if(Char('{'))
		SkipBlock(*this);
	else {
		SkipExp();
		PassChar(';');
	}
	stack_level++;
}

bool  Esc::PCond()
{
	PassChar('(');
	bool c = IsTrue(GetExp());
	PassChar(')');
	return c;
}

void Esc::FinishSwitch()
{
	while(no_break && no_return && no_continue) {
		if(Id("case")) {
			SRVal r;
			Exp(r);
			PassChar(':');
		}
		else
		if(Id("default"))
			PassChar(':');
		else
			if(Char('}'))
				return;
		DoStatement();
	}
	while(!Char('}'))
		SkipStatement();
}

void  Esc::DoStatement()
{
	Limit();
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
		Pos pos = GetPos();
		do {
			SetPos(pos);
			DoStatement();
			PassId("while");
			no_continue = true;
		}
		while(PCond() && no_break && no_return);
		PassChar(';');
		no_break = true;
		loop--;
	}
	else
	if(Id("while")) {
		loop++;
		Pos pos = GetPos();
		for(;;) {
			SetPos(pos);
			if(!PCond() || !no_break || !no_return || !no_continue) {
				SkipStatement();
				break;
			}
			DoStatement();
			no_continue = true;
		}
		no_break = true;
		loop--;
	}
	else
	if(Id("for")) {
		loop++;
		PassChar('(');
		SRVal var;
		if(!IsChar(';'))
			Exp(var);
		if(Id("in") || Char(':')) {
			EscValue range = GetExp();
			PassChar(')');
			Pos stmt = GetPos();
			int i = 0;
			for(;;) {
				SetPos(stmt);
				if(range.IsArray()) {
					if(i >= range.GetCount())
						break;
					Assign(var, (int64)i);
				}
				else
				if(range.IsMap()) {
					const VectorMap<EscValue, EscValue>& map = range.GetMap();
					if(i >= map.GetCount())
						break;
					if(map.IsUnlinked(i)) {
						i++;
						continue;
					}
					Assign(var, map.GetKey(i));
				}
				if(!no_break || !no_return || !no_continue) {
					SkipStatement();
					break;
				}
				DoStatement();
				no_continue = true;
				i++;
			}
			SkipStatement();
		}
		else {
			PassChar(';');
			Pos cond;
			if(!IsChar(';')) {
				cond = GetPos();
				SkipExp();
			}
			PassChar(';');
			Pos after;
			if(!IsChar(')')) {
				after = GetPos();
				SkipExp();
			}
			PassChar(')');
			Pos stmt = GetPos();
			for(;;) {
				bool c = true;
				if(cond.ptr) {
					SetPos(cond);
					c = IsTrue(GetExp());
				}
				SetPos(stmt);
				if(!c || !no_break || !no_return || !no_continue) {
					SkipStatement();
					break;
				}
				DoStatement();
				no_continue = true;
				if(after.ptr) {
					SetPos(after);
					SRVal r;
					Exp(r);
				}
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
	if(Id("continue")) {
		if(!loop)
			ThrowError("misplaced 'continue'");
		no_continue = false;
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
			return_value = GetExp();
			PassChar(';');
		}
		else
			return_value = EscValue();
	}
	else
	if(Id("switch")) {
		loop++;
		PassChar('(');
		EscValue a = GetExp();
		PassChar(')');
		PassChar('{');
		while(!Char('}')) {
			if(Id("case")) {
				EscValue b = GetExp();
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
	if(Char('#')) {
		int type = 0;
		if(Char('.'))
			type = 1;
		else
		if(Char(':'))
			type = 2;
		String id = ReadId();
		EscValue l = ReadLambda(*this);
		if(type == 1) {
			if(self.IsVoid())
				ThrowError("no instance");
			self.MapSet(id, l);
		}
		else
		if(type == 2)
			global.GetAdd(id) = l;
		else
			var.GetAdd(id) = l;
	}
	else
	if(Char('{')) {
		while(!Char('}') && no_break && no_return && no_continue)
			DoStatement();
	}
	else
	if(!Char(';')) {
		SRVal v;
		Exp(v);
		PassChar(';');
	}
}

void  Esc::Run()
{
	no_return = no_break = no_continue = true;
	loop = 0;
	skipexp = 0;
	while(!IsEof() && no_return && no_break && no_continue)
		DoStatement();
}

}
