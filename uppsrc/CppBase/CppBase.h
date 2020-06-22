#ifndef DOCPP_H
#define DOCPP_H

#include <Core/Core.h>

namespace Upp {
	
// These functions can only but executed by single thread, unless said otherwise
	
bool   IsCPPFile(const String& file);
bool   IsHFile(const String& path);

String GetStdDefs();

void   SetPPDefs(const String& defs);

void   PPSync(const String& include_path);

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
	NAMESPACE,
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

void   CleanPP();
void   SerializePPFiles(Stream& s);
void   SweepPPFiles(const Index<String>& keep);

void InvalidateFileTimeCache();
void InvalidateFileTimeCache(const String& path);
Time GetFileTimeCached(const String& path);

String               NormalizeSourcePath(const String& path, const String& currdir);
String               NormalizeSourcePath(const String& path);

void                 ClearSources();
const Index<String>& GetAllSources();
void                 GatherSources(const String& master_path, const String& path_);
String               GetMasterFile(const String& file);
const VectorMap<String, String>& GetAllSourceMasters();

const char **CppKeyword();

enum CppAccess {
	PUBLIC,
	PROTECTED,
	PRIVATE,
};

enum FileTypeEnum {
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
	String         tparam; // complete template parameters list, like '<class T>'
	String         param;
	String         pname;
	String         ptype; // fn: types of parameters, struct: base classes
	String         qptype;
	String         tname;
	String         ctname;
	String         using_namespaces;
	byte           access = PUBLIC;
	byte           kind = STRUCT;
	int16          at = 0;
	bool           virt = false;

	bool           decla = false;
	bool           lvalue = false;
	bool           isptr = false;
	
	byte           filetype = FILE_OTHER;
	bool           impl = false;
	int            file = 0;
	int            line = 0;

	bool           qualify = true;

	bool           IsType() const      { return IsCppType(kind); }
	bool           IsCode() const      { return IsCppCode(kind); }
	bool           IsData() const      { return IsCppData(kind); }
	bool           IsMacro() const     { return IsCppMacro(kind); }
	bool           IsTemplate() const  { return IsCppTemplate(kind); }
	
	void           Serialize(Stream& s);
	
	void           Dump(Stream& s) const;
	String         ToString() const;
};

int FindItem(const Array<CppItem>& x, const String& qitem);
int FindName(const Array<CppItem>& x, const String& name, int i = 0);

struct CppBase : ArrayMap<String, Array<CppItem> > {
	String         types_md5;
	Index<String>  namespaces;

	bool           IsType(int i) const;
	void           Sweep(const Index<int>& file, bool keep = true);
	void           RemoveFiles(const Index<int>& remove_file) { Sweep(remove_file, false); }
	void           RemoveFile(int filei);
	void           Append(CppBase&& base);

	void           Dump(Stream& s);
};

class ScopeInfo { // information about scope
	bool           bvalid; // baselist is valid
	bool           nvalid; // scopes is valid
	Vector<String> baselist; // list of all base classes of scope
	Vector<String> scopes; // list of scopes (Upp::String::Init::, Upp::String::, Upp::)
	int            scopei; // index of this scope in base
	String         usings; // using namespaces contained in scopes
	
	void           Bases(int i, Vector<int>& g);
	void           Init();

public:
	const CppBase&            base;
	VectorMap<String, String> cache;

	const Vector<String>& GetBases();
	const Vector<String>& GetScopes(const String& usings);
	int                   GetScope() const              { return scopei; }
	void                  NoBases()                     { baselist.Clear(); bvalid = true; }

	ScopeInfo(const CppBase& base, int scopei = -1);
	ScopeInfo(int scopei, const CppBase& base);
	ScopeInfo(const CppBase& base, const String& scope);
	ScopeInfo(const ScopeInfo& f);
};

struct ParserContext {
	struct Context {
		String         ns;
		String         scope;
		String         ctname;
		Vector<int>    tparam;
		Index<int>     typenames;
		int            access;
		String         namespace_using;

		void operator<<=(const Context& t);

		String  Dump() const;
		
		Context() {}
		rval_default(Context);
	};

	Context     context;
	String      current_scope;
	String      current_key;
	String      current_name;
	CppItem     current;
	bool        inbody;

	struct Local : Moveable<Local> {
		String type;
		bool   isptr;
		int    line;
	};

	VectorMap<String, Local>  local;

	Vector<String> GetNamespaces() const;
	bool           IsInBody() const                   { return inbody; }
};

// Parse CAN be run in parallel
void Parse(CppBase& base, const String& src, int file, int filetype, const String& path,
           Event<int, const String&> error, const Vector<String>& namespace_stack,
           const Index<String>& namespace_using);


void PreprocessParse(CppBase& base, Stream& in, int file, const String& path,
                     Event<int, const String&> error);
String PreprocessCpp(const String& src, const String& path);

ParserContext AssistParse(CppBase& base, const String& src, const String& path_, Event<int, const String&> error);

void SimpleParse(CppBase& cpp, const String& txt, const String& cls);

void   Qualify(CppBase& base);

String Qualify(const CppBase& base, const String& scope, const String& type, const String& usings);

String ResolveTParam(const CppBase& codebase, const String& type, const Vector<String>& tparam);
void   ResolveTParam(const CppBase& codebase, Vector<String>& type, const Vector<String>& tparam);

String ParseTemplatedType(const String& type, Vector<String>& tparam);

bool HasCPPFileKeyword(const String& path, const String& id);

String GetPPMD5(const String& fn);
String GetDependeciesMD5(const String& path, Index<String>& visited);

Index<String> GetExpressionType(const CppBase& codebase, const ParserContext& parser, const Vector<String>& xp);

}

#endif
