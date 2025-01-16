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

int Pdb::SizeOfType(const String& name)
{
	TypeInfo f = GetTypeInfo(name);
	if(f.ref)
		return win64 ? 8 : 4;
	return SizeOfType(f.type);
}

adr_t Pdb::Align(adr_t adr, int type)
{
	if(type == 1)
		return adr;
	if(type == 2)
		return (adr + 1) & ~1;
	if(type <= 4)
		return (adr + 3) & ~3;
	return (adr + 7) & ~7;
}

#define READINT0(type) { \
	type x; \
	if(v.address < 10000) \
		x = (type)GetCpuRegister(*v.context, (int)v.address); \
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

adr_t Pdb::PeekPtr(adr_t address)
{
	adr_t r = 0;
	if(!Copy(address, &r, win64 ? 8 : 4))
		ThrowError("??");
	return r;
}

byte Pdb::PeekByte(adr_t address)
{
	byte b;
	if(!Copy(address, &b, 1))
		ThrowError("??");
	return b;
}

word Pdb::PeekWord(adr_t address)
{
	word w;
	if(!Copy(address, &w, 2))
		ThrowError("??");
	return w;
}

dword Pdb::PeekDword(adr_t address)
{
	dword w;
	if(!Copy(address, &w, 4))
		ThrowError("??");
	return w;
}

dword Pdb::Peek64(adr_t address)
{
	dword w;
	if(!Copy(address, &w, 8))
		ThrowError("??");
	return w;
}

