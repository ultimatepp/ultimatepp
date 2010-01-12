#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

TextCtrl::TextCtrl()
{
	Unicode();
	undosteps = 10;
	Clear();
	undoserial = 0;
	incundoserial = false;
	undo_op = false;
	WhenBar = THISBACK(StdBar);
	charset = CHARSET_UNICODE;
	color[INK_NORMAL] = SColorText;
	color[INK_DISABLED] = SColorDisabled;
	color[INK_SELECTED] = SColorHighlightText;
	color[PAPER_NORMAL] = SColorPaper;
	color[PAPER_READONLY] = SColorFace;
	color[PAPER_SELECTED] = SColorHighlight;
	processtab = true;
	nobg = false;
}

TextCtrl::~TextCtrl() {}

void TextCtrl::MiddleDown(Point p, dword flags)
{
	if(IsReadOnly())
		return;
	if(AcceptText(Selection())) {
		WString w = GetWString(Selection());
		selclick = false;
		LeftDown(p, flags);
		Paste(w);
		Action();
	}
}

void TextCtrl::CancelMode()
{
	selclick = false;
	dropcaret = Null;
	isdrag = false;
}

void TextCtrl::Clear()
{
	cline = cpos = 0;
	total = 0;
	line.Clear();
	line.Shrink();
	ClearLines();
	line.Add();
	InsertLines(0, 1);
	DirtyFrom(0);
	anchor = -1;
	cursor = 0;
	SelectionChanged();
	Refresh();
	undo.Clear();
	redo.Clear();
	ClearDirty();
	SetSb();
	PlaceCaret(0);
}

void TextCtrl::DirtyFrom(int line) {}
void TextCtrl::SelectionChanged() {}
void TextCtrl::ClearLines() {}
void TextCtrl::InsertLines(int line, int count) {}
void TextCtrl::RemoveLines(int line, int count) {}
void TextCtrl::PreInsert(int pos, const WString& text) {}
void TextCtrl::PostInsert(int pos, const WString& text) {}
void TextCtrl::PreRemove(int pos, int size) {}
void TextCtrl::PostRemove(int pos, int size) {}
void TextCtrl::RefreshLine(int i) {}
void TextCtrl::InvalidateLine(int i) {}
void TextCtrl::SetSb() {}
void TextCtrl::PlaceCaret(int newcursor, bool sel) {}

void   TextCtrl::CachePos(int pos) {
	int p = pos;
	cline = GetLinePos(p);
	cpos = pos - p;
}

void   TextCtrl::Load(Stream& s, byte charset) {
	Clear();
	line.Clear();
	ClearLines();
	String ln;
	total = 0;
	while(!s.IsEof()) {
		int c = s.Get();
		if(c == '\n') {
			WString w = ToUnicode(ln, charset);
			line.Add(w);
			total += w.GetLength() + 1;
			ln.Clear();
			ln.Reserve(1000);
		}
		if(c >= ' ' || c == '\t')
			ln.Cat(c);
	}
	ln.Shrink();
	WString w = ToUnicode(ln, charset);
	line.Add(w);
	total += w.GetLength();
	InsertLines(0, line.GetCount());
	Update();
	SetSb();
	SetCharset(charset);
	PlaceCaret(0);
}

void   TextCtrl::Save(Stream& s, byte charset, bool crlf) const {
	charset = ResolveCharset(charset);
	for(int i = 0; i < line.GetCount(); i++) {
		if(i)
			if(crlf)
				s.PutCrLf();
			else
				s.PutEol();
		String txt = charset == CHARSET_UTF8 ? line[i].text
		                                     : FromUnicode(line[i], charset);
		const char *e = txt.End();
		for(const char *w = txt; w != e; w++)
			s.Put(*w == DEFAULTCHAR ? '?' : *w);
	}
}

void   TextCtrl::Set(const String& s, byte charset) {
	StringStream ss(s);
	Load(ss, charset);
}

String TextCtrl::Get(byte charset) const
{
	StringStream ss;
	Save(ss, charset);
	return ss;
}

int TextCtrl::GetInvalidCharPos(byte charset) const
{
	int q = 0;
	if(charset != CHARSET_UTF8)
		for(int i = 0; i < line.GetCount(); i++) {
			WString txt = line[i];
			WString ctxt = ToUnicode(FromUnicode(txt, charset), charset);
			for(int w = 0; w < txt.GetLength(); w++)
				if(txt[w] != ctxt[w])
					return q + w;
			q += txt.GetLength() + 1;
		}
	return -1;
}

