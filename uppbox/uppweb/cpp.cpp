#include "www.h"

String CppAsQtf(const String& str)
{
	static const char *cpp[] = {
		"__asm", "__cdecl", "__declspec", "__except", "__fastcall",
		"__finally", "__inline", "__int16", "__int32", "__int64",
		"__int8", "__leave", "__stdcall", "__try", "__uuidof",
		"alignas", "alignof", "and", "and_eq", "asm", "auto",
		"bitand", "bitor", "bool", "break", "case", "catch",
		"char", "char8_t", "char16_t", "char32_t", "class",
		"co_await", "co_return", "co_yield", "compl", "concept",
		"const", "const_cast", "consteval", "constexpr", "constinit",
		"continue", "decltype", "default", "delete", "dllexport",
		"dllimport", "do", "double", "dynamic_cast", "else", "enum",
		"explicit", "export", "extern", "false", "final", "float",
		"for", "force_inline", "friend", "goto", "if", "import",
		"inline", "int", "long", "module", "mutable", "namespace",
		"never_inline", "new", "noexcept", "not", "not_eq", "nullptr",
		"operator", "or", "or_eq", "override", "private", "protected",
		"public", "register", "reinterpret_cast", "requires", "return",
		"short", "signed", "sizeof", "static", "static_assert",
		"static_cast", "struct", "switch", "template", "this", "thread",
		"thread_local", "throw", "true", "try", "typedef", "typeid",
		"typename", "union", "unsigned", "using", "virtual",
		"void", "volatile", "wchar_t", "while", "xor", "xor_eq",
		"include", "define", "ifdef", "ifndef", "endif",
		nullptr
	};
	static Index<String> keyword;
	if(keyword.GetCount() == 0)
		for(const char **q = cpp; *q; q++)
			keyword.Add(*q);

	const char *s = str;
	StringBuffer qtf;
	qtf << "[l0r0<C+76 ";
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
			if((byte)*s >= 32) {
				if(strchr("!+-*^/%~&|=[]:?<>.#", *s))
					qtf << "[@B `" << *s << "]";
				else
				if((byte)*s < 128)
					qtf << '`' << *s;
				else
					qtf << *s;
			}
			s++;
		}
	}
	return qtf;
}
