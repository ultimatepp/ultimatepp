#include <RichEdit/RichEdit.h>

namespace Upp {

void RichEdit::FindReplaceAddHistory() {
	if(!String(~findreplace.find).IsEmpty())
		findreplace.find.AddHistory();
	if(!String(~findreplace.replace).IsEmpty())
		findreplace.replace.AddHistory();
}

void RichEdit::CloseFindReplace()
{
	if(!persistent_findreplace && findreplace.IsOpen())
		findreplace.Close();
}

bool compare3(const wchar *s, const wchar *a, const wchar *b, int len)
{
	const wchar *e = s + len;
	while(s < e) {
		if(*s != *a && *s != *b)
			return false;
		s++;
		a++;
		b++;
	}
	return true;
}

struct RichFindIterator : RichText::Iterator {
	int     cursor;
	int     fpos;
	WString upperw, lowerw;
	bool    ww;
	int     len;
	bool    back;

	virtual bool operator()(int pos, const RichPara& para)
	{
		WString ptext = para.GetText();
		if((back || pos + ptext.GetLength() > cursor) && ptext.GetLength() >= len) {
			const wchar *q = ptext;
			const wchar *e = ptext.End() - len;
			if(cursor >= pos && !back)
				q += cursor - pos;
			while(q <= e) {
				int p = int(q - ~ptext + pos);
				if(back && p >= cursor)
					return true;
				if(compare3(q, upperw, lowerw, len) &&
				   (!ww || (q + len == e || !IsLetter(q[len])) &&
				           (q == ptext || !IsLetter(q[-1])))) {
					fpos = p;
					if(!back)
						return true;
				}
				q++;
			}
		}
		return false;
	}
};

int  RichEdit::FindPos(bool back)
{
	RichFindIterator fi;
	WString w = findreplace.find.GetText();
	if(findreplace.ignorecase) {
		fi.upperw = ToUpper(w);
		fi.lowerw = ToLower(w);
	}
	else
		fi.upperw = fi.lowerw = w;
	fi.len = w.GetLength();
	fi.ww = findreplace.wholeword;
	fi.back = back;
	fi.fpos = -1;
	if(w.GetLength()) {
		fi.cursor = back ? min(anchor, cursor) : max(anchor, cursor);
		text.Iterate(fi);
		return fi.fpos;
	}
	return -1;
}

void RichEdit::Find(bool back)
{
	bool bak = persistent_findreplace;
	persistent_findreplace = true; // keep it open
	FindReplaceAddHistory();
	if(notfoundfw)
		Move(back ? text.GetLength() : 0, false);
	found = notfoundfw = false;
	int pos = FindPos(back);
	CancelSelection();
	if(pos >= 0) {
		anchor = pos;
		cursor = pos + findreplace.find.GetText().GetLength();
		Finish();
		found = true;
		Size sz = findreplace.GetSize();
		Rect sw = GetScreenView();
		Rect r = sw.CenterRect(sz);
		Rect cr = GetCaretRect();
		if(cr.top < sz.cy + 2 * cr.Height()) {
			r.bottom = sw.bottom - 8;
			r.top = r.bottom - sz.cy;
		}
		else {
			r.top = sw.top + 24;
			r.bottom = r.top + sz.cy;
		}
		findreplace.SetRect(r);
		if(!findreplace.IsOpen())
			findreplace.Open();
		SetFocus();
	}
	else {
		CancelSelection();
		CloseFindReplace();
		notfoundfw = true;
	}
	persistent_findreplace = bak;
}

RichText RichEdit::ReplaceText()
{
	RichText clip;
	RichPara p;
	formatinfo.ApplyTo(p.format);
	p.part.Add();
	formatinfo.ApplyTo(p[0].format);
	p.part.Top().text = findreplace.replace.GetText();
	clip.Cat(p);
	return clip;
}

void RichEdit::Replace()
{
	NextUndo();
	if(IsSelection() && found) {
		FindReplaceAddHistory();
		int c = min(cursor, anchor);
		Remove(c, abs(cursor - anchor));
		anchor = cursor = c;
		Insert(cursor, ReplaceText(), false);
		cursor += findreplace.replace.GetText().GetLength();
		anchor = cursor;
		Finish();
		Find(false);
	}
}

void RichEdit::OpenFindReplace()
{
	NextUndo();
	if(!findreplace.IsOpen()) {
		Size sz = findreplace.GetSize();
		findreplace.SetRect(GetScreenView().CenterRect(sz));
		int l, h;
		if(GetSelection(l, h)) {
			findreplace.amend.Hide();
			findreplace.prev.Hide();
			findreplace.ok.SetLabel(t_("Replace"));
			findreplace.Title(t_("Replace in selection"));
			findreplace.cancel <<= findreplace.Breaker(IDCANCEL);
			findreplace.ok <<= findreplace.Breaker(IDOK);
			if(findreplace.Execute() == IDOK) {
				int len = findreplace.find.GetText().GetLength();
				int rlen = findreplace.replace.GetText().GetLength();
				RichText rtext = ReplaceText();
				cursor = l;
				for(;;) {
					int pos = FindPos(false);
					if(pos < 0 || pos + len >= h)
						break;
					Select(pos, len);
					Remove(pos, len);
					Insert(pos, ReplaceText(), false);
					cursor += pos + rlen;
					h += rlen - len;
				}
				CancelSelection();
				Move(h, false);
			}
			FindReplaceAddHistory();
			findreplace.amend.Show();
			findreplace.prev.Show();
			findreplace.ok.SetLabel(t_("Find"));
			SetupFindReplace0();
		}
		else {
			findreplace.Open();
			findreplace.find.SetFocus();
		}
	}
}

void RichEdit::SetupFindReplace0()
{
	findreplace.Title(t_("Find / Replace"));
	findreplace.cancel ^= [=] { findreplace.Close(); };
	findreplace.ok ^= [=] { Find(false); };
}

void RichEdit::SetupFindReplace()
{
	CtrlLayoutOKCancel(findreplace, "");
	SetupFindReplace0();
	findreplace.amend ^= [=] { Replace(); };
	findreplace.prev ^= [=] { Find(true); };
	notfoundfw = found = false;
	findreplace.NoCenter();
}

}
