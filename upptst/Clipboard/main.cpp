#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Progress pi("%d");
	for(;;) {
		if(pi.StepCanceled())
			break;
		WString txt = AsString(Random()).ToWString();
		ClearClipboard();
		AppendClipboardText(txt.ToString());
		WriteClipboardUnicodeText(txt);
		WString txt2 = ReadClipboardUnicodeText();
		DDUMP(txt2);
		ASSERT(txt == txt2);
	}
}
