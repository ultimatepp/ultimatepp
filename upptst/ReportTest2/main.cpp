#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	Report r;

	static const String sDotLine = "}}&[A+1H8;h(255.50.25)L1 &]{{25:10:10:10:10:10~a0";
	static const String sLine = "}}&[A+1H8;h(255.50.25) &]{{25:10:10:10:10:10~a0";

	String qtf2;
	qtf2 << "[A+59 {{25:10:10:10:10:10~a0 "
	     << "[@(255.50.25)* [ KLÍČOVÉ SLOVO]:: [> ZOBRAZENÍ]:: [> KLIKY]:: [> CTR]:: "
	     << "[> CENA]:: [> CPC]]"
	     << sLine << ' ';
	qtf2 << "]" << sDotLine << ' ' << "}}";

	r << "^^" + qtf2 + "^^next page^^nextpage";
	Perform(r);
}
