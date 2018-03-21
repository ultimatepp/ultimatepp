#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

#ifdef _DEBUG
String Pdb::Val::ToString() const
{
	String r;
	r << "Pdb::Val type: " << type << ", ref: " << ref << ", array: " << array
	  << ", rvalue: " << rvalue;
	return r;
}
#endif

void Pdb::ThrowError(const char *s)
{
	throw CParser::Error(s);
}

int Pdb::SizeOfType(int ti)
{
	if(ti >= 0)
		return GetType(ti).size;
	switch(ti) {
	case BOOL1:
	case SINT1:
	case UINT1:
		return 1;
	case SINT2:
	case UINT2:
		return 2;
	case SINT4:
	case UINT4:
	case FLT:
		return 4;
	case SINT8:
	case UINT8:
	case DBL:
		return 8;
	}
	return 0;
}

#define READINT0(type) { \
	type x; \
	if(v.address < 10000) \
		x = (type)GetCpuRegister(ctx, (int)v.address); \
	else \
	if(!Copy(v.address, &x, sizeof(x))) \
		ThrowError("??"); \
	v.ival = x; \
	break; \
} \

#define READINT(q, type) \
case q: READINT0(type)

#define READFLT(q, type) \
case q: { \
	type x; \
	if(!Copy(v.address, &x, sizeof(x))) \
		ThrowError("??"); \
	v.fval = x; \
	break; \
} \

Pdb::Val Pdb::GetRVal(Pdb::Val v, Thread& ctx)
{
	if(v.rvalue)
		return v;
	v.rvalue = true;
	if(v.array) {
		v.array = false;
		v.ref++;
	}
	else
	if(v.ref) {
		if(!Copy(v.address, &v.address, win64 ? 8 : 4))
			ThrowError("??");
	}
	else
	if(v.bitcnt) {
		dword w;
		if(!Copy(v.address, &w, win64 ? 8 : 4))
			ThrowError("??");
		w = (w >> v.bitpos) & (0xffffffff >> (32 - v.bitcnt));
		if(v.type == SINT1 || v.type == SINT2 || v.type == SINT4 || v.type == SINT8) {
			if(w & (1 << (v.bitcnt - 1)))
				w |= (0xffffffff << v.bitcnt);
			v.ival = (int32)w;
		}
		else
			v.ival = w;
	}
	else
		switch(v.type) {
		READINT(BOOL1, bool)
		READINT(UINT1, byte);
		READINT(SINT1, int8);
		READINT(UINT2, uint16);
		READINT(SINT2, int16);
		READINT(UINT4, uint32);
		READINT(SINT4, int32);
		READINT(UINT8, uint64);
		READINT(SINT8, int64);
		READFLT(FLT, float);
		READFLT(DBL, double);
		case PFUNC:
			if(win64) {
				READINT0(uint64);
			}
			else {
				READINT0(uint32)
			}
			break;
		case UNKNOWN:
			break;
		default:
			ThrowError("Invalid operand");
		}
	return v;
}

#define GETINT(q, type) case q: return (type)v.ival;

int64 Pdb::GetInt(Pdb::Val v, Thread& ctx)
{
	v = GetRVal(v, ctx);
	switch(v.type) {
	GETINT(BOOL1, bool)
	GETINT(UINT1, byte);
	GETINT(SINT1, int8);
	GETINT(UINT2, uint16);
	GETINT(SINT2, int16);
	GETINT(UINT4, uint32);
	GETINT(SINT4, int32);
	GETINT(UINT8, uint64);
	GETINT(SINT8, int64);
	case FLT:
	case DBL:
		if(v.fval >= _I64_MIN && v.fval <= _I64_MAX)
			return (int64)v.fval;
	}
	ThrowError("Invalid operand");
	return 0;
}

#define GETINT(q, type) case q: return (type)v.ival;

