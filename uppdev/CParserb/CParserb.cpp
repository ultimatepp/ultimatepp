#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String s = LoadFile(GetDataFile("test.txt"));
	CParser p(s);
	while(!p.IsEof()) {
		if(p.IsId()) {
			CParser::Pos pos = p.GetPos();
			DLOG(p.ReadId() << ": " << pos.line << ", " << pos.GetColumn());
		}
		else
			p.SkipTerm();
	}
}
