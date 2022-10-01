#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const VectorMap<int, String> utf8text = {
		{ 0, "" },
		{ 0, "-- Complete UTF-8 sequences --" },
		{ 0, "ABCDEFG\x24" },
		{ 0, "ABCDEFG\xC2\xA3" },
		{ 0, "ABCDEFG\xED\x95\x9C" },
		{ 0, "ABCDEFG\xF0\x90\x8D\x88" },
		{ 0, "-- Incomplete UTF-8 sequences --" },
		{ 1, "ABCDEFG\xC2" },			// 2-byte UTF8 sequence.
		{ 2, "ABCDEFG\xED\x95" },		// 3-byte UTF8 sequence.
		{ 3, "ABCDEFG\xF0\x90\x8D" },	// 4-byte UTF8 sequence.
	};
	
	for(const auto& keyval : ~utf8text) {
		ASSERT(keyval.key == CheckUtf8Split(keyval.value));
	}

	DLOG("OK");
}