void   TextCtrl::ClearDirty()
{
	dirty = 0;
	ClearModify();
	WhenState();
}

TextCtrl::UndoData TextCtrl::PickUndoData()
{
	UndoData data;
	data.undo = undo;
	data.redo = redo;
	data.undoserial = undoserial;
	return data;
}

void TextCtrl::SetPickUndoData(pick_ TextCtrl::UndoData& data)
{
	undo = data.undo;
	redo = data.redo;
	undoserial = data.undoserial;
	incundoserial = true;
}

void TextCtrl::Set(const WString& s)
{
	Clear();
	Insert0(0, s);
}

void  TextCtrl::SetData(const Value& v)
{
	Set((WString)v);
}

Value TextCtrl::GetData() const
{
	return GetW();
}

String TextCtrl::GetEncodedLine(int i, byte charset) const
{
	charset = ResolveCharset(charset);
	if(charset == CHARSET_UTF8)
		return line[i].text;
	return FromUnicode(FromUtf8(line[i].text), charset);
}

int   TextCtrl::GetLinePos(int& pos) const {
	if(pos < cpos && cpos - pos < pos) {
		int i = cline;
		int ps = cpos;
		for(;;) {
			ps -= line[--i].GetLength() + 1;
			if(ps <= pos) {
				pos = pos - ps;
				return i;
			}
		}
	}
	else {
		int i = 0;
		if(pos >= cpos) {
			pos -= cpos;
			i = cline;
		}
		for(;;) {
			int n = line[i].GetLength() + 1;
			if(pos < n) return i;
			pos -= n;
			i++;
			if(i >= line.GetCount()) {
				pos = line.Top().GetLength();
				return line.GetCount() - 1;
			}
		}
	}
}

int   TextCtrl::GetPos(int ln, int lpos) const {
	ln = minmax(ln, 0, line.GetCount() - 1);
	int i, pos;
	if(ln < cline && cline - ln < ln) {
		pos = cpos;
		i = cline;
		while(i > ln)
			pos -= line[--i].GetLength() + 1;
	}
	else {
		if(ln >= cline) {
			pos = cpos;
			i = cline;
		}
		else {
			pos = 0;
			i = 0;
		}
		while(i < ln)
			pos += line[i++].GetLength() + 1;
	}
	return pos + min(line[ln].GetLength(), lpos);
}

WString TextCtrl::GetW(int pos, int size) const
{
	int i = GetLinePos(pos);
	WString r;
	for(;;) {
		if(i >= line.GetCount()) break;
		WString ln = line[i++];
		int sz = min(ln.GetLength() - pos, size);
		r.Cat(ln.Mid(pos, sz));
		size -= sz;
		if(size == 0) break;
#ifdef PLATFORM_WIN32
		r.Cat('\r');
#endif
		r.Cat('\n');
		size--;
		if(size == 0) break;
		pos = 0;
	}
	return r;
}

String TextCtrl::Get(int pos, int size, byte charset) const
{
	return FromUnicode(GetW(pos, size), charset);
}

int  TextCtrl::GetChar(int pos) const {
	int i = GetLinePos(pos);
	WString ln = line[i];
	int c = ln.GetLength() == pos ? '\n' : ln[pos];
	return c;
}

int TextCtrl::Insert0(int pos, const WString& txt) {
	int inspos = pos;
	PreInsert(inspos, txt);
	if(pos < cpos)
		cpos = cline = 0;
	int i = GetLinePos(pos);
	DirtyFrom(i);
	int size = 0;
	WString ln;
	Vector<Ln> iln;
	for(const wchar *s = txt; s < txt.End(); s++)
		if(*s >= ' ' || *s == '\t') {
			ln.Cat(*s);
			size++;
		}
		else
		if(*s == '\n') {
			iln.Add(ln);
			size++;
			ln.Clear();
		}

	WString l = line[i];
	if(iln.GetCount()) {
		iln[0] = l.Mid(0, pos) + WString(iln[0]);
		ln.Cat(l.Mid(pos));
		line[i] = ln;
		InvalidateLine(i);
		line.Insert(i, iln);
		InsertLines(i, iln.GetCount());
		Refresh();
	}
	else {
		line[i] = l.Mid(0, pos) + ln + l.Mid(pos);
		InvalidateLine(i);
		RefreshLine(i);
	}
	total += size;
	SetSb();
	Update();
	PostInsert(inspos, txt);
	return size;
}

