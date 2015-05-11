#include "CppBase.h"

// #define LOGNEXT _DBG_

NAMESPACE_UPP

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

#define case_id \
	case '_':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h': \
	case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q': \
	case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z': \
	case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
	case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
	case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case '$'

#define case_nonzero_digit \
	case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9'


const char *_CppKeyword[] = {
#define CPPID(x) #x,
#include "keyword.i"
#undef  CPPID
	NULL
};

const char **CppKeyword() { return _CppKeyword; }


LexSymbolStat::LexSymbolStat() :
  minSymbol(0)
{
}

void LexSymbolStat::Reset(int minSymbol, int maxSymbol)
{
	ASSERT(minSymbol <= maxSymbol);
	v.Clear();
	this->minSymbol = minSymbol;
	v.SetCount(maxSymbol - minSymbol + 1, 0);
}

void LexSymbolStat::IncStat(int symbol)
{
	int symbolIndex = symbol - minSymbol;
	if(symbolIndex >= 0 && symbolIndex < v.GetCount())
	  v[symbolIndex]++;
}

int  LexSymbolStat::GetStat(int symbol) const
{
	int symbolIndex = symbol - minSymbol;
	return (symbolIndex >= 0 && symbolIndex < v.GetCount()) ?
	       v[symbolIndex] :
	       0;
}

int  LexSymbolStat::SumStat(const Vector<int> & symbols) const
{
	int sum = 0;
	for(int i = 0; i < symbols.GetCount(); i++)
		sum += GetStat(symbols[i]);
	return sum;
}

void LexSymbolStat::Merge(const LexSymbolStat & other)
{
	if(v.GetCount() == 0) {
		minSymbol = other.minSymbol;
		v <<= other.v;
		return;
	}
	ASSERT(other.minSymbol == minSymbol && other.v.GetCount() == v.GetCount());
	for(int i = 0; i < v.GetCount(); i++)
		v[i] += other.v[i];
}


Lex::Lex()
:	statsCollected(false)
{
	const char **cppk = CppKeyword();
	for(int i = 0; cppk[i]; i++)
		id.Add(cppk[i]);
	endkey = id.GetCount();
	braceslevel = body = 0;
}

void Lex::Init(const char *s)
{
	ptr = s;
}

void Lex::StartStatCollection()
{
	symbolStat.Reset(-200, endkey+256);
	statsCollected = true;
}

const LexSymbolStat& Lex::FinishStatCollection()
{
	statsCollected = false;
	return symbolStat;
}

int Lex::GetCharacter()
{
	if(*ptr == '\0') return t_eof;
	int c = *ptr++;
	if(c == '\\') {
		c = *ptr++;
		switch(c) {
		case 'a': return '\a';
		case 'b': return '\b';
		case 't': return '\t';
		case 'v': return '\v';
		case 'n': return '\n';
		case 'r': return '\r';
		case 'f': return '\f';
		case 'x':
			c = 0;
			if(isxdigit(*ptr)) {
				c = (*ptr >= 'A' ? ToUpper(*ptr) - 'A' + 10 : *ptr - '0');
				ptr++;
				if(isxdigit(*ptr)) {
					c = 16 * c + (*ptr >= 'A' ? ToUpper(*ptr) - 'A' + 10 : *ptr - '0');
					ptr++;
				}
			}
			break;
		default:
			if(c >= '0' && c <= '7') {
				c -= '0';
				if(*ptr >= '0' && *ptr <= '7')
					c = 8 * c + *ptr++ - '0';
				if(*ptr >= '0' && *ptr <= '7')
					c = 8 * c + *ptr++ - '0';
			}
		}
	}
	return (byte)c;
}

