#ifndef DOCPP_H
#define DOCPP_H

#include <Core/Core.h>

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

class Lex {
#ifdef _DEBUG
	const char *pp;
#endif
	const char *ptr;
	const char *pos;

	Index<String> id;
	Index<int>    ignore;
	int           endkey;

	struct Term  : Moveable<Term>{
		const  char *ptr;
		int    code;
		String text;
		double number;
	};

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

	const char *Pos(int pos = 0);
	int         operator[](int pos)         { return Code(pos); }
	operator    int()                       { return Code(0); }
	void        operator++()                { Get(); }

	void        operator=(const char *s)    { ptr = s; term.Clear(); }
	void        Ignore(const Vector<String>& ignore);

	Lex();
};

struct SrcFile {
	String      text;
	Vector<int> linepos;
};

SrcFile PreProcess(Stream& in);


enum Kind {
	MACRO, ENUM, TYPEDEF, VARIABLE,
	STRUCT, FUNCTION,
	INSTANCEFUNCTION, INSTANCEVARIABLE,
	CLASSFUNCTION, CLASSVARIABLE,
	CONSTRUCTOR, DESTRUCTOR,
	STRUCTTEMPLATE, FUNCTIONTEMPLATE,
	INSTANCEFUNCTIONTEMPLATE, CLASSFUNCTIONTEMPLATE,
	INLINEFRIEND,
	OTHER
};

enum {
	PRIVATE, PROTECTED, PUBLIC
};

struct CppItem {
	String         package;

	int            kind;
	String         name;
	int            access;
	String         tparam;
	String         header;
	String         param;
	String         ender;
	String         pname;
	String         tname;

	String         file;
	int            line;

	CppItem()      { line = 0; }
};

typedef ArrayMap<String, CppItem>      CppNest;
typedef ArrayMap<String, CppNest>      CppNamespace;
typedef ArrayMap<String, CppNamespace> CppBase;

class Parser {
	struct Context {
		String      nameing;
		String      nesting;
		Index<int>  typenames;
		int         access;

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

		String  header;
		String  ender;
		String  template_params;
		String  tnames;
		String  natural;

		Decla();
	};

	struct Decl : Decla {
		Array<Decl> param;
	};

	SrcFile file;
	Lex     lex;
	String  package, filename;
	Context context;

	int     lpos, line;

	CppBase *base;

	int    RPtr();

	bool   Key(int code);
	bool   EatBody();

	void   Cv();
	String SimpleType();
	void   Qualifier();
	void   ParamList(Decl& d);
	void   Declarator(Decl& d, const char *p);
	void   EatInitializers();
	Decl   Type();
	Array<Decl> Declaration(bool l0 = false, bool more = false);
	void   Do();
	void   Elipsis(Decl& d);
	Decl&  Finish(Decl& d, const char *p);
	bool   Nest(const String& tp, const String& tn);

	String TemplateParams(String& pnames);
	String TemplateParams();
	String TemplatePnames();
	String Name();
	String Constant();
	String ReadOper();

	int    GetLine(const char *pos);
	void   Line();
	void   Check(bool b, const char *err);
	void   CheckKey(int c);

	CppItem& Item(const String& nameing, const String& nesting, const String& item);

public:
	struct Error : public String { Error() {}; Error(const char *s) : String(s) {} };

	void   ThrowError(const String& e);

	void   Do(Stream& s, const Vector<String>& ignore, CppBase& base,
	          const String& package, const String& file)
	  throw(Parser::Error);
};

void Parse(Stream& s, const Vector<String>& ignore, CppBase& base,
           const String& package, const String& file) throw(Parser::Error);

#endif
