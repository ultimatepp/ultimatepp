#include <CtrlLib/CtrlLib.h>

using namespace Upp;

const char *font[] = {
/*	"sans-serif",
	"Arial",
	"Arial Unicode MS",
	"MS UI Gothic", 
	"MS Mincho",
	"Arial",
	"AlArabiya"
	"FreeSerif",
	"Kochi Mincho",
	"Kochi Gothic",
	"Sazanami Mincho",
	"Sazanami Gothic",
	"Gulim",
	"SimSun",
	"PMingLiU",
	"Symbol",
*/	"FreeMono",
	"FontAwesome",
};

GUI_APP_MAIN
{
	Progress pi("Font %d");
	for(int i = 0; i < __countof(font); i++) {
		if(pi.StepCanceled())
			return;
		Font fnt;
		if(Font::FindFaceNameIndex(font[i]) > 0) {
			fnt.FaceName(font[i]);
			dword l = 0;
			for(int ch = 32; ch < 4096; ch++) {
				if(GetGlyphInfo(fnt, ch).IsNormal()) {
					l |= (0x80000000 >> (ch >> 7));
					ch |= 127;
				}
			}
			dword h = 0;
			for(int ch = 32; ch < 65536; ch++) {
				if(GetGlyphInfo(fnt, ch).IsNormal()) {
					h |= (0x80000000 >> ((dword)ch >> 11));
					ch |= 2047;
				}
			}
			LOG(AsCString(font[i]) << ", 0x" << FormatIntHex(l) << ", 0x" << FormatIntHex(h) << ',');
		}
	}
}
