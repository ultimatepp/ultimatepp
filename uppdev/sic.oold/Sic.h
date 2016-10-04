#include <Core/Core.h>

enum { SV_VOID, SV_NUMBER, SV_STRING, SV_MAP, SV_LAMBDA };

struct SValMap;
struct SLambda;

class SVal : Moveable<SVal> {
	struct SValMap;

	int                   type;
	String                string;
	double                number;
	int                   ipos;

	struct SLambdaRef;
	union {
		SValMap       *map;
		SLambdaRef    *lambda;
	};

	void                  Free();
	void                  Assign(const SVal& s);

public:
	int  GetType() const       { return type; }
	bool IsVoid() const        { return type == SV_VOID; }
	bool IsNumber() const      { return type == SV_NUMBER; }
	bool IsString() const      { return type == SV_STRING; }
	bool IsMap() const         { return type == SV_MAP; }
	bool IsLambda() const      { return type == SV_LAMBDA; }

	double                       GetNumber() const   { ASSERT(IsNumber()); return number; }
	const String&                GetString() const   { ASSERT(IsString()); return string; }
	String&                      GetString()         { ASSERT(IsString()); return string; }
	const SLambda&               GetLambda() const;
	const ArrayMap<SVal, SVal>&  GetMap() const;

	unsigned GetHashValue() const;
	bool     operator==(const SVal& a) const;
	bool     operator!=(const SVal& a) const         { return !(*this == a); }

	void   operator=(const Nuller&);
	void   operator=(double d);
	void   operator=(const String& s);

	SLambda& CreateLambda();

	void   SetMap();
	SVal&  Set(SVal key);
	SVal&  Add();

	void   operator=(const SVal& s);

	String Dump() const;

	SVal();
	SVal(const SVal& src);
	SVal(const Nuller& s);
	SVal(const String& s);
	SVal(double n);
	~SVal();
};

inline unsigned GetHashValue(const SVal& v)
{
	return v.GetHashValue();
}

bool IsTrue(const SVal& a);

struct Sic : public CParser {
	struct SRVal : Moveable<SRVal> {
		SVal *lvalue;
		SVal  rvalue;

		operator const SVal&()         { return lvalue ? *lvalue : rvalue; }
		const SVal *operator->() const { return lvalue ? lvalue : &rvalue; }
		const SVal& operator~() const  { return lvalue ? *lvalue : rvalue; }

		SRVal()                  { lvalue = NULL; }
		SRVal(const SVal& a)     { rvalue = a; lvalue = NULL; }
		SRVal(double d)          { rvalue = d; lvalue = NULL; }
		SRVal(const String& s)   { rvalue = s; lvalue = NULL; }
	};

	struct Var : Moveable<Var> {
		SVal *ref;
		SVal  var;

		Var() { ref = NULL; }
	};

	ArrayMap<String, SVal>& global;
	SVal                   *self;
	ArrayMap<String, Var>   var;
	int   loop;
	bool  no_break, no_return;
	SVal  return_value;
	int   stack_level;

	int    DoCompare(const SVal& a);
	SVal   ReadLambda();
	String ReadName();
	SVal   ExecuteLambda(const String& id, SVal lambda, SVal *self, Vector<SRVal>& arg);
	SRVal  Operator(Sic::SRVal a, Sic::SRVal b, const char *oper, const char *op,
	                bool sym = false);

	SRVal Term();
	SRVal Unary();
	SRVal Mul();
	SRVal Add();
	SRVal Shift();
	SRVal Compare();
	SRVal Equal();
	SRVal BinAnd();
	SRVal BinXor();
	SRVal BinOr();
	SRVal And();
	SRVal Or();
	SRVal Cond();
	SRVal Assign();
	SRVal Exp();

	void  SkipTerm();
	void  SkipStatement();
	void  SkipExp();
	bool  PCond();
	void  FinishSwitch();
	void  DoStatement();

	void  Run();

	Sic(ArrayMap<String, SVal>& global, const char *s) : global(global), CParser(s)
	{ stack_level = 0; self = NULL; }
};

struct SLambda {
	Vector<String> arg;
	Vector<bool>   ref;
	String         code;
	SVal           (*escape)(Sic& sic, Vector<Sic::SRVal>& var);

	SLambda() { escape = NULL; }
};

void Escape(ArrayMap<String, SVal>& global,
            const char *function,
            SVal (*fn)(Sic& sic, Vector<Sic::SRVal>& arg));

void Scan(ArrayMap<String, SVal>& global, const char *file);

void StdLib(ArrayMap<String, SVal>& global);

SVal Execute(ArrayMap<String, SVal>& global, const char *fnname, const Vector<SVal>& arg);
SVal Execute(ArrayMap<String, SVal>& global, const char *fnname);