double Pdb::GetFlt(Pdb::Val v, Thread& ctx)
{
	v = GetRVal(v, ctx);
	if(v.type == DBL || v.type == FLT)
		return v.fval;
	return (double)GetInt(v, ctx);
}

void Pdb::ZeroDiv(double x)
{
	if(x == 0)
		ThrowError("Divide by zero");
}

Pdb::Val Pdb::Compute(Pdb::Val v1, Pdb::Val v2, int oper, Thread& ctx)
{
	LLOG("Compute " << char(oper));
	LLOG("v1: " << v1);
	LLOG("v2: " << v2);
	if(v1.ref || v1.array) {
		int q = (int)GetInt(v2, ctx) * (v1.ref > 1 ? 4 : SizeOfType(v1.type));
		v1 = GetRVal(v1, ctx);
		switch(oper) {
		case '+': v1.address += q; break;
		case '-': v1.address -= q; break;
		default: ThrowError("Invalid pointer arithmetics");
		}
		return v1;
	}
	if(v2.ref || v2.array) {
		int q = (int)GetInt(v1, ctx) * (v2.ref ? 4 : SizeOfType(v2.type));
		v2 = GetRVal(v2, ctx);
		if(oper == '+')
			v2.address += q;
		else
			ThrowError("Invalid pointer arithmetics");
		return v2;
	}
	Pdb::Val res;
	res.type = max(v1.type, v2.type);
	res.rvalue = true;
	if(res.type == DBL || res.type == FLT) {
		double a = GetFlt(v1, ctx);
		double b = GetFlt(v2, ctx);
		switch(oper) {
		case '+': res.fval = a + b; break;
		case '-': res.fval = a - b; break;
		case '*': res.fval = a * b; break;
		case '/': ZeroDiv(b); res.fval = a / b; break;
		default: ThrowError("Invalid operands for operation");
		}
	}
	else {
		int64 a = GetInt(v1, ctx);
		int64 b = GetInt(v2, ctx);
		switch(oper) {
		case '+': res.ival = a + b; break;
		case '-': res.ival = a - b; break;
		case '*': res.ival = a * b; break;
		case '/': ZeroDiv((double)b); res.ival = a / b; break;
		case '%': ZeroDiv((double)b); res.ival = a % b; break;
		default: ThrowError("Invalid operands for operation");
		}
	}
	return res;
}

Pdb::Val Pdb::RValue(int64 i)
{
	Val v;
	v.rvalue = true;
	v.ival = i;
	v.type = i >= -128 && i <= 127 ? SINT1 :
	         i >= -32768 && i <= 32767 ? SINT2 :
	         i >= INT_MIN && i <= INT_MAX ? SINT4 :
	         SINT8;
	return v;
}

Pdb::Val Pdb::DeRef(Pdb::Val v, Thread& ctx)
{
	if(v.ref <= 0)
		ThrowError("Only pointer can be dereferenced");
	v = GetRVal(v, ctx);
	v.ref--;
	v.rvalue = false;
	return v;
}

Pdb::Val Pdb::Ref(Pdb::Val v)
{
	if(v.rvalue)
		ThrowError("R-value cannot be referenced");
	v.rvalue = true;
	v.ref++;
	return v;
}

Pdb::Val Pdb::Field0(Pdb::Val v, const String& field)
{
	Val v1;
	v1.type = Null;
	if(v.rvalue || v.ref || v.array || v.type < 0)
		return v1;
	const Type& t = GetType(v.type);
	int q = t.member.Find(field);
	if(q >= 0) {
		Val r = t.member[q];
		r.address += v.address;
		return r;
	}
	q = t.static_member.Find(field);
	if(q >= 0)
		return t.static_member[q];
	for(int i = 0; i < t.base.GetCount(); i++) {
		v1 = t.base[i];
		v1.address += v.address;
		if(v1.type >= 0) {
			if(GetType(v1.type).name == field)
				return v1;
			v1 = Field0(v1, field);
			if(!IsNull(v1.type))
				return v1;
		}
	}
	return v1;
}

