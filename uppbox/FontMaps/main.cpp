#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Progress pi;
	pi.SetTotal(Font::GetFaceCount());
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		String f = Font::GetFaceName(i);
		pi.SetText(f);
		if(pi.StepCanceled())
			break;
		Font fnt;
		fnt.FaceName(f);
		dword l = 0;
		int n = 0;
		for(int ch = 32; ch < 4096; ch++) {
			if(GetGlyphInfo(fnt, ch).IsNormal()) {
				l |= (0x80000000 >> (ch / 128));
		//		ch = (ch + 127) & ~127;
				n++;
			}
		}
		dword h = 0;
		for(int ch = 32; ch < 65536; ch++) {
			if(GetGlyphInfo(fnt, ch).IsNormal()) {
				h |= (0x80000000 >> (ch / 2048));
		//		ch = (ch + 2047) & ~2047;
				n++;
			}
		}
		LOG("{ " << AsCString(f) << ", 0x" << FormatIntHex(l) << ", 0x" << FormatIntHex(h) << " }, // " << n);
	}
	PromptOK("OK");
}
