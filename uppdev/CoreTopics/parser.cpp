#include "Core.h"

NAMESPACE_UPP

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

void CParser::ThrowError(const char *s) {
	LLOG("CParser::Error: " << s);
	LLOG(~String(term, min(strlen((const char *)term), 512U)));
	Error err(fn + Format("(%d): ", line) + s);
//	err.term = (const char *)term;
	throw err;
}

bool CParser::Spaces0() {
	LTIMING("Spaces");
	if(*term > ' ' &&
	   !(term[0] == '/' && term[1] == '/') &&
	   !(term[0] == '/' && term[1] == '*'))
		return false;
	for(;;) {
		if(term[0] == '/' && term[1] == '/') {
			term += 2;
			while(*term && *term != '\n')
				term++;
		}
		else
		if(term[0] == '/' && term[1] == '*') {
			term += 2;
			while(*term) {
				if(term[0] == '*' && term[1] == '/') {
					term += 2;
					break;
				}
				if(*term++ == '\n') line++;
			}
		}
		if(!*term) break;
		if(*term > ' ') break;
		if(*term == '\n') line++;
		term++;
	}
	return true;
}

bool CParser::Id0(const char *s) {
	LTIMING("Id");
	const char *t = term + 1;
	s++;
	while(*s) {
		if(*t != *s)
			return false;
		t++;
		s++;
	}
	if(IsAlNum(*t) || *t == '_')
		return false;
	term = t;
	DoSpaces();
	return true;
}

void CParser::PassId(const char *s) throw(Error) {
	LTIMING("PassId");
	if(!Id(s))
		ThrowError(String("missing '") + s + "\'");
}

void CParser::PassChar(char c) throw(Error) {
	LTIMING("PassChar");
	if(!Char(c))
		ThrowError(String("missing '") + c + "\'");
}

void CParser::PassChar2(char c1, char c2) throw(Error) {
	LTIMING("PassChar2");
	if(!Char2(c1, c2))
		ThrowError(String("missing '") + c1 + c2 + "\'");
}

void CParser::PassChar3(char c1, char c2, char c3) throw(Error) {
	LTIMING("PassChar3");
	if(!Char3(c1, c2, c3))
		ThrowError(String("missing '") + c1 + c2 + c3 + "\'");
}

String CParser::ReadId() throw(Error) {
	LTIMING("ReadId");
	if(!IsId())
		ThrowError("missing id");
	String result;
	const char *b = term;
	const char *p = b;
	while(iscid(*p))
		p++;
	term = p;
	DoSpaces();
	return String(b, (int)(uintptr_t)(p - b));
}

String CParser::ReadIdt() throw(Error) {
	if(!IsId())
		ThrowError("missing id");
	StringBuffer result;
	int lvl = 0;
	while(IsAlNum(*term) || *term == '_' || *term == '<' || *term == '>' ||
	      *term == ':' || (*term == ',' || *term == ' ' ) && lvl > 0 ) {
		if(*term == '<') lvl++;
		if(*term == '>') lvl--;
		result.Cat(*term++);
	}
	DoSpaces();
	return result;
}

bool CParser::IsInt() {
	LTIMING("IsInt");
	const char *t = term;
	if(*t == '-' || *t == '+') {
		t++;
		while(*t <= ' ')
			t++;
	}
	return IsDigit(*t);
}

int  CParser::ReadInt() throw(Error) {
	LTIMING("ReadInt");
	int sign = 1;
	if(*term == '-') {
		sign = -1;
		term++;
	}
	else
	if(*term == '+')
		term++;
	Spaces();
	if(!IsDigit(*term))
		ThrowError("missing number");
	int n = 0;
	while(IsDigit(*term))
		n = 10 * n + *term++ - '0';
	DoSpaces();
	return sign * n;
}

bool CParser::IsNumber(int base)
{
	if(IsDigit(*term))
		return true;
	int q = ToUpper(*term) - 'A';
	return q >= 0 && q < base - 10;
}

uint32  CParser::ReadNumber(int base) throw(Error)
{
	LTIMING("ReadNumber");
	uint32 n = 0;
	int q = ctoi(*term);
	if(q < 0 || q >= base)
		ThrowError("missing number");
	for(;;) {
		int c = ctoi(*term);
		if(c < 0 || c >= base)
			break;
		n = base * n + c;
		term++;
	}
	DoSpaces();
	return n;
}

uint64  CParser::ReadNumber64(int base) throw(Error)
{
	LTIMING("ReadNumber");
	uint64 n = 0;
	int q = ctoi(*term);
	if(q < 0 || q >= base)
		ThrowError("missing number");
	for(;;) {
		int c = ctoi(*term);
		if(c < 0 || c >= base)
			break;
		n = base * n + c;
		term++;
	}
	DoSpaces();
	return n;
}

double CParser::ReadDouble() throw(Error)
{
	LTIMING("ReadDouble");
	int sign = 1;
	if(*term == '-') {
		sign = -1;
		term++;
	}
	else
	if(*term == '+')
		term++;
	Spaces();
	if(!IsDigit(*term))
		ThrowError("missing number");
	double n = 0;
	while(IsDigit(*term))
		n = 10 * n + *term++ - '0';
	if(Char('.')) {
		double q = 1;
		while(IsDigit(*term)) {
			q = q / 10;
			n += q * (*term++ - '0');
		}
	}
	if(Char('e') || Char('E'))
		n *= pow(10.0, ReadInt());
	DoSpaces();
	return sign * n;
}

