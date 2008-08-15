#ifndef DOCPP_H
#define DOCPP_H

#include <Core/Core.h>

int           GetCppFileIndex(const String& path);
const String& GetCppFile(int i);

enum {
	Tmarker_before_first = 255,
#define CPPID(x)   tk_##x,
#include "keyword.i"
#undef  CPPID
};

class CppWordsHash {
	dword w[4];

public:
	void Clear()  { w[0] = w[1] = w[2] = w[3] = 0; }
	void SetAll() { w[0] = w[1] = w[2] = w[3] = ~0; }
	void AddWord(const String& s);
	void AddWords(const char *s);

	bool operator&(const CppWordsHash& b) const {
		return (w[0] & b.w[0]) | (w[1] & b.w[1]) | (w[2] & b.w[2]) | (w[3] & b.w[3]);
	}

	bool IsAll() const { return (w[0] & w[1] & w[2] & w[3]) == (dword)~0; }

	CppWordsHash() { Clear(); }
};

CppWordsHash AllCppWords();

enum {
	t_eof,
	t_string = -200,
	t_integer,
	t_double,
	t_character,
	t_dblcolon,
	t_mulass,
	t_divass,
	t_modass,
	t_xorass,
	t_neq,
	t_dot_asteriks,
	t_elipsis,
	t_inc,
	t_addass,
	t_dec,
	t_arrow_asteriks,
	t_arrow,
	t_subass,
	t_and,
	t_andass,
	t_or,
	t_orass,
	t_eq,
	t_shl,
	t_shlass,
	t_le,
	t_shr,
	t_shrass,
	t_ge,
	te_integeroverflow,
	te_badcharacter,
	te_badstring,
};

const char **CppKeyword();
bool  InScList(const char *s, const char *list);

class LexSymbolStat
{
public:
	LexSymbolStat();
	void Reset(int minSymbol, int maxSymbol);
	void IncStat(int symbol);
	int  GetStat(int symbol) const;
	int  SumStat(const Vector<int> & symbols) const;
	void Merge(const LexSymbolStat & other);

private:
	Vector<int> v;
	int minSymbol;
};


class Lex {
#ifdef _DEBUG
	const char *pp;
#endif
	const char *ptr;
	const char *pos;

	Index<String> id;
	Index<int>    ignore;
	int           endkey;
	int           braceslevel;
	int           ignore_low, ignore_high;

	struct Term  : Moveable<Term>{
		const  char *ptr;
		int    code;
		String text;
		double number;
	};

	bool statsCollected;
	LexSymbolStat symbolStat;
	BiVector<Term> term;


	bool Char(int c)                 { if(*ptr == c) { ptr++; return true; } else return false; }
	void AddCode(int code)           { Term& tm = term.AddTail(); tm.code = code; tm.ptr = pos; }
	void AssOp(int noass, int ass)   { AddCode(Char('=') ? ass : noass); }
	void Next();
	bool Prepare(int pos);
	int  GetCharacter();
public:
	int         Code(int pos = 0);
	bool        IsId(int pos = 0);
	String      Id(int pos = 0);
	int         Int(int pos = 0);
	double      Double(int pos = 0);
	int         Chr(int pos = 0);
	String      Text(int pos = 0);

	void        Get(int n = 1);
	int         GetCode()                   { int q = Code(); Get(); return q; }
	String      GetId()                     { String q = Id(); Get(); return q; }
	int         GetInt()                    { int q = Int(); Get(); return q; }
	double      GetDouble()                 { double q = Double(); Get(); return q; }
	int         GetChr()                    { int q = Chr(); Get(); return q; }
	String      GetText()                   { String q = Text(); Get(); return q; }

	int         Id(const String& s)         { return id.FindAdd(s) + 256; }

	int         GetBracesLevel() const      { return braceslevel; }

	const char *Pos(int pos = 0);
	int         operator[](int pos)         { return Code(pos); }
	operator    int()                       { return Code(0); }
	void        operator++()                { Get(); }

	void        Init(const char *s, const Vector<String>& ignore);
	void        StartStatCollection();
	const LexSymbolStat & FinishStatCollection();

	Lex();
};

struct SrcFile {
	SrcFile();
	String      text;
	Vector<int> linepos;
	int preprocessorLinesRemoved;
	int blankLinesRemoved;
	int commentLinesRemoved;
};

