#include "FontInfo.h"

static const char *sFontReplacements[] = {
	"sans-serif",
	"Arial",
	"Arial Unicode MS",
	"Symbol",
	"文泉驛等寬正黑",
	"文泉驛正黑",
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
};

bool Replace(Font fnt, int chr, Font& rfnt)
{
//	ASSERT(chr >= 0 && chr <
	LOG("Replace: " << fnt << ": " << chr);
	static Vector<int> rface;
	ONCELOCK {
		for(int i = 0; i < __countof(sFontReplacements) && rface.GetCount() < 20; i++) {
			int q = Font::FindFaceNameIndex(sFontReplacements[i]);
			if(q > 0)
				rface.Add(q);
		}
	}

	Font f = fnt;
	for(int i = 0; i < rface.GetCount(); i++)
		if(IsNormal(f.Face(rface[i]), chr)) {
			rfnt = f;
			LOG("Replacement: " << rfnt);
			return true;
		}
	LOG("No replace");
	return false;
}
