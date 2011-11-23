NAMESPACE_UPP

struct CalcPacket;
class  CalcNode;
class  CalcContext;

typedef RefPtr<CalcNode> CalcNodePtr;

class CalcNode : public RefBase
{
public:
	CalcNode(String name = Null, pick_ Vector<CalcNodePtr> args = Vector<CalcNodePtr>()) : name(name), args(args) {}
	CalcNode(String name, const Vector<CalcNodePtr>& args, int deep) : name(name), args(args, 0) {}
	CalcNode(String name, CalcNodePtr arg) : name(name) { args.SetCount(1); args[0] = arg; }
	CalcNode(String name, CalcNodePtr arg1, CalcNodePtr arg2) : name(name) { args.SetCount(2); args[0] = arg1; args[1] = arg2; }
	CalcNode(const CalcNode& node) : name(node.name), args(node.args, 0) {}
	virtual ~CalcNode();

	virtual Value       Calc(CalcContext& context) const = 0;
	virtual CalcNodePtr Clone() const                  { return CalcNodePtr(); }
	virtual String      Format() const                 { return Null; }
	virtual bool        IsConst() const                { return false; }
	virtual CalcNodePtr Optimize(CalcContext& context) { return this; }

	Value               TryCalc(CalcContext& context) const; // catches Exc to return ErrorValue

	double              CalcDouble(CalcContext& context) const;
	String              CalcString(CalcContext& context) const;
	int                 CalcInt(CalcContext& context) const;
	int64               CalcInt64(CalcContext& context) const;
	Date                CalcDate(CalcContext& context) const;
	Time                CalcTime(CalcContext& context) const;
	bool                CalcBool(CalcContext& context) const;

	String              GetName() const { return name; }

public:
	String              name;
	Vector<CalcNodePtr> args;

protected:
	void                Expect(const char *s, String found) const; // throws Exc

};

class CalcConstNode : public CalcNode {
public:
	CalcConstNode(Value value) : CalcNode("(const)"), value(value) {}

	virtual CalcNodePtr Clone() const { return new CalcConstNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual bool        IsConst() const                { return true; }

public:
	Value               value;
};

class CalcLambdaNode : public CalcNode {
public:
	CalcLambdaNode(CalcNodePtr arg) : CalcNode("#", arg) {}

	virtual CalcNodePtr Clone() const { return new CalcLambdaNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
};

class CalcLogOrNode : public CalcNode {
public:
	CalcLogOrNode(CalcNodePtr left, CalcNodePtr right) : CalcNode("||", left, right) {}

	virtual CalcNodePtr Clone() const { return new CalcLogOrNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual CalcNodePtr Optimize(CalcContext& context);
};

class CalcLogAndNode : public CalcNode {
public:
	CalcLogAndNode(CalcNodePtr left, CalcNodePtr right) : CalcNode("&&", left, right) {}

	virtual CalcNodePtr Clone() const { return new CalcLogAndNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual CalcNodePtr Optimize(CalcContext& context);
};

class CalcSelectNode : public CalcNode {
public:
	CalcSelectNode(CalcNodePtr cond, CalcNodePtr left, CalcNodePtr right)
		: CalcNode("?:") { args.SetCount(3); args[0] = cond; args[1] = left; args[2] = right; }

	virtual CalcNodePtr Clone() const { return new CalcSelectNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual CalcNodePtr Optimize(CalcContext& context);
	enum { COND, IFTRUE, IFFALSE };
};

class CalcSwitchNode : public CalcNode {
public:
	CalcSwitchNode(CalcNodePtr cond, pick_ Vector<CalcNodePtr>& list) : CalcNode("{}")
	{ args.Reserve(list.GetCount() + 1); args.Add(cond); args.AppendPick(list); }

	virtual CalcNodePtr Clone() const { return new CalcSwitchNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual CalcNodePtr Optimize(CalcContext& context);
};

class CalcSequenceNode : public CalcNode {
public:
	CalcSequenceNode(pick_ Vector<CalcNodePtr> nodes);
	CalcSequenceNode(CalcNodePtr node1, CalcNodePtr node2);

