#include "CodeEditor.h"

namespace Upp {

HlStyle HighlightSetup::hl_style[HL_COUNT];
byte    HighlightSetup::hilite_scope = 0;
byte    HighlightSetup::hilite_ifdef = 1;
byte    HighlightSetup::hilite_bracket = 1;
bool    HighlightSetup::thousands_separator = true;

bool    HighlightSetup::indent_spaces = false;
bool    HighlightSetup::no_parenthesis_indent = false;
int     HighlightSetup::indent_amount = 4;

#define HL_COLOR(x, a, b)    #x,
static const char *s_hl_color[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, x, b)    x,
static const char *s_hl_name[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, b, x)    x,
static bool s_hl_font[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

void HighlightSetup::DefaultHlStyles()
{
	if(IsDarkTheme())
		DarkTheme();
	else
		WhiteTheme();
}

void HighlightSetup::InitOnce()
{
	ONCELOCK {
		static bool initialised;
		if(!initialised) {
			initialised = true;
			DefaultHlStyles();
		}
	}
}

const HlStyle& HighlightSetup::GetHlStyle(int i)
{
	InitOnce();
	return hl_style[i];
}

const char *HighlightSetup::GetHlName(int i)
{
	InitOnce();
	return s_hl_name[i];
}

bool HighlightSetup::HasHlFont(int i)
{
	InitOnce();
	return s_hl_font[i];
}

void  HighlightSetup::SetHlStyle(int i, Color c, bool bold, bool italic, bool underline)
{
	InitOnce();
	HlStyle& st = hl_style[i];
	st.color = c;
	st.bold = bold;
	st.italic = italic;
	st.underline = underline;
}

void HighlightSetup::LoadHlStyles(const char *s)
{
	CParser p(s);
	try {
		while(!p.IsEof()) {
			String id = p.ReadId();
			Color c = ReadColor(p);
			bool bold = false;
			bool italic = false;
			bool underline = false;
			for(;;)
				if(p.Id("bold"))
					bold = true;
				else
				if(p.Id("italic"))
					italic = true;
				else
				if(p.Id("underline"))
					underline = true;
				else
					break;
			for(int i = 0; i < HL_COUNT; i++)
				if(id == s_hl_color[i]) {
					SetHlStyle(i, c, bold, italic, underline);
					break;
				}
			p.PassChar(';');
		}
	}
	catch(CParser::Error) {
		DefaultHlStyles();
	}
}

String HighlightSetup::StoreHlStyles()
{
	String r;
	for(int i = 0; i < HL_COUNT; i++) {
		const HlStyle& s = GetHlStyle(i);
		r << Format("%-25s", s_hl_color[i]) << ' ' << FormatColor(s.color);
		if(s.bold)
			r << " bold";
		if(s.italic)
			r << " italic";
		if(s.underline)
			r << " underline";
		r << ";\r\n";
	}
	return r;
}

void HighlightSetup::HostColors()
{
	SetHlStyle(INK_NORMAL, SColorText);
	SetHlStyle(INK_DISABLED, SColorDisabled);
	SetHlStyle(INK_SELECTED, SColorHighlightText);
	SetHlStyle(PAPER_NORMAL, SColorPaper);
	SetHlStyle(PAPER_READONLY, SColorFace);
	SetHlStyle(PAPER_SELECTED, SColorHighlight);
	
	SetHlStyle(WHITESPACE, Blend(SColorLight, SColorHighlight));
	SetHlStyle(WARN_WHITESPACE, Blend(SColorLight, SRed));
}

void HighlightSetup::DarkTheme(bool host)
{
	SetHlStyle(INK_NORMAL,                White);
	SetHlStyle(PAPER_NORMAL,              Color(1, 1, 1));
	SetHlStyle(INK_SELECTED,              Color(1, 1, 1));
	SetHlStyle(PAPER_SELECTED,            Color(97, 217, 255));
	SetHlStyle(INK_DISABLED,              Color(184, 184, 184));
	SetHlStyle(PAPER_READONLY,            Color(24, 24, 24));
	SetHlStyle(INK_COMMENT,               Color(173, 255, 173), false, true);
	SetHlStyle(INK_COMMENT_WORD,          Color(235, 235, 255), true, true);
	SetHlStyle(PAPER_COMMENT_WORD,        Color(99, 99, 0));
	SetHlStyle(INK_CONST_STRING,          Color(248, 162, 162));
	SetHlStyle(INK_CONST_STRINGOP,        Color(214, 214, 255));
	SetHlStyle(INK_RAW_STRING,            Color(235, 235, 255));
	SetHlStyle(INK_OPERATOR,              Color(214, 214, 255));
	SetHlStyle(INK_KEYWORD,               Color(214, 214, 255), true);
	SetHlStyle(INK_BREAK_KEYWORD,         Color(186, 142, 255), true, false, true);
	SetHlStyle(INK_UPP,                   Color(108, 236, 236));
	SetHlStyle(PAPER_LNG,                 Color(13, 13, 0));
	SetHlStyle(INK_ERROR,                 Color(255, 185, 185));
	SetHlStyle(INK_PAR0,                  White);
	SetHlStyle(INK_PAR1,                  Color(173, 255, 173));
	SetHlStyle(INK_PAR2,                  Color(255, 149, 255));
	SetHlStyle(INK_PAR3,                  Color(214, 214, 86));
	SetHlStyle(INK_CONST_INT,             Color(255, 137, 137));
	SetHlStyle(INK_CONST_FLOAT,           Color(255, 149, 255));
	SetHlStyle(INK_CONST_HEX,             Color(235, 235, 255));
	SetHlStyle(INK_CONST_OCT,             Color(235, 235, 255));
	SetHlStyle(PAPER_BRACKET0,            Color(26, 26, 0));
	SetHlStyle(PAPER_BRACKET,             Color(99, 99, 0), true);
	SetHlStyle(PAPER_BLOCK1,              Color(16, 16, 31));
	SetHlStyle(PAPER_BLOCK2,              Color(5, 20, 5));
	SetHlStyle(PAPER_BLOCK3,              Color(3, 3, 0));
	SetHlStyle(PAPER_BLOCK4,              Color(20, 5, 20));
	SetHlStyle(INK_MACRO,                 Color(255, 149, 255));
	SetHlStyle(PAPER_MACRO,               Color(11, 11, 0));
	SetHlStyle(PAPER_IFDEF,               Color(0, 18, 18));
	SetHlStyle(INK_IFDEF,                 Color(131, 131, 131));
	SetHlStyle(INK_UPPER,                 White);
	SetHlStyle(INK_SQLBASE,               White);
	SetHlStyle(INK_SQLFUNC,               White);
	SetHlStyle(INK_SQLBOOL,               White);
	SetHlStyle(INK_UPPMACROS,             Color(108, 236, 236));
	SetHlStyle(INK_UPPLOGS,               Color(173, 255, 173));
	SetHlStyle(INK_DIFF_FILE_INFO,        White, true);
	SetHlStyle(INK_DIFF_HEADER,           Color(103, 202, 255));
	SetHlStyle(INK_DIFF_ADDED,            Color(182, 196, 255));
	SetHlStyle(INK_DIFF_REMOVED,          Color(255, 185, 185));
	SetHlStyle(INK_DIFF_COMMENT,          Color(173, 255, 173));
	SetHlStyle(PAPER_SELWORD,             Color(99, 99, 0));
	SetHlStyle(PAPER_ERROR,               Color(90, 40, 40));
	SetHlStyle(PAPER_ERROR_FILE,          Color(170, 40, 40)); // Color(0xE2, 0x55, 0)); ?
	SetHlStyle(PAPER_WARNING,             Color(21, 21, 0));
	SetHlStyle(SHOW_LINE,                 Color(27, 75, 26));
	SetHlStyle(SHOW_COLUMN,               Color(56, 33, 29));
	SetHlStyle(SHOW_BORDER,               Color(70, 50, 50));
	SetHlStyle(WHITESPACE,                Color(68, 128, 176));
	SetHlStyle(WARN_WHITESPACE,           Color(206, 141, 141));

	if(host)
		HostColors();
}

void HighlightSetup::WhiteTheme(bool host)
{
	SetHlStyle(INK_COMMENT, Green, false, true);
	SetHlStyle(PAPER_COMMENT_WORD, Yellow, false, false);
	SetHlStyle(INK_COMMENT_WORD, Blue, true, true);
	SetHlStyle(INK_CONST_STRING, Red);
	SetHlStyle(INK_RAW_STRING, Blue);
	SetHlStyle(INK_CONST_STRINGOP, LtBlue);
	SetHlStyle(INK_CONST_INT, Red);
	SetHlStyle(INK_CONST_FLOAT, Magenta);
	SetHlStyle(INK_CONST_HEX, Blue);
	SetHlStyle(INK_CONST_OCT, Blue);

	SetHlStyle(INK_OPERATOR, LtBlue);
	SetHlStyle(INK_KEYWORD, LtBlue, true);
	SetHlStyle(INK_BREAK_KEYWORD, Magenta, true, false, true);
	SetHlStyle(INK_UPP, Cyan);
	SetHlStyle(PAPER_LNG, Color(255, 255, 224));
	SetHlStyle(INK_ERROR, LtRed);
	SetHlStyle(INK_PAR0, Black);
	SetHlStyle(INK_PAR1, Green);
	SetHlStyle(INK_PAR2, Magenta);
	SetHlStyle(INK_PAR3, Brown);

	SetHlStyle(INK_UPPER, Black);
	SetHlStyle(INK_SQLBASE, Black);
	SetHlStyle(INK_SQLFUNC, Black);
	SetHlStyle(INK_SQLBOOL, Black);
	SetHlStyle(INK_UPPMACROS, Cyan);
	SetHlStyle(INK_UPPLOGS, Green);
	
	SetHlStyle(INK_DIFF_FILE_INFO, Black, true);
	SetHlStyle(INK_DIFF_HEADER, Color(28, 127, 200));
	SetHlStyle(INK_DIFF_ADDED, Color(28, 42, 255));
	SetHlStyle(INK_DIFF_REMOVED, Color(255, 0, 0));
	SetHlStyle(INK_DIFF_COMMENT, Green);

	SetHlStyle(PAPER_BLOCK1, Blend(LtBlue, White, 240));
	SetHlStyle(PAPER_BLOCK2, Blend(LtGreen, White, 240));
	SetHlStyle(PAPER_BLOCK3, Blend(LtYellow, White, 240));
	SetHlStyle(PAPER_BLOCK4, Blend(LtMagenta, White, 240));

	SetHlStyle(INK_MACRO, Magenta);
	SetHlStyle(PAPER_MACRO, Color(255, 255, 230));
	SetHlStyle(PAPER_IFDEF, Color(230, 255, 255));
	SetHlStyle(INK_IFDEF, Color(170, 170, 170));

	SetHlStyle(PAPER_BRACKET0, LtYellow);
	SetHlStyle(PAPER_BRACKET, Yellow, true);

	SetHlStyle(PAPER_SELWORD, Yellow);

	SetHlStyle(PAPER_ERROR, Blend(White(), LtRed(), 50));
	SetHlStyle(PAPER_ERROR_FILE, Blend(White(), LtRed(), 150));
	SetHlStyle(PAPER_WARNING, Blend(White(), Yellow(), 50));

	SetHlStyle(SHOW_LINE, Color(199, 247, 198));
	SetHlStyle(SHOW_COLUMN, Color(247, 224, 220));
	SetHlStyle(SHOW_BORDER, Color(250, 220, 220));

	SetHlStyle(INK_NORMAL, Black());
	SetHlStyle(INK_DISABLED, Color(109, 109, 109));
	SetHlStyle(INK_SELECTED, White());
	SetHlStyle(PAPER_NORMAL, White());
	SetHlStyle(PAPER_READONLY, Color(240, 240, 240));
	SetHlStyle(PAPER_SELECTED, Color(0, 120, 215));
	
	SetHlStyle(WHITESPACE, Color(126, 186, 234));
	SetHlStyle(WARN_WHITESPACE, Color(191, 126, 126));

	if(host)
		HostColors();
}

}
