#include <CtrlLib/CtrlLib.h>

using namespace Upp;

const char *font[] = {
	"sans-serif",
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
};

GUI_APP_MAIN
{
	LOG("struct...");
	for(int i = 0; i < __countof(font); i++) {
		Font fnt;
		fnt.FaceName(font[i]);
		dword l = 0;
		for(int ch = 32; ch < 4096; ch++) {
			if(GetGlyphInfo(fnt, ch).IsNormal()) {
				l |= (0x80000000 >> (ch / 128));
				ch = (ch + 127) & ~127;
			}
		}
		dword h = 0;
		for(int ch = 32; ch < 65536; ch++) {
			if(GetGlyphInfo(fnt, ch).IsNormal()) {
				h |= (0x80000000 >> (ch / 2048));
				ch = (ch + 2047) & ~2047;
			}
		}
		LOG(AsCString(font[i]) << ", 0x" << FormatIntHex(l) << ", 0x" << FormatIntHex(h) << ',');
	}
}
