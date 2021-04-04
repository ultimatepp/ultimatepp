#include <CtrlLib/CtrlLib.h>

using namespace Upp;

int Sep(int c)
{
	return c == ';' || c == ',' ? ';' : 0;
}

GUI_APP_MAIN
{
	TopWindow win;
	SuggestCtrl ctrl;

	static const char *cpp[] = {
		"namespace", "asm", "__asm", "else", "struct",
		"enum", "switch", "auto", "__except", "template",
		"explicit", "this",
		"bool", "extern", "mutable", "thread",
		"break", "false", "throw",
		"case", "__fastcall", "true",
		"catch", "__finally", "new", "try",
		"__cdecl", "float", "__try",
		"char", "wchar_t", "for", "operator", "typedef",
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
		"force_inline",
		"and", "bitor", "or", "xor", 
		"compl", "bitand", "and_eq", "or_eq",
		"xor_eq", "not", "not_eq",
		"char16_t", "char32_t", "constexpr", "decltype",
		"noexcept", "nullptr", "static_assert",
		"override", "final"
	};

	for(int i = 0; i < __countof(cpp); i++)
		ctrl.AddList(cpp[i]);
	ctrl.Delimiter(Sep);
	ctrl.CompareFilter(CharFilterToUpperAscii);
	win.Add(ctrl.HSizePos().TopPos(0));
	win.Run();
}
