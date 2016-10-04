#include <Core/Core.h>

static bool sMatchOr(CParser& p, const Index<String>& flag);

static bool sMatchFlag(CParser& p, const Index<String>& flag)
{
	if(p.Char('!'))
		return !sMatchFlag(p, flag);
	if(p.Char('(')) {
		bool b = sMatchOr(p, flag);
		p.PassChar(')');
		return b;
	}
	if(p.IsEof())
		return true;
	return flag.Find(p.ReadId()) >= 0;
}

static bool sMatchAnd(CParser& p, const Index<String>& flag)
{
	bool b = sMatchFlag(p, flag);
	while(p.IsId() || p.IsChar('!') || p.IsChar('(') || p.Char2('&', '&') || p.Char('&'))
		b = sMatchFlag(p, flag) && b;
	return b;
}

static bool sMatchOr(CParser& p, const Index<String>& flag)
{
	bool b = sMatchAnd(p, flag);
	while(p.Char2('|', '|') || p.Char('|'))
		b = sMatchFlag(p, flag) || b;
	return b;
}

bool MatchExp(const char *exp, const Index<String>& flag)
{
	CParser p(exp);
	try {
		return sMatchOr(p, flag);
	}
	catch(CParser::Error e) {
		LOG("Invalid When expression: " << exp);
		return false;
	}
}

CONSOLE_APP_MAIN
{
	Index<String> flag;
	flag.Add("A");
	flag.Add("B");
	DUMP(MatchExp("", flag));
	DUMP(MatchExp("A", flag));
	DUMP(MatchExp("A B", flag));
	DUMP(MatchExp("A !B", flag));
	DUMP(MatchExp("A C", flag));
	DUMP(MatchExp("A !C", flag));
	DUMP(MatchExp("A !(C)", flag));
	DUMP(MatchExp("A !(C || D)", flag));
	DUMP(MatchExp("A !(A & B)", flag));
	DUMP(MatchExp("A !(C || B)", flag));
	DUMP(MatchExp("D | (A & B)", flag));
	DUMP(MatchExp("D | (A & C)", flag));
	DUMP(MatchExp("D | (A | C)", flag));
	DUMP(MatchExp("D & !(A & C)", flag));
	DUMP(MatchExp("D & !(A &)", flag));
}
