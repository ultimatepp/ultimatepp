#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(dword code = 0; code <= 0x10ffff; code++) {
		if(code < 0xee00 || code > 0xeeff) {
			String s = ToUtf8(&code, 1);
			WString t = ToUtf32(s);
			ASSERT(t[0] == code);
			ASSERT(Utf32Len(s) == 1);
			
			Vector<char16> ws = ToUtf16(&code, 1);
			t = ToUtf32(ws);
			ASSERT(t[0] == code);
			ASSERT(Utf32Len(ws) == 1);
			
			ws = ToUtf16(s);
			t = ToUtf32(ws);
			ASSERT(t[0] == code);
			ASSERT(Utf32Len(ws) == 1);
			
			s = ToUtf8(ws);
			t = ToUtf32(s);
			ASSERT(t[0] == code);
			ASSERT(Utf32Len(ws) == 1);
		}
	}
	
	LOG("Single code test passed");
	
	SeedRandom(0);
	for(int i = 0; i < 10000; i++) {
		int n = Random(1000);
		WString text;
		while(text.GetCount() < n) {
			int code = Random(0x110000);
			if(!(code >= 0xee00 && code < 0xeeff || code >= 0xD800 && code < 0xe000))
				text.Cat(code);
		}

		String s = ToUtf8(text);
		WString t = ToUtf32(s);
		ASSERT(t == text);
		ASSERT(Utf32Len(s) == n);
		
		Vector<char16> ws = ToUtf16(text);
		t = ToUtf32(ws);
		if(t != text) {
			DUMP(text);
			DUMP(t);
			DUMP(ws);
		}
		ASSERT(t == text);
		ASSERT(Utf32Len(ws) == n);
		
		ws = ToUtf16(s);
		t = ToUtf32(ws);
		ASSERT(t == text);
		ASSERT(Utf32Len(ws) == n);
		
		s = ToUtf8(ws);
		t = ToUtf32(s);
		ASSERT(t == text);
		ASSERT(Utf32Len(ws) == n);
	}

	LOG("Text test passed");

	SeedRandom(0);
	for(int i = 0; i < 30000; i++) {
		int n = Random(1000);
		String text;
		while(text.GetCount() < n)
			text.Cat(Random(256));
		
		ASSERT(ToUtf8(ToUtf32(text)) == text);
		ASSERT(ToUtf8(ToUtf16(text)) == text);
	}

	LOG("Error escape test passed");

	LOG("========== OK");
}
