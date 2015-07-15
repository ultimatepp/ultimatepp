#include "ide.h"

bool Ide::FindLineError(const String& ln, FindLineErrorCache& cache, ErrorInfo& f)
{
	VectorMap<String, String> bm = GetMethodVars(method);
	bool is_java = (bm.Get("BUILDER", Null) == "JDK");
	const char *s = ln;
	while(*s == ' ' || *s == '\t')
		s++;
	for(; s < ln.End(); s++) {
		if(*s != '\"' && (byte)*s >= 32 && *s != '(' && (f.file.GetLength() < 3 || *s != ':'))
			f.file.Cat(*s);
		else {
			if(*s == '\"') {
				f.file = Null;
				s++;
				while(*s && *s != '\"')
					f.file.Cat(*s++);
				if(*s)
					s++;
			}
			String file = f.file;
			int e = file.GetLength();
			while(e > 0 && file[e - 1] == ' ')
				e--;
			file.Trim(e);
			file = TrimLeft(file);
			String upp = GetUppDir();
		#ifdef PLATFORM_WIN32
			if(file[0] == '\\' || file[0] == '/')
				file = String(upp[0], 1) + ':' + file;
		#endif
			if(!IsFullPath(file) && *file != '\\' && *file != '/') {
				if(cache.wspc_paths.IsEmpty()) {
					::Workspace  wspc;
					wspc.Scan(main);
					for(int i = 0; i < wspc.GetCount(); i++)
						cache.wspc_paths.Add(GetFileDirectory(PackagePath(wspc[i])));
				}
				for(int i = 0; i < cache.wspc_paths.GetCount(); i++) {
					String path = AppendFileName(cache.wspc_paths[i], file);
					int q = cache.ff.Find(path);
					if(q >= 0) {
						if(cache.ff[q]) {
							file = path;
							break;
						}
					}
					else {
						bool b = false;
						String ext = ToLower(GetFileExt(path));
						if(findarg(ext, ".obj", ".lib", ".o", ".so", ".a", ".", "") < 0) {
							FindFile ff;
							b = ff.Search(path) && ff.IsFile();
						}
						cache.ff.Add(path, b);
						if(b) {
							file = path;
							break;
						}
					}
				}
			}
			file = FollowCygwinSymlink(file);
			if(IsFullPath(file) && FileExists(file) && IsTextFile(file)) {
				f.file = file;
				while(*s && !IsDigit(*s)) {
					if(*s == '/' || IsAlpha(*s))
						return false;
					s++;
				}
				f.lineno = f.linepos = 0;
				CParser p(s);
				if(p.IsInt())
					f.lineno = p.ReadInt();
				if(p.Char(':') && p.IsInt()) 
					f.linepos = p.ReadInt();
				const char *ms = p.GetPtr();
				f.kind = 3;
				if(ln.Find("warning") >= 0)
					f.kind = 2;
				else {
					int q = ln.Find("error");
					if(q >= 0 && (q == 0 || !IsAlpha(ln[q - 1])) && (q + 5 >= ln.GetCount() || !IsAlpha(ln[q - 1])))
						f.kind = 1;
				}
				const char *hs = ms;
				while(!IsLetter(*hs) && *hs)
					hs++;
				f.message = *hs ? hs : ms;
				f.message = TrimLeft(f.message);
				Vector<String> conf = SplitFlags(mainconfigparam, true);
				String uppout = GetVar("OUTPUT");
				int upplen = uppout.GetLength();
				if(is_java && f.file.GetLength() > upplen
				&& !MemICmp(f.file, uppout, upplen) && f.file[upplen] == DIR_SEP) { // check for preprocessed file
					FileIn fi(f.file);
					if(fi.IsOpen()) {
						String fake_file = f.file;
						int fake_line = 1;
						int file_line = 1;
						while(!fi.IsEof())
						{
							String line = fi.GetLine();
							const char *p = line;
							if(p[0] == '/' && p[1] == '/' && p[2] == '#')
							{
								p += 3;
								if(p[0] == 'l' && p[1] == 'i' && p[2] == 'n' && p[3] == 'e')
									p += 4;
								while(*p == ' ' || *p == '\t')
									p++;
								if(IsDigit(*p))
								{
									fake_line = stou(p, &p);
									while(*p == ' ' || *p == '\t')
										p++;
									if(*p == '\"')
										p++;
									fake_file.Clear();
									while(*p && *p != '\"')
										if(*p == '/')
										{
											fake_file.Cat('/');
											if(p[1] == '/')
												p++;
											p++;
										}
										else
											fake_file.Cat(*p++);
								}
								file_line++;
								continue;
							}
							if(f.lineno <= file_line) {
								f.file = fake_file;
								f.lineno = fake_line;
								f.linepos = 0;
								break;
							}
							file_line++;
							fake_line++;
						}
					}
				}
				return f.lineno > 0;
			}
			else
				f.file.Cat(*s); // File is not complete, e.g.: C:\Program Files (x86)\Microsoft Visual Studio 10.0\Vc\Include\string.h(186)
		}
	}
	return false;
}

