#include "DocTypes.h"

namespace Upp {

int  QtfParser::GetNumber() {
	int n = 0;
	int sgn = 1;
	if(*term == '-') {
		sgn = -1;
		term++;
	}
	while(IsDigit(*term))
		n = n * 10 + *term++ - '0';
	return sgn * n;
}

String QtfParser::GetString() {
	String s;
	for(;;) {
		if(*term == '\0') return s;
		if(*term == '^') {
			term++;
			return s;
		}
		s.Cat(*term++);
	}
}

Paragraph& QtfParser::Par() {
	return table.GetRows() ? table.LastCell() : paragraph;
}

void QtfParser::Flush(String& text) {
	if(text.GetLength()) {
		Par().SetParaFont(state.font);
		if(state.type == NORMAL)
			Par().Cat(text, state.font, state.color, state.value);
		else {
			Font f = state.font;
			int h = f.GetHeight();
			f.Height(2 * h / 3);
			Par().Cat(text, f, state.color, state.value, state.type == SUPERSCRIPT ? -abs(h / 3)
				                                                                   :  abs(h / 5));
		}
		paragraph.SetStyle(state);
	}
	text.Clear();
}

Color QtfParser::GetColor() {
	static Color color[] = { Black, LtGray, White, Red, Green,
							 Blue, LtRed, WhiteGray, LtCyan, Yellow };
	int c = *term++;
	if(c == '(') {
		byte r = GetNumber();
		if(Key(')')) {
			r &= 255;
			return Color(r, r, r);
		}
		Key('.');
		byte g = GetNumber();
		Key('.');
		byte b = GetNumber();
		Key(')');
		return Color(r, g, b);
	}
	else
	if(c >= '0' && c <= '9')
		return color[c - '0'];
	else
		return Red;
}

void QtfParser::Picture(int type) {
	int cx = 0;
	int cy = 0;
	term++;
	cx = GetNumber();
	Key(':');
	cy = GetNumber();
	Key(' ');
	String name;
	while(*term != ']' && *term)
		name.Cat(*term++);
	if(type == 0) {
#ifdef PLATFORM_WIN32
#ifdef SYSTEMDRAW
		Drawing iw = LoadWMF(GetExeDirFile(name));
		if(!iw) iw = LoadWMF(name);
#else
		Drawing iw = Drawing::LoadWMF(GetExeDirFile(name));
		if(!iw) iw = Drawing::LoadWMF(name);
#endif
		if(iw) Par().Cat(PaintRect(DrawingDisplay(), iw), cx, cy, state.value);
#endif
	}
	else if(type == 3) {
		bool isx = (name.GetLength() == 8);
		for(const char *p = name; isx && *p; p++)
			isx = IsXDigit(*p);
		if(isx)
			Par().Cat(*(PaintRect *)(strtoul(name, NULL, 16)), cx, cy, state.value);
	}
//	else
//		Par().Cat(type == 1 ? PaintRect(NameNImageDisplay(), name)
//		                    : PaintRect(NameSImageDisplay(), name), cx, cy, state.value);
	if(*term) term++;
}

void QtfParser::Celln(int& a, int& b) {
	a = -1;
	b = -1;
	if(IsDigit(*term))
		a = GetNumber();
	if(*term == '/') {
		term++;
		b = GetNumber();
	}
}

QtfParser::Code QtfParser::Error(const char *s) {
	paragraph.Clear();
	paragraph.Cat("ERROR: " + String(s) + ": " + String(term, min<int>(strlen(term), 20)),
	              Arial(84).Bold().Underline(), Red);
	term = "";
	return PARAGRAPH;
}

QtfParser::Code QtfParser::Parse() {
	String text;
	paragraph.Clear();
	table.Clear();
	if(*term == '\0') {
		paragraph.SetStyle(state);
		paragraph.SetParaFont(state.font);
		Code e = ender;
		ender = END;
		return e;
	}
	ender = END;
	if(inraw)
		goto toinraw;
	while(Key(']'))
		if(stack.GetCount()) {
			state = stack.Top();
			stack.Drop();
		}
		else
			return Error("[ UNMATCHED");
	if(Key2('^') || Key('\f')) return PAGE;
	if(Key2('+')) {
		table.AddRow();
		table.AddCell();
	}
	Par().SetStyle(state);
	for(;;) {
		if(*term == '\0') {
			Flush(text);
			return IsTable() ? TABLE : PARAGRAPH;
		}
		if(Term2('^') || *term == '\f') {
			Flush(text);
			return IsTable() ? TABLE : PARAGRAPH;
		}
		if(Term2('+')) {
			Flush(text);
			if(IsTable()) {
				term += 2;
				return TABLE;
			}
			else
				return PARAGRAPH;
		}
		if(Key('\n') || Key('&')) {
			Flush(text);
			Par().SetParaFont(state.font);
			if(!IsTable()) {
				ender = *term ? PARAGRAPH : END;
				return PARAGRAPH;
			}
			Cell().NewPar();
			Par().SetStyle(state);
			continue;
		}
		if(Key('\t') || Key2('|')) {
			Flush(text);
			if(!IsTable()) return PARAGRAPH;
			table.AddCell();
			continue;
		}
		if(Key('\v') || Key2('-')) {
			Flush(text);
			if(!IsTable()) return PARAGRAPH;
			table.AddRow();
			table.AddCell();
			continue;
		}
		if(Key('`') || Key('\\')) {
			text.Cat(*term++);
			continue;
		}
		if(Key2(':')) {
			Flush(text);
			if(!IsTable())
				return Error("NOT IN TABLE");
			while(*term != ' ' && *term != '\n') {
				int a, b;
				switch(*term++) {
				case 0: return END;
				case '^': Cell().Top(); break;
				case '=': Cell().VertCenter(); break;
				case 'v': Cell().Bottom(); break;
				case 'l': Celln(a, b); if(a >= 0) Cell().LeftWidth(a);
					      if(b >= 0) Cell().LeftSpace(b); break;
				case 'r': Celln(a, b); if(a >= 0) Cell().RightWidth(a);
					      if(b >= 0) Cell().RightSpace(b); break;
				case 't': Celln(a, b); if(a >= 0) Cell().TopWidth(a);
					      if(b >= 0) Cell().TopSpace(b); break;
				case 'b': Celln(a, b); if(a >= 0) Cell().BottomWidth(a);
					      if(b >= 0) Cell().BottomSpace(b); break;
				case 'a': Celln(a, b); if(a >= 0) Cell().FrameWidth(a);
					      if(b >= 0) Cell().FrameSpace(b); break;
				case '@': Cell().Paper(GetColor()); break;
				case '<': table.Lm(GetNumber()); break;
				case '>': table.Rm(GetNumber()); break;
				case 'f': table.FrameWidth(GetNumber()); break;
				case 'h': table.HeaderRows(GetNumber()); break;
				case '_': table.FrameColor(GetColor()); break;
				case 's': table.Before(GetNumber()); break;
				case '!': Cell().Clear(); break;
				case '~': Cell().FrameWidth(0); table.FrameWidth(0); break;
				case '/': Cell().Keep(); break;
				case '+': Cell().Span(GetNumber()); break;
				default:
					term--;
					if(IsDigit(*term))
						Cell().Ratio(GetNumber());
					else
						term++;
				}
			}
			term++;
			continue;
		}
		if(Key('[')) {
			Flush(text);
			if(*term == 'P')
				Picture(0);
			else
			if(*term == 'I')
				Picture(1);
			else
			if(*term == 'J')
				Picture(2);
			else
			if(*term == 'K')
				Picture(3);
			else {
				stack.Add(state);
				for(;;) {
					int c = *term++;
					if(c == ' ' || c == '\n') break;
					switch(c) {
					case 0: return END;
					case '[': stack.Add(state); break;
					case '/': state.font.Italic(!state.font.IsItalic()); break;
					case '_': state.font.Underline(!state.font.IsUnderline()); break;
					case '*': state.font.Bold(!state.font.IsBold()); break;
					case '-': state.font.Strikeout(!state.font.IsStrikeout()); break;
					case '`': state.type = SUPERSCRIPT; break;
					case ',': state.type = SUBSCRIPT; break;
					case 'A': state.font = state.font.Face(Font::ARIAL); break;
					case 'R': state.font = state.font.Face(Font::ROMAN); break;
					case 'C': state.font = state.font.Face(Font::COURIER); break;
					case 'G': state.font = state.font.Face(Font::STDFONT); break;
					case 'S': state.font = state.font.Face(Font::SYMBOL); break;
#ifdef PLATFORM_WIN32
					case 'W': state.font = state.font.Face(Font::WINGDINGS); break;
#endif
					case '.': state.font = state.font.Face(GetNumber()); break;
					case '^': state.value = GetString(); break;
					case '<': Par().Left(); state.align = ALIGN_LEFT; break;
					case '>': Par().Right(); state.align = ALIGN_RIGHT; break;
					case '=': Par().Center(); state.align = ALIGN_CENTER; break;
					case '#': Par().Justify(); state.align = ALIGN_JUSTIFY; break;
					case 'l': Par().Lm(state.lm = GetNumber()); break;
					case 'r': Par().Rm(state.rm = GetNumber()); break;
					case 'i': Par().Indent(state.indent = GetNumber()); break;
					case 'b': Par().Before(state.before = GetNumber()); break;
					case 'a': Par().After(state.after = GetNumber()); break;
					case 'o': Par().Bullet(); state.bullet = Paragraph::StdBullet();
						      state.bulletindent = 150; break;
					case 'O': {
							int a, b;
							Celln(a, b);
							state.bullet = a == 1 ? Paragraph::WhiteBullet() :
							               a == 2 ? Paragraph::BoxBullet() :
							               a == 3 ? Paragraph::WhiteBoxBullet() :
							                        Paragraph::StdBullet(),
							state.bulletindent = b < 0 ? 150 : b;
							Par().Bullet(state.bullet, state.bulletindent);
						}
						break;
					case '!': {
							String fn;
							for(;;) {
								if(*term == '!') {
									term++;
									break;
								}
								if(*term == '\0')
									break;
								fn.Cat(*term++);
							}
							state.font.Face(Font::FindFaceNameIndex(fn));
						}
						break;
					case '+': state.font.Height(GetNumber()); break;
					case '@':
						state.color = GetColor();
						break;
					default:
						if(c >= '0' && c <= '9') {
							static int fsize[] = {
								50, 67, 84, 100, 134, 167, 200, 234, 300, 400
							};
							state.font.Height(fsize[c - '0']);
						}
						break;
					}
				}
			}
		}
		else
		if(Key(']')) {
			Flush(text);
			if(stack.GetCount()) {
				state = stack.Top();
				stack.Drop();
			}
		}
		else
		if(*term == '_') {
			text.Cat(31);
			term++;
		}
		else
		if(*term == '\1') {
			term++;
		toinraw:
			for(;;) {
				if(*term == '\0') break;
				if(*term == '\1') {
					inraw = false;
					term++;
					break;
				}
				if(*term == '\n' && !IsTable()) {
					inraw = true;
					term++;
					Flush(text);
					ender = PARAGRAPH;
					return PARAGRAPH;
				}
				text.Cat(*term++);
			}
		}
		else
		if((byte)*term >= 31)
			text.Cat(*term++);
		else
			term++;
	}
	String spc(" ");
	Flush(spc);
}

void QtfParser::Init() {
	state.font = Arial(-84);
	state.color = Black;
	state.Init();
	charset = 0;
	ender = END;
}

}