SrcFile PreProcess(Stream& in);

enum Kind {
	STRUCT,
	STRUCTTEMPLATE,
	TYPEDEF,
	CONSTRUCTOR,
	DESTRUCTOR,
	FUNCTION,
	INSTANCEFUNCTION,
	CLASSFUNCTION,
	FUNCTIONTEMPLATE,
	INSTANCEFUNCTIONTEMPLATE,
	CLASSFUNCTIONTEMPLATE,
	INLINEFRIEND,
	VARIABLE,
	INSTANCEVARIABLE,
	CLASSVARIABLE,
	ENUM,
	MACRO,
};

inline bool IsCppType(int i)
{
	return i >= STRUCT && i <= TYPEDEF;
}

inline bool IsCppCode(int i) {
	return i >= CONSTRUCTOR && i <= INLINEFRIEND;
};

inline bool IsCppData(int i) {
	return i >= VARIABLE && i <= ENUM;
}

inline bool IsCppMacro(int i) {
	return i == MACRO;
}

inline bool IsCppTemplate(int i) {
	return i == STRUCTTEMPLATE || i >= FUNCTIONTEMPLATE && i <= CLASSFUNCTIONTEMPLATE;
}

enum {
	PUBLIC,
	PROTECTED,
	PRIVATE,
};

struct CppPos : Moveable<CppPos> {
	bool   impl;
	int    line;
	int    file;

	String GetFile() const                       { return GetCppFile(file); }
//	void   Serialize(Stream& s)                  { s % impl % line % file; }

	CppPos()                                     { line = 0; impl = false; }
};

struct CppSimpleItem {
	String         natural;
	String         type;
	String         qtype;
	String         tparam;
	String         param;
	String         pname;
	String         ptype;
	String         qptype;
	String         tname;
	byte           access;
	byte           kind;
	int16          at;
	bool           virt;
	bool           decla;

	bool           IsType() const      { return IsCppType(kind); }
	bool           IsCode() const      { return IsCppCode(kind); }
	bool           IsData() const      { return IsCppData(kind); }
	bool           IsMacro() const     { return IsCppMacro(kind); }
	bool           IsTemplate() const  { return IsCppTemplate(kind); }

	CppSimpleItem() { decla = false; virt = false; at = 0; }
};

struct CppItem : CppSimpleItem {
	Vector<CppPos> pos;
	String         key;
	CppWordsHash   words;

	void Serialize(Stream& s);

	CppItem()      { words.SetAll(); }
};

struct CppNest {
	int            namespacel;
	Index<String>  key;
	Index<String>  name;
	Array<CppItem> item;

	void           Remove(const Vector<int>& rm)                 { key.Remove(rm); name.Remove(rm); item.Remove(rm); }
	int            GetCount() const                              { return item.GetCount(); }
	const CppItem& operator[](int i) const                       { return item[i]; }
	CppItem&       GetAdd(const String& key, const String& name);

	CppNest()                                                    { namespacel = 0; }
};

struct CppBase {
	ArrayMap<String, CppNest> nest;

	CppNest&       operator[](int i)                             { return nest[i]; }
	int            GetCount() const                              { return nest.GetCount(); }
	const String&  GetKey(int i) const                           { return nest.GetKey(i); }
	void           Clear()                                       { nest.Clear(); }
	int            Find(const String& s) const                   { return nest.Find(s); }
	CppNest&       GetAdd(const String& s)                       { return nest.GetAdd(s); }

	bool           IsType(int i) const;

//	Vector<int> GetBases(int i) const;
};

class Parser {
	struct Context {
		int         namespacel;
		String      nesting;
		Vector<int> tparam;
		Index<int>  typenames;
		int         access;
		bool        noclass;

		void operator<<=(const Context& t);

		String  Dump() const;
	};

	struct Decla {
		bool    s_static:1;
		bool    s_extern:1;
		bool    s_register:1;
		bool    s_auto:1;
		bool    s_mutable:1;
		bool    s_explicit:1;
		bool    s_virtual:1;
		String  name;
		bool    function:1;
		bool    type_def:1;
		bool    isfriend:1;
		bool    istemplate:1;
		bool    istructor:1;
		bool    isdestructor:1;
		bool    isptr:1;
		bool    nofn:1;

		String  tnames;
		String  type;
		String  natural;

