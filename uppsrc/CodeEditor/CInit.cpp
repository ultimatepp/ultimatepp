#include "CodeEditor.h"

NAMESPACE_UPP

Vector <Index<String> > CSyntax::keyword;
Vector <Index<String> > CSyntax::name;
Index<String>           CSyntax::kw_upp;
int                     CSyntax::kw_macros;
int                     CSyntax::kw_logs;
int                     CSyntax::kw_sql_base;
int                     CSyntax::kw_sql_func;

int CSyntax::InitUpp(const char **q)
{
	while(*q)
		kw_upp.Add(*q++);
	return kw_upp.GetCount();
}

void CSyntax::InitKeywords()
{
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
		"override", "final",
		NULL
	};
	static const char *cs[] = {
		"abstract", "event", "new", "struct",
		"as", "explicit", "null", "switch",
		"base", "extern", "object", "this",
		"bool", "false", "operator", "throw",
		"break", "finally", "out", "true",
		"byte", "fixed", "override", "try",
		"case", "float", "params", "typeof",
		"catch", "for", "private", "uint",
		"char", "foreach", "protected", "ulong",
		"checked", "goto", "public", "unchecked",
		"class", "if", "readonly", "unsafe",
		"const", "implicit", "ref", "ushort",
		"continue", "in", "return", "using",
		"decimal", "int", "sbyte", "virtual",
		"default", "interface", "sealed", "volatile",
		"delegate", "internal", "short", "void",
		"do", "is", "sizeof", "while",
		"double", "lock", "stackalloc",
		"else", "long", "static",
		"enum", "namespace", "string",
		"await", "async", "throws", "awaits",
		NULL
	};
	static const char *upp[] = {
		"byte", "word", "dword", "__countof", "pick_", "wchar", "NULL", "Null",
		"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "qword",
		"INTERLOCKED_", "INTERLOCKED", "ONCELOCK", "ONCELOCK_", "INITBLOCK", "EXITBLOCK",
		"rval_", "rval_default",
		NULL
	};
	static const char *usc[] = {
		"void", "self", "if", "else", "while", "do", "case",
		"default", "break", "return", "switch", "operator", "for",
		"fn", "group", "ctrl", "subctrl", "template", "enum_property",
		"raw", "int", "double", "String", "bool",
		"Text", "Qtf", "Doc", "Font", "Color", "macro",
		"true", "false",
		NULL
	};
	static const char *usclib[] = {
		"Color", "Point", "Size", "Rect", "RectC",
		"StdFont", "Arial", "Roman", "Courier", "GetImageSize",
		"GetTextSize", "print", "Black", "Gray", "LtGray",
		"WhiteGray", "White", "Red", "Green", "Brown", "Blue",
		"Magenta", "Cyan", "Yellow", "LtRed", "LtGreen", "LtYellow",
		"LtBlue", "LtMagenta", "LtCyan", "SBlack", "SGray", "SLtGray",
		"SWhiteGray", "SWhite", "SRed", "SGreen", "SBrown", "SBlue",
		"SMagenta", "SCyan", "SYellow", "SLtRed", "SLtGreen", "SLtYellow",
		"SLtBlue", "SLtMagenta", "SLtCyan", "IntNull", "DblNullLim",
		"DrawRect", "DrawText", "DrawSmartText", "DrawImage", "is_number",
		"GetSmartTextSize", "GetQtfHeight", "DrawQtf",
		"is_array", "is_map", "is_void", "int", "to_string", "count",
		"len", "remove", "insert", "mid", "keys", "values", "exists",
		"erase", "rand", "OpenFileOut", "OpenFileAppend", "OpenFileIn",
		"GetMinSize", "GetStdSize",
		NULL
	};
	static const char *java[] = {
		"abstract", "assert", "boolean", "break", "byte", "case",
		"catch", "char", "class", "const", "continue",
		"default", "do", "double", "else", "enum", "extends",
		"false", "final", "finally", "float", "for",
		"goto", "if", "implements", "import", "instanceof",
		"int", "interface", "long", "native", "new",
		"null", "package", "private", "protected", "public",
		"return", "short", "static", "strictfp", "super", "switch",
		"synchronized", "this", "throw", "throws", "transient",
		"true", "try", "void", "volatile", "while",
		NULL
	};
	static const char *javascript[] = {
		"break", "continue", "do", "for", "import", "new", "this", "void",
		"case", "default", "else", "function", "in", "return", "typeof", "while",
		"comment", "delete", "export", "if", "label", "switch", "var", "with",
		"catch", "enum", "throw", "class", "extends", "try", "const", "finally",
		"debugger", "super", "true", "false", "undefined",
		NULL
	};
	static const char *css[] = {
		"azimuth", "background-attachment", "background-color", "background-image", "background-position",
		"background-repeat", "background", "border-collapse", "border-color", "border-spacing", "border-style",
		"border-top", "border-right", "border-bottom", "border-left", "border-top-color", "border-right-color",
		"border-bottom-color", "border-left-color", "border-top-style", "border-right-style", "border-bottom-style",
		"border-left-style", "border-top-width", "border-right-width", "border-bottom-width", "border-left-width",
		"border-width", "border", "bottom", "caption-side", "clear", "clip", "color", "content", "counter-increment",
		"counter-reset", "cue-after", "cue-before", "cue", "cursor", "direction", "display", "elevation", "empty-cells",
		"float", "font-family", "font-size", "font-style", "font-variant", "font-weight", "font", "height", "left",
		"letter-spacing", "line-height", "list-style-image", "list-style-position", "list-style-type", "list-style",
		"margin-right", "margin-left", "margin-top", "margin-bottom", "margin", "max-height", "max-width", "min-height",
		"min-width", "orphans", "outline-color", "outline-style", "outline-width", "outline", "overflow", "padding-top",
		"padding-right", "padding-bottom", "padding-left", "padding", "page-break-after", "page-break-before",
		"page-break-inside", "pause-after", "pause-before", "pause", "pitch-range", "pitch", "play-during", "position",
		"quotes", "richness", "right", "speak-header", "speak-numeral", "speak-punctuation", "speak", "speech-rate",
		"stress", "table-layout", "text-align", "text-decoration", "text-indent", "text-transform", "top",
		"unicode-bidi", "vertical-align", "visibility", "voice-family", "volume", "white-space", "widows", "width",
		"word-spacing", "z-index",
		// CSS3
		"opacity",
		"background-clip", "background-origin", "background-size", "border-bottom-left-radius", "border-bottom-right-radius",
		"border-image", "border-image-outset", "border-image-repeat", "border-image-slice", "border-image-source",
		"border-image-width", "border-radius", "border-top-left-radius", "border-top-right-radius", "box-decoration-break", 
		"box-shadow",
		"overflow-x", "overflow-y",
		"align-content", "align-items", "align-self", "flex", "flex-basis", "flex-direction", "flex-flow", "flex-grow",
		"flex-shrink", "flex-wrap", "justify-content", "order",
		"hanging-punctuation", "hyphens", "line-break", "overflow-wrap", "tab-size", "text-align-last", "text-justify",
		"word-break", "word-wrap",
		"text-decoration-color", "text-decoration-line", "text-decoration-style", "text-shadow", "text-underline-position",
		"animation", "animation-delay", "animation-direction", "animation-duration", "animation-fill-mode",
		"animation-iteration-count", "animation-name", "animation-timing-function", "animation-play-state",
		"backface-visibility", "perspective", "perspective-origin", "transform", "transform-origin", "transform-style",
		"transition", "transition-property", "transition-duration", "transition-timing-function", "transition-delay",
		"box-sizing", "nav-down", "nav-index", "nav-left", "nav-right", "nav-up", "outline-offset", "resize", "text-overflow",
		"break-after", "break-before", "break-inside", "column-count", "column-fill", "column-gap", "column-rule",
		"column-rule-color", "column-rule-style", "column-rule-width", "column-span", "column-width", "columns",
		"marks",
		"filter",
		"image-orientation", "image-rendering", "image-resolution", "object-fit", "object-position",
		"mask", "mask-type",
		"mark", "mark-after", "mark-before", "phonemes", "rest", "rest-after", "rest-before", "voice-balance",
		"voice-duration", "voice-pitch", "voice-pitch-range", "voice-rate", "voice-stress", "voice-volume",
		"marquee-direction", "marquee-play-count", "marquee-speed", "marquee-style",
		NULL
	};
	static const char *cssn[] = {
		"em", "px", "pt",
		"left-side", "far-left", "left", "center-left", "center", "center-right", "right", "far-right", "right-side",
		"behind", "leftwards", "rightwards", "inherit", "scroll", "fixed", "transparent", "none", "top", "bottom",
		"repeat", "repeat-x", "repeat-y", "no-repeat", "background-color", "background-image", "background-repeat",
		"background-attachment", "background-position", "collapse", "separate", "border-top-color", "auto", "both",
		"normal", "attr", "open-quote", "close-quote", "no-open-quote", "no-close-quote", "cue-before", "cue-after",
		"crosshair", "default", "pointer", "move", "e-resize", "ne-resize", "nw-resize", "n-resize", "se-resize",
		"sw-resize", "s-resize", "w-resize", "text", "wait", "help", "progress", "ltr", "rtl", "inline", "block",
		"list-item", "inline-block", "table", "inline-table", "table-row-group", "table-header-group",
		"table-footer-group", "table-row", "table-column-group", "table-column", "table-cell", "table-caption",
		"below", "level", "above", "higher", "lower", "show", "hide", "italic", "oblique", "small-caps", "bold",
		"bolder", "lighter", "font-style",
		"font-variant", "font-weight", "font-size", "line-height", "font-family", "caption", "icon", "menu",
		"message-box", "small-caption", "status-bar", "inside", "outside", "disc", "circle", "square", "decimal",
		"decimal-leading-zero", "lower-roman", "upper-roman", "lower-greek", "lower-latin", "upper-latin", "armenian",
		"georgian", "lower-alpha", "upper-alpha", "list-style-type", "list-style-position", "list-style-image",
		"invert", "outline-color", "outline-style", "outline-width", "visible", "hidden", "always", "avoid",
		"x-low", "low", "medium", "high", "x-high", "mix", "static", "relative", "absolute", "once", "digits",
		"continuous", "code", "spell-out", "x-slow", "slow", "fast", "x-fast", "faster", "slower", "justify",
		"underline", "overline", "line-through", "blink", "capitalize", "uppercase", "lowercase", "embed",
		"bidi-override", "baseline", "sub", "super", "text-top", "middle", "text-bottom", "silent", "x-soft", "soft",
		"loud", "x-loud", "pre", "nowrap", "pre-wrap", "pre-line", "solid",
		NULL
	};
	static const char *upp_macros[] = {
		"CLASSNAME", "THISBACK", "THISBACK1", "THISBACK2", "THISBACK3", "THISBACK4",
		"PTEBACK", "PTEBACK1", "PTEBACK2",  "PTEBACK3",  "PTEBACK4", 
		"QUOTE", "XASSERT", "NEVER", "XNEVER", "CHECK", "XCHECK", "ASSERT", "ASSERT_",
		"NAMESPACE_UPP", "END_UPP_NAMESPACE", "NEVER_", "SKYLARK", "RPC_METHOD", "RPC_GMETHOD",
		NULL
	};
	static const char *upp_logs[] = {
		"LOG", "LOGF", "DUMP", "DUMPC", "DUMPCC", "DUMPCCC", "DUMPM",
		"LLOG", "LLOGF", "LDUMP", "LDUMPC", "LDUMPCC", "LDUMPCCC", "LDUMPM",
		"DLOG", "DLOGF", "DDUMP", "DDUMPC", "DDUMPCC", "DDUMPCCC", "DDUMPM",
		"RLOG", "RLOGF", "RDUMP", "RDUMPC", "RDUMPCC", "RDUMPCCC", "RDUMPM",
		"LOGBEGIN", "LOGEND", "LOGBLOCK", "LOGHEXDUMP", "LOGSRCPOS", 
		"RLOGBEGIN", "RLOGEND", "RLOGBLOCK", "RLOGHEXDUMP", "RLOGSRCPOS", "RQUOTE",
		"RTIMING", "TIMING", "LTIMING", "DTIMING", "RTIMESTOP", "TIMESTOP", "LTIMESTOP", "DTIMESTOP",
		"LOGHEX", "DUMPHEX", "DLOGHEX", "DDUMPHEX", "RLOGHEX", "RDUMPHEX", "LLOGHEX", "LDUMPHEX",
		"DEBUGCODE",
		NULL
	};
	static const char *sql_base[] = {
		"Select", "Update", "Insert", "Delete", "From",
		"Join", "InnerJoin", "LeftJoin", "RightJoin", "FullJoin", "OuterJoin",
		"Where", "On", "OrderBy", "GroupBy",
		"Of", "As", "StartWith", "ConnectBy", "Having", "ForUpdate", "NoWait", "Limit",
		"Offset", "Hint", "SQL",
		NULL
	};
	static const char *sql_func[] = {
		"Decode", "Distinct", "All", "SqlAll", "Count", "Descending",
		"SqlMax", "SqlMin", "SqlSum", "Avg", "Stddev", "Variance",
		"Greatest", "Least", "ConvertCharset", "ConvertAscii",
		"Upper", "Lower", "Substr", "Instr", "Wild", "SqlDate", "AddMonths", "LastDay",
		"MonthsBetween", "NextDay", "SqlNvl", "Prior", "NextVal", "CurrVal", "SqlArg",
		NULL
	};
	static const char *sql_bool[] = {
		"SqlIsNull", "NotNull", "Like", "LikeUpperAscii", "NotLike", "Between",
		"NotBetween", "In", "NotIn", "Exists", "NotExists",
		NULL
	};
	static const char *tfile[] = {
		"T_",
		NULL,
	};
	static const char *tlng[] = {
		"enUS", "enGB", "enAU", "enCA", "enNZ", "enIE", "enZA", "enJM", "enCB", "enBZ",
		"enTT", "bgBG", "csCZ", "daDK", "deDE", "deCH", "deAT", "deLU", "deLI", "elGR",
		"esES", "esMX", "esES", "esGT", "esCR", "esPA", "esDO", "esVE", "esCO", "esPE",
		"esAR", "esEC", "esCL", "esUY", "esPY", "esBO", "esSV", "esHN", "esNI", "esPR",
		"fiFI", "frFR", "frBE", "frCA", "frCH", "frLU", "huHU", "isIS", "itIT", "itCH",
		"nlNL", "nlBE", "noNO", "noNO", "plPL", "ptBR", "ptPT", "roRO", "ruRU", "hrHR",
		"srSP", "srSP", "skSK", "svSE", "svFI", "trTR", "slSI", "afZA", "sqAL", "euES",
		"beBY", "caES", "etEE", "foFO", "idID", "lvLV", "ltLT", "ukUA", "zhCN", "zhTW",
		"koKR", "jaJP",
		NULL
	};
	static const char *lay[] = {
		"LAYOUT", "ITEM", "UNTYPED", "END_LAYOUT",
		NULL
	};
	static const char *sch[] = {
		"BIT", "BIT_ARRAY", "BIT_", "BIT_ARRAY_",
		"BOOL", "BOOL_ARRAY", "BOOL_", "BOOL_ARRAY_",
		"INT", "INT_ARRAY", "INT_", "INT_ARRAY_",
		"INT64", "INT64_ARRAY", "INT64_", "INT64_ARRAY_",
		"DOUBLE", "DOUBLE_ARRAY", "DOUBLE_", "DOUBLE_ARRAY_",
		"DATE", "DATE_ARRAY", "DATE_", "DATE_ARRAY_",
		"DATETIME", "DATETIME_ARRAY", "DATETIME_", "DATETIME_ARRAY_",
		"TIME", "TIME_ARRAY", "TIME_", "TIME_ARRAY_",
		"STRING", "STRING_ARRAY", "STRING_", "STRING_ARRAY_",
		"LONG", "LONG_", "LONGRAW", "LONGRAW_", "BLOB", "BLOB_", "CLOB", "CLOB_",
		"AUTO_INCREMENT", "KEY", "NOT_NULL", "TIMESTAMP", "COMMENT", "SEQUENCE", "SEQUENCE_",
		"PRIMARY_KEY", "INDEX", "PARTIAL_INDEX", "UNIQUE", "SQLDEFAULT", "REFERENCES", "REFERENCES_",
		"REFERENCES_CASCADE", "REFERENCES_CASCADE_", "DUAL_PRIMARY_KEY", "DUAL_UNIQUE",
		"UNIQUE_LIST", "SQLCHECK",
		"TABLE", "TABLE_", "END_TABLE", "TABLE_I", "TABLE_I_", "TABLE_II",
		"TABLE_II_", "TABLE_III", "TABLE_III_", "VAR", "VAR_",
		"COLUMN", "COLUMN_ARRAY", "ATTRIBUTE", "INLINE_ATTRIBUTE",
		"TYPE", "TYPE_I", "TYPE_II", "TYPE_III", "END_TYPE",
		"TYPE_", "TYPE_I_", "TYPE_II_", "TYPE_III_", "SERIAL", "ISERIAL",
		"INDEX_LIST", "PRIMARY_KEY_LIST",
		NULL
	};
	static const char *sql[] = {
		"ABORT", "ADD", "AFTER", "ALL", "ALTER", "ANALYZE", "AND", "AS", "ATTACH",
		"AUTOINCREMENT", "BEFORE", "BEGIN", "BETWEEN", "BY", "CASCADE", "CASE", "CHECK",
		"COLLATE", "COLUMN", "COMMIT", "CONFLICT", "CONSTRAINT", "CREATE", "CROSS",
		"CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP", "DATABASE", "DEFAULT",
		"DELETE", "DESC", "DETACH", "DISTINCT", "DROP", "EACH", "END", "EXCEPT",
		"EXCLUSIVE", "EXISTS", "FOREIGN", "FROM", "FULL", "GROUP", "HAVING",
		"IN", "INDEX", "INITIALLY", "INNER", "INSERT", "INSTEAD", "INTERSECT", "INTO",
		"IS", "ISNULL", "JOIN", "KEY", "LEFT", "LIKE", "LIMIT", "MATCH", "NATURAL", "NOT",
		"NOTNULL", "NULL", "OF", "OFFSET", "ON", "OR", "ORDER", "OUTER", "PRIMARY", "RAISE",
		"REFERENCES", "REPLACE", "RESTRICT", "UPDATE", "SET", "WHERE", "NEW", "OLD", "TRIGGER",
		"SELECT", "TABLE",
		NULL
	};
	static const char *empty[] = {
		NULL
	};
	static const char *javascriptn[] = {
		"alert", "eval", "toString", "valueOf", "length",
		NULL
	};

	LoadSyntax(cpp, upp); // Order here is important, must be the same as enum
	LoadSyntax(usc, usclib);
	LoadSyntax(java, empty);
	LoadSyntax(tfile, tlng);
	LoadSyntax(usc, usclib);
	LoadSyntax(lay, empty);
	LoadSyntax(sch, empty);
	LoadSyntax(sql, empty);
	LoadSyntax(cs, empty);
	LoadSyntax(javascript, javascriptn);
	LoadSyntax(css, cssn);
	LoadSyntax(empty, empty);

	kw_macros = InitUpp(upp_macros);
	kw_logs = InitUpp(upp_logs);
	kw_sql_base = InitUpp(sql_base);
	kw_sql_func = InitUpp(sql_func);
	InitUpp(sql_bool);
}

int CSyntax::LoadSyntax(const char *keywords[], const char *names[])	// Changed
{
	Index <String> &key = keyword.Add()	;
	while(*keywords)
		key.Add(*keywords++);
	Index <String> &nam = name.Add();
	while(*names)
		nam.Add(*names++);	
	return keyword.GetCount() - 1;
}

END_UPP_NAMESPACE