	virtual CalcNodePtr Clone() const { return new CalcSequenceNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
	virtual CalcNodePtr Optimize(CalcContext& context);
};

class CalcFunctionNode : public CalcNode {
public:
	CalcFunctionNode(String name)                                            : CalcNode(name) {}
	CalcFunctionNode(String name, CalcNodePtr arg)                           : CalcNode(name, arg) {}
	CalcFunctionNode(String name, CalcNodePtr arg1, CalcNodePtr arg2)        : CalcNode(name, arg1, arg2) {}
	CalcFunctionNode(String name, pick_ Vector<CalcNodePtr>& args)           : CalcNode(name, args) {}
	CalcFunctionNode(String name, const Vector<CalcNodePtr>& args, int deep) : CalcNode(name, args, 0) {}

	void                ScanArgs(CalcPacket& packet) const;

	virtual CalcNodePtr Clone() const  { return new CalcFunctionNode(*this); }
	virtual Value       Calc(CalcContext& context) const;
	virtual String      Format() const;
};

void        RunDlgCalc(CalcContext& context);

CalcNodePtr GetVariableNode(String varname);
CalcNodePtr GetFunctionNode(String fnname, pick_ Vector<CalcNodePtr>& args);

inline bool IsIdent(byte c)
{
	return IsAlNum(c) || c == '_' || c == '$' || c == '@';
}

int CharFilterCalcIdent(int c);
int CharFilterCalcUpperIdent(int c);

struct CalcPacket
{
	CalcPacket(CalcContext& context, const String& name) : context(context), name(name)
		, help(false)
		{}

	String         GetTypeNames() const;

	CalcContext&   context;
	String         name;
	Vector<Value>  args;
	Value          result;
	bool           help;
};

typedef bool (*CalcProc)(CalcPacket& packet);
typedef VectorMap<String, CalcProc> CalcProcMap;

typedef Gate1<CalcPacket&> CalcGate;
typedef VectorMap<String, CalcGate> CalcGateMap;

class HelpCalc
{
public:
	HelpCalc(CalcGate proc, const char *ident, const char *topic, String (*groupfn)(), const char *function);

	class Compare
	{
	public:
		enum MODE { ALPHABETIC, GROUPS };
		Compare(MODE mode = ALPHABETIC) : mode(mode), lang(GetLanguageInfo()) {}

		bool operator () (const HelpCalc *a, const HelpCalc *b) const;

	private:
		MODE mode;
		const LanguageInfo& lang;
	};

	String         GetTitle() const;

public:
//	int            ord;
	CalcGate       proc;
	String       (*groupfn)();
	String         ident;
	String         topic;
	String         function;

private:
	void Init(CalcGate proc, const char *ident, const char *topic, String (*groupfn)(), const char *function);
};

typedef Vector<const HelpCalc *> HelpCalcMap;

HelpCalcMap&   GetHelpCalcMap();
void           AddHelpCalc(const HelpCalc& calc);
Vector<String> GetHelpCalcGroups();

/*
class HelpCalcIndex
{
public:
	typedef VectorMap<String, const HelpCalc *> IdMap;

	static HelpCalcIndex& Get();

	int                   Add(const HelpCalc& calc) { index.Add(calc.ident, &calc); return index.GetCount(); }

	void                  PutSummary() const;
	void                  PutGroups() const;
	void                  AddIndexEntries() const; // adds entries to dynamic index

	const IdMap&          GetIndex() const { return index; }

private:
	IdMap                 index;

private:
	HelpCalcIndex();
};
*/

String HelpCalcLink(const char *id, const char *ref = " (viz str._#)", bool bold = true);

class CalcSymbols : Moveable<CalcSymbols>, DeepCopyOption<CalcSymbols>
{
public:
	CalcSymbols() {}
	CalcSymbols(const CalcSymbols& l, int deep)
	: var_index(l.var_index, 0)
	, var_value(l.var_value, 0)
	, var_const(l.var_const, 0)
	, functions(l.functions, 0) {}

	void                     Clear();
	void                     Add(String name, CalcGate proc)   { functions.Add(name, proc); }
	void                     Set(String name, Value v, bool c) { int i = var_index.FindAdd(name); var_value.DoIndex(i) = v; var_const.DoIndex(i) = c; }
	void                     Remove(String name);