		Decla();
	};

	struct Decl : Decla {
		Array<Decl> param;
	};

	struct RecursionCounter
	{
		int change;
		int &count;
		RecursionCounter(int &count_i, int change_i = 1) :
		  count(count_i), change(change_i)
		{
		  count += change;
		}
		~RecursionCounter() { count -= change; }
	};

	Context     context;
	SrcFile     file;
	Lex         lex;
	int         filei;
	bool        inbody;

	Callback2<int, const String&> err;

	int     lpos, line;

	CppBase *base;

	int    RPtr();

	bool   Key(int code);
	bool   EatBody();

	void   Cv();
	String SimpleType(Decla& d);
	void   Qualifier();
	void   ParamList(Decl& d);
	void   Declarator(Decl& d, const char *p);
	void   EatInitializers();
	Decl   Type();
	void   Vars(Array<Decl>& r, const char *p, bool type_def, bool more);
	Array<Decl> Declaration(bool l0 = false, bool more = false);
	void   Elipsis(Decl& d);
	Decl&  Finish(Decl& d, const char *p);
	bool   Nest(const String& tp, const String& tn);

	String TemplateParams(String& pnames);
	String TemplateParams();
	String TemplatePnames();
	String Name(String& h);
	String Name();
	String Constant();
	String ReadOper();

	int    GetLine(const char *pos);
	void   Line();
	void   Check(bool b, const char *err);
	void   CheckKey(int c);

	void   SetNestCurrent();
	void   NestBody();
	void   Do();

	CppItem& Item(const String& nesting, const String& item, const String& name, bool impl);
	CppItem& Item(const String& nesting, const String& item, const String& name);

	CppItem& Fn(const Decl& d, const String& templ, bool body, int kind);

	struct Error {};

	void   ThrowError(const String& e);
	void   Resume(int bl);

	void   MatchPars();
	bool   TryDecl();
	void   Statement();
	void   Locals(const String& type);
	String Tparam(int& q);

public:
	struct FunctionStat
	{
		FunctionStat(const String & nesting,
		             const CppItem & cppItem,
		             const LexSymbolStat &symbolStat,
		             int maxScopeDepth);
		String nesting;
		const CppItem & cppItem;
		const LexSymbolStat &symbolStat;
		int maxScopeDepth;
	};

	bool                      IsInBody() const                   { return inbody; }

	typedef Callback1<const FunctionStat &> FnEndCallback;

	bool                      dobody;
	String                    current_nest;
	int                       current_namespacel;
	String                    current_key;
	CppItem                   current;
	int                       currentScopeDepth;
	int                       maxScopeDepth;
	VectorMap<String, String> local;
	FnEndCallback             whenFnEnd;
	LexSymbolStat             symbolsOutsideFunctions;

	const SrcFile &getPreprocessedFile() { return file; }

	void   Do(Stream& s, const Vector<String>& ignore, CppBase& base, const String& fn,
	          Callback2<int, const String&> err, const Vector<String>& typenames = Vector<String>());

	Parser() : dobody(false) {}
};

String NoTemplatePars(const String& type);

class Nestfo {
	bool           bvalid, nvalid;
	Vector<String> baselist;
	Vector<String> nests;
	int            nesti;
	void           Bases(int i, Vector<int>& g);
	void           Init();

public:
	const CppBase&            base;
	VectorMap<String, String> cache;

	const Vector<String>& GetBases();
	const Vector<String>& GetNests();
	int                   GetNest() const               { return nesti; }
	void                  NoBases()                     { baselist.Clear(); bvalid = true; }

	Nestfo(const CppBase& base, int nesti = -1);
	Nestfo(int nesti, const CppBase& base);
	Nestfo(const CppBase& base, const String& nest);
	Nestfo(const Nestfo& f);
};

String Qualify(Nestfo& nf, const String& type);
String Qualify(const CppBase& base, const String& nest, const String& type);
void   QualifyTypes(CppBase& base, const String& nest, CppItem& m);
String QualifyKey(const CppBase& base, const String& nest, const String& type);

void   Qualify(CppBase& base, const CppWordsHash& words);

void Parse(Stream& s, const Vector<String>& ignore, CppBase& base, const String& fn,
           Callback2<int, const String&> err);
void Remove(CppBase& base, const Vector<String>& fn);

#endif
