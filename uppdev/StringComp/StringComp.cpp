#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{

	ASSERT(0);

// start of U++ String version
	const String a = "AddFrame";
	const String b = "AddFrameSize";

	DUMP(a < b);
	DUMP(a == b);
	DUMP(a > b);

	DUMP(a < "AddFrameSize");
	DUMP(a == "AddFrameSize");
	DUMP(a > "AddFrameSize");

	String less = a < b ? "true" : "false";
	String equal = a == b ? "true" : "false";
	String greater = a > b ? "true" : "false";
//	PromptOK(Format("%s-|<-|%s:-|%s&%s-|==-|%s:-|%s&%s-|>-|%s:-|%s", a, b, less, a, b, equal, a, b, greater));
// end of U++ String version


{
// start of std::string version
	const std::string a = "AddFrame";
	const std::string b = "AddFrameSize";

	String less = a < b ? "true" : "false";
	String equal = a == b ? "true" : "false";
	String greater = a > b ? "true" : "false";
	String au(a.c_str());
	String bu(b.c_str());
//	PromptOK(Format("%s-|<-|%s:-|%s&%s-|==-|%s:-|%s&%s-|>-|%s:-|%s", au, bu, less, au, bu, equal, au, bu, greater));
// end of std::string version
}
}