void Lex::Next()
{
	grounding = false;
	while((byte)*ptr <= ' ') {
		if(*ptr == '\2')
			grounding = true;
		if(*ptr == '\0') return;
		ptr++;
	}
	pos = ptr;
	int c = (byte)*ptr++;
	if(c == '\0') return;
	switch(c) {
	case_id: {
			String x;
			x.Reserve(12);
			x.Cat(c);
			while(iscid(*ptr))
				x.Cat(*ptr++);
			int q = id.FindAdd(x);
			if(q == tk_rval_ - 256) { // simple hack for transitionary macro
				AddCode('&');
				AddCode('&');
			}
			else
				AddCode(q + 256);
			break;
		}
	case ':': AddCode(Char(':') ? t_dblcolon : ':'); break;
	case '*': AssOp('*', t_mulass); break;
	case '/': AssOp('/', t_divass); break;
	case '%': AssOp('%', t_modass); break;
	case '^': AssOp('^', t_xorass); break;
	case '!': AssOp('!', t_neq); break;
	case '.':
		if(Char('*')) AddCode(t_dot_asteriks);
		else
		if(*ptr == '.' && ptr[1] == '.') {
			AddCode(t_elipsis);
			ptr += 2;
		}
		else
			AddCode('.');
		break;
	case '+':
		if(Char('+')) AddCode(t_inc);
		else
			AssOp('+', t_addass);
		return;
	case '-':
		if(Char('-')) AddCode(t_dec);
		else
		if(Char('>'))
			AddCode(Char('*') ? t_arrow_asteriks : t_arrow);
		else
			AssOp('-', t_subass);
		break;
	case '&':
		if(Char('&'))
			AddCode(t_and);
		else
			AssOp('&', t_andass);
		break;
	case '|':
		if(Char('|'))
			AddCode(t_or);
		else
			AssOp('|', t_orass);
		break;
	case '=':
		AssOp('=', t_eq);
		break;
	case '<':
		if(Char('<'))
			AssOp(t_shl, t_shlass);
		else
			AssOp('<', t_le);
		break;
	case '>':
		if(Char('>'))
			AssOp(t_shr, t_shrass);
		else
			AssOp('>', t_ge);
		break;
	case '0': {
			dword w = 0;
			if(Char('x') || Char('X')) {
				for(;;) {
					int d;
					if(*ptr >= '0' && *ptr <= '9')
						d = *ptr - '0';
					else
					if(*ptr >= 'A' && *ptr <= 'F')
						d = *ptr - 'A' + 10;
					else
					if(*ptr >= 'a' && *ptr <= 'f')
						d = *ptr - 'a' + 10;
					else
						break;
					if(w >= 0x8000000u - d) {
						AddCode(te_integeroverflow);
						return;
					}
					w = w * 16 + d - '0';
					ptr++;
				}
			}
			else
				while(*ptr >= '0' && *ptr <= '7') {
					int d = *ptr++ - '0';
					if(w >= 0x1000000u - d) {
						AddCode(te_integeroverflow);
						return;
					}
					w = w * 8 + d - '0';
				}
			Term& tm = term.AddTail();
			tm.code = t_integer;
			tm.ptr = pos;
			tm.number = w;
		}
		break;
	case_nonzero_digit: {
			double w = c - '0';
			bool fp = false;
			while(*ptr >= '0' && *ptr <= '9')
				w = w * 10 + *ptr++ - '0';
			if(*ptr == '.') { //TODO TO BE Completed !!!
				fp = true;
				ptr++;
				double x = 0.1;
				while(*ptr >= '0' && *ptr <= '9') {
					w += x * (*ptr++ - '0');
					x /= 10;
				}
			}
			Term& tm = term.AddTail();
			if(fp || w < INT_MIN || w > INT_MAX)
				tm.code = t_double;
			else
				tm.code = t_integer;
			tm.ptr = pos;
			tm.number = w;
		}
		break;
	case '\'': {
			Term& tm = term.AddTail();
			tm.code = t_character;
			tm.ptr = pos;
			tm.text = String(GetCharacter(), 1);
			if(*ptr == '\'')
				ptr++;
			else
				tm.code = te_badcharacter;
		}
		break;
	case '\"': {
			Term& tm = term.AddTail();
			tm.code = t_string;
			tm.ptr = pos;
			for(;;) {
				while(*ptr != '\"') {
					if((byte)*ptr < ' ' && *ptr != 9) {
						tm.code = te_badstring;
						return;
					}
					tm.text.Cat(GetCharacter());
				}
				ptr++;
				while(*ptr && (byte)*ptr <= ' ') ptr++;
				if(*ptr != '\"') break;
				ptr++;
			}
		}
		break;
	default:
		AddCode(c);
		return;
	}
}