	Index<String>            var_index;
	Vector<Value>            var_value;
	Vector<bool>             var_const;
	CalcGateMap              functions;
};

class CalcSymbolsLevel : public CalcSymbols
{
public:
	CalcSymbolsLevel(Vector<CalcSymbols *>& owner) : owner(owner) { owner.Add(this); }
	~CalcSymbolsLevel()                                           { owner.Drop(); }

private:
	Vector<CalcSymbols *>& owner;
};

template <class T>
class CalcLocalItem;

template <class T>
class CalcLocalMap
{
public:
	CalcLocalMap() : head(0), tail(0) {}

	void Add(CalcLocalItem<T> *item);
	void Set(T *this_ptr, CalcSymbols& map);
	void Set(T *this_ptr) { Set(this_ptr, this_ptr -> stack.Top()); }

private:
	CalcLocalItem<T> *head, *tail;
};

template <class T>
class CalcLocalItem
{
public:
	CalcLocalItem(CalcLocalMap<T>& map, const String& name, bool (T::*method)(CalcPacket&));

	String             name;
	bool          (T::*method)(CalcPacket&);
	CalcLocalItem<T>  *next;
};

template <class T>
CalcLocalItem<T>::CalcLocalItem(CalcLocalMap<T>& map, const String& name, bool (T::*method)(CalcPacket&))
: next(0)
, name(name)
, method(method)
{
	map.Add(this);
}

template <class T>
void CalcLocalMap<T>::Add(CalcLocalItem<T> *item)
{
	if(head == 0)
		head = tail = item;
	else
		tail = tail -> next = item;
}

template <class T>
void CalcLocalMap<T>::Set(T *this_ptr, CalcSymbols& map)
{
	for(CalcLocalItem<T> *p = head; p; p = p -> next)
		map.functions.Add(p -> name, callback(this_ptr, p -> method));
}

#define DECLARE_LOCAL_MAP() static CalcLocalMap<CLASSNAME>& GetLocalMap();
#define IMPLEMENT_LOCAL_MAP() GLOBAL_VAR(CalcLocalMap<BASECLASS>, BASECLASS::GetLocalMap)

class CalcContext : Moveable<CalcContext>, DeepCopyOption<CalcContext>
{
public:
	typedef CalcContext CLASSNAME;

		class Nesting
	{
	public:
		Nesting(CalcContext& context) : context(context) { context.PushLevel(); }
		~Nesting()                                       { context.PopLevel(); }

	private:
		CalcContext& context;
	};

	class Global {
	public:
		Global(const String& name, CalcProc proc);
	};

	CalcContext();
	CalcContext(const CalcContext& context, int);
	virtual ~CalcContext();

	Value               TryEvaluate(String expr);

	Value               Evaluate(String expr);
	double              EvaluateDouble(String expr);
	String              EvaluateString(String expr);
	int                 EvaluateInt(String expr);
	int64               EvaluateInt64(String expr);
	Date                EvaluateDate(String expr);
	Time                EvaluateTime(String expr);
	bool                EvaluateBool(String expr, bool null_value = false);

	String              OptimizeConstant(String expr);
	CalcNodePtr         OptimizeConstant(CalcNodePtr src);

	String              Convert(String s, bool throw_errors, const class Convert& convert = NoConvert());
	static void         ParseConvert(String s, Vector<String>& sparts, Vector<String>& cparts);
	static void         ParseConvert(String s, Vector<String>& sparts, Vector<CalcNodePtr>& cparts, bool throw_errors);
	String              CalcConvert(const Vector<String>& sparts, const Vector<CalcNodePtr>& cparts, bool throw_errors, const UPP::Convert& convert = NoConvert());

	virtual Value       Calc(CalcPacket& packet);
	virtual Value       TryCalc(CalcPacket& packet);
	virtual String      GetTypeName(Value value) const;
	virtual String      FormatNull(Value value, bool no_text = false);
	virtual String      FormatText(Value value, const UPP::Convert& convert = NoConvert());

	virtual void        PushLevel();
	virtual void        PopLevel();

	CalcContext&        Add(String name, CalcGate proc)           { stack.Top().Add(name, proc); return *this; }
	CalcContext&        Set(String name, Value v, bool c = false) { stack.Top().Set(name, v, c); return *this; }
	CalcContext&        Remove(String name)                       { stack.Top().Remove(name); return *this; }
	Value               Get(String name) const;

