#include <Core/Core.h>

using namespace Upp;

void ValidateUtf8(const String& src, int begin, int end, int pos)
{
	// Checks for malformed, imcomplete and overlong UTF-8 sequences.
	// Replaces each malformed/illegal byte with the replacement character. (Recommended method)
	// The length of the original line and decoded/replated line MUST be equal for these sequences.
	
	String dest;
	StringStream ss(src);
	bool check_utf8 = pos >= begin && pos < end;
	while(!ss.IsEof()) {
		int c = check_utf8 ? ss.GetUtf8() : ss.Get();
		dest.Cat(c < 0 ? 0xFFFD : c);
	}
	DLOG(dest);
	if(check_utf8)
		ASSERT(dest.GetLength() == src.GetLength());
}

CONSOLE_APP_MAIN
{
	// This autotest uses Marcus Kuhn's UTf-8 stress test text.
	// StdLogSetup(LOG_COUT);
	String text = LoadDataFile("utf8_stress_test.txt");
	ASSERT(!IsNull(text));
	StringStream ss(text);
	int pos = 0;
	int begin = text.Find("3  Malformed sequences");
	int end   = text.Find("5.3 Noncharacter code positions");
	ASSERT(begin >= 0 && begin < end);
	while(!ss.IsEof()) {
		String line = ss.GetLine();
		ValidateUtf8(line, begin, end, pos++);
	}
}