Pdb::Val Pdb::Field(Pdb::Val v, const String& field)
{
	if(v.type < 0)
		ThrowError("Must be struct, class or union");
	v = Field0(v, field);
	if(IsNull(v.type))
		ThrowError("Unknown member \'" + field + "\'");
	return v;
}

Pdb::Val Pdb::Term(CParser& p, Thread& ctx)
{
	if(p.Char2('0', 'x') || p.Char2('0', 'X'))
		return RValue(p.ReadNumber64(16));
	if(p.IsChar2('0', '.')) {
		Val v;
		v.type = DBL;
		v.rvalue = true;
		v.fval = p.ReadDouble();
		return v;
	}
	if(p.Char('0'))
		return RValue(p.IsNumber() ? p.ReadNumber64(8) : 0);
	if(p.IsNumber()) {
		double d = p.ReadDouble();
		if(d >= INT_MIN && d <= INT_MAX && (int)d == d) {
			return RValue((int)d);
		}
		else {
			Val v;
			v.type = DBL;
			v.rvalue = true;
			v.fval = d;
			return v;
		}
	}
	if(p.IsChar('\'')) {
		String s = p.ReadString('\'');
		if(s.GetLength() != 1)
			ThrowError("Invalid character literal");
		return RValue(s[0]);
	}
	if(p.Char('(')) {
		Val v = Exp0(p, ctx);
		p.PassChar(')');
		return v;
	}
	if(!p.IsId())
		ThrowError("Missing id");
	String id = p.ReadId();
	while(p.Char2(':', ':') && p.IsId())
		id << "::" << p.ReadId();
	if(current_frame) {
		Frame& f = *current_frame;
		int q = f.local.Find(id);
		if(q >= 0)
			return f.local[q];
		q = f.param.Find(id);
		if(q >= 0)
			return f.param[q];
		q = f.local.Find("this");
		if(q >= 0) {
			Val v = Field0(DeRef(f.local[q], ctx), id);
			if(!IsNull(v.type))
				return v;
		}
		String scope = f.fn.name;
		do {
			String n = id;
			int q = scope.ReverseFind("::");
			if(q >= 0) {
				scope.Trim(q);
				n = scope + "::" + id;
			}
			else
				scope.Clear();
			Val v = GetGlobal(n);
			if(v.address)
				return v;
		}
		while(scope.GetCount());
	}
	ThrowError("\'" + id + "\' undefined");
	return Val();
}

String ReadType(CParser& p)
{
	String t = p.ReadId();
	if(p.Char('<')) {
		t << '<';
		int l = 1;
		while(l > 0) {
			if(p.Char('<')) {
				t << '<';
				l++;
			}
			else
			if(p.Char('>')) {
				t << '>';
				l--;
			}
			else
			if(p.IsNumber())
				t << p.ReadInt();
			else
			if(p.Char(','))
				t << ',';
			else
				t << p.ReadId();
		}
	}
	return t;
}

