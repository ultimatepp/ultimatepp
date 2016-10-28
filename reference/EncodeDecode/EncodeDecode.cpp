#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String data = "Hello world, this is test! Also '=' to test QP.";
	String enc;
	
	enc = HexEncode(data);
	DUMP(enc);
	DUMP(HexDecode(enc));
	DUMP(HexEncode(data, 8, ':'));

	enc = Base64Encode(data);
	DUMP(enc);
	DUMP(Base64Decode(enc));

	enc = UrlEncode(data);
	DUMP(enc);
	DUMP(UrlDecode(enc));

	enc = QPEncode(data);
	DUMP(enc);
	DUMP(QPDecode(enc));
	
	DUMP(DeHtml("<foo&bar>"));
}
