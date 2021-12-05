#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int llen = 0;
	auto Eol = [&] {
		VppLog() << '\n';
		llen = 0;
	};
	auto Out = [&](const String& s) {
		llen += s.GetCount() + 1;
		if(llen > 120) {
			Eol();
			llen = s.GetCount() + 1;
		}
		VppLog() << s << ' ';
	};
	auto Is = [&](auto is) {
		for(int i = 0; i < 0x110000; i++)
			if(is(i))
				Out(Format64Hex(i));
		Eol();
	};
	auto To = [&](auto to) {
		for(int i = 0; i < 0x110000; i++) {
			int c = to(i);
			if(c > 32)
				Out(Format64Hex(i) + "->" + Format64Hex(to(i)));
		}
		Eol();
	};
	DLOG("===== IsLetter");
	Is([](int c) { return IsLetter(c); });
	DLOG("===== IsLower");
	Is([](int c) { return IsLower(c); });
	DLOG("===== IsUpper");
	Is([](int c) { return IsUpper(c); });
	DLOG("===== IsMark");
	Is([](int c) { return IsMark(c); });
	DLOG("===== IsRTL");
	Is([](int c) { return IsRTL(c); });
	DLOG("===== ToUpper");
	To([](int c) { return IsUpper(c) ? ToUpper(c) : 0; });
	DLOG("===== ToLower");
	To([](int c) { return IsLower(c) ? ToLower(c) : 0; });
	DLOG("===== ToAscii");
	To([](int c) { return ToAscii(c); });
	DLOG("===== ToUpperAscii");
	To([](int c) { return ToAscii(c); });
	DLOG("===== ToLowerAscii");
	To([](int c) { return ToAscii(c); });
	
	DLOG("===== Decompose");
	for(int i = 0; i < 0x110000; i++) {
		WString dc = UnicodeDecompose(i, true);
		if(dc.GetCount()) {
			VppLog() << Format64Hex(i) << "->";
			for(int c : dc)
				VppLog() << Format64Hex(c) << ' ';
			Eol();
		}
	}

	auto LoadLog = [](const String& path) {
		String s = Filter(LoadFile(path), [](int c) { return c == '\r' ? 0 : c; });
		return s.Mid(max(s.FindAfter("\n"), 0));
	};
	
	String log = LoadLog(GetStdLogPath());
	String etalon = LoadLog(GetDataFile("Etalon.log"));

	SaveFile(GetHomeDirFile("log.txt"), log);
	SaveFile(GetHomeDirFile("etalon.txt"), etalon);
	
	ASSERT(log == etalon);
	
	Cout() << "================= OK\r\n";
}
