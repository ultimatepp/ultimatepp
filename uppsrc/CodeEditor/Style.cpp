#include "CodeEditor.h"

NAMESPACE_UPP

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

const HlStyle& HighlightSetup::GetHlStyle(int i)
{
	return hl_style[i];
}

const char *HighlightSetup::GetHlName(int i)
{
	return s_hl_name[i];
}

bool HighlightSetup::HasHlFont(int i)
{
	return s_hl_font[i];
}

void  HighlightSetup::SetHlStyle(int i, Color c, bool bold, bool italic, bool underline)
{
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

void HighlightSetup::DefaultHlStyles()
{
	SetHlStyle(INK_COMMENT, Green, false, true);
	SetHlStyle(PAPER_COMMENT_WORD, Yellow, false, false);
	SetHlStyle(INK_COMMENT_WORD, Blue, true, true);
	SetHlStyle(INK_CONST_STRING, Red);

	SetHlStyle(INK_CONST_STRINGOP, LtBlue);
	SetHlStyle(INK_CONST_INT, Red);
	SetHlStyle(INK_CONST_FLOAT, Magenta);
	SetHlStyle(INK_CONST_HEX, Blue);
	SetHlStyle(INK_CONST_OCT, Blue);

	SetHlStyle(INK_OPERATOR, LtBlue);
	SetHlStyle(INK_KEYWORD, LtBlue, true);
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

	SetHlStyle(INK_NORMAL, SColorText);
	SetHlStyle(INK_DISABLED, SColorDisabled);
	SetHlStyle(INK_SELECTED, SColorHighlightText);
	SetHlStyle(PAPER_NORMAL, SColorPaper);
	SetHlStyle(PAPER_READONLY, SColorFace);
	SetHlStyle(PAPER_SELECTED, SColorHighlight);
	
	SetHlStyle(PAPER_SELWORD, Yellow);

	SetHlStyle(PAPER_ERROR, Blend(White(), LtRed(), 50));
	SetHlStyle(PAPER_WARNING, Blend(White(), Yellow(), 50));

	SetHlStyle(SHOW_LINE, Color(199, 247, 198));
}

END_UPP_NAMESPACE
