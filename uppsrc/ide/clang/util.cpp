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

int FindId(const String& s, const String& id) {
	if(id.GetCount() == 0)
		return -1;
	int q = 0;
	for(;;) {
		q = s.Find(id, q);
		if(q < 0)
			return -1;
		if((q == 0 || !iscid(s[q - 1])) && // character before id
		   (q + id.GetCount() >= s.GetCount() || !iscid(s[q + id.GetCount()]))) // and after..
			return q;
		q++;
	}
};
