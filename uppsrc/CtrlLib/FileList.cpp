#include "CtrlLib.h"

namespace Upp {

int GetTextSize(const FontInfo& fi, const wchar *text, const wchar *end) {
	int w = 0;
	while(text < end)
		w += fi[*text++];
	return w;
}

int GetTextSize(const FontInfo& fi, const WString& text) {
	return GetTextSize(fi, text, text.End());
}

const wchar *GetTextFitLim(const FontInfo& fi, const wchar *txt, const wchar *end, int& cx) {
	for(;;) {
		if(fi[*txt] > cx || txt >= end)
			return txt;
		cx -= fi[*txt++];
	}
}

int GetTextFitCount(const FontInfo& fi, const WString& s, int& cx) {
	return (int)(GetTextFitLim(fi, s, s.End(), cx) - s.Begin());
}

const wchar *strdirsep(const wchar *s) {
	while(*s) {
		if(*s == '\\' || *s == '/') return s;
		s++;
	}
	return NULL;
}

void DrawFileName(Draw& ww, int x0, int y, int wcx0, int cy, const WString& mname, bool isdir, Font font,
                  Color ink, Color extink, const WString& desc, Font descfont, bool justname, Color uln)
{
	for(int pass = IsNull(uln); pass < 2; pass++) {
		NilDraw nd;
		Draw *w = pass ? &ww : &nd;
		FontInfo fi = font.Info();
		int extpos = (isdir ? -1 : mname.ReverseFind('.'));
		int slash = isdir ? -1 : max(mname.ReverseFind('\\'), mname.ReverseFind('/'));
		if(extpos < slash)
			extpos = -1;
		const wchar *ext = extpos >= slash && extpos >= 0 ? mname.Begin() + extpos + 1
		                                                  : mname.End();
		const wchar *name = mname;
		if(justname && slash >= 0)
			name += slash + 1;
		int txtcx = GetTextSize(fi, name);
		int x = x0;
		int wcx = wcx0;
		if(txtcx <= wcx) {
			if(pass == 0)
				ww.DrawRect(x0, y + fi.GetAscent() + 1, txtcx, 1, uln);
			ww.DrawText(x, y, name, font, ink, (int)(ext - name));
			ww.DrawText(x + GetTextSize(fi, name, ext), y, ext, font, extink, (int)(mname.End() - ext));
			if(!IsEmpty(desc) && pass)
				DrawTextEllipsis(ww, x + GetTextSize(fi, name), y, wcx - txtcx,
				                 desc, "...", descfont, extink);
			x += txtcx;
			return;
		}
		else {
			int dot3 = 3 * fi['.'];
			if(2 * dot3 > wcx) {
				int n = GetTextFitCount(fi, name, wcx);
				w->DrawText(x, y, name, font, ink, n);
				x += GetTextSize(fi, name, name + n);
			}
			else {
				const wchar *end = mname.End();
				int dircx = 2 * fi['.'] + fi[DIR_SEP];
				const wchar *bk = strdirsep(name);
				if(bk) {
					wcx -= dircx;
					w->DrawText(x, y, ".." DIR_SEPS, font, SColorDisabled, 3);
					x += dircx;
					do {
						txtcx -= GetTextSize(fi, name, bk + 1);
						name = bk + 1;
						if(txtcx < wcx) {
							w->DrawText(x, y, name, font, ink, (int)(ext - name));
							x += GetTextSize(fi, name, ext);
							w->DrawText(x, y, ext, font, extink, (int)(end - ext));
							x += GetTextSize(fi, ext, end);
							goto end;
						}
						bk = strdirsep(name);
					}
					while(bk);
				}
				wcx -= dot3;
				int extcx = GetTextSize(fi, ext, end);
				if(2 * extcx > wcx || ext == end) {
					int n = GetTextFitCount(fi, name, wcx);
					w->DrawText(x, y, name, font, ink, n);
					x += GetTextSize(fi, name, name + n);
					w->DrawText(x, y, "...", font, SColorDisabled, 3);
					x += dot3;
				}
				else {
					wcx -= extcx;
					int n = (int)(GetTextFitLim(fi, name, end, wcx) - name);
					w->DrawText(x, y, name, font, ink, n);
					x += GetTextSize(fi, name, name + n);
					w->DrawText(x, y, "...", font, SColorDisabled, 3);
					w->DrawText(x + dot3, y, ext, font, extink, (int)(end - ext));
					x += dot3 + extcx;
				}
			}
		}
	end:
		if(pass == 0)
			ww.DrawRect(x0, y + fi.GetAscent() + 1, x - x0, 1, uln);
	}
}

void FileList::Paint(Draw& w, const Rect& r, const Value& q,
		             Color ink, Color paper, dword style) const
{
	const File& m = ValueTo<File>(q);
	bool inv = ((style & Display::SELECT) || (style & Display::CURSOR)) && (style & Display::FOCUS);
	w.DrawRect(r, inv ? SColorHighlight() : paper);
	int x = r.left + 2;
	w.DrawImage(x, r.top + (r.Height() - m.icon.GetSize().cy) / 2, m.icon);
	x += iconwidth;
	x += 2;
	FontInfo fi = m.font.Info();
	DrawFileName(w, x, r.top + (r.Height() - fi.GetHeight()) / 2,
	             r.right - x - 2, r.Height(), WString(m.name), m.isdir, m.font,
	             inv ? SColorHighlightText : m.ink,
	             inv ? SColorHighlightText : m.extink,
	             WString(m.desc), m.descfont, justname, m.underline);
}

Size FileList::GetStdSize(const Value& q) const
{
	const File& m = ValueTo<File>(q);
	FontInfo fi = m.font.Info();
	int cx = GetTextSize(fi, WString(m.name)) + 2 + iconwidth + 2 + 3;
	if(!IsNull(m.desc))
		cx += GetTextSize(m.descfont.Info(), WString(m.desc)) + fi.GetHeight();
	return Size(cx, GetItemHeight());
}

void FileList::StartEdit() {
	if(GetCursor() < 0) return;
	Rect r = GetItemRect(GetCursor());
	const File& cf = Get(GetCursor());
	Font f = cf.font;
	int fcy = f.Info().GetHeight();
	r.left += iconwidth + 2;
	r.top += (r.Height() - fcy - 4) / 2;
	r.bottom = r.top + fcy + 2;
	edit.SetRect(r);
	edit.SetFont(cf.font);
	edit <<= cf.name.ToWString();
	edit.Show();
	edit.SetFocus();
	int pos = int(GetFileExtPos(cf.name) - ~cf.name);
	edit.SetSelection(0, pos);
	sb.Disable();
}

void FileList::EndEdit() {
	KillEdit();
	int b = edit.HasFocus();
	edit.Hide();
	if(b) SetFocus();
}

void FileList::OkEdit() {
	EndEdit();
	int c = GetCursor();
	if(c >= 0 && c < GetCount())
		WhenRename(Get(c).name, ~edit);
}

void FileList::KillEdit()
{
	sb.Enable();
	KillTimeCallback(TIMEID_STARTEDIT);
}

void FileList::LeftDown(Point p, dword flags) {
	int c = GetCursor();
	if(IsEdit()) {
		OkEdit();
		c = -1;
	}
	ColumnList::LeftDown(p, flags);
	KillEdit();
	if(c == GetCursor() && c >= 0 && !HasCapture() && renaming && WhenRename && !(flags & (K_SHIFT|K_CTRL)))
		SetTimeCallback(750, THISBACK(StartEdit), TIMEID_STARTEDIT);
}

bool FileList::FindChar(int from, int chr) {
	for(int i = max(0, from); i < GetCount(); i++) {
		WString x = Get(i).name.ToWString();
		if((int)ToUpper(ToAscii(x[0])) == chr) {
			ClearSelection();
			SetCursor(i);
			return true;
		}
	}
	return false;
}

bool FileList::Key(dword key, int count) {
	if(key == K_ESCAPE && IsEdit()) {
		EndEdit();
		return true;
	}
	if(key == K_ENTER && IsEdit()) {
		OkEdit();
		return true;
	}
	if(accelkey) {
		int c = ToUpper((int)key);
		if(key < 256 && IsAlNum(c)) {
			if(!FindChar(GetCursor() + 1, c))
				FindChar(0, c);
			return true;
		}
	}
	return ColumnList::Key(key, count);
}

void FileList::Insert(int ii,
                      const String& name, const Image& icon, Font font, Color ink,
				      bool isdir, int64 length, Time time, Color extink,
				      const String& desc, Font descfont, Value data, Color uln,
				      bool unixexe, bool hidden)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.name = name;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = IsNull(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	ColumnList::Insert(ii, v, !m.isdir || selectdir);
	KillEdit();
}

void FileList::Set(int ii,
                   const String& name, const Image& icon, Font font, Color ink,
				   bool isdir, int64 length, Time time, Color extink,
				   const String& desc, Font descfont, Value data, Color uln,
				   bool unixexe, bool hidden)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.name = name;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = IsNull(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	ColumnList::Set(ii, v, !m.isdir || selectdir);
	KillEdit();
}

void FileList::Set(int ii, const File& f)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m = f;
	ColumnList::Set(ii, v, !m.isdir);
	KillEdit();
}