void Ide::FindError()
{
	FindLineError(console.GetLine(console.GetCursor()));
}

bool Ide::Next(int tab, ArrayCtrl& a, int d)
{
	if(btabs.GetCursor() == tab) {
		int c = a.GetCursor();
		for(;;) {
			c += d;
			if(c >= 0 && c < a.GetCount()) {
				Value v = a.Get(c, "INFO");
				if(v.Is<ErrorInfo>() && !IsNull(v.To<ErrorInfo>().file)) {
					a.SetCursor(c);
					return true;
				}
			}
			else {
				if(d > 0)
					a.GoBegin();
				else
					a.GoEnd();
				return true;
			}
		}
	}
	return false;
}

void Ide::FindNextError()
{
	if(Next(BERRORS, error, 1) || Next(BFINDINFILES, ffound, 1))
		return;
	int ln = console.GetLine(console.GetCursor());
	int l = ln;
	for(l = ln; l < console.GetLineCount(); l++)
		if(FindLineError(l)) return;
	for(l = 0; l < ln; l++)
		if(FindLineError(l)) return;
}

void Ide::FindPrevError() {
	if(Next(BERRORS, error, -1) || Next(BFINDINFILES, ffound, -1))
		return;
	int ln = console.GetLine(console.GetCursor());
	int l = ln;
	One<Host> host = CreateHost(false);
	for(l = ln - 2; l >= 0; l--)
		if(FindLineError(l)) return;
	for(l = console.GetLineCount() - 1; l > ln; l--)
		if(FindLineError(l)) return;
}

void Ide::ClearErrorEditor()
{
	if(!mark_lines)
		return;

	for(int i = 0; i < filedata.GetCount(); i++) {
		ClearErrorEditor(filedata.GetKey(i));
	}
	
	SetErrorFiles(Vector<String>());
}

void Ide::ClearErrorEditor(String file)
{
	linking = false;

	if(!mark_lines)
		return;
	if(file == editfile)
		editor.ClearErrors();
	else {
		FileData& fd = Filedata(file);
		ClearErrors(fd.lineinfo);
	}
}

void Ide::SetErrorEditor()
{
	if(error.GetCount()) {
		SetBottom(BERRORS);
//		if(!error.IsCursor())
//			error.GoBegin();
	}

	if(!mark_lines)
		return;

	bool refresh = false;
	String    hfile;
	EditorBar hbar;
	Vector<String> errorfiles;
	FindLineErrorCache cache;
	for(int i = 0; i < console.GetLineCount(); i++) {
		ErrorInfo f;
		if(FindLineError(console.GetUtf8Line(i), cache, f)) {
			String file = NormalizePath(f.file);
		#ifdef PLATFORM_WIN32
			errorfiles.Add(ToLower(file));
		#else
			errorfiles.Add(file);
		#endif
			if(editfile == file) {
				editor.SetError(f.lineno - 1, f.kind);
				refresh = true;
			}
			else {
				if(hfile != file) {
					if(hfile.GetCount())
						Filedata(hfile).lineinfo = hbar.GetLineInfo();
					hbar.SetLineInfo(Filedata(file).lineinfo, -1);
					hfile = file;
				}
				hbar.SetError(f.lineno - 1, f.kind);
			}
		}
	}
	if(hfile.GetCount())
		Filedata(hfile).lineinfo = hbar.GetLineInfo();
	if(refresh)
		editor.RefreshFrame();
	SetErrorFiles(errorfiles);
}