void TextCtrl::Remove0(int pos, int size) {
	int rmpos = pos, rmsize = size;
	PreRemove(rmpos, rmsize);
	total -= size;
	if(pos < cpos)
		cpos = cline = 0;
	int i = GetLinePos(pos);
	DirtyFrom(i);
	WString ln = line[i];
	int sz = min(ln.GetLength() - pos, size);
	ln.Remove(pos, sz);
	size -= sz;
	line[i] = ln;
	if(size == 0) {
		InvalidateLine(i);
		RefreshLine(i);
	}
	else {
		size--;
		int j = i + 1;
		for(;;) {
			int sz = line[j].GetLength() + 1;
			if(sz > size) break;
			j++;
			size -= sz;
		}
		WString p1 = line[i];
		WString p2 = line[j];
		p1.Insert(p1.GetLength(), p2.Mid(size, p2.GetLength() - size));
		line[i] = p1;
		line.Remove(i + 1, j - i);
		RemoveLines(i + 1, j - i);
		InvalidateLine(i);
		Refresh();
	}
	Update();
	PostRemove(rmpos, rmsize);
	SetSb();
}

void TextCtrl::Undodo()
{
	while(undo.GetCount() > undosteps)
		undo.DropHead();
	redo.Clear();
}

void TextCtrl::NextUndo()
{
	undoserial += incundoserial;
	incundoserial = false;
}

void TextCtrl::IncDirty() {
	dirty++;
	if(dirty == 0 || dirty == 1)
	{
		if(dirty)
			SetModify();
		else
			ClearModify();
		WhenState();
	}
}

void TextCtrl::DecDirty() {
	dirty--;
	if(dirty == 0 || dirty == -1)
	{
		if(dirty)
			SetModify();
		else
			ClearModify();
		WhenState();
	}
}

int TextCtrl::InsertU(int pos, const WString& txt, bool typing) {
	int sz = Insert0(pos, txt);
	if(undosteps) {
		if(undo.GetCount() > 1 && typing && *txt != '\n' && IsDirty()) {
			UndoRec& u = undo.Tail();
			if(u.typing && u.pos + u.size == pos) {
				u.size += txt.GetLength();
				return sz;
			}
		}
		UndoRec& u = undo.AddTail();
		incundoserial = true;
		IncDirty();
		u.serial = undoserial;
		u.pos = pos;
		u.size = sz;
		u.typing = typing;
	}
	return sz;
}

void TextCtrl::RemoveU(int pos, int size) {
	if(size + pos > total)
		size = total - pos;
	if(size <= 0) return;
	if(undosteps) {
		UndoRec& u = undo.AddTail();
		incundoserial = true;
		IncDirty();
		u.serial = undoserial;
		u.pos = pos;
		u.size = 0;
		u.text = Get(pos, size, CHARSET_UTF8);
		u.typing = false;
	}
	Remove0(pos, size);
}

int TextCtrl::Insert(int pos, const WString& _txt, bool typing) {
	WString txt = _txt;
	if(charset != CHARSET_UNICODE)
		for(int i = 0; i < txt.GetCount(); i++)
			if(FromUnicode(txt[i], charset) == DEFAULTCHAR)
				txt.Set(i, '?');
	int sz = InsertU(pos, txt, typing);
	Undodo();
	return sz;
}

int TextCtrl::Insert(int pos, const String& txt, byte charset)
{
	return Insert(pos, ToUnicode(txt, charset), false);
}

void TextCtrl::Remove(int pos, int size) {
	RemoveU(pos, size);
	Undodo();
}

void TextCtrl::Undo() {
	if(undo.IsEmpty()) return;
	undo_op = true;
	int nc = 0;
	int s = undo.Tail().serial;
	while(undo.GetCount()) {
		const UndoRec& u = undo.Tail();
		if(u.serial != s)
			break;
		UndoRec& r = redo.AddTail();
		r.serial = s;
		r.typing = false;
		nc = r.pos = u.pos;
		CachePos(r.pos);
		if(u.size) {
			r.size = 0;
			r.text = Get(u.pos, u.size, CHARSET_UTF8);
			Remove0(u.pos, u.size);
		}
		else {
			WString text = FromUtf8(u.text);
			r.size = Insert0(u.pos, text);
			nc += r.size;
		}
		undo.DropTail();
		DecDirty();
	}
	ClearSelection();
	PlaceCaret(nc, false);
	Action();
	undo_op = false;
}

