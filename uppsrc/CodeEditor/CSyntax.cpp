#include "CodeEditor.h"

namespace Upp {

const wchar *eatstring(const wchar *s) {
	int delim = *s++;
	while(*s)
		if(*s == '\\' && s[1])
			s += 2;
		else
		if(*s == delim)
			return s + 1;
		else
			s++;
	return s;
}

inline const wchar *strnext(const wchar *p, const wchar *end, int ch) {
	while(p < end) {
		if(*p == ch) return p;
		p++;
	}
	return NULL;
}

void CSyntax::ClearBraces() {
	cl = bl = pl = 0;
	spar = 0;
	brk.Clear();
	blk.Clear();
	bid.Clear();
	bid.Add(0);
	par.Clear();
}

void CSyntax::Clear() {
	ClearBraces();
	linecont = linecomment = comment = string = false;
	macro = MACRO_OFF;
	stmtline = endstmtline = seline = -1;
	was_namespace = false;
	raw_string.Clear();
	ifstack.Clear();
}

const wchar *isstmt(const wchar *p) {
	static const char *stmt[] = {
		"if", "else", "while", "do", "for"
	};
	for(const char **q = stmt; q < stmt + __countof(stmt); q++) {
		const char *k = *q;
		const wchar *s = p;
		for(;;) {
			if(*k== '\0') {
				if(!iscidl(*s)) return s;
				break;
			}
			if(*s != *k)
				break;
			s++;
			k++;
		}
	}
	return NULL;
}

static WString sReadLn(const wchar *p)
{
	WStringBuffer wbuf;
	while(*p && *p <= ' ')
		p++;
	while(*p && !(*p == '/' && (p[1] == '/' || p[1] == '*'))) {
		if(*p <= ' ') {
			while(*++p && *p <= ' ')
				;
			if(*p && !(*p == '/' && (p[1] == '/' || p[1] == '*')))
				wbuf.Cat(' ');
		}
		else
			wbuf.Cat(*p++);
	}
	return WString(wbuf);
}

int LastC(const wchar *b, const wchar *e)
{
	if(e == b)
		return 0;
	return *e;
}

void CSyntax::Grounding(const wchar *b, const wchar *e)
{
	if(b >= e || comment || !iscib(*b))
		return;
	e--;
	while(e > b && (*e == '\t' || *e == ' ' ))
		e--;
	if(*e != ':')
		ClearBraces();
}

void CSyntax::ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size)
{
	CTIMING("ScanSyntax");
	Grounding(ln, e);
	if(!linecont) {
		linecomment = false;
		string = false;
	}
	if(macro != CSyntax::MACRO_CONT)
		macro = CSyntax::MACRO_OFF;
	linecont = e > ln && e[-1] == '\\';
	const wchar *p = ln;
	int lindent = 0, pos = 0;
	while(p < e && (*p == '\t' || *p == ' ')) {
		if(*p++ == '\t' || ++pos >= tab_size) {
			pos = 0;
			lindent++;
		}
	}
	if(!comment && *p == '#') {
		while(++p < e && (*p == ' ' || *p == '\t'))
			p++;
		const wchar *id = p;
		while(p < e && iscidl(*p))
			p++;
		int idlen = int(p - id);
		if(id[0] == 'i' && id[1] == 'f'
		&& (idlen == 2 || idlen == 5 && id[2] == 'd' && id[3] == 'e' && id[4] == 'f'
		|| idlen == 6 && id[2] == 'n' && id[3] == 'd' && id[4] == 'e' && id[5] == 'f')) {
			IfState& ifstate = ifstack.Add();
			ifstate.state = IfState::IF;
			ifstate.iftext = sReadLn(ln);
			ifstate.ifline = line + 1;
		}
		switch(idlen)
		{
		case 6:
			if(id[0] == 'd' && id[1] == 'e' && id[2] == 'f' && id[3] == 'i' && id[4] == 'n' && id[5] == 'e')
				macro = CSyntax::MACRO_CONT;
			break;

		case 4:
			if(id[0] == 'e' && id[1] == 'l') {
				if(id[2] == 'i' && id[3] == 'f') {
					if(ifstack.GetCount() == 0) {
						IfState& ifstate = ifstack.Add();
						ifstate.ifline = 0;
						ifstate.state = IfState::ELSE_ERROR;
					}
					else {
						IfState& ifstate = ifstack.Top();
						if(ifstate.state == IfState::IF || ifstate.state == IfState::ELIF) {
							ifstate.state = IfState::ELIF;
							ifstate.iftext = WString().Cat() << sReadLn(ln) << ", " << ifstate.iftext;
						}
						else
							ifstate.state = IfState::ELSE_ERROR;
					}
				}
				else
				if(id[2] == 's' && id[3] == 'e') {
					if(ifstack.GetCount() == 0) {
						IfState& ifstate = ifstack.Add();
						ifstate.ifline = 0;
						ifstate.state = IfState::ELSE_ERROR;
					}
					else {
						IfState& ifstate = ifstack.Top();
						if(ifstate.state == IfState::IF || ifstate.state == IfState::ELIF) {
							ifstate.state = IfState::ELSE;
							ifstate.iftext = "#else, " + ifstate.iftext;
						}
						else
							ifstate.state = IfState::ELSE_ERROR;
					}
				}
			}
			break;

		case 5:
			if(id[0] == 'e' && id[1] == 'n' && id[2] == 'd' && id[3] == 'i' && id[4] == 'f')
			{
				int itop = ifstack.GetCount() - 1;
				if(itop < 0) {
					IfState& ifstate = ifstack.Add();
					ifstate.ifline = 0;
					ifstate.state = IfState::ENDIF_ERROR;
				}
				else if(ifstack[itop].state != IfState::ENDIF_ERROR)
					ifstack.Trim(itop);
			}
			break;
		}
		if(cl == 0) {
			blk_end.y = line;
			blk_end.x = int(e - ln);
		}
		p = e;
	}
	if(macro == CSyntax::MACRO_CONT && !(p < e && e[-1] == '\\'))
		macro = CSyntax::MACRO_END;
	for(;;) {
		if(comment) {
			p = strnext(p, e, '*');
			if(!p) break;
			if(*++p == '/') {
				comment = false;
				p++;
			}
		}
		else {
			int pc = 0;
			for(;;) {
				int raw_n;
				if(p >= e) goto finish;
				const wchar *pp;
				if(!iscidl(pc) && (pp = isstmt(p)) != NULL) {
					stmtline = line;
					spar = 0;
					pc = 0;
					p = pp;
				}
				else
				if(RawString(p, raw_n))
					p += raw_n;
				else
				if(raw_string.GetCount()) {
					const wchar *s = p;
					const wchar *r = raw_string;
					while(*s && *r) {
						if(*s != *r)
							break;
						s++;
						r++;
					}
					if(*r == '\0') {
						p = s;
						raw_string.Clear();
					}
					else
						p++;
					if(p >= e) goto finish;
				}
				else
				if(!iscidl(pc) && p[0] == 'n' && p[1] == 'a' && p[2] == 'm' && p[3] == 'e' &&
				   p[4] == 's' && p[5] == 'p' && p[6] == 'a' && p[7] == 'c' && p[8] == 'e' &&
				   !iscidl(p[9])) {
					was_namespace = true;
					p += 9;
				}
				else {
					int c = *p++;
					auto Cl0Pos = [&](Point& t) {
						if(cl == 0) {
							t.y = line;
							t.x = int(p - 1 - ln);
						}
					};
					if(c == '/') break;
					if(c == '\'' || c == '\"') {
						p = eatstring(p - 1);
						if(p >= e) {
							string = true;
							goto finish;
						}
					}
					else
					if(c == ';' && spar == 0) {
						seline = stmtline;
						endstmtline = line;
						stmtline = -1;
						was_namespace = false;
						Cl0Pos(blk_end);
					}
					else
					if(c == '{') {
						if(was_namespace) {
							brk.Add(0);
							was_namespace = false;
							Cl0Pos(blk_end);
						}
						else {
							Cl0Pos(blk_start);
							cl++;
							brk.Add('}');
							bid.Add(lindent + 1);
						}
						blk.Add() = line;
						stmtline = -1;
						par.Clear();
					}
					else
					if(c == '}') {
						if(brk.GetCount()) {
							if(brk.Top()) {
								cl--;
								if(bid.GetCount() > 1)
									bid.Drop();
								Cl0Pos(blk_end);
							}
							brk.Drop();
						}
						if(blk.GetCount())
							blk.Drop();
						stmtline = -1;
						par.Clear();
					}
					else
					if(c == '(') {
						pl++;
						brk.Add(')');
						Isx& m = par.Add();
						m.line = line;
						m.pos = int(p - ln);
						spar++;
					}
					else
					if(c == '[') {
						bl++;
						brk.Add(']');
						Isx& m = par.Add();
						m.line = line;
						m.pos = int(p - ln);
						spar++;
					}
					else
					if(c == ')') {
						if(brk.GetCount()) {
							pl--;
							brk.Drop();
						}
						if(par.GetCount())
							par.Drop();
						spar--;
					}
					else
					if(c == ']') {
						if(brk.GetCount()) {
							bl--;
							brk.Drop();
						}
						if(par.GetCount())
							par.Drop();
						spar--;
					}
					pc = c;
				}
			}
			if(*p == '/') {
				linecomment = true;
				goto finish;
			}
			if(*p == '*') {
				comment = true;
				p++;
			}
		}
	}

finish:;
/*	bool hdr = !wascomment && !comment && !linecomment && !string && macro != MACRO_CONT &&
	           (cl == 0 || cl == 1 && blk_start);
	if(hdr != header_line) {
		if(header_line)
			block_header_end = line;
		else
			block_header_start = line;
		header_line = hdr;
	}
	if(cl == 0 && semicolon)
		block_header_end = block_header_start = -1;

	DLOG(line << " " << hdr << ", cl: " << cl << ", start: " << block_header_start << ", end: " << block_header_end
	     << WString(ln, e));*/
}

void CSyntax::Serialize(Stream& s)
{
	s % comment;
	s % linecomment;
	s % string;
	s % linecont;
	s % was_namespace;
	s % macro;
	s % raw_string;

	s % cl % bl % pl;

	s % brk;
	s % blk;
	s % bid;
	s % par;
	s % ifstack;
	
	s % blk_start % blk_end;

	s % stmtline;
	s % endstmtline;
	s % seline;
	s % spar;
	
	s % highlight;
};

}