	void                AddExternal(CalcSymbols& e)                    { externals.Add(&e); }
	void                AddExternal(const Vector<CalcSymbols *>& ext)  { externals.Append(ext); }
	void                RemoveExternal(CalcSymbols& e);

	static CalcProcMap& GetGlobals();

public:
	enum { MAX_STACK_DEPTH = 1000 };

	int                 language;
	int                 nesting;
	bool                opt_const;
	Array<CalcSymbols>  stack;
	Vector<CalcSymbols *> externals;
};

template <>
struct CalcType<const CalcNode *> : public CalcRawType<const CalcNode *>
{
	static Value           ToValue(const CalcNode *cn) { return cn ? RawValue<const CalcNode *>(cn) : Value(); }
	static const CalcNode *ValueTo(Value v)            { return IsType(v) ? RawValue<const CalcNode *>::Extract(v) : 0; }
	static String          Describe();
};

template <>
struct CalcType<CalcNodePtr> : public CalcRawType<CalcNodePtr>
{
	static Value           ToValue(CalcNodePtr cn)     { return !!cn ? RawToValue(cn) : Value(); }
	static CalcNodePtr     ValueTo(Value v)            { return IsTypeRaw<CalcNodePtr>(v) ? UPP::ValueTo<CalcNodePtr>(v) : NULL; }
	static String          Describe();
};

class CalcParser
{
public:
	CalcParser(bool sql_style = false);

	virtual CalcNodePtr   Scan(const char *text);
	virtual CalcNodePtr   ScanVoid(const char *text);

protected:
	virtual CalcNodePtr   ScanAny();
//	virtual CalcNodePtr   ScanLambda();
	virtual CalcNodePtr   ScanSequence();
	virtual CalcNodePtr   ScanSelect();
	virtual CalcNodePtr   ScanLogOr();
	virtual CalcNodePtr   ScanLogAnd();
	virtual CalcNodePtr   ScanEq();
	virtual CalcNodePtr   ScanCompare();
	virtual CalcNodePtr   ScanBitXor();
	virtual CalcNodePtr   ScanBitAnd();
	virtual CalcNodePtr   ScanBitOr();
	virtual CalcNodePtr   ScanAdditive();
	virtual CalcNodePtr   ScanShift();
	virtual CalcNodePtr   ScanMultiplicative();
	virtual CalcNodePtr   ScanPow();
	virtual CalcNodePtr   ScanPrefix();
	virtual CalcNodePtr   ScanPostfix(CalcNodePtr node);
	virtual void          ScanArgs(Vector<CalcNodePtr>& dest);

	void                  Clear(const char *t);

	int                   Skip();
	bool                  Expect(const String& s);

	String                GetIdent();
	Value                 GetNumberOrDate();
	String                GetString();
	String                GetSqlString();

	enum OPERATOR
	{
		OP_NONE,
		OP_QUESTION, OP_COLON, OP_SEMICOLON,
		OP_LOG_AND, OP_LOG_OR,
		OP_LT, OP_LE, OP_GE, OP_GT,
		OP_EQ, OP_NE,
		OP_BIT_AND, OP_BIT_OR, OP_BIT_XOR,
		OP_LSHIFT, OP_RSHIFT,
		OP_ADD, OP_SUB,
		OP_MUL, OP_DIV, OP_MOD,
		OP_POW,
		OP_LOG_NOT, OP_BIT_NOT,
		OP_LAMBDA,
		OP_LPAR, OP_RPAR,
		OP_LBRACKET, OP_RBRACKET,
		OP_LBRACE, OP_RBRACE,
		OP_COMMA,
		OP_DOT, OP_DOTS,
		OP_SQL_BETWEEN, OP_SQL_IN, OP_SQL_LIKE,
		OP_SQL_IS, OP_SQL_NOT, OP_SQL_NULL,

		OP_LAST,
	};

	virtual int           GetOperator();
	int                   SkipOperator();
	bool                  Check(int op);
	bool                  Force(int op, const char *expect);

protected:
	bool                  sql_style;

	const char           *start;
	const char           *pos;