void Ide::GoToError(const ErrorInfo& f)
{
	if(IsNull(f.file))
		return;
	String file = NormalizePath(f.file);
	editastext.FindAdd(file);
	editashex.RemoveKey(file);
	EditFile(file);
	editor.SetCursor(editor.GetPos(editor.GetLineNo(f.lineno - 1), max(f.linepos - 1, 0)));
	editor.CenterCursor();
	editor.SetFocus();
	Sync();
}

void Ide::GoToError(ArrayCtrl& a)
{
	Value v = a.Get("INFO");
	if(v.Is<ErrorInfo>())
		GoToError(ValueTo<ErrorInfo>(v));
}

bool Ide::FindLineError(int l) {
	ErrorInfo f;
	FindLineErrorCache cache;
	if(FindLineError(console.GetUtf8Line(l), cache, f)) {
		GoToError(f);
		console.SetSelection(console.GetPos(l), console.GetPos(l + 1));
		if(btabs.GetCursor() != BCONSOLE && btabs.GetCursor() != BFINDINFILES)
			ShowConsole();
		return true;
	}
	return false;
}

void Ide::PutLinking()
{
	linking = true;
	linking_line.Clear();
}

void Ide::PutLinkingEnd(bool ok)
{
	if(!ok && linking) {
		addnotes = true;
		error_count++;
		error.Add(Null, Null, AttrText("Linking has failed").Bold()
			                  .NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color));
		for(int i = 0; i < linking_line.GetCount(); i++) {
			ErrorInfo f;
			if(!FindLineError(linking_line[i], error_cache, f)) {
				f.file = Null;
				f.lineno = Null;
				f.message = TrimLeft(linking_line[i]);
			}
			int linecy;
			error.Add(f.file, f.lineno,
			          AttrText(FormatErrorLine(f.message, linecy))
			          .NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color),
			          RawToValue(f));
			error.SetLineCy(error.GetCount() - 1, linecy);
		}
	}
	SyncErrorsMessage();
	linking = false;
}

bool IsDarkMismatch()
{
	return IsDark(SColorPaper()) != IsDark(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_NORMAL).color);
}

void Ide::TopAlignedDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	w.DrawText(r.left, r.top, AsString(q));
}

WString Ide::FormatErrorLine(const String& text, int& linecy)
{
	WString txt;
	int cx = error.HeaderObject().GetTabWidth(2) - error.HeaderTab(2).GetMargin() * 2;
	int x = 0;
	Font fnt = StdFont();
	WString h = text.ToWString();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0)
		s++;
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.Cat('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		txt.Cat(*s);
		x += chcx;
		s++;
	}
	return txt;
}

WString Ide::FormatErrorLineEP(const String& text, const char *ep, int& linecy)
{
	WString txt;
	int cx = error.HeaderObject().GetTabWidth(2) - error.HeaderTab(2).GetMargin() * 2;
	int x = 0;
	Font fnt = StdFont();
	WString h = text.ToWString();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0) {
		s++;
		if(*ep)
			ep++;
	}
	int lep = ' ';
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.Cat('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		if(lep != *ep) {
			txt.Cat(decode(*ep, '~', 2, '^', 3, 1));
			lep = *ep;
		}
		txt.Cat(*s);
		x += chcx;
		s++;
		if(*ep)
			ep++;
	}
	return txt;
}

struct ElepDisplay : public Display {
	Size    DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;

	virtual Size GetStdSize(const Value& q) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
};