void FileList::SetIcon(int ii, const Image& icon)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m = Get(ii);
	m.icon = icon;
	ColumnList::Set(ii, v, !m.isdir);
	KillEdit();
}

void FileList::Add(const File& f)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m = f;
	ColumnList::Add(v, !m.isdir);
}

void FileList::Add(const String& name, const Image& icon, Font font, Color ink,
				   bool isdir, int64 length, Time time, Color extink,
				   const String& desc, Font descfont, Value data, Color uln,
				   bool unixexe, bool hidden)
{
	Value v;
	File& m = CreateRawValue<File>(v);
	m.isdir = isdir;
	m.unixexe = unixexe;
	m.hidden = hidden;
	m.icon = icon;
	m.name = name;
	m.font = font;
	m.ink = ink;
	m.length = length;
	m.time = time;
	m.extink = IsNull(extink) ? ink : extink;
	m.desc = desc;
	m.descfont = descfont;
	m.data = data;
	m.underline = uln;
	ColumnList::Add(v, !m.isdir || selectdir);
}

const FileList::File& FileList::Get(int i) const
{
	return ValueTo<File>(ColumnList::Get(i));
}

String FileList::GetCurrentName() const
{
	return GetCount() && GetCursor() >= 0 && GetCursor() < GetCount() ? Get(GetCursor()).name
	       : Null;
}