Pdb::Val Pdb::Post(CParser& p, Thread& ctx)
{
	Val v = Term(p, ctx);
	LLOG("Post: " << v);
	for(;;) {
		if(p.Char(':'))
			v = Field(v.ref ? DeRef(v, ctx) : v, ReadType(p));
		else
		if(p.Char('.') || p.Char2(':', ':') || p.Char2('-', '>'))
			v = Field(v.ref ? DeRef(v, ctx) : v, p.ReadId());
		else
		if(p.Char('[')) {
			int i = (int)GetInt(Exp0(p, ctx), ctx);
			p.Char(']');
			v = DeRef(Compute(v, RValue(i), '+', ctx), ctx);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Unary(CParser& p, Thread& ctx)
{
	if(p.Char('-'))
		return Compute(RValue(0), Unary(p, ctx), '-', ctx);
	if(p.Char('+'))
		return GetRVal(Unary(p, ctx), ctx);
	if(p.Char('*'))
		return DeRef(Unary(p, ctx), ctx);
	if(p.Char('&'))
		return Ref(Unary(p, ctx));
	if(p.Char('!')) {
		Val v = GetRVal(Unary(p, ctx), ctx);
		if(v.type == FLT || v.type == DBL)
			return RValue(!GetFlt(v, ctx));
		else
			return RValue(!GetInt(v, ctx));
	}
	return Post(p, ctx);
}

Pdb::Val Pdb::Multiplicative(CParser& p, Thread& ctx)
{
	Val v = Unary(p, ctx);
	for(;;) {
		if(p.Char('*'))
			v = Compute(v, Unary(p, ctx), '*', ctx);
		else
		if(p.Char('/'))
			v = Compute(v, Unary(p, ctx), '/', ctx);
		else
		if(p.Char('%'))
			v = Compute(v, Unary(p, ctx), '%', ctx);
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Additive(CParser& p, Thread& ctx)
{
	Val v = Multiplicative(p, ctx);
	for(;;) {
		if(p.Char('+'))
			v = Compute(v, Multiplicative(p, ctx), '+', ctx);
		else
		if(p.Char('-'))
			v = Compute(v, Multiplicative(p, ctx), '-', ctx);
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Compare(Val v1, CParser& p, int r1, int r2, Thread& ctx)
{
	int q;
	Val v2 = Additive(p, ctx);
	if(findarg(max(v1.type, v2.type), DBL, FLT) >= 0)
		q = SgnCompare(GetFlt(v1, ctx), GetFlt(v2, ctx));
	else
		q = SgnCompare(GetInt(v1, ctx), GetInt(v2, ctx));
	q = sgn(q);
	return RValue(q == r1 || q == r2);
}

Pdb::Val Pdb::Comparison(CParser& p, Thread& ctx)
{
	Val v = Additive(p, ctx);
	for(;;) {
		if(p.Char2('=', '='))
			v = Compare(v, p, 0, 0, ctx);
		else
		if(p.Char2('!', '='))
			v = Compare(v, p, -1, 1, ctx);
		else
		if(p.Char2('<', '='))
			v = Compare(v, p, -1, 0, ctx);
		else
		if(p.Char2('>', '='))
			v = Compare(v, p, 1, 0, ctx);
		else
		if(p.Char('<'))
			v = Compare(v, p, -1, -1, ctx);
		else
		if(p.Char('>'))
			v = Compare(v, p, 1, 1, ctx);
		else
			break;
	}
	return v;
}

void Pdb::GetBools(Val v1, Val v2, bool& a, bool& b, Thread& ctx)
{
	if(findarg(max(v1.type, v2.type), DBL, FLT) >= 0) {
		a = (bool)GetFlt(v1, ctx);
		b = (bool)GetFlt(v2, ctx);
	}
	else {
		a = (bool)GetInt(v1, ctx);
		b = (bool)GetInt(v2, ctx);
	}
}

Pdb::Val Pdb::LogAnd(CParser& p, Thread& ctx)
{
	Val v = Comparison(p, ctx);
	for(;;) {
		if(p.Char2('&', '&')) {
			bool a, b;
			GetBools(v, Comparison(p, ctx), a, b, ctx);
			v = RValue(a && b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::LogOr(CParser& p, Thread& ctx)
{
	Val v = LogAnd(p, ctx);
	for(;;) {
		if(p.Char2('|', '|')) {
			bool a, b;
			GetBools(v, LogAnd(p, ctx), a, b, ctx);
			v = RValue(a || b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Exp0(CParser& p, Thread& ctx)
{
	DR_LOG("Evaluating Expression: " << p.GetPtr());
	LLOG("Evaluating Expression: " << p.GetPtr());
	return LogOr(p, ctx);
}

Pdb::Val Pdb::Exp(CParser& p, Thread& ctx)
{
	Pdb::Val v = Exp0(p, ctx);
	if(!p.IsEof())
		ThrowError("Invalid expression");
	return v;
}

#endif
