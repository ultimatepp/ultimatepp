#ifndef _ESC_ESC_H_
#define _ESC_ESC_H_

#include <Core/Core.h>


NAMESPACE_UPP

enum EscTypeKind { ESC_VOID, ESC_DOUBLE, ESC_ARRAY, ESC_MAP, ESC_LAMBDA, ESC_INT64 };

String EscTypeName(int sv_type);

struct EscEscape;
class  EscLambda;
struct EscHandle;
struct Esc;

class EscValue : Moveable<EscValue> {
	struct RefCount {
		Atomic   refcount;
		RefCount()              { AtomicWrite(refcount, 1); }
	};

	int              type;
	mutable unsigned hash;

	struct EscMap;
	struct EscArray;

	union {
		double         number;
		int64          i64;
		EscArray      *array;
		EscMap        *map;
		EscLambda     *lambda;
	};

	void                  Free();
	void                  Assign(const EscValue& s);

	void                  InitString(const WString& w);
	Vector<EscValue>&     CloneArray();

	VectorMap<EscValue, EscValue>& CloneMap();

	static int             total;
	static int             max_total;

public:
	static int             GetTotalCount();
	static void            SetMaxTotalCount(int n);
	static int             GetMaxTotalCount();

	bool IsVoid() const                          { return type == ESC_VOID; }
	EscValue();

	bool                   IsNumber() const      { return findarg(type, ESC_DOUBLE, ESC_INT64) >= 0; }
	double                 GetNumber() const;
	bool                   IsInt64() const       { return type == ESC_INT64; }
	int64                  GetInt64() const;
	bool                   IsInt() const;
	int                    GetInt() const;
	EscValue(double n);
	EscValue(int64 n);
	EscValue(int n);

	bool                    IsArray() const      { return type == ESC_ARRAY; }
	const Vector<EscValue>& GetArray() const;
	EscValue                ArrayGet(int i) const;
	EscValue                ArrayGet(int i, int n) const;
	bool                    ArraySet(int i, EscValue val);
	bool                    Replace(int i, int n, EscValue a);
	void                    SetEmptyArray();
	void                    ArrayAdd(EscValue val);
	bool                    Append(EscValue a);

	operator WString() const;
	operator String() const                     { return operator WString().ToString(); }

	EscValue(const char *s)                     { InitString(WString(s)); }
	EscValue(const WString& s)                  { InitString(s); }
	EscValue(const String& s)                   { InitString(s.ToWString()); }

	bool                                 IsMap() const         { return type == ESC_MAP; }
	const VectorMap<EscValue, EscValue>& GetMap() const;
	EscValue                             MapGet(EscValue key) const;
	void                                 MapSet(EscValue key, EscValue value);
	void                                 SetEmptyMap();

	bool                                 IsLambda() const     { return type == ESC_LAMBDA; }
	const EscLambda&                     GetLambda() const;
	EscLambda&                           CreateLambda();


	void    Escape(const char *method, Callback1<EscEscape&> escape);
	void    Escape(const char *method, EscHandle *h, Callback1<EscEscape&> escape);
	bool    HasNumberField(const char *id) const;
	int     GetFieldInt(const char *id) const;

	int     GetType() const                                   { return type; }
	String  GetTypeName() const                               { return UPP::EscTypeName(type); }

	int     GetCount() const;

	unsigned GetHashValue() const;
	bool     operator==(const EscValue& a) const;
	bool     operator!=(const EscValue& a) const              { return !(*this == a); }

	String ToString(int maxlen = INT_MAX, int indent_step = 4, bool hex = false, int indent = 0) const;

	EscValue& operator=(const EscValue& s);
	EscValue(const EscValue& src);

	~EscValue();
};

struct EscValue::EscArray : EscValue::RefCount {
	Vector<EscValue> array;

	void     Retain()        { AtomicInc(refcount); }
	void     Release()       { if(AtomicDec(refcount) == 0) delete this; }
};

struct EscValue::EscMap : EscValue::RefCount {
	VectorMap<EscValue, EscValue> map;
	int                           count;

	void     Retain()        { AtomicInc(refcount); }
	void     Release()       { if(AtomicDec(refcount) == 0) delete this; }

	EscMap()                 { count = 0; }
};

struct EscHandle {
	Atomic   refcount;

	void       Retain()        { AtomicInc(refcount); }
	void       Release()       { if(AtomicDec(refcount) == 0) delete this; }

	EscHandle()              { AtomicWrite(refcount, 0); }
	virtual ~EscHandle()     {}
};

class EscLambda {
	Atomic   refcount;

	void     Retain()        { AtomicInc(refcount); }
	void     Release()       { if(AtomicDec(refcount) == 0) delete this; }

	EscLambda()                 { AtomicWrite(refcount, 1); varargs = false; handle = NULL; }
	~EscLambda()                { if(handle) handle->Release(); }

	friend class EscValue;

public:
	Vector<String>        arg;
	Vector<String>        def;
	Vector<bool>          inout;
	String                code;
	EscHandle            *handle;
	Callback1<EscEscape&> escape;
	bool                  varargs;
	String                filename;
	int                   line;

private:
	EscLambda(const EscLambda&);
	void operator=(const EscLambda&);
};

template <>
inline unsigned GetHashValue(const EscValue& v)
{
	return v.GetHashValue();
}

bool     IsTrue(const EscValue& a);