Size ElepDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	WString txt = q;
	int st = 1;
	const wchar *s = txt;
	const wchar *b = s;
	int x = 0;
	int y = 0;
	int cx = 0;
	int linecy = StdFont().GetLineHeight();
	for(;;) {
		if((byte)*s < ' ') {
			int tcx = GetTextSize(b, StdFont(), int(s - b)).cx;
			if(st != 1 && (style & CURSOR) == 0)
				w.DrawRect(x + r.left, y + r.top, tcx, linecy,
				           HighlightSetup::GetHlStyle(st == 2 ? HighlightSetup::PAPER_WARNING
				                                              : HighlightSetup::PAPER_ERROR).color);
			w.DrawText(x + r.left, y + r.top, b, StdFont(), ink, int(s - b));
			x += tcx;
			cx = max(cx, tcx + x);
			if(*s == '\0')
				break;
			if(*s == '\n') {
				x = 0;
				y += linecy;
			}
			else
				st = *s;
			b = ++s;
		}
		else
			s++;
	}
	return Size(cx, y + linecy);
}

Size ElepDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return DoPaint(w, Rect(0, 0, INT_MAX, INT_MAX), q, Null, Null, 0);
}

void ElepDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

void Ide::ConsoleLine(const String& line, bool assist)
{
	if(linking) {
		linking_line.Add(line);
		return;
	}
	ErrorInfo f;
	if(FindLineError(line, error_cache, f)) {
		if(assist)
			f.kind = 1;
		if(findarg(f.kind, 1, 2) >= 0 || error.GetCount() == 0) {
			Color paper = HighlightSetup::GetHlStyle(f.kind == 1 ? HighlightSetup::PAPER_ERROR
			                                                     : HighlightSetup::PAPER_WARNING).color;
			if(f.kind == 1)
				error_count++;
			else
				warning_count++;
			if(IsDarkMismatch())
				paper = SColorPaper();
			int linecy;
			error.Add(f.file, f.lineno,
			          AttrText(FormatErrorLine(f.message, linecy)).NormalPaper(paper),
			          RawToValue(f));
			error.SetLineCy(error.GetCount() - 1, linecy);
			SyncErrorsMessage();
			addnotes = true;
			return;
		}
	}
	else {
		f.lineno = Null;
		f.file = Null;
		f.message = line;
	}
	if(addnotes) {
		int cnt = error.GetCount();
		if(cnt == 0)
			return;
		ValueArray n = error.Get(cnt - 1, "NOTES");
		bool iserrorpos = true;
		for(const char *s = f.message; *s; s++)
			if(*s != ' ' && *s != '~' && *s != '^')
				iserrorpos = false;
		int i = n.GetCount() - 1;
		if(iserrorpos && i >= 0) {
			ErrorInfo f0 = ValueTo<ErrorInfo>(n[i]);
			f0.error_pos = f.message;
			n.Set(i, RawToValue(f0));
		}
		else
			n.Add(RawToValue(f));
		error.Set(cnt - 1, "NOTES", n);
	}
}

void Ide::SyncErrorsMessage()
{
	String h;
	String cnt;
	if(IsDarkMismatch()) {
		h = "Message";
		if(error_count)
			cnt << error_count << " error(s)";
		if(warning_count) {
			if(error_count)
				cnt << ", ";
			cnt << warning_count << " warning(s)";
		}
	}
	else  {
		h = "\1[g Message";
		if(error_count)
			cnt << "[*@r " << error_count << " error" << (error_count > 1 ? "s]" : "]");
		if(warning_count) {
			if(error_count)
				cnt << ", ";
			cnt << "[@o " << warning_count << " warning" << (warning_count > 1 ? "s]" : "]");
		}
	}
	if(cnt.GetCount())
		h << " (" << cnt << ")";
	error.HeaderTab(2).SetText(h);
}

void Ide::ConsoleRunEnd()
{
	addnotes = false;
}

void Ide::ShowFound()
{
	if(ffound.IsCursor())
		GoToError(ffound);
}

void Ide::CopyFound(bool all)
{
	String txt;
	for(int i = 0; i < ffound.GetCount(); i++) {
		if(all)
			txt << ffound.Get(i, 0) << " (" << ffound.Get(i, 1) << "): ";
		String h = ffound.Get(i, 2);
		if(*h == '\1')
			h = Split(~h + 1, '\1', false).Top();
		txt << h << "\r\n";
	}
	WriteClipboardText(txt);
}

