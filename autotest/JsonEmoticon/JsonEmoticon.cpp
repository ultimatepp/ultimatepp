#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	dword emoticon = 0x1f60d; // Smiling Face With Heart-shaped Eyes
	
	String text = "Český 아침글";
	Vector<dword> h = ToUtf32(text);
	h.Add('\n');
	h.Add(emoticon);
	h.Add(31);
	DUMP(h);
	
	dword ref[] = { 268, 101, 115, 107, 253, 32, 50500, 52840, 44544, 10, 128525, 31 };
	
	ASSERT(__countof(ref) == h.GetCount());
	
	for(int i = 0; i < h.GetCount(); i++)
		ASSERT(h[i] == ref[i]);
	
	text = ToUtf8(h);
	
	String json = AsJSON(text);
	
	DUMP(json);
	
	ASSERT(json == "\"Český 아침글\\n\\uD83D\\uDE0D\\u001F\"");
	
	String text2 = ParseJSON(json);
	
	DUMP(text2);
	
	ASSERT(text2 == text);
	ASSERT(ToUtf32(text2) == h);
	
	LOG("=================== OK");
}