void     SkipBlock(CParser& p);
EscValue ReadLambda(CParser& p);
EscValue ReadLambda(const char *s);

struct Esc : public CParser {
	struct SRVal : Moveable<SRVal> {
		EscValue *lval;
		EscValue  rval;
		EscValue  sbs;

		SRVal()                    { lval = NULL; }
		SRVal(const EscValue& v)   { lval = NULL; rval = v; }
		SRVal(double n)            { lval = NULL; rval = n; }
		SRVal(int64 n)             { lval = NULL; rval = n; }
		SRVal(uint64 n)            { lval = NULL; rval = (int64)n; }
		SRVal(bool n)              { lval = NULL; rval = (int64)n; }
	};

	ArrayMap<String, EscValue>& global;
	EscValue                    self;
	ArrayMap<String, EscValue>  var;

	int      skipexp;
	int      loop;
	bool     no_break, no_return;
	int&     op_limit;
	int      r_stack_level;
	EscValue return_value;

	static int stack_level;

	void       OutOfMemory();

	void       TestLimit();
	double     DoCompare(const EscValue& a, const EscValue& b, const char *op);
	double     DoCompare(const SRVal& a, const char *op);
	String     ReadName();
	EscValue   ExecuteLambda(const String& id, EscValue lambda, SRVal self, Vector<SRVal>& arg);

	void       Assign(EscValue& val, const Vector<EscValue>& sbs, int si, const EscValue& src);

	EscValue   Get(const SRVal& val);
	void       Assign(const SRVal& val, const EscValue& src);

	EscValue   GetExp();

	double Number(const EscValue& a, const char *oper);
	int64  Int(const EscValue& a, const char *oper);
	double Number(const SRVal& a, const char *oper);
	int64  Int(const SRVal& a, const char *oper);

	EscValue   MulArray(EscValue array, EscValue times);

	void  Subscript(SRVal& r, SRVal _self, String id);
	void  Subscript(SRVal& r);
	void  Term(SRVal& r);
	void  Unary(SRVal& r);
	void  Mul(SRVal& r);
	void  Add(SRVal& r);
	void  Shift(SRVal& r);
	void  Compare(SRVal& r);
	void  Equal(SRVal& r);
	void  BinAnd(SRVal& r);
	void  BinXor(SRVal& r);
	void  BinOr(SRVal& r);
	void  And(SRVal& r);
	void  Or(SRVal& r);
	void  Cond(SRVal& r);
	void  Assign(SRVal& r);
	void  Exp(SRVal& r);

	void  SkipTerm();
	void  SkipStatement();
	void  SkipExp();
	bool  PCond();
	void  FinishSwitch();
	void  DoStatement();

	void  Run();

	Esc(ArrayMap<String, EscValue>& global, const char *s, int& oplimit,
	    const String& fn, int line = 1)
	: CParser(s, fn, line), global(global), op_limit(oplimit)
	{ r_stack_level = stack_level;  skipexp = false; }
	~Esc() { stack_level = r_stack_level; }
};

struct EscEscape {
	Esc&             esc;
	EscValue         self;
	Array<EscValue>& arg;
	EscValue         ret_val;
	String           id;

	EscValue&    operator[](int i)             { return arg[i]; }
	int          GetCount() const              { return arg.GetCount(); }
	void         operator=(const EscValue& v)  { ret_val = v; }
	operator     EscValue&()                   { return ret_val; }
	void         ThrowError(const char *s)     { esc.ThrowError(s); }

	String       DumpType(int i);
	String       InCall();

	void         CheckNumber(int i);
	void         CheckArray(int i);
	void         CheckMap(int i);

	double       Number(int i);
	int          Int(int i);

	EscEscape(Esc& esc, EscValue self, Array<EscValue>& arg)
	  : esc(esc), self(self), arg(arg) {}
};

void Escape(ArrayMap<String, EscValue>& globals, const char *function, void (*escape)(EscEscape& e));
void Escape(ArrayMap<String, EscValue>& globals, const char *function, Callback1<EscEscape&> escape);

void Scan(ArrayMap<String, EscValue>& global, const char *code, const char *filename = "");

void StdLib(ArrayMap<String, EscValue>& global);

void     LambdaArgs(CParser& p, EscLambda& l);

EscValue Execute(ArrayMap<String, EscValue>& global, EscValue *self,
                 const EscValue& lambda, Vector<EscValue>& arg, int oplimit = 50000);
EscValue Execute(ArrayMap<String, EscValue>& global, EscValue *self,
                 const char *name, Vector<EscValue>& arg, int oplimit = 50000);
EscValue Execute(ArrayMap<String, EscValue>& global, const char *name, int oplimit = 50000);

EscValue Evaluatex(const char *expression, ArrayMap<String, EscValue>& global, int oplimit = 50000);
EscValue Evaluate(const char *expression, ArrayMap<String, EscValue>& global, int oplimit = 50000);

EscValue EscFromStdValue(const Value& v);
Value    StdValueFromEsc(const EscValue& v);
void     StdValueLib(ArrayMap<String, EscValue>& global);

bool     IsDate(const EscValue& v);
bool     IsTime(const EscValue& v);

String   Expand(const String& doc, ArrayMap<String, EscValue>& global,
                int oplimit = 50000, String (*format)(const Value& v) = StdFormat);

END_UPP_NAMESPACE

#endif