void TextCtrl::Redo() {
	if(!redo.GetCount()) return;
	NextUndo();
	int s = redo.Tail().serial;
	int nc = 0;
	while(redo.GetCount()) {
		const UndoRec& r = redo.Tail();
		if(r.serial != s)
			break;
		nc = r.pos + r.size;
		CachePos(r.pos);
		if(r.size)
			RemoveU(r.pos, r.size);
		else
			nc += InsertU(r.pos, FromUtf8(r.text));
		redo.DropTail();
		IncDirty();
	}
	ClearSelection();
	PlaceCaret(nc, false);
	Action();
}

void  TextCtrl::ClearSelection() {
	anchor = -1;
	Refresh();
	WhenSel();
}

void   TextCtrl::SetSelection(int l, int h) {
	if(l != h) {
		PlaceCaret(minmax(l, 0, total), false);
		PlaceCaret(minmax(h, 0, total), true);
	}
	else
		SetCursor(l);
}

bool   TextCtrl::GetSelection(int& l, int& h) const {
	if(anchor < 0) {
		l = h = cursor;
		return false;
	}
	else {
		l = min(anchor, cursor);
		h = max(anchor, cursor);
		return true;
	}
}

String TextCtrl::GetSelection(byte charset) const {
	int l, h;
	if(GetSelection(l, h))
		return Get(l, h - l, charset);
	return String();
}

WString TextCtrl::GetSelectionW() const {
	int l, h;
	if(GetSelection(l, h))
		return GetW(l, h - l);
	return WString();
}

bool   TextCtrl::RemoveSelection() {
	int l, h;
	if(anchor < 0) return false;
	GetSelection(l, h);
	Remove(l, h - l);
	anchor = -1;
	Refresh();
	PlaceCaret(l);
	Action();
	return true;
}

void TextCtrl::RefreshLines(int l1, int l2) {
	int h = max(l1, l2);
	for(int i = min(l1, l2); i <= h; i++)
		RefreshLine(i);
}

void TextCtrl::Cut() {
	if(!IsReadOnly() && IsSelection()) {
		Copy();
		RemoveSelection();
	}
}

void TextCtrl::Copy() {
	int l, h;
	if(!GetSelection(l, h)) {
		int i = GetLine(cursor);
		l = GetPos(i);
		h = l + line[i].GetLength() + 1;
	}
	WString txt = GetW(l, h - l);
	ClearClipboard();
	AppendClipboardUnicodeText(txt);
	AppendClipboardText(txt.ToString());
}

void TextCtrl::SelectAll() {
	SetSelection();
}

int  TextCtrl::Paste(const WString& text) {
	if(IsReadOnly()) return 0;
	RemoveSelection();
	int n = Insert(cursor, text);
	PlaceCaret(cursor + n);
	Refresh();
	return n;
}

void TextCtrl::Paste() {
	WString w = ReadClipboardUnicodeText();
	if(w.IsEmpty())
		w = ReadClipboardText().ToWString();
	Paste(w);
	Action();
}

void TextCtrl::StdBar(Bar& menu) {
	NextUndo();
	if(undosteps) {
		menu.Add(undo.GetCount(), t_("Undo"), THISBACK(Undo))
			.Key(K_ALT_BACKSPACE)
			.Key(K_CTRL_Z);
		menu.Add(redo.GetCount(), t_("Redo"), THISBACK(Redo))
			.Key(K_SHIFT|K_ALT_BACKSPACE)
			.Key(K_SHIFT_CTRL_Z);
		menu.Separator();
	}
	menu.Add(IsEditable() && IsSelection(),
			t_("Cut"), CtrlImg::cut(), THISBACK(Cut))
		.Key(K_SHIFT_DELETE)
		.Key(K_CTRL_X);
	menu.Add(IsSelection(),
			t_("Copy"), CtrlImg::copy(), THISBACK(Copy))
		.Key(K_CTRL_INSERT)
		.Key(K_CTRL_C);
	menu.Add(IsEditable()
		#ifdef PLATFORM_WIN32
			&& ::IsClipboardFormatAvailable(CF_TEXT)
		#endif
			,
			t_("Paste"), CtrlImg::paste(), THISBACK(DoPaste))
		.Key(K_SHIFT_INSERT)
		.Key(K_CTRL_V);
	menu.Add(IsEditable() && IsSelection(),
			t_("Erase"), CtrlImg::remove(), THISBACK(DoRemoveSelection))
		.Key(K_DELETE);
	menu.Separator();
	menu.Add(GetLength(),
			t_("Select all"), THISBACK(SelectAll))
		.Key(K_CTRL_A);
}

String TextCtrl::GetSelectionData(const String& fmt) const
{
	return GetTextClip(GetSelectionW(), fmt);
}

END_UPP_NAMESPACE
