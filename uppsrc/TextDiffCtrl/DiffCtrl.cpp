#include "TextDiffCtrl.h"

namespace Upp {

#define IMAGECLASS DiffImg
#define IMAGEFILE <TextDiffCtrl/Diff.iml>
#include <Draw/iml_source.h>

TextDiffCtrl::TextDiffCtrl()
{
	left.Gutter(30);
	right.NoGutter();
	Horz(left, right);
	left.WhenScroll = right.ScrollWhen(left);
	right.WhenScroll = left.ScrollWhen(right);
	right.HideSb();
	left.WhenLeftDouble = Proxy(WhenLeftLine);
	right.WhenLeftDouble = Proxy(WhenRightLine);
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

//	Point left_pos = left.GetPos();
//	Point right_pos = right.GetPos();
//	int sb_pos = left.GetSb();

	int outln = 0;
	left.SetCount(0);
	right.SetCount(0);
	int firstdiff = -1;
	for(int i = 0; i < sections.GetCount(); i++) {
		const TextSection& sec = sections[i];
		bool diff = !sec.same;
		if(firstdiff < 0 && diff)
			firstdiff = outln;
		int maxcount = max(sec.count1, sec.count2);
		left.AddCount(maxcount);
		int l;
		for(l = 0; l < sec.count1; l++) {
			int level = (diff ? l < sec.count2 && SmallDiff(ll[sec.start1 + l], rl[sec.start2 + l]) ? 1 : 2 : 0);
			left.Set(outln + l, ll[sec.start1 + l], diff, sec.start1 + l + 1, level);
		}
		for(; l < maxcount; l++)
			left.Set(outln + l, Null, diff, Null, 2);
		right.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			right.Set(outln + l, rl[sec.start2 + l], diff, sec.start2 + l + 1, level);
		}
		for(; l < maxcount; l++)
			right.Set(outln + l, Null, diff, Null, 2);
		outln += maxcount;
	}
	if(firstdiff >= 0)
		left.SetSb(max(firstdiff - 2, 0));
	left.ClearSelection();
	right.ClearSelection();
}

void TextDiffCtrl::Set(const String& l, const String& r)
{
	StringStream sl(l);
	StringStream sr(r);
	Set(sl, sr);
}

INITBLOCK {
	RegisterGlobalConfig("diff");
}

void DiffDlg::Execute(const String& f)
{
	editfile = f;
	l <<= editfile;
	Title(editfile);
	String h;
	{
		LoadFromGlobal(h, "diff");
		StringStream ss(h);
		SerializePlacement(ss);
	}
	TopWindow::Execute();
	{
		StringStream ss;
		SerializePlacement(ss);
		h = ss;
		StoreToGlobal(h, "diff");
	}
}

void DiffDlg::Write()
{
	if(PromptYesNo("Do you want to overwrite&[* " + DeQtf(editfile) + "] ?")) {
		SaveFile(editfile, extfile);
		Break(IDOK);
	}
}

Callback3<const String&, Vector<LineEdit::Highlight>&, const WString&> DiffDlg::WhenHighlight;

static void sDoHighlight(Vector<LineEdit::Highlight>& hl, const WString& ln, const String *path)
{
	DiffDlg::WhenHighlight(*path, hl, ln);
}

DiffDlg::DiffDlg()
{
	Add(diff.SizePos());
	Sizeable().Zoomable();
	diff.InsertFrameLeft(p);
	int cy = EditField::GetStdHeight();
	p.Height(cy);
	p.Add(l.VSizePos().HSizePos(0, 6 * cy));
	p.Add(write.VSizePos().RightPos(0, 6 * cy));
	write <<= THISBACK(Write);
	write.SetLabel("Overwrite <-");
	l.SetReadOnly();
	diff.left.WhenHighlight = callback1(sDoHighlight, &editfile);
	diff.right.WhenHighlight = callback1(sDoHighlight, &editfile);
}

void FileDiff::Open()
{
	if(!fs.ExecuteOpen())
		return;
	String f = ~fs;
	r <<= f;
	diff.Set(LoadFile(editfile), extfile = LoadFile(f));
}

void FileDiff::Execute(const String& f)
{
	editfile = f;
	Open();
	if(IsNull(r))
		return;
	DiffDlg::Execute(f);
}

FileDiff::FileDiff(FileSel& fs_)
: fs(fs_)
{
	r.Height(EditField::GetStdHeight());
	Icon(DiffImg::Diff());
	diff.InsertFrameRight(r);
	r <<= THISBACK(Open);
}

FileSel& DiffFs() {
	static FileSel fs;
	ONCELOCK {
		fs.Type("Patch file (*.diff, *.patch)", "*.diff *.patch");
		fs.AllFilesType();
	}
	return fs;
}

};