void Ide::FFoundMenu(Bar& bar)
{
	bar.Add("Copy text", THISBACK1(CopyFound, false));
	bar.Add("Copy all", THISBACK1(CopyFound, true));
}

void Ide::SelError()
{
	if(removing_notes)
		return;
	if(error.IsCursor()) {
		Value v = error.Get("NOTES");
		if(v != "0") {
			int sc = error.GetScroll();
			removing_notes = true;
			for(int i = error.GetCount() - 1; i >= 0; i--)
				if(error.Get(i, "NOTES") == "0") 
					error.Remove(i);
			removing_notes = false;
			error.ScrollTo(sc);
			error.ScrollIntoCursor();
			ValueArray n = v;
			int ii = error.GetCursor();
			for(int i = 0; i < n.GetCount(); i++) {
				const ErrorInfo& f = ValueTo<ErrorInfo>(n[i]);
				error.Insert(++ii);
				error.Set(ii, 0, f.file);
				error.Set(ii, 1, f.lineno);
				int linecy;
				if(f.error_pos.GetCount()) {
					error.Set(ii, 2, FormatErrorLineEP(f.message, f.error_pos, linecy));
					error.SetDisplay(ii, 2, Single<ElepDisplay>());
				}
				else
					error.Set(ii, 2, FormatErrorLine(f.message, linecy));
				error.Set(ii, "INFO", n[i]);
				error.Set(ii, "NOTES", "0");
				error.SetLineCy(ii, linecy);
			}
		}
		GoToError(error);
	}	
}

void Ide::ShowError()
{
	if(error.IsCursor())
		GoToError(error);
}

void Ide::FoundDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	String s = q;
	if(*s == '\1') {
		Vector<String> h = Split(~s + 1, '\1', false);
		if(h.GetCount() < 4)
			return;
		One<EditorSyntax> es = EditorSyntax::Create(h[0]);
		es->IgnoreErrors();
		WString ln = h[3].ToWString();
		Vector<LineEdit::Highlight> hln;
		hln.SetCount(ln.GetCount() + 1);
		for(int i = 0; i < ln.GetCount(); i++) {
			LineEdit::Highlight& h = hln[i];
			h.paper = paper;
			h.ink = SColorText();
			h.chr = ln[i];
			h.font = StdFont();
		}
		HighlightOutput hl(hln);
		es->Highlight(ln.Begin(), ln.End(), hl, NULL, 0, 0);
		int fcy = GetStdFontCy();
		int y = r.top + (r.GetHeight() - fcy) / 2;
		int x = r.left;
		w.DrawRect(r, paper);
		int sl = utf8len(~h[3], atoi(h[1]));
		int sh = utf8len(~h[3] + sl, atoi(h[2])) + sl;
		for(int i = 0; i < hln.GetCount(); i++) {
			Font fnt = StdFont();
			LineEdit::Highlight& h = hln[i];
			fnt.Bold(h.font.IsBold());
			fnt.Italic(h.font.IsItalic());
			fnt.Underline(h.font.IsUnderline());
			int cw = fnt[h.chr];
			if(h.chr == '\t')
				cw = 4 * fnt[' '];
			Color hpaper = HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color;
			Color hink = h.ink;
			if(IsDarkMismatch()) {
				hpaper = paper;
				hink = ink;
			}
			if(i >= sl && i < sh && !(style & (CURSOR|SELECT|READONLY)))
				w.DrawRect(x, y, cw, fcy, hpaper);
			if(h.chr != '\t')
				w.DrawText(x, y, &h.chr, fnt, hink, 1);
			x += cw;
		}
	}
	else
		StdDisplay().Paint(w, r, q, ink, paper, style);
}

void Ide::FoundFileDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	DrawFileName(w, r, q, ink);
}

Size Ide::FoundFileDisplay::GetStdSize(const Value& q) const
{
	return GetDrawFileNameSize(q);
}
