#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Test : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawText(100, 100, WString(0x9500, 1));
	}
};

GUI_APP_MAIN
{
/*	Font f = Arial(40);
	DDUMP(f[0xbd]);
	DDUMP(f['X']);
	
	WString x("销");
	DDUMP(FormatIntHex(x[0]));

	for(int i = 1; i < Font::GetFaceCount(); i++)
		if(Font(i, 10).IsNormal(0x9500))
			DDUMP(Font::GetFaceName(i));
*/
	DDUMP(GetGlyphInfo(Font().FaceName("PMingLiU").Height(20), 0x9500).IsNormal());
	Test().Run();
}
