#include "clang.h"

bool LibClangEnabled = true;
bool AssistDiagnostics;
bool AutoIndexer;
bool RelaxedIndexerDependencies = true;
int  IndexerThreads;
int  ParsedFiles;
int  LibClangCppVersion = 17;

void ClangConfigSerialize(Stream& s)
{
	int version = 1;
	s % version
	  % LibClangEnabled
	  % AssistDiagnostics
	  % AutoIndexer
	  % IndexerThreads
	  % ParsedFiles
	;
	
	if(version >= 1)
		s % LibClangCppVersion;
}

void ClangConfigSetDefaults()
{
	uint64 total, avail;
	GetSystemMemoryStatus(total, avail);
	int mem_mb = int(total >> 20);
	
	ParsedFiles = clamp((mem_mb - 4000) / 500, 1, 12);

	IndexerThreads = max(min(CPU_Cores() - 2, (mem_mb - 4000) / 1000), 1);

//	AutoIndexer = CPU_Cores() >= 8 && mem_mb > 8000;
	AutoIndexer = true;

	LibClangCppVersion = 17;
}

INITBLOCK {
	ClangConfigSetDefaults();
}

void PutAssist(const char *s)
{
	GuiLock __;
	if(AssistDiagnostics)
		PutConsole(s);
}

bool IsCppSourceFile(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	return findarg(ext, ".cpp", ".cc", ".cxx", ".icpp") >= 0;
}

bool IsSourceFile(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	return findarg(ext, ".cpp", ".cc", ".cxx", ".icpp", ".c") >= 0;
}

bool IsHeaderFile(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	return findarg(ext, ".h", ".hxx", ".hpp", ".hh") >= 0;
}

bool IsTypedef(int kind)
{
	return kind == CXCursor_TypedefDecl;
}

bool IsStruct(int kind)
{
	return findarg(kind, CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl,
	                     CXCursor_ClassTemplate) >= 0;
}

bool IsTemplate(int kind)
{
	return findarg(kind, CXCursor_FunctionTemplate, CXCursor_ClassTemplate) >= 0;
}

bool IsFunction(int kind)
{
	return findarg(kind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_Constructor,
	                     CXCursor_Destructor, CXCursor_ConversionFunction, CXCursor_CXXMethod) >= 0;
}

bool IsMethod(int kind)
{
	return kind == CXCursor_CXXMethod;
}

bool IsVariable(int kind)
{
	return findarg(kind, CXCursor_VarDecl, CXCursor_FieldDecl) >= 0;
}

int FindId(const String& s, const String& id) {
	if(id.GetCount() == 0)
		return -1;
	int q = 0;
	int r = -1;
	for(;;) {
		q = s.Find(id, q);
		if(q < 0)
			break;
		if((q == 0 || !iscid(s[q - 1])) && // character before id
		   (q + id.GetCount() >= s.GetCount() || !iscid(s[q + id.GetCount()]))) { // and after..
			r = q; // need to find last one...
			q += id.GetCount();
		}
		else
			q++;
	}
	return r;
};

String GetClass(const AnnotationItem& m)
{
	String cls = m.id;
	int q;
	if(m.kind == CXCursor_Constructor) {
		q = cls.Find("::" + m.name + "(");
		if(q >= 0)
			q += 2;
	}
	else
	if(m.kind == CXCursor_Destructor) {
		q = cls.Find('~');
	}
	else
		q = FindId(cls, m.name);

	if(q >= 0) {
		cls.Trim(q);
		if(m.nspace.GetCount())
			cls.TrimStart(m.nspace + "::");
		return cls;
	}
	
	return Null;
}

String GetNameFromId(const String& id)
{
	String name;
	try {
		CParser p(id);
		while(!p.IsEof())
			if(p.IsId())
				name = p.ReadId();
			else
			if(!p.Char(':'))
				break;
	}
	catch(CParser::Error) {}
	return name;
}

String MakeDefinition(const AnnotationItem& m, const String& klass)
{
	String result;
	String pretty = m.pretty;
	pretty.TrimStart("static ");
	int q = FindId(pretty, m.name);
	if(q < 0)
		result << pretty;
	else
		result << pretty.Mid(0, q) << klass << pretty.Mid(q);

	const char *s = result;
	int lvl = 0;
	String r;
	while(*s) {
		if(*s == '(')
			lvl++;
		if(*s == ')')
			lvl--;
		if(lvl == 1 && *s == '=') { // skip default parameter
			while(r.GetCount() && s[-1] == ' ') {
				s--;
				r.TrimLast();
			}
			while(*s) {
				if((*s == ',' || *s == ')') && lvl == 1) {
					r.Cat(*s++);
					break;
				}
				if(*s == '(')
					lvl++;
				if(*s == ')')
					lvl--;
				s++;
			}
		}
		else
			r.Cat(*s++);
	}
	r << "\n{\n}\n\n";
	return r;
}

String MakeDefinition(const AnnotationItem& m)
{
	return MakeDefinition(m, GetClass(m));
}
