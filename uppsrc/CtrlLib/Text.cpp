#include <CtrlLib/CtrlLib.h>

#define LTIMING(x)  // RTIMING(x)

namespace Upp {

TextCtrl::TextCtrl()
{
	Unicode();
	undosteps = 1000;
	Clear();
	undoserial = 0;
	incundoserial = false;
	undo_op = false;
	WhenBar = THISBACK(StdBar);
	charset = CHARSET_UTF8;
	color[INK_NORMAL] = SColorText;
	color[INK_DISABLED] = SColorDisabled;
	color[INK_SELECTED] = SColorHighlightText;
	color[PAPER_NORMAL] = SColorPaper;
	color[PAPER_READONLY] = SColorFace;
	color[PAPER_SELECTED] = SColorHighlight;
	color[WHITESPACE] = Blend(SColorLight, SColorHighlight);
	color[WARN_WHITESPACE] = Blend(SColorLight, SRed);
	
	processtab = true;
	processenter = true;
	nobg = false;
	rectsel = false;
#ifdef CPU_64
	max_total = 2047 * 1024 * 1024;
#else
#ifdef _DEBUG
	max_total = 100 * 1024 * 1024;
#else
	max_total = 200 * 1024 * 1024;
#endif
#endif
	max_line_len = 100000;
	truncated = false;
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
	column_typing = false;
	dropcaret = Null;
	isdrag = false;
}

void TextCtrl::Clear()
{
	GuiLock __;
	view = NULL;
	viewlines = 0;
	cline = 0;
	cpos = 0;
	total = 0;
	truncated = false;
	lin.Clear();
	ClearLines();
	lin.Add();
	InsertLines(0, 1);
	DirtyFrom(0);
	undo.Clear();
	redo.Clear();
	ClearDirty();
	anchor = -1;
	cursor = 0;
	SetSb();
	PlaceCaret(0);
	SelectionChanged();
	Refresh();
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
void TextCtrl::PlaceCaret(int64 newcursor, bool sel) {}

int TextCtrl::RemoveRectSelection() { return 0; }
WString TextCtrl::CopyRectSelection() { return Null; }
int TextCtrl::PasteRectSelection(const WString& s) { return 0; }

void   TextCtrl::CachePos(int64 pos)
{
	GuiLock __;
	int64 p = pos;
	cline = GetLinePos64(p);
	cpos = pos - p;
}

void   TextCtrl::CacheLinePos(int linei)
{
	GuiLock __;
	if(linei >= 0 && linei < GetLineCount()) {
		cpos = GetPos64(linei);
		cline = linei;
	}
}

bool   TextCtrl::IsUnicodeCharset(byte charset)
{
	return findarg(charset, CHARSET_UTF8, CHARSET_UTF8_BOM, CHARSET_UTF16_LE, CHARSET_UTF16_BE,
	                        CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM) >= 0;
}

int   TextCtrl::Load0(Stream& in, byte charset_, bool view) {
	GuiLock __;
	Clear();
	lin.Clear();
	ClearLines();
	total = 0;
	SetCharset(charset_);
	truncated = false;
	viewlines = 0;
	this->view = NULL;
	view_all = false;
	offset256.Clear();
	total256.Clear();
	view_cache[0].blk = view_cache[1].blk = -1;
	if(view) {
		this->view = &in;
		SetReadOnly();
	}
	if(charset == CHARSET_UTF8_BOM && in.GetLeft() >= 3) {
		int64 pos = in.GetPos();
		byte h[3];
		if(!(in.Get(h, 3) == 3 && h[0] == 0xEF && h[1] == 0xBB && h[2] == 0xBF))
			in.Seek(pos);
		charset = CHARSET_UTF8;
	}
	int be16 = findarg(charset, CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM);
	if(be16 >= 0 && in.GetLeft() >= 2) {
		int64 pos = in.GetPos();
		dword h = in.Get16le();
		if(h != (be16 ? 0xfffe : 0xfeff))
			in.Seek(pos);
		charset = be16 ? CHARSET_UTF16_BE : CHARSET_UTF16_LE;
	}
	
	if(view) {
		view_loading_pos = in.GetPos();
		view_loading_lock = 0;
		ViewLoading();
		PlaceCaret(0);
		return 0;
	}

	int m = LoadLines(lin, INT_MAX, total, in, charset, max_line_len, max_total, truncated);

	InsertLines(0, lin.GetCount());
	Update();
	SetSb();
	PlaceCaret(0);
	return m;
}

int TextCtrl::LoadLines(Vector<Ln>& ls, int n, int64& total, Stream& in, byte charset,
                        int max_line_len, int max_total, bool& truncated,
                        int *view_line_count) const
{
	StringBuffer ln;
	bool cr = false;
	byte b8 = 0;
	auto line_count = [&] { return view_line_count ? *view_line_count : ls.GetCount(); };
	if(charset == CHARSET_UTF16_LE || charset == CHARSET_UTF16_BE) {
		WStringBuffer wln;
		auto put_wln = [&]() {
			if(view_line_count)
				(*view_line_count)++;
			else {
				Ln& ln = ls.Add();
				ln.len = wln.GetCount();
				ln.text = ToUtf8(~wln, ln.len);
			}
		};
		for(;;) {
			int c = charset == CHARSET_UTF16_LE ? in.Get16le() : in.Get16be();
			if(c < 0) {
				total += wln.GetCount();
				put_wln();
				goto finish;
			}
			if(c == '\r')
				cr = true;
			else
			if(c == '\n') {
			truncate_line:
				total += wln.GetCount() + 1;
				put_wln();
				if(line_count() >= n)
					goto finish;
				wln.Clear();
			}
			else {
				wln.Cat(c);
				if(wln.GetCount() >= max_line_len)
					goto truncate_line;
			}
		}
	}
	else {
		for(;;) {
			byte h[200];
			int size;
			int64 pos = in.GetPos();
			const byte *s = in.GetSzPtr(size);
			if(size == 0)  {
				size = in.Get(h, 200);
				s = h;
				if(size == 0)
					break;
			}
			const byte *posptr = s;
			const byte *e = s + size;
			while(s < e) {
				const byte *b = s;
				const byte *ee = s + min(size_t(e - s), size_t(max_line_len - ln.GetCount()));
				{
					while(s < ee && *s != '\r' && *s != '\n') {
						b8 |= *s++;
						while(s < ee && *s >= ' ' && *s < 128) // Interestingly, this speeds things up
							s++;
						while(s < ee && *s >= ' ')
							b8 |= *s++;
					}
				}
				if(b < s) {
					if(s - b + ln.GetCount() > max_total)
						ln.Cat((const char *)b, max_total - ln.GetCount());
					else
						ln.Cat((const char *)b, (const char *)s);
				}
				auto put_ln = [&]() -> bool {
					if(view_line_count) {
						(*view_line_count)++;
						total += (charset == CHARSET_UTF8 && (b8 & 0x80) ? Utf32Len(~ln, ln.GetCount())
						                                                 : ln.GetCount()) + 1;
					}
					else {
						Ln& l = ls.Add();
						if(charset == CHARSET_UTF8) {
							l.len = (b8 & 0x80) ? Utf32Len(~ln, ln.GetCount()) : ln.GetCount();
							l.text = ln;
						}
						else {
							l.len = ln.GetCount();
							l.text = ToCharset(CHARSET_UTF8, ln, charset);
						}
						if(total + l.len + 1 > max_total) {
							ls.Drop();
							truncated = true;
							return false;
						}
						total += l.len + 1;
					}
					return true;
				};
				while(ln.GetCount() >= max_line_len) {
					int ei = max_line_len;
					if(charset == CHARSET_UTF8)
						while(ei > 0 && ei > max_line_len - 6 && !((byte)ln[ei] < 128 || IsUtf8Lead((byte)ln[ei]))) // break lse at whole utf8 codepoint if possible
							ei--;
					String nln(~ln + ei, ln.GetCount() - ei);
					ln.SetCount(ei);
					truncated = true;
					if(!put_ln())
						goto out_of_limit;
					if(line_count() >= n) {
						in.Seek(s - posptr + pos);
						goto finish;
					}
					ln = nln;
				}
				if(s < e && *s == '\r') {
					s++;
					cr = true;
				}
				if(s < e && *s == '\n') {
					if(!put_ln())
						goto out_of_limit;
					s++;
					if(line_count() >= n) {
						in.Seek(s - posptr + pos);
						goto finish;
					}
					ln.Clear();
					b8 = 0;
				}
			}
		}
	}

out_of_limit:
	{
		WString w = ToUnicode(~ln, ln.GetCount(), charset);
		if(total + w.GetLength() <= max_total) {
			if(view_line_count) {
				(*view_line_count)++;
				total += w.GetCount();
			}
			else {
				Ln& ln = ls.Add();
				ln.len = w.GetCount();
				ln.text = ToUtf8(~w, ln.len);
				total += ln.len;
			}
		}
	}
finish:
	return ls.GetCount() > 1 ? cr ? LE_CRLF : LE_LF : LE_DEFAULT;
}

void TextCtrl::ViewLoading()
{
	GuiLock __;
	if(view_all || !view)
		return;
	int start = msecs();
	view->Seek(view_loading_pos);
	int lines0 = viewlines;
	for(;;) {
		offset256.Add(view->GetPos());
		Vector<Ln> l;
		bool b;
		int64 t = 0;

		int line_count = 0;
		LoadLines(l, 256, t, *view, charset, 10000, INT_MAX, b, &line_count);
		viewlines += line_count;
		total += t;
		total256.Add((int)t);
		
	#ifdef CPU_32
		enum { MAX_LINES = 128000000 };
	#else
		enum { MAX_LINES = INT_MAX - 512 };
	#endif

		if(view->IsEof() || viewlines > MAX_LINES) {
			WhenViewMapping(view->GetPos());
			view_all = true;
			break;
		}
		
		if(view_loading_lock) {
			view_loading_pos = view->GetPos();
			WhenViewMapping(view_loading_pos);
			break;
		}
		
		if(msecs(start) > 20) {
			view_loading_pos = view->GetPos();
			PostCallback([=] { ViewLoading(); });
			WhenViewMapping(view_loading_pos);
			break;
		}
	}
	InsertLines(lines0, viewlines - lines0);
	SetSb();
	Update();
}

void TextCtrl::UnlockViewMapping()
{
	view_loading_lock--;
	ViewLoading();
}

void TextCtrl::WaitView(int line, bool progress)
{
	if(view) {
		if(progress) {
			LockViewMapping();
			Progress pi("Scanning the file");
			pi.Delay(1000);
			while(view && !view_all && viewlines < line) {
				if(pi.SetCanceled(int(view_loading_pos >> 10), int(view->GetSize()) >> 10))
					break;
				ViewLoading();
			}
			UnlockViewMapping();
		}
		else
			while(view && !view_all && viewlines <= line)
				ViewLoading();
	}
}

void TextCtrl::SerializeViewMap(Stream& s)
{
	GuiLock __;
	int version = 0;
	s / version;
	s.Magic(327845692);
	s % view_loading_pos
	  % total
	  % viewlines
	  % view_all
	  % total256
	  % offset256
	;
	if(s.IsLoading()) {
		SetSb();
		Update();
		Refresh();
	}
}

const TextCtrl::Ln& TextCtrl::GetLn(int i) const
{
	if(view) {
		GuiLock __;
		int blk = i >> 8;
		if(view_cache[0].blk != blk)
			Swap(view_cache[0], view_cache[1]); // trivial LRU
		if(view_cache[0].blk != blk) {
			Swap(view_cache[0], view_cache[1]); // trivial LRU
			view->Seek(offset256[blk]);
			int64 t = 0;
			bool b;
			view_cache[0].line.Clear();
			view_cache[0].blk = blk;
			LoadLines(view_cache[0].line, 256, t, *view, charset, 10000, INT_MAX, b);
		}
		return view_cache[0].line[i & 255];
	}
	else
		return lin[i];
}

const String& TextCtrl::GetUtf8Line(int i) const
{
	return GetLn(i).text;
}

int TextCtrl::GetLineLength(int i) const
{
	return GetLn(i).len;
}

void   TextCtrl::Save(Stream& s, byte charset, int line_endings) const {
	if(charset == CHARSET_UTF8_BOM) {
		static byte bom[] = { 0xEF, 0xBB, 0xBF };
		s.Put(bom, 3);
		charset = CHARSET_UTF8;
	}
	if(charset == CHARSET_UTF16_LE_BOM) {
		s.Put16le(0xfeff);
		charset = CHARSET_UTF16_LE;
	}
	if(charset == CHARSET_UTF16_BE_BOM) {
		s.Put16be(0xfeff);
		charset = CHARSET_UTF16_BE;
	}
	charset = ResolveCharset(charset);
	String le = "\n";
#ifdef PLATFORM_WIN32
	if(line_endings == LE_DEFAULT)
		le = "\r\n";
#endif
	if(line_endings == LE_CRLF)
		le = "\r\n";
	int be16 = findarg(charset, CHARSET_UTF16_LE, CHARSET_UTF16_BE);
	if(be16 >= 0) {
		String wle;
		for(int i = 0; i < le.GetCount(); i++) {
			if(be16)
				wle.Cat(0);
			wle.Cat(le[i]);
			if(!be16)
				wle.Cat(0);
		}
		for(int i = 0; i < GetLineCount(); i++) {
			if(i)
				s.Put(wle);
			WString txt = GetWLine(i);
			const wchar *e = txt.End();
			if(be16)
				for(const wchar *w = txt; w != e; w++)
					if(*w < 0x10000)
						s.Put16be((word)*w);
					else {
						char16 h[2];
						ToUtf16(h, w, 1);
						s.Put16be(h[0]);
						s.Put16be(h[1]);
					}
			else
				for(const wchar *w = txt; w != e; w++)
					if(*w < 0x10000)
						s.Put16le((word)*w);
					else {
						char16 h[2];
						ToUtf16(h, w, 1);
						s.Put16le(h[0]);
						s.Put16le(h[1]);
					}
		}
		return;
	}
	for(int i = 0; i < GetLineCount(); i++) {
		if(i)
			s.Put(le);
		if(charset == CHARSET_UTF8)
			s.Put(GetUtf8Line(i));
		else {
			String txt = FromUnicode(GetWLine(i), charset);
			const char *e = txt.End();
			for(const char *w = txt; w != e; w++)
				s.Put(*w == DEFAULTCHAR ? '?' : *w);
		}
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
	if(!IsUnicodeCharset(charset))
		for(int i = 0; i < GetLineCount(); i++) {
			WString txt = GetWLine(i);
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
	data.undo = pick(undo);
	data.redo = pick(redo);
	data.undoserial = undoserial;
	return data;
}

void TextCtrl::SetPickUndoData(TextCtrl::UndoData&& data)
{
	undo = pick(data.undo);
	redo = pick(data.redo);
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
	String h = GetUtf8Line(i);
	return charset == CHARSET_UTF8 ? h : FromUnicode(ToUtf32(h), charset);
}

int   TextCtrl::GetLinePos64(int64& pos) const {
	GuiLock __;
	if(pos < cpos && cpos - pos < pos && !view) {
		int i = cline;
		int64 ps = cpos;
		for(;;) {
			ps -= GetLineLength(--i) + 1;
			if(ps <= pos) {
				pos = pos - ps;
				return i;
			}
		}
	}
	else {
		int i = 0;
		if(view) {
			GuiLock __;
			int blk = 0;
			for(;;) {
				int n = total256[blk];
				if(pos < n)
					break;
				pos -= n;
				if(++blk >= total256.GetCount()) {
					pos = GetLineLength(GetLineCount() - 1);
					return GetLineCount() - 1;
				}
			}
			i = blk << 8;
		}
		else
		if(pos >= cpos) {
			pos -= cpos;
			i = cline;
		}
		for(;;) {
			int n = GetLineLength(i) + 1;
			if(pos < n) return i;
			pos -= n;
			i++;
			if(i >= GetLineCount()) {
				pos = GetLineLength(GetLineCount() - 1);
				return GetLineCount() - 1;
			}
		}
	}
	return 0; // just silencing GCC warning, cannot get here
}

int64  TextCtrl::GetPos64(int ln, int lpos) const {
	GuiLock __;
	ln = minmax(ln, 0, GetLineCount() - 1);
	int i;
	int64 pos;
	if(ln < cline && cline - ln < ln && !view) {
		pos = cpos;
		i = cline;
		while(i > ln)
			pos -= GetLineLength(--i) + 1;
	}
	else {
		pos = 0;
		i = 0;
		if(view) {
			for(int j = 0; j < ln >> 8; j++) {
				pos += total256[j];
				i += 256;
			}
		}
		else
		if(ln >= cline) {
			pos = cpos;
			i = cline;
		}
		while(i < ln)
			pos += GetLineLength(i++) + 1;
	}
	return pos + min(GetLineLength(ln), lpos);
}

WString TextCtrl::GetW(int64 pos, int size) const
{
	int i = GetLinePos64(pos);
	WStringBuffer r;
	for(;;) {
		if(i >= GetLineCount()) break;
		WString ln = GetWLine(i++);
		int sz = min(LimitSize(ln.GetLength() - pos), size);
		if(pos == 0 && sz == ln.GetLength())
			r.Cat(ln);
		else
			r.Cat(ln.Mid((int)pos, sz));
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
	return WString(r);
}

String TextCtrl::Get(int64 pos, int size, byte charset) const
{
	if(charset == CHARSET_UTF8) {
		int i = GetLinePos64(pos);
		StringBuffer r;
		for(;;) {
			if(i >= GetLineCount()) break;
			int sz = min(LimitSize(GetLineLength(i) - pos), size);
			const String& h = GetUtf8Line(i);
			const char *s = h;
			int n = h.GetCount();
			i++;
			if(pos == 0 && sz == n)
				r.Cat(s, n);
			else
				r.Cat(ToUtf32(s, n).Mid((int)pos, sz).ToString());
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
		return String(r);
	}
	return FromUnicode(GetW(pos, size), charset);
}

int  TextCtrl::GetChar(int64 pos) const {
	if(pos < 0 || pos >= GetLength64())
		return 0;
	int i = GetLinePos64(pos);
	WString ln = GetWLine(i);
	int c = ln.GetLength() == pos ? '\n' : ln[(int)pos];
	return c;
}

int TextCtrl::GetLinePos32(int& pos) const
{
	int64 p = pos;
	int l = GetLinePos64(p);
	pos = (int)p;
	return l;
}

bool TextCtrl::GetSelection32(int& l, int& h) const
{
	int64 ll, hh;
	bool b = GetSelection(ll, hh);
	if(hh >= INT_MAX) {
		l = h = (int)cursor;
		return false;
	}
	l = (int)ll;
	h = (int)hh;
	return b;
}

int TextCtrl::GetCursor32() const
{
	int64 h = GetCursor64();
	return h < INT_MAX ? (int)h : 0;
}

int TextCtrl::GetLength32() const
{
	int64 h = GetLength64();
	return h < INT_MAX ? (int)h : 0;
}

int TextCtrl::Insert0(int pos, const WString& txt) { // TODO: Do this with utf8
	GuiLock __;
	int inspos = pos;
	PreInsert(inspos, txt);
	if(pos < cpos)
		cpos = cline = 0;
	int i = GetLinePos32(pos);
	DirtyFrom(i);
	int size = 0;

	WStringBuffer lnb;
	Vector<WString> iln;
	const wchar *s = txt;
	while(s < txt.End())
		if(*s >= ' ') {
			const wchar *b = s;
			while(*s >= ' ') // txt is zero teminated...
				s++;
			int sz = int(s - b);
			lnb.Cat(b, sz);
			size += sz;
		}
		else
		if(*s == '\t') {
			lnb.Cat(*s);
			size++;
			s++;
		}
		else
		if(*s == '\n') {
			iln.Add(lnb);
			size++;
			lnb.Clear();
			s++;
		}
		else
			s++;
	WString ln = lnb;
	WString l = GetWLine(i);
	if(iln.GetCount()) {
		iln[0] = l.Mid(0, pos) + iln[0];
		ln.Cat(l.Mid(pos));
		SetLine(i, ln);
		InvalidateLine(i);
		LineInsert(i, iln.GetCount());
		for(int j = 0; j < iln.GetCount(); j++)
			SetLine(i + j, iln[j]);
		InsertLines(i, iln.GetCount());
		Refresh();
	}
	else {
		SetLine(i, l.Mid(0, pos) + ln + l.Mid(pos));
		InvalidateLine(i);
		RefreshLine(i);
	}
	total += size;
	SetSb();
	Update();
	ClearSelection();
	PostInsert(inspos, txt);
	return size;
}

void TextCtrl::Remove0(int pos, int size) {
	GuiLock __;
	int rmpos = pos, rmsize = size;
	PreRemove(rmpos, rmsize);
	total -= size;
	if(pos < cpos)
		cpos = cline = 0;
	int i = GetLinePos32(pos);
	DirtyFrom(i);
	WString ln = GetWLine(i);
	int sz = min(LimitSize(ln.GetLength() - pos), size);
	ln.Remove(pos, sz);
	size -= sz;
	SetLine(i, ln);
	if(size == 0) {
		InvalidateLine(i);
		RefreshLine(i);
	}
	else {
		size--;
		int j = i + 1;
		for(;;) {
			int sz = GetLineLength(j) + 1;
			if(sz > size) break;
			j++;
			size -= sz;
		}
		WString p1 = GetWLine(i);
		WString p2 = GetWLine(j);
		p1.Insert(p1.GetLength(), p2.Mid(size, p2.GetLength() - size));
		SetLine(i, p1);
		LineRemove(i + 1, j - i);
		RemoveLines(i + 1, j - i);
		InvalidateLine(i);
		Refresh();
	}
	Update();
	ClearSelection();
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
	if(column_typing)
		column_typing = false;
	else
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
		if(undo.GetCount() && typing && *txt != '\n' && IsDirty()) {
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
		size = int(total - pos);
	if(size <= 0) return;
	if(undosteps) {
		UndoRec& u = undo.AddTail();
		incundoserial = true;
		IncDirty();
		u.serial = undoserial;
		u.pos = pos;
		u.size = 0;
		u.SetText(Get(pos, size, CHARSET_UTF8));
		u.typing = false;
	}
	Remove0(pos, size);
}

int TextCtrl::Insert(int pos, const WString& _txt, bool typing) {
	if(pos + _txt.GetCount() > max_total)
		return 0;
	WString txt = _txt;
	if(!IsUnicodeCharset(charset))
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
			r.SetText(Get(u.pos, u.size, CHARSET_UTF8));
			Remove0(u.pos, u.size);
		}
		else {
			WString text = ToUtf32(u.GetText());
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
			nc += InsertU(r.pos, ToUtf32(r.GetText()));
		redo.DropTail();
		IncDirty();
	}
	ClearSelection();
	PlaceCaret(nc, false);
	Action();
}

void  TextCtrl::ClearSelection() {
	if(anchor >= 0) {
		anchor = -1;
		Refresh();
		SelectionChanged();
		WhenSel();
	}
}

void   TextCtrl::SetSelection(int64 l, int64 h) {
	if(l != h) {
		PlaceCaret(minmax(l, (int64)0, total), false);
		PlaceCaret(minmax(h, (int64)0, total), true);
	}
	else
		SetCursor(l);
}

bool   TextCtrl::GetSelection(int64& l, int64& h) const {
	if(anchor < 0 || anchor == cursor) {
		l = h = cursor;
		return false;
	}
	else {
		l = min(anchor, cursor);
		h = max(anchor, cursor);
		return !rectsel;
	}
}

String TextCtrl::GetSelection(byte charset) const {
	int64 l, h;
	if(GetSelection(l, h))
		return Get(l, LimitSize(h - l), charset);
	return String();
}

WString TextCtrl::GetSelectionW() const {
	int64 l, h;
	if(GetSelection(l, h))
		return GetW(l, LimitSize(h - l));
	return WString();
}

bool   TextCtrl::RemoveSelection() {
	int64 l, h;
	if(anchor < 0) return false;
	if(IsRectSelection())
		RemoveRectSelection();
	else {
		if(!GetSelection(l, h))
			return false;
		Remove((int)l, int(h - l));
		anchor = -1;
		Refresh();
		PlaceCaret(l);
	}
	Action();
	return true;
}

void TextCtrl::RefreshLines(int l1, int l2) {
	int h = max(l1, l2);
	for(int i = min(l1, l2); i <= h; i++)
		RefreshLine(i);
}

void TextCtrl::Cut() {
	if(!IsReadOnly() && IsAnySelection()) {
		Copy();
		RemoveSelection();
	}
}

void TextCtrl::Copy() {
	int64 l, h;
	if(!GetSelection(l, h) && !IsAnySelection()) {
		int i = GetLine(cursor);
		l = GetPos64(i);
		h = l + GetLineLength(i) + 1;
	}
	WString txt;
	if(IsRectSelection())
		txt = CopyRectSelection();
	else
		txt = GetW(l, LimitSize(h - l));
	ClearClipboard();
	AppendClipboardUnicodeText(txt);
	AppendClipboardText(txt.ToString());
}

void TextCtrl::SelectAll() {
	SetSelection();
}

int  TextCtrl::Paste(const WString& text) {
	if(IsReadOnly()) return 0;
	int n;
	if(IsRectSelection())
		n = PasteRectSelection(text);
	else {
		RemoveSelection();
		n = Insert((int)cursor, text);
		PlaceCaret(cursor + n);
	}
	Refresh();
	return n;
}

String TextCtrl::GetPasteText()
{
	return Null;
}

void TextCtrl::Paste() {
	WString w = ReadClipboardUnicodeText();
	if(w.IsEmpty())
		w = ReadClipboardText().ToWString();
	if(w.IsEmpty())
		w = GetPasteText().ToWString();
	Paste(w);
	Action();
}

void TextCtrl::StdBar(Bar& menu) {
	NextUndo();
	if(undosteps) {
		menu.Add(undo.GetCount() && IsEditable(), t_("Undo"), CtrlImg::undo(), THISBACK(Undo))
			.Key(K_ALT_BACKSPACE)
			.Key(K_CTRL_Z);
		menu.Add(redo.GetCount() && IsEditable(), t_("Redo"), CtrlImg::redo(), THISBACK(Redo))
			.Key(K_SHIFT|K_ALT_BACKSPACE)
			.Key(K_CTRL_Y)
			.Key(K_SHIFT_CTRL_Z);
		menu.Separator();
	}
	menu.Add(IsEditable() && IsAnySelection(),
			t_("Cut"), CtrlImg::cut(), THISBACK(Cut))
		.Key(K_SHIFT_DELETE)
		.Key(K_CTRL_X);
	menu.Add(IsAnySelection(),
			t_("Copy"), CtrlImg::copy(), THISBACK(Copy))
		.Key(K_CTRL_INSERT)
		.Key(K_CTRL_C);
	bool canpaste = IsEditable() && IsClipboardAvailableText();
	menu.Add(canpaste,
			t_("Paste"), CtrlImg::paste(), THISBACK(DoPaste))
		.Key(K_SHIFT_INSERT)
		.Key(K_CTRL_V);
	LineEdit *e = dynamic_cast<LineEdit *>(this);
	if(e) {
		menu.Add(canpaste,
				 t_("Paste in column"), CtrlImg::paste_vert(), callback(e, &LineEdit::DoPasteColumn))
			.Key(K_ALT_V|K_SHIFT);
		menu.Add(e->IsRectSelection(),
				 t_("Sort"), CtrlImg::sort(), callback(e, &LineEdit::Sort));
	}
	menu.Add(IsEditable() && IsAnySelection(),
			t_("Erase"), CtrlImg::remove(), THISBACK(DoRemoveSelection))
		.Key(K_DELETE);
	menu.Separator();
	menu.Add(GetLength64(),
			t_("Select all"), CtrlImg::select_all(), THISBACK(SelectAll))
		.Key(K_CTRL_A);
}

String TextCtrl::GetSelectionData(const String& fmt) const
{
	return GetTextClip(GetSelectionW(), fmt);
}

void TextCtrl::EditPos::Serialize(Stream& s) {
	int version = 1;
	s / version;
	if(version >= 1)
		s % sby % cursor;
	else {
		int c = (int)cursor;
		s % sby % c;
		cursor = c;
	}
}

}
