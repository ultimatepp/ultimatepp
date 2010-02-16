#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	WString text = ReadClipboardUnicodeText();
	WString r;
	const wchar *s = text;
	while(*s) {
		if(WString("hkrdtn").Find(*s) >= 0 && WString("yý").Find(s[1]) >= 0 ||
		   WString("žščřcjďťňdtn").Find(*s) >= 0 && WString("ií").Find(s[1]) >= 0) {
			r.Cat(*s);
			r.Cat("_");
			s += 2;
			if(*s == ' ')
				r.Cat(' ');
		}
		else
		if(s[0] == 'c' && s[1] == 'h' && WString("yý").Find(s[2]) >= 0) {
			r.Cat("ch_");
			if(*s == ' ')
				r.Cat(' ');
			s += 3;
		}
		else {
			r.Cat(*s);
			if(*s == '.' && s[1] != ' ')
				r.Cat(' ');
			s++;
		}
	}
	Report report;
	report << "[R4/ " + DeQtfLf(r.ToString());
	Perform(report);
}
