#include <Core/Core.h>

using namespace Upp;

void ValidateUtf8(const String& src, int begin, int end, int pos)
{
	// Checks for malformed, imcomplete and overlong UTF-8 sequences.
	// Replaces each malformed/illegal byte with the replacement character. (Recommended method)
	// The length of the original line and decoded/replaced line MUST be equal for these sequences.
	
	String dest;
	StringStream ss(src);
	bool check_utf8 = pos >= begin && pos < end;
	while(!ss.IsEof()) {
		int c = check_utf8 ? ss.GetUtf8() : ss.Get();
		dest.Cat(c < 0 ? 0xFFFD : c);
	}
	if(check_utf8)
		ASSERT(dest.GetLength() == src.GetLength());
}


void CheckLine(const String& l)
{
	StringStream ss(l);
	WString q;
	while(!ss.IsEof())
		q.Cat(ss.GetUtf8());
	if(ss.IsOK() != CheckUtf8(l)) {
		DDUMP(AsCString(l));
		DDUMP(ss.IsOK());
		DDUMP(CheckUtf8(l));
	}
	ASSERT(ss.IsOK() == CheckUtf8(l));
	if(ss.IsOK())
		ASSERT(ToUtf8(q) == l);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	FileIn in(GetDataFile("utf8_stress_test.txt"));

/* in case something does not work
	char h[] = "2.1.5  5 bytes (U-00200000):        \"øˆ€€€\"                                       |";
	String q(h, sizeof(h));
	DDUMPHEX(q);
	CheckUtf8(q);
	CheckLine(q);
*/
	
	while(!in.IsEof()) {
		String l = in.GetLine();
		CheckLine(l);
	}
	
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < 0x110000; i++) {
			if(i >= 0xee00 && i <= 0xeeff) // skip error escapes
				continue;
			WString q = "Test ";
			q.Cat(i);
			if(pass)
				q.Cat(" ..");
			String s = ToUtf8(q);
			StringStream ss(s);
			WString qq;
			while(!ss.IsEof())
				qq.Cat(ss.GetUtf8());
			if(qq != q) {
				DDUMPHEX(i);
				DDUMPHEX(s);
				DDUMPC(qq);
				DDUMPC(q);
			}
			ASSERT(qq == q);
		}

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
	
	LOG("============= OK");
}
