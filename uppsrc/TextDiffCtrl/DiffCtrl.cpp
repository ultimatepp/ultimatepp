#include "TextDiffCtrl.h"

namespace Upp {

#define IMAGECLASS DiffImg
#define IMAGEFILE <TextDiffCtrl/Diff.iml>
#include <Draw/iml_source.h>

TextDiffCtrl::TextDiffCtrl()
{
	left.SetLeft();
	next.SetImage(DiffImg::Next());
	prev.SetImage(DiffImg::Prev());
	left.scroll.y.AddFrame(prev);
	left.scroll.y.AddFrame(next);
	next << [=] { FindDiff(true); };
	prev << [=] { FindDiff(false); };
	Horz(left, right);
	left.WhenScroll = right.ScrollWhen(left);
	right.WhenScroll = left.ScrollWhen(right);
	right.HideSb();
	left.WhenLeftDouble = THISBACK(GetLeftLine);
	right.WhenLeftDouble = THISBACK(GetRightLine);
	left.WhenCursor = [=] {
		if(!cl) {
			cl++;
			right.SetLine(left.GetLine());
			cl--;
		}
	};
	
	right.WhenCursor =  [=] {
		if(!cl) {
			cl++;
			left.SetLine(right.GetLine());
			cl--;
		}
	};
}

void TextDiffCtrl::GetLeftLine(int number, int line)
{
	WhenLeftLine(number);
}

void TextDiffCtrl::GetRightLine(int number, int line)
{
	WhenRightLine(number);
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
			left.Set(outln + l, ll[sec.start1 + l], diff, sec.start1 + l + 1, level, diff && l < sec.count2 ? rl[sec.start2 + l] : Null, sec.start1 + l + 1);
		}
		for(; l < maxcount; l++)
			left.Set(outln + l, Null, diff, Null, 2, Null, Null);
		right.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			right.Set(outln + l, rl[sec.start2 + l], diff, sec.start2 + l + 1, level,  diff && l < sec.count1 ? ll[sec.start1 + l] : Null, sec.start2 + l + 1);
		}
		for(; l < maxcount; l++)
			right.Set(outln + l, Null, diff, Null, 2, Null, Null);
		outln += maxcount;
	}
	if(firstdiff >= 0)
		left.SetSb(max(firstdiff - 2, 0));
	left.ClearSelection();
	right.ClearSelection();
}

String TextDiffCtrl::Merge(bool l, bool cr)
{
	ASSERT(left.GetCount() == right.GetCount());
	const TextCompareCtrl& target = l ? left : right;
	const TextCompareCtrl& source = l ? right : left;
	String r;
	String eol = cr ? "\r\n" : "\n";
	for(int i = 0; i < target.GetCount(); i++) {
		if(source.IsSelected(i) && source.HasLine(i))
			r << source.GetText(i) << eol;
		else
		if(target.HasLine(i))
			r << target.GetText(i) << eol;
	}
	r.TrimEnd(eol);
	return r;
}

String TextCompareCtrl::RemoveSelected(bool cr)
{
	String r;
	String eol = cr ? "\r\n" : "\n";
	for(int i = 0; i < GetCount(); i++)
		if(!IsSelected(i) && HasLine(i))
			r << GetText(i) << eol;
	r.TrimEnd(eol);
	return r;
}

void TextDiffCtrl::Set(const String& l, const String& r)
{
	StringStream sl(l);
	StringStream sr(r);
	Set(sl, sr);
}

void TextDiffCtrl::FindDiff(bool fw)
{
	int i = left.scroll.GetY() + (fw ? 2 * left.scroll.GetPage().cy / 3 : -1);
	while(i > 0 && i < left.lines.GetCount()) {
		if(left.lines[i].diff) {
			left.SetSb(max(i - 2, 0));
			return;
		}
		i += fw ? 1 : -1;
	}
}

bool TextDiffCtrl::Key(dword key, int count)
{
	switch(key) {
	case K_F3:
		FindDiff(true);
		return true;
	case K_SHIFT_F3:
		FindDiff(false);
		return true;
	}
	return Splitter::Key(key, count);
}

INITBLOCK {
	RegisterGlobalConfig("diff");
}

