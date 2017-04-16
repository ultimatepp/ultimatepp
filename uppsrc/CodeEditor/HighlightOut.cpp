#include "CodeEditor.h"

namespace Upp {

HighlightOutput::HighlightOutput(Vector<LineEdit::Highlight>& v)
:	v(v)
{
	pos = 0;
	def = v[0];
	def.chr = ' ';
	v.Top().chr = ' ';
}

HighlightOutput::~HighlightOutput()
{
	for(int i = 0; i < v.GetCount(); i++)
		if(v[i].chr == '_') {
			v[i].font.NoBold();
			v[i].font.NoItalic();
		}
}

void HighlightOutput::Set(int pos, int count, const HlStyle& ink)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0) {
		LineEdit::Highlight& x = v[pos++];
		x.ink = ink.color;
		if(ink.bold)
			x.font.Bold();
		if(ink.italic)
			x.font.Italic();
		if(ink.underline)
			x.font.Underline();
	}
}

void HighlightOutput::SetFont(int pos, int count, const HlStyle& f)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0) {
		LineEdit::Highlight& x = v[pos++];
		if(f.bold)
			x.font.Bold();
		if(f.italic)
			x.font.Italic();
		if(f.underline)
			x.font.Underline();
	}
}

void HighlightOutput::SetPaper(int pos, int count, Color paper)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].paper = paper;
}

void HighlightOutput::SetFlags(int pos, int count, word flags)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].flags = flags;
}

void HighlightOutput::SetInk(int pos, int count, Color ink)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].ink = ink;
}

void HighlightOutput::Put(int count, const HlStyle& ink, Color paper)
{
	Set(pos, count, ink);
	SetPaper(pos, count, paper);
	pos += count;
}

void HighlightOutput::Put(int count, const HlStyle& ink, const HlStyle& paper)
{
	Put(count, ink, paper.color);
}

const wchar *HighlightOutput::CString(const wchar *p)
{
	Put(hl_style[INK_CONST_STRING]);
	const wchar delim = *p;
	p++;
	while(*p && *p != delim)
		if(*p == '\\') {
			const wchar *t = p++;
			if(*p == 'x' || *p == 'X') {
				p++;
				if(IsXDigit(*p))
					p++;
				if(IsXDigit(*p))
					p++;
				Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else
			if(*p >= '0' && *p <= '7') {
				p++;
				if(*p >= '0' && *p <= '7') p++;
				if(*p >= '0' && *p <= '7') p++;
				Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else {
				Put(hl_style[INK_CONST_STRINGOP]);
				if(*p) {
					Put(hl_style[INK_CONST_STRINGOP]);
					p++;
				}
			}
		}
		else
		if(*p == '%')
			if(p[1] == '%') {
				Put(2, hl_style[INK_CONST_STRING]);
				p += 2;
			}
			else {
				const wchar *t = p++;
				while(!IsAlpha(*p) && *p && *p != '`' && *p != '\"' && *p != '\'' && *p != '\\')
					p++;
				while(IsAlpha(*p) && *p)
					p++;
				Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
		else {
			Put(hl_style[INK_CONST_STRING]);
			p++;
		}
	if(*p == delim)	{
		Put(hl_style[INK_CONST_STRING]);
		p++;
	}
	return p;
}

}
