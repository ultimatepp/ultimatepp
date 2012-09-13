#ifndef DOCPP_H
#define DOCPP_H

#include <Core/Core.h>

NAMESPACE_UPP

int           GetCppFileIndex(const String& path);
const String& GetCppFile(int i);

enum {
	Tmarker_before_first = 255,
#define CPPID(x)   tk_##x,
#include "keyword.i"
#undef  CPPID
};

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
		bool   grounding;
		
		Term() { grounding = false; }
	};

	bool statsCollected;
	LexSymbolStat symbolStat;
	BiVector<Term> term;
	int            body;
	bool           grounding;

	bool Char(int c)                 { if(*ptr == c) { ptr++; return true; } else return false; }
	void AddCode(int code)           { Term& tm = term.AddTail(); tm.code = code; tm.ptr = pos; tm.grounding = grounding; }
	void AssOp(int noass, int ass)   { AddCode(Char('=') ? ass : noass); }
	void Next();
	bool Prepare(int pos);
	int  GetCharacter();
	void ThrowError(const char *e);

public:
	Callback1<const String&> WhenError;

	struct Grounding {};

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
	void        ClearBracesLevel()          { braceslevel = 0; }
	
	void        BeginBody()                 { body++; }
	void        EndBody()                   { body--; }
	void        ClearBody()                 { body = 0; }
	bool        IsBody() const              { return body; }
	void        SkipToGrounding();

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
	FRIENDCLASS,
};

