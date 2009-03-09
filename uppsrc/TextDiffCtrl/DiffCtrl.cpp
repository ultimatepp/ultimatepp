#include "TextDiffCtrl.h"

namespace Upp {

TextDiffCtrl::TextDiffCtrl()
{
	left.Gutter(30);
	right.NoGutter();
	Horz(left, right);
	left.WhenScroll = right.ScrollWhen(left);
	right.HideSb();
	right.WhenScroll = left.ScrollWhen(right);
//	compright.NumberBgColor(HistoryBg());
}

static bool SmallDiff(const char *s1, const char *s2)
{
	for(;;) {
		while(*s1 && (byte)*s1 <= ' ')
			s1++;
		while(*s2 && (byte)*s2 <= ' ')
			s2++;
		if(!*s1 || !*s2)
			return !*s1 && !*s2;
		if(*s1 != *s2)
			return false;
		while(*s1 && *s1 == *s2)
			s1++, s2++;
		if((byte)s1[-1] <= ' ')
			continue;
		if((byte)*s1 > ' ' || (byte)*s2 > ' ')
			return false;
	}
}

void TextDiffCtrl::Set(Stream& l, Stream& r)
{
	Vector<String> ll = GetLineMap(l);
	Vector<String> rl = GetLineMap(r);
	Array<TextSection> sections = CompareLineMaps(ll, rl);

	Point left_pos = left.GetPos();
	Point right_pos = right.GetPos();
	int sb_pos = left.GetSb();

	int outln = 0;
	left.SetCount(0);
	right.SetCount(0);
	for(int i = 0; i < sections.GetCount(); i++) {
		const TextSection& sec = sections[i];
		bool diff = !sec.same;
		Color c1 = (diff ? LtBlue() : SBlack()), c2 = (diff ? LtBlue() : SBlack());
		int maxcount = max(sec.count1, sec.count2);
		left.AddCount(maxcount);
		int l;
		for(l = 0; l < sec.count1; l++) {
			int level = (diff ? l < sec.count2 && SmallDiff(ll[sec.start1 + l], rl[sec.start2 + l]) ? 1 : 2 : 0);
			left.Set(outln + l, ll[sec.start1 + l], c1, sec.start1 + l + 1, level);
		}
		for(; l < maxcount; l++)
			left.Set(outln + l, Null, c1, Null, 2);
		right.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			right.Set(outln + l, rl[sec.start2 + l], c2, sec.start2 + l + 1, level);
		}
		for(; l < maxcount; l++)
			right.Set(outln + l, Null, c2, Null, 2);
		outln += maxcount;
	}
}

void TextDiffCtrl::Set(const String& l, const String& r)
{
	StringStream sl(l);
	StringStream sr(r);
	Set(sl, sr);
}

};