int FileList::Find(const char *s) {
	for(int i = 0; i < GetCount(); i++)
		if(Get(i).name == s) return i;
	return -1;
}

bool FileList::FindSetCursor(const char *name) {
	int i = Find(name);
	if(i < 0) return false;
	SetCursor(i);
	return true;
}

struct FileList::FileOrder : public ValueOrder {
	const FileList::Order *order;

	virtual bool operator()(const Value& a, const Value& b) const {
		return (*order)(ValueTo<File>(a), ValueTo<File>(b));
	}
};

void FileList::Sort(const Order& order)
{
	KillEdit();
	FileOrder fo;
	fo.order = &order;
	int i = GetCursor();
	String fn;
	if(i >= 0)
		fn = Get(i).name;
	ColumnList::Sort(fo);
	if(i >= 0)
		FindSetCursor(fn);
}

FileList::FileList() {
	iconwidth = DPI(16);
	ItemHeight(max(Draw::GetStdFontCy(), DPI(17)));
	Ctrl::Add(edit);
	edit.Hide();
	edit.SetFrame(BlackFrame());
	renaming = false;
	justname = false;
	accelkey = false;
	selectdir = false;
	SetDisplay(*this);
}

FileList::~FileList() {}

struct FileListSort : public FileList::Order {
	int  kind;

	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		if(a.isdir != b.isdir)
			return a.isdir;
		if(a.name == "..")
			return b.name != "..";
		if(b.name == "..")
			return false;
		int q = 0;
		int k = kind & ~FILELISTSORT_DESCENDING;
		if(k == FILELISTSORT_TIME)
			q = SgnCompare(a.time, b.time);
		else
		if(k == FILELISTSORT_SIZE)
			q = SgnCompare(a.length, b.length);
		else
		if(k == FILELISTSORT_EXT) {
			const char *ae = strrchr(a.name, '.');
			const char *be = strrchr(b.name, '.');
			if(ae == NULL || be == NULL)
				q = ae ? -1 : be ? 1 : 0;
			else
				q = stricmp(ae, be);
		}
		if(!q)
			q = stricmp(a.name, b.name);
		return kind & FILELISTSORT_DESCENDING ? q > 0 : q < 0;
	}
};

void SortBy(FileList& list, int kind)
{
	FileListSort fs;
	fs.kind = kind;
	list.Sort(fs);
}

}