bool Lex::Prepare(int pos) {
	while(term.GetCount() <= pos) {
		if(*ptr == '\0') return false;
		Next();
	}
#ifdef _DEBUG
	pp = term[0].ptr;
#endif
	return true;
}

int Lex::Code(int pos)
{
	if(!Prepare(pos)) return t_eof;
	return term[pos].code;
}

bool   Lex::IsId(int pos)
{
	return Code(pos) >= endkey + 256;
}

void Lex::ThrowError(const char *e)
{
	WhenError(e);
	throw Parser::Error();
}

String Lex::Id(int pos)
{
	if(!IsId(pos))
		ThrowError("expected id");
	return id[Code(pos) - 256];
}

void Lex::Get(int n)
{
	while(n--) {
		if(term.GetCount()) {
			if(body && term.Head().grounding)
				throw Grounding();
			int chr = term.Head().code;
			if(statsCollected)
				symbolStat.IncStat(chr);
			if(chr == '{')
				braceslevel++;
			else
			if(chr == '}')
				braceslevel--;
			term.DropHead();
		}
		if(term.GetCount() == 0)
			Next();
		if(term.GetCount() == 0)
			break;
	}
#ifdef LOGNEXT
	Dump(0);
#endif
}

void Lex::Dump(int pos)
{
#ifdef LOGNEXT
	int code = Code(pos);
	switch(code) {
	case t_string: LOG(AsCString(Text(pos))); break;
	case t_double: LOG(Double(pos)); break;
	case t_integer: LOG(Int(pos)); break;
	case t_character: LOG("char " << AsCString(String(Chr(pos), 1))); break;
	default:
		if(code < 0)
			LOG(decode(Code(),
				t_dblcolon, "::",
				t_mulass, "*=",
				t_divass, "/=",
				t_modass, "%=",
				t_xorass, "^=",
				t_neq, "!=",
				t_dot_asteriks, ".*",
				t_elipsis, "...",
				t_inc, "++",
				t_addass, "+=",
				t_dec, "--",
				t_arrow_asteriks, "->*",
				t_arrow, "->",
				t_subass, "-=",
				t_and, "&&",
				t_andass, "&=",
				t_or, "||",
				t_orass, "|=",
				t_eq, "==",
				t_shl, "<<",
				t_shlass, "<<=",
				t_le, "<=",
				t_shr, ">>",
				t_shrass, ">>=",
				t_ge, ">=",
				te_integeroverflow, "<integer overflow>",
				te_badcharacter, "<bad char>",
				te_badstring, "<bad string>",
				"???"));
		else
		if(code < 256)
			LOG((char)code);
		else
			LOG(id[code - 256]);
	}
#endif
}

void Lex::SkipToGrounding()
{
	for(;;) {
		if(term.GetCount() == 0)
			Next();
		if(term.GetCount() == 0)
			break;
		int chr = term.Head().code;
		if(chr == t_eof)
			return;
		if(term.Head().grounding)
			return;
		if(chr == '{')
			braceslevel++;
		else
		if(chr == '}')
			braceslevel--;
		term.DropHead();
	}
}

int Lex::Int(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_integer)
		ThrowError("expected integer literal");
	return (int)term[pos].number;
}

double Lex::Double(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_double)
		ThrowError("expected floating point literal");
	return term[pos].number;
}

String Lex::Text(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_string)
		ThrowError("expected string literal");
	return term[pos].text;
}

int Lex::Chr(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_character)
		ThrowError("expected character literal");
	return (byte)*term[pos].text;
}

const char *Lex::Pos(int pos)
{
	Prepare(pos);
	return pos < term.GetCount() ? term[pos].ptr : ptr;
}

END_UPP_NAMESPACE
