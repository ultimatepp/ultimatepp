#include "clang.h"

bool IsCppSourceFile(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	return findarg(ext, ".cpp", ".cc", ".cxx") >= 0;
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

bool IsVariable(int kind)
{
	return findarg(kind, CXCursor_VarDecl, CXCursor_FieldDecl) >= 0;
}

int FindId(const char *s, const String& id)
{
	try {
		CParser p(s);
		while(!p.IsEof()) {
			const char *q = p.GetPtr();
			if(p.Id(id))
				return q - s;
			else
				p.Skip();
		}
	}
	catch(CParser::Error) {}
	return -1;
}
