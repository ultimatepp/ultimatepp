#include "www.h"

String CppAsQtf(const String& str)
{
	static const char *cpp[] = {
		"__asm", "else", "struct",
		"enum", "switch", "auto", "__except", "template",
		"explicit", "this",
		"bool", "extern", "mutable", "thread",
		"break", "false", "throw",
		"case", "__fastcall", "namespace", "true",
		"catch", "__finally", "new", "try",
		"__cdecl", "float", "__try",
		"char", "for", "operator", "typedef",
		"class", "friend", "private", "typeid",
		"const", "goto", "protected", "typename",
		"const_cast", "if", "public", "union",
		"continue", "inline", "register", "unsigned",
		"__declspec", "__inline", "reinterpret_cast", "using",
		"using", "default", "int", "return",
		"delete", "__int8", "short", "__uuidof",
		"dllexport", "__int16", "signed", "virtual",
		"dllimport", "__int32", "sizeof", "void",
		"do", "__int64", "static", "volatile",
		"double", "__leave", "static_cast",
		"dynamic_cast", "long", "__stdcall", "while",
		"include", "define", "ifdef", "ifndef", "endif",
		NULL
	};
	static Index<String> keyword;
	if(keyword.GetCount() == 0) 
		for(const char **q = cpp; *q; q++)
			keyword.Add(*q);

	const char *s = str;
	StringBuffer qtf;
	qtf << "[l0r0<C1 ";
	while(*s)
	{
		if(iscib(*s)) {
			const char *b = s;
			while(iscid(*s))
				s++;
			String id(b, s);
			if(keyword.Find(id) >= 0)
				qtf << "[@B " << DeQtf(id) << "]";
			else
				qtf << DeQtf(id);
		}
		else {
			if(*s == '\t')
				qtf << "____";
			else
			if(*s == '\n')
				qtf << '&';
			else
			if((byte)*s >= 32)
				if(strchr("!+-*^/%~&|=[]:?<>.#", *s))
					qtf << "[@B `" << *s << "]";
				else
					qtf << '`' << *s;
			s++;
		}
	}
	return qtf;
}