Pdb::Val Pdb::GetRVal(Pdb::Val v)
{ // read data from debugee
	if(v.rvalue)
		return v;
	v.rvalue = true;
	if(v.array) {
		v.array = false;
		v.ref++;
	}
	else
	if(v.ref) { // Fetch pointer from the debugee memory
		v.address = PeekPtr(v.address);
	}
	else
	if(v.bitcnt) { // Fetch bitfield from debugee memory
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
	else {
		if(v.address < 10000 && !v.context)
			ThrowError("??"); // Register value is missing context
		switch(v.type) { // resolve primitive value
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
	}
	return v;
}

#define GETINT(q, type) case q: return (type)v.ival;

int64 Pdb::GetInt64(Pdb::Val v)
{
	v = GetRVal(v);
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
		if(FitsInInt64(v.fval))
			return (int64)v.fval;
	}
	ThrowError("Invalid operand");
	return 0;
}

int64 Pdb::GetInt64Attr(Pdb::Val v, const char *a)
{
	return GetInt64(GetAttr(v, a));
}

#define GETINT(q, type) case q: return (type)v.ival;

double Pdb::GetFlt(Pdb::Val v)
{
	v = GetRVal(v);
	if(v.type == DBL || v.type == FLT)
		return v.fval;
	return (double)GetInt64(v);
}

void Pdb::ZeroDiv(double x)
{
	if(x == 0)
		ThrowError("Divide by zero");
}

Pdb::Val Pdb::Compute(Pdb::Val v1, Pdb::Val v2, int oper)
{
	LLOG("Compute " << char(oper));
	LLOG("v1: " << v1);
	LLOG("v2: " << v2);
	if(v1.ref || v1.array) {
		int q = (int)GetInt64(v2) * (v1.ref > 1 ? 4 : SizeOfType(v1.type));
		v1 = GetRVal(v1);
		switch(oper) {
		case '+': v1.address += q; break;
		case '-': v1.address -= q; break;
		default: ThrowError("Invalid pointer arithmetics");
		}
		return v1;
	}
	if(v2.ref || v2.array) {
		int q = (int)GetInt64(v1) * (v2.ref ? 4 : SizeOfType(v2.type));
		v2 = GetRVal(v2);
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
		double a = GetFlt(v1);
		double b = GetFlt(v2);
		switch(oper) {
		case '+': res.fval = a + b; break;
		case '-': res.fval = a - b; break;
		case '*': res.fval = a * b; break;
		case '/': ZeroDiv(b); res.fval = a / b; break;
		default: ThrowError("Invalid operands for operation");
		}
	}
	else {
		int64 a = GetInt64(v1);
		int64 b = GetInt64(v2);
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

Pdb::Val Pdb::DeRef(Pdb::Val v)
{
	if(v.ref <= 0)
		ThrowError("Only pointer can be dereferenced");
	v = GetRVal(v);
	v.ref--;
	v.rvalue = false;
	return v;
}

Pdb::Val Pdb::Ref(Pdb::Val v)
{
	if(v.rvalue)
		ThrowError("R-value cannot be referenced");
	if(v.reference)
		v.reference = false;
	else {
		v.rvalue = true;
		v.ref++;
	}
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

Pdb::Val Pdb::Term(CParser& p)
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
		Val v = Exp0(p);
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
			Val v = Field0(DeRef(f.local[q]), id);
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

Pdb::Val Pdb::Post(CParser& p)
{
	Val v = Term(p);
	LLOG("Post: " << v);
	for(;;) {
		auto DoIndex = [&](bool key) {
			int ech = key ? ')' : ']';
			int64 i = Null;
			if(!p.Char(ech))
				i = (int)GetInt64(Exp0(p));
			p.Char(ech);
			Pretty p;
			if(PrettyVal(v, 0, 0, p) && p.kind != SINGLE_VALUE && p.data_type.GetCount() &&
			   (IsNull(i) && p.data_count || abs(i) < p.data_count)) {
			    if(IsNull(i))
			        i = p.data_count - 1;
			    else
			    if(i < 0)
			        i = p.data_count + i;
			    Pretty p;
				PrettyVal(v, i, 1, p);
				Val item;
				int q = 0;
				if(p.data_type.GetCount() > 1) {
					if(!key)
						q = 1;
				}
				(TypeInfo &)item = GetTypeInfo(p.data_type[q]);
				item.context = v.context;
				item.address = q < p.data_ptr.GetCount() ? p.data_ptr[q] : 0;
				v = item;
			}
			else
				v = DeRef(Compute(v, RValue(Nvl(i)), '+'));
		};
		
		if(p.Char(':'))
			v = Field(v.ref ? DeRef(v) : v, ReadType(p));
		else
		if(p.Char('.') || p.Char2(':', ':') || p.Char2('-', '>'))
			v = Field(v.ref ? DeRef(v) : v, p.ReadId());
		else
		if(p.Char('['))
			DoIndex(false);
		else
		if(p.Char('('))
			DoIndex(true);
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Unary(CParser& p)
{
	if(p.Char('-'))
		return Compute(RValue(0), Unary(p), '-');
	if(p.Char('+'))
		return GetRVal(Unary(p));
	if(p.Char('#')) {
		Pretty pp;
		if(PrettyVal(Unary(p), 0, 0, pp))
			return RValue((int)pp.data_count);
		ThrowError("Value not recognized as high-level type");
	}
	if(p.Char('*'))
		return DeRef(Unary(p));
	if(p.Char('&'))
		return Ref(Unary(p));
	if(p.Char('!')) {
		Val v = GetRVal(Unary(p));
		if(v.type == FLT || v.type == DBL)
			return RValue(!GetFlt(v));
		else
			return RValue(!GetInt64(v));
	}
	return Post(p);
}

Pdb::Val Pdb::Multiplicative(CParser& p)
{
	Val v = Unary(p);
	for(;;) {
		if(p.Char('*'))
			v = Compute(v, Unary(p), '*');
		else
		if(p.Char('/'))
			v = Compute(v, Unary(p), '/');
		else
		if(p.Char('%'))
			v = Compute(v, Unary(p), '%');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Additive(CParser& p)
{
	Val v = Multiplicative(p);
	for(;;) {
		if(p.Char('+'))
			v = Compute(v, Multiplicative(p), '+');
		else
		if(p.Char('-'))
			v = Compute(v, Multiplicative(p), '-');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Compare(Val v1, CParser& p, int r1, int r2)
{
	int q;
	Val v2 = Additive(p);
	if(findarg(max(v1.type, v2.type), DBL, FLT) >= 0)
		q = SgnCompare(GetFlt(v1), GetFlt(v2));
	else
		q = SgnCompare(GetInt64(v1), GetInt64(v2));
	q = sgn(q);
	return RValue(q == r1 || q == r2);
}

Pdb::Val Pdb::Comparison(CParser& p)
{
	Val v = Additive(p);
	for(;;) {
		if(p.Char2('=', '='))
			v = Compare(v, p, 0, 0);
		else
		if(p.Char2('!', '='))
			v = Compare(v, p, -1, 1);
		else
		if(p.Char2('<', '='))
			v = Compare(v, p, -1, 0);
		else
		if(p.Char2('>', '='))
			v = Compare(v, p, 1, 0);
		else
		if(p.Char('<'))
			v = Compare(v, p, -1, -1);
		else
		if(p.Char('>'))
			v = Compare(v, p, 1, 1);
		else
			break;
	}
	return v;
}

void Pdb::GetBools(Val v1, Val v2, bool& a, bool& b)
{
	if(findarg(max(v1.type, v2.type), DBL, FLT) >= 0) {
		a = (bool)GetFlt(v1);
		b = (bool)GetFlt(v2);
	}
	else {
		a = (bool)GetInt64(v1);
		b = (bool)GetInt64(v2);
	}
}

Pdb::Val Pdb::LogAnd(CParser& p)
{
	Val v = Comparison(p);
	for(;;) {
		if(p.Char2('&', '&')) {
			bool a, b;
			GetBools(v, Comparison(p), a, b);
			v = RValue(a && b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::LogOr(CParser& p)
{
	Val v = LogAnd(p);
	for(;;) {
		if(p.Char2('|', '|')) {
			bool a, b;
			GetBools(v, LogAnd(p), a, b);
			v = RValue(a || b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Exp0(CParser& p)
{
	DR_LOG("Evaluating Expression: " << p.GetPtr());
	LLOG("Evaluating Expression: " << p.GetPtr());
	return LogOr(p);
}

Pdb::Val Pdb::Exp(CParser& p)
{
	Pdb::Val v = Exp0(p);
	if(!p.IsEof())
		ThrowError("Invalid expression");
	return v;
}

#endif