	int                   op_last;
	const char           *op_begin;
	const char           *op_end;
};

template <class BTA, class R>
inline bool CalcCast(CalcPacket& packet, R (*fn)(), BTA * = 0)
{
	if(packet.help)
	{ // generate help info
		packet.result = CalcType<R>::Describe();
		return true;
	}
	if(packet.args.GetCount() != 0)
		return false;
	packet.result = CalcType<R>::ToValue(fn());
	return true;
}

template <class BTA, class R>
inline bool CalcCastPacket(CalcPacket& packet, R (*fn)(CalcPacket& packet), BTA * = 0)
{
	if(packet.help)
	{ // generate help info
		packet.result = CalcType<R>::Describe();
		return true;
	}
	if(packet.args.GetCount() != 0)
		return false;
	packet.result = CalcType<R>::ToValue(fn(packet));
	return true;
}

template <class BTA, class O, class M, class R>
inline bool CalcCastMember(CalcPacket& packet, O *clss, R (M::*fn)(), BTA * = 0)
{
	if(packet.help) { // generate help info
		packet.result = CalcType<R>::Describe();
		return true;
	}
	if(packet.args.GetCount() != 0)
		return false;
	packet.result = CalcType<R>::ToValue((clss ->* fn)());
	return true;
}

template <class BTA, class O, class M, class R>
inline bool CalcCastMemberPacket(CalcPacket& packet, O *clss, R (M::*fn)(), BTA * = 0)
{
	if(packet.help) { // generate help info
		packet.result = CalcType<R>::Describe();
		return true;
	}
	if(packet.args.GetCount() != 0)
		return false;
	packet.result = CalcType<R>::ToValue((clss ->* fn)(packet));
	return true;
}

struct BTA0 { enum { NIL = 0 }; };
struct BTA1 { enum { NIL = 1 }; };

#define E__Class(I)   class C##I
#define E__Arg(I)     C##I
#define E__Descr(I)   packet.args.Add(CalcType<C##I>::Describe())
#define E__ChkArg(I) \
	if(BTA::NIL && IsNull(packet.args[I - 1])) \
		null = true; \
	else if(!CalcType<C##I>::IsType(packet.args[I - 1])) \
		return false;
#define E__CvArg(I) CalcType<C##I>::ValueTo(packet.args[I - 1])

#define CalcCastTemplate(NM, I) \
template <class BTA, class R, __List##I(E__Class)> \
inline bool NM(CalcPacket& packet, R (*fn)(CP_ARG_PREFIX __List##I(E__Arg)), BTA * = 0) \
{ \
	if(packet.help) \
	{ \
		packet.result = CalcType<R>::Describe(); \
		__List##I(E__Descr); \
		return true; \
	} \
	if(packet.args.GetCount() != I) \
		return false; \
	bool null = false; \
	__Expand##I(E__ChkArg) \
	if(BTA::NIL && null) \
	{ \
		packet.result = Value(); \
		return true; \
	} \
	packet.result = CalcType<R>::ToValue(fn(CP_PREFIX \
		__List##I(E__CvArg))); \
	return true; \
}

#define CalcCastMemberTemplate(NM, I) \
template <class BTA, class O, class M, class R, __List##I(E__Class)> \
inline bool NM(CalcPacket& packet, O *clss, R (M::*fn)(CP_ARG_PREFIX __List##I(E__Arg)), BTA * = 0) \
{ \
	if(packet.help) { \
		packet.result = CalcType<R>::Describe();  \
		__List##I(E__Descr); \
		return true; \
	} \
	if(packet.args.GetCount() != I) \
		return false; \
	bool null = false; \
	__Expand##I(E__ChkArg) \
	if(BTA::NIL && null) { \
		packet.result = Value(); \
		return true; \
	} \
	packet.result = CalcType<R>::ToValue((clss ->* fn)(CP_PREFIX \
		__List##I(E__CvArg))); \
	return true; \
} \

#define CP_ARG_PREFIX
#define CP_PREFIX

CalcCastTemplate(CalcCast, 1)
CalcCastTemplate(CalcCast, 2)
CalcCastTemplate(CalcCast, 3)
CalcCastTemplate(CalcCast, 4)
CalcCastTemplate(CalcCast, 5)
CalcCastTemplate(CalcCast, 6)

CalcCastMemberTemplate(CalcCastMember, 1)
CalcCastMemberTemplate(CalcCastMember, 2)
CalcCastMemberTemplate(CalcCastMember, 3)
CalcCastMemberTemplate(CalcCastMember, 4)
CalcCastMemberTemplate(CalcCastMember, 5)
CalcCastMemberTemplate(CalcCastMember, 6)

#undef  CP_ARG_PREFIX
#undef  CP_PREFIX

#define CP_ARG_PREFIX CalcPacket& packet,
#define CP_PREFIX packet,

CalcCastTemplate(CalcCastPacket, 1)
CalcCastTemplate(CalcCastPacket, 2)
CalcCastTemplate(CalcCastPacket, 3)
CalcCastTemplate(CalcCastPacket, 4)
CalcCastTemplate(CalcCastPacket, 5)
CalcCastTemplate(CalcCastPacket, 6)

CalcCastMemberTemplate(CalcCastMemberPacket, 1)
CalcCastMemberTemplate(CalcCastMemberPacket, 2)
CalcCastMemberTemplate(CalcCastMemberPacket, 3)
CalcCastMemberTemplate(CalcCastMemberPacket, 4)
CalcCastMemberTemplate(CalcCastMemberPacket, 5)
CalcCastMemberTemplate(CalcCastMemberPacket, 6)

#undef  CP_ARG_PREFIX
#undef  CP_PREFIX

#define FGENID2(tag, lnum) __##tag##lnum##__
#define FGENID1(tag, lnum) FGENID2(tag, lnum)
#define FGENID(a, b) COMBINE(COMBINE(a, b), __LINE__)

#define SUBEXPAND(M) M

#ifndef NOHELP
#define FDECLTH(tag, topic, id, group, proc) \
static GLOBAL_VARP(HelpCalc, FGENID(chlp, tag), (callback(proc), id, "Calc$" topic, group, ASSTRING(proc))); \
INITBLOCK_(FGENID(hblk, tag)) { FGENID(chlp, tag)(); } \
static void FGENID(chlt, tag)(String& out) { out.Cat(FGENID(chlp, tag)().GetTitle()); } \
RegisterHelpTopicInfoTag(COMBINE(tag, __LINE__), "Calc$" topic, __FILE__, callback(&FGENID(chlt, tag)), CNULL)
#else//NOHELP
#define FDECLTH(tag, topic, id, group, proc)
#endif//NOHELP

#define FDECLT(null, tag, topic, id, group, call) \
static bool FGENID(proc, tag)(CalcPacket& packet) { return CalcCast<null>(packet, call); } \
INITBLOCK_(FGENID(gblk, tag)) { static CalcContext::Global FGENID(init, tag)(id, &FGENID(proc, tag)); } \
FDECLTH(tag, topic, id, group, &FGENID(proc, tag))

#define FDECLTA(null, tag, topic, id, group, call) \
static bool FGENID(proc, tag)(CalcPacket& packet) { return CalcCastPacket<null>(packet, call); } \
INITBLOCK_(FGENID(gblk, tag)) { static CalcContext::Global FGENID(init, tag)(id, &FGENID(proc, tag)); } \
FDECLTH(tag, topic, id, group, &FGENID(proc, tag))

#define FDECLQ(topic, id, group, gate) \
INITBLOCK_(FGENID(gblk, tag)) { static CalcContext::Global FGENID(init, dflt)(id, gate); } \
FDECLTH(dflt, topic, id, group, gate)

#define FDECL(id, call, group)        FDECLT(BTA0, dflt, ASSTRING(call),               id,           group, call)
#define FDECLX(id, group)             FDECLT(BTA0, id,  ASSTRING(id),                 ASSTRING(id), group, id)
#define FDECLP(id, x, group)          FDECLT(BTA0, id,  "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

#define FDECL0(id, call, group)       FDECLT(BTA1, dflt, ASSTRING(call),               id,           group, call)
#define FDECLX0(id, group)            FDECLT(BTA1, id,  ASSTRING(id),                 ASSTRING(id), group, id)
#define FDECLP0(id, x, group)         FDECLT(BTA1, id,  "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

#define FDECLA(id, call, group)       FDECLTA(BTA0, dflt, ASSTRING(call),               id,           group, call)
#define FDECLAX(id, group)            FDECLTA(BTA0, id,  ASSTRING(id),                 ASSTRING(id), group, id)
#define FDECLAP(id, x, group)         FDECLTA(BTA0, id,  "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

#define FDECLA0(id, call, group)      FDECLTA(BTA1, dflt, ASSTRING(call),               id,           group, call)
#define FDECLAX0(id, group)           FDECLTA(BTA1, id,  ASSTRING(id),                 ASSTRING(id), group, id)
#define FDECLAP0(id, x, group)        FDECLTA(BTA1, id,  "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

#define MDECLT(null, topic, id, group, call) \
struct FGENID(clcp, SUBEXPAND(MFILEID)) : public BASECLASS \
{ FGENID(clcp, SUBEXPAND(MFILEID))(); bool CalcIt(CalcPacket& packet) { return CalcCastMember<null>(packet, this, &BASECLASS::call); } }; \
static GLOBAL_VARP(HelpCalc, FGENID(chlp, SUBEXPAND(MFILEID)), (callback(static_cast<BASECLASS *>(0), \
		brutal_cast<bool (BASECLASS::*)(CalcPacket&)>(&FGENID(clcp, SUBEXPAND(MFILEID))::CalcIt)), \
		id, "Calc$" topic, group, ASSTRING(call))); \
INITBLOCK_(FGENID(mblk, tag)) { \
	FGENID(chlp, SUBEXPAND(MFILEID))(); \
	static CalcLocalItem<BASECLASS> FGENID(clci, SUBEXPAND(MFILEID))(BASECLASS::GetLocalMap(), id, \
			brutal_cast<bool (BASECLASS::*)(CalcPacket&)>(&FGENID(clcp, SUBEXPAND(MFILEID))::CalcIt)); \
} \
static void FGENID(chlt, SUBEXPAND(MFILEID))(String& out) { out.Cat(FGENID(chlp, SUBEXPAND(MFILEID))().GetTitle()); } \
RegisterHelpTopicInfo("Calc$" topic, __FILE__, callback(&FGENID(chlt, SUBEXPAND(MFILEID))), CNULL)

#define MDECLTA(null, topic, id, group, call) \
struct FGENID(clcp, SUBEXPAND(MFILEID)) : public BASECLASS \
{ FGENID(clcp, SUBEXPAND(MFILEID))(); bool CalcIt(CalcPacket& packet) { return CalcCastMemberPacket<null>(packet, this, &BASECLASS::call); } }; \
static GLOBAL_VARP(HelpCalc, FGENID(chlp, SUBEXPAND(MFILEID)), (callback(static_cast<BASECLASS *>(0), \
		brutal_cast<bool (BASECLASS::*)(CalcPacket&)>(&FGENID(clcp, SUBEXPAND(MFILEID))::CalcIt)), \
		id, "Calc$" topic, group, ASSTRING(call))); \
INITBLOCK_(FGENID(mblk, tag)) { \
	FGENID(chlp, SUBEXPAND(MFILEID))(); \
	static CalcLocalItem<BASECLASS> FGENID(clci, SUBEXPAND(MFILEID))(BASECLASS::GetLocalMap(), id, \
			brutal_cast<bool (BASECLASS::*)(CalcPacket&)>(&FGENID(clcp, SUBEXPAND(MFILEID))::CalcIt)); \
} \
static void FGENID(chlt, SUBEXPAND(MFILEID))(String& out) { out.Cat(FGENID(chlp, SUBEXPAND(MFILEID))().GetTitle()); } \
RegisterHelpTopicInfo("Calc$" topic, __FILE__, callback(&FGENID(chlt, SUBEXPAND(MFILEID))), CNULL)

#define MDECL(id, x, group)   MDECLT(BTA0, ASSTRING(call),               id,           group, call)
#define MDECL0(id, x, group)  MDECLT(BTA1, ASSTRING(id),                 id,           group, COMBINE3(C, id, x))
#define MDECLP(id, x, group)  MDECLT(BTA0, "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))
#define MDECLP0(id, x, group) MDECLT(BTA1, "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

#define MDECLA(id, x, group)   MDECLTA(BTA0, ASSTRING(call),               id,           group, call)
#define MDECLA0(id, x, group)  MDECLTA(BTA1, ASSTRING(id),                 id,           group, COMBINE3(C, id, x))
#define MDECLAP(id, x, group)  MDECLTA(BTA0, "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))
#define MDECLAP0(id, x, group) MDECLTA(BTA1, "C" ASSTRING(id) ASSTRING(x), ASSTRING(id), group, COMBINE3(C, id, x))

END_UPP_NAMESPACE