bool DiffDlg::Key(dword key, int count)
{
	switch(key) {
	case K_F5:
	case K_INSERT:
	case K_ENTER:
	case K_SPACE:
		Write();
		return true;
	case K_F8:
	case K_DELETE:
		remove.WhenAction();
		return true;
	}
	return TopWindow::Key(key, count);
}

void DiffDlg::Close()
{
	if(serialize_placement) {
		StringStream ss;
		SerializePlacement(ss);
		String h = ss;
		StoreToGlobal(h, "diff");
	}
	TopWindow::Close();
}

void DiffDlg::Set(const String& f)
{
	editfile = f;
	l <<= editfile;
	Title(editfile);
	if(serialize_placement) {
		String h;
		LoadFromGlobal(h, "diff");
		StringStream ss(h);
		SerializePlacement(ss);
	}
}

void DiffDlg::Refresh()
{
	int sc = diff.GetSc();
	diff.Set(LoadFile(editfile), extfile);
	diff.Sc(sc);
}

bool HasCrs(const String& path)
{
	FileIn in(path);
	if(in) {
		while(!in.IsEof()) {
			int c = in.Get();
			if(c == '\r')
				return true;
			if(c == '\n')
				return false;
		}
	}
	return false;
}

void DiffDlg::Write()
{
	revert.Enable();
	if(diff.right.IsSelection()) {
		SaveFile(editfile, diff.Merge(true, HasCrs(editfile)));
		Refresh();
		return;
	}
	if(PromptYesNo("Do you want to overwrite&[* " + DeQtf(editfile) + "] ?")) {
		SaveFile(editfile, extfile);
		Close();
	}
}

Event<const String&, Vector<LineEdit::Highlight>&, const WString&> DiffDlg::WhenHighlight;

DiffDlg::DiffDlg()
{
	Add(diff.SizePos());
	Sizeable().Zoomable();
	diff.InsertFrameLeft(p);

	p.Height(EditField::GetStdHeight());
	p.Add(l.VSizePos().HSizePosZ(0, 222));
	p.Add(write.VSizePos().RightPosZ(0, 70));
	p.Add(remove.VSizePos().RightPosZ(74, 70));
	p.Add(revert.VSizePos().RightPosZ(148, 70));

	write << [=] { Write(); };
	write.SetLabel("Copy");
	write.SetImage(DiffImg::CopyLeft());
	write.Tip("F5");

	revert.Disable();
	revert.SetLabel("Revert");
	revert.SetImage(CtrlImg::undo());
	revert << [=] {
		if(PromptYesNo("Revert changes?")) {
			SaveFile(editfile, backup);
			Refresh();
		}
	};
	
	remove.SetLabel("Remove");
	remove.SetImage(CtrlImg::remove());
	remove.Tip("F8");
	remove << [=] {
		SaveFile(editfile, diff.left.RemoveSelected(HasCrs(editfile)));
		Refresh();
		revert.Enable();
	};
	
	diff.left.WhenSel << [=] {
		remove.Enable(diff.left.IsSelection());
	};
	
	l.SetReadOnly();

	diff.right.WhenHighlight = diff.left.WhenHighlight = [=](Vector<LineEdit::Highlight>& hl, const WString& ln) {
		DiffDlg::WhenHighlight(editfile, hl, ln);
	};
}

void FileDiff::Open()
{
	if(IsNull(r)) {
		if(!fs.ExecuteOpen())
			return;
		r <<= ~fs;
	}
	if(IsNull(r))
		return;
	Finish();
}

void FileDiff::Finish()
{
	backup = LoadFile(editfile);
	diff.Set(backup, extfile = LoadFile(~~r));
}

FileDiff::FileDiff(FileSel& fs_)
: fs(fs_)
{
	r.Height(EditField::GetStdHeight());
	Icon(DiffImg::Diff());
	diff.InsertFrameRight(r);
	r <<= THISBACK(Open);
}

void FileDiff::Set(const String& f)
{
	editfile = f;
	Open();
}

void FileDiff::Set(const String& lpath, const String& rpath)
{
	r <<= rpath;
	DiffDlg::Set(lpath);
	Finish();
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
