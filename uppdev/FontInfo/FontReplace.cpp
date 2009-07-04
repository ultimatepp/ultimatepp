#include "FontInfo.h"

static const char *sFontReplacements[] = {
	"Symbol",
	"Arial Unicode MS",
	"Arial",
	"MS UI Gothic",
	"MS Mincho",
	"Arial",
	"Gulim",
	"SimSun",
	"PMingLiU",
};

bool Replace(Font fnt, int chr, Font& rfnt)
{
	static Vector<int> rface;
	ONCELOCK {
		for(int i = 0; i < __countof(sFontReplacements); i++) {
			int q = Font::FindFaceNameIndex(sFontReplacements[i]);
			if(q > 0)
				rface.Add(q);
		}
		DDUMP(rface.GetCount());
	}
	static Font last;
	static Font lastR;
	if(last == fnt && GetGlyphInfo(lastR, chr).IsNormal()) {
		rfnt = lastR;
		return true;
	}
	Font f = fnt;
	for(int i = 0; i < rface.GetCount(); i++) {
		f.Face(rface[i]);
		DDUMP(f.GetFaceName());
		if(GetGlyphInfo(f, chr).IsNormal()) {
			LOG("-----------");
			last = fnt;
			rfnt = lastR = f;
			return true;
		}
	}
	return false;
}