String CParser::ReadOneString(int delim, bool chkend) throw(Error) {
	if(!IsChar(delim))
		ThrowError("missing string");
	term++;
	StringBuffer result;
	for(;;) {
		if(*term == delim) {
			term++;
			DoSpaces();
			return result;
		}
		else
		if(*term == '\\') {
			switch(*++term) {
			case 'a': result.Cat('\a'); term++; break;
			case 'b': result.Cat('\b'); term++; break;
			case 't': result.Cat('\t'); term++; break;
			case 'v': result.Cat('\v'); term++; break;
			case 'n': result.Cat('\n'); term++; break;
			case 'r': result.Cat('\r'); term++; break;
			case 'f': result.Cat('\f'); term++; break;
			case 'x': {
				int hex = 0;
				if(IsXDigit(*++term)) {
					hex = ctoi(*term);
					if(IsXDigit(*++term)) {
						hex = 16 * hex + (*term >= 'A' ? ToUpper(*term) - 'A' + 10 : *term - '0');
						if(IsXDigit(*++term)) {
							hex = 16 * hex + (*term >= 'A' ? ToUpper(*term) - 'A' + 10 : *term - '0');
							term++;
						}
					}
				}
				result.Cat(hex);
				break;
			}
			default:
				if(*term >= '0' && *term <= '7') {
					int oct = *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					result.Cat(oct);
				}
				else
					result.Cat(*term++);
				break;
			}
		}
		else {
			if(*term < ' ') {
				if(chkend) {
					ThrowError("Unterminated string");
					return result;
				}
				if(*term == '\0')
					return result;
			}
			result.Cat(*term++);
		}
	}
	DoSpaces();
	return result;
}

String CParser::ReadOneString(bool chkend) throw(Error)
{
	return ReadOneString('\"', chkend);
}

String CParser::ReadString(int delim, bool chkend) throw(Error) {
	LTIMING("ReadString");
	String result;
	do
		result.Cat(ReadOneString(delim, chkend));
	while(IsChar(delim));
	return result;
}

String CParser::ReadString(bool chkend) throw(Error)
{
	return ReadString('\"', chkend);
}

char CParser::GetChar()
{
	char c = *term++;
	if(c == '\n')
		line++;
	return c;
}

void CParser::SkipTerm()
{
	LTIMING("SkipTerm");
	if(IsId())
		while(IsAlNum(*term) || *term == '_')
			term++;
	else
	if(IsNumber())
		while(IsDigit(*term))
			term++;
	else
	if(IsString())
		ReadString();
	else
	if(IsChar('\''))
		ReadString('\'', false);
	else
	if(*term)
		term++;
	DoSpaces();
}

CParser::Pos  CParser::GetPos()
{
	Pos p;
	p.line = line;
	p.fn = fn;
	p.ptr = (const char *)term;
	return p;
}

void CParser::SetPos(const CParser::Pos& p)
{
	line = p.line;
	fn = p.fn;
	term = p.ptr;
	DoSpaces();
}

CParser::CParser(const char *ptr)
: term(ptr)
{
	line = 1;
	skipspaces = true;
	Spaces();
}

CParser::CParser(const char *ptr, const char *fn, int line)
: term(ptr), line(line), fn(fn)
{
	skipspaces = true;
	Spaces();
}

CParser::CParser()
{
	term = NULL;
	line = 0;
	skipspaces = true;
}

void CParser::Set(const char *_ptr, const char *_fn, int _line)
{
	term = _ptr;
	fn = _fn;
	line = _line;
	skipspaces = true;
	Spaces();
}

void CParser::Set(const char *_ptr)
{
	Set(_ptr, "", 1);
}


inline void NextCStringLine(String& t, const char *linepfx, int& pl)
{
	t << "\"\r\n" << (linepfx ? linepfx : "") << "\"";
	pl = t.GetLength();
}

String AsCString(const char *s, const char *lim, int linemax, const char *linepfx, dword flags)
{
	String t;
	t.Cat('\"');
	int pl = 0;
	bool wasspace = false;
	while(s < lim) {
		if(t.GetLength() - pl > linemax && (!(flags & ASCSTRING_SMART) || wasspace))
			NextCStringLine(t, linepfx, pl);
		wasspace = false;
		switch(*s) {
		case '\a': t.Cat("\\a"); break;
		case '\b': t.Cat("\\b"); break;
		case '\f': t.Cat("\\f"); break;
		case '\t': t.Cat("\\t"); break;
		case '\v': t.Cat("\\v"); break;
		case '\r': t.Cat("\\r"); break;
		case '\"': t.Cat("\\\""); break;
		case '\\': t.Cat("\\\\"); break;
		case '\n': t.Cat("\\n"); wasspace = true; break;
		default:
			if(byte(*s) < 32 || (byte)*s >= 0x7f && (flags & ASCSTRING_OCTALHI) || (byte)*s == 0xff) {
				char h[4];
				int q = (byte)*s;
				h[0] = '\\';
				h[1] = (3 & (q >> 6)) + '0';
				h[2] = (7 & (q >> 3)) + '0';
				h[3] = (7 & q) + '0';
				t.Cat(h, 4);
			}
			else {
				t.Cat(*s);
				if(*s == ' ')
					wasspace = true;
			}
			break;
		}
		s++;
	}
	t.Cat('\"');
	return t;
}

String AsCString(const char *s, int linemax, const char *linepfx, dword flags)
{
	return AsCString(s, s + strlen(s), linemax, linepfx, flags);
}

String AsCString(const String& s, int linemax, const char *linepfx, dword flags)
{
	return AsCString(s, s.End(), linemax, linepfx, flags);
}

END_UPP_NAMESPACE