inline bool IsCppType(int i)
{
	return i >= STRUCT && i <= TYPEDEF || i == FRIENDCLASS;
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

enum {
	FILE_H,
	FILE_HPP,
	FILE_CPP,
	FILE_C,
	FILE_OTHER,
};

struct CppItem {
	String         qitem;
	String         item;
	String         name;
	String         uname;
	String         natural;
	String         type;
	String         qtype;
	String         tparam;
	String         param;
	String         pname;
	String         ptype;
	String         qptype;
	String         tname;
	String         ctname;
	byte           access;
	byte           kind;
	int16          at;
	bool           virt;
	bool           decla;
	bool           lvalue;
	bool           isptr;
	
	byte           filetype;
	bool           impl;
	int            file;
	int            line;

	byte           qualify_type, qualify_param;
	int            serial;

	bool           IsType() const      { return IsCppType(kind); }
	bool           IsCode() const      { return IsCppCode(kind); }
	bool           IsData() const      { return IsCppData(kind); }
	bool           IsMacro() const     { return IsCppMacro(kind); }
	bool           IsTemplate() const  { return IsCppTemplate(kind); }
	
	void           Serialize(Stream& s);

	CppItem()      { at = decla = virt = false; qualify_type = qualify_param = true; serial = -1; isptr = false; }
};

int FindItem(const Array<CppItem>& x, const String& qitem);
int GetCount(const Array<CppItem>& x, int i);
int FindNext(const Array<CppItem>& x, int i);
int FindName(const Array<CppItem>& x, const String& name, int i = 0);

struct CppBase : ArrayMap<String, Array<CppItem> > {
	int            serial;
	String         serial_md5;

	bool           IsType(int i) const;
	
	CppBase() { serial = 0; }
};

class Parser {
	struct Context {
		String      scope;
		String      ctname;
		Vector<int> tparam;
		Index<int>  typenames;
		int         access;

		void operator<<=(const Context& t);

		String  Dump() const;
	};

	struct Decla {
		bool    s_static;
		bool    s_extern;
		bool    s_register;
		bool    s_auto;
		bool    s_mutable;
		bool    s_explicit;
		bool    s_virtual;
		String  name;
		bool    function;
		bool    type_def;
		bool    isfriend;
		bool    istemplate;
		bool    istructor;
		bool    isdestructor;
		bool    isptr;
		bool    nofn;
		bool    castoper;

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
		  change(change_i), count(count_i)
		{
		  count += change;
		}
		~RecursionCounter() { count -= change; }
	};

	Context     context;
	SrcFile     file;
	Lex         lex;
	int         filei;
	byte        filetype;
	bool        inbody;

	Callback2<int, const String&> err;

	int     lpos, line;

	CppBase *base;

	int    RPtr();

	bool   Key(int code);
	bool   EatBody();

	void   Cv();
	String TType();
	String SimpleType(Decla& d);
	void   Qualifier();
	void   ParamList(Decl& d);
	void   Declarator(Decl& d, const char *p);
	void   EatInitializers();
	Decl   Type();
	void   Vars(Array<Decl>& r, const char *p, bool type_def, bool more);
	Array<Decl> Declaration0(bool l0 = false, bool more = false);
	Array<Decl> Declaration(bool l0 = false, bool more = false);
	bool   IsParamList(int q);
	void   Elipsis(Decl& d);
	Decl&  Finish(Decl& d, const char *p);
	bool   Scope(const String& tp, const String& tn);

	String TemplateParams(String& pnames);
	String TemplateParams();
	String TemplatePnames();
	String Name(String& h, bool& castoper);
	String Name(bool& castoper);
	String Constant();
	String ReadOper(bool& castoper);

	int    GetLine(const char *pos);
	void   Line();
	void   Check(bool b, const char *err);
	void   CheckKey(int c);

	void   SetScopeCurrent();
	void   ScopeBody();
	void   Do();

	CppItem& Item(const String& scope, const String& item, const String& name, bool impl);
	CppItem& Item(const String& scope, const String& item, const String& name);

	CppItem& Fn(const Decl& d, const String& templ, bool body,
	            const String& tname, const String& tparam);

	struct Error {};

	void   ThrowError(const String& e);
	void   Resume(int bl);

	void   MatchPars();
	bool   TryDecl();
	void   Statement();
	void   Locals(const String& type);
	String Tparam(int& q);
	
	friend class Lex; // Fix to make Lex::ThrowError

	typedef Parser CLASSNAME;

public:
	struct FunctionStat
	{
		FunctionStat(const String & scope,
		             const CppItem & cppItem,
		             const LexSymbolStat &symbolStat,
		             int maxScopeDepth);
		String scope;
		const CppItem & cppItem;
		const LexSymbolStat &symbolStat;
		int maxScopeDepth;
	};

	bool                      IsInBody() const                   { return inbody; }

	typedef Callback1<const FunctionStat &> FnEndCallback;

	bool                      dobody;
	String                    current_scope;
	String                    current_key;
	String                    current_name;
	CppItem                   current;
	int                       currentScopeDepth;
	int                       maxScopeDepth;

	struct Local : Moveable<Local> {
		String type;
		bool   isptr;
	};

	VectorMap<String, Local>  local;
	FnEndCallback             whenFnEnd;
	LexSymbolStat             symbolsOutsideFunctions;

	const SrcFile &getPreprocessedFile() { return file; }

	void   Do(Stream& s, const Vector<String>& ignore, CppBase& base, const String& fn,
	          Callback2<int, const String&> err, const Vector<String>& typenames = Vector<String>());

	Parser() : dobody(false) { 	lex.WhenError = THISBACK(ThrowError); }
};

String NoTemplatePars(const String& type);

class Scopefo {
	bool           bvalid, nvalid;
	Vector<String> baselist;
	Vector<String> scopes;
	int            scopei;
	void           Bases(int i, Vector<int>& g);
	void           Init();

public:
	const CppBase&            base;
	VectorMap<String, String> cache;

	const Vector<String>& GetBases();
	const Vector<String>& GetScopes();
	int                   GetScope() const               { return scopei; }
	void                  NoBases()                     { baselist.Clear(); bvalid = true; }

	Scopefo(const CppBase& base, int scopei = -1);
	Scopefo(int scopei, const CppBase& base);
	Scopefo(const CppBase& base, const String& scope);
	Scopefo(const Scopefo& f);
};

String Qualify(const CppBase& base, const String& scope, const String& type);
void   QualifyTypes(CppBase& base, const String& scope, CppItem& m);
String QualifyKey(const CppBase& base, const String& scope, const String& type);

void   Qualify(CppBase& base);

void Parse(Stream& s, const Vector<String>& ignore, CppBase& base, const String& fn,
           Callback2<int, const String&> err);
void Remove(CppBase& base, const Vector<String>& fn);

END_UPP_NAMESPACE

#endif
