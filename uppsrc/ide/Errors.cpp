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
			int e = f.file.GetLength();
			while(e > 0 && f.file[e - 1] == ' ')
				e--;
			f.file.Trim(e);
			f.file = TrimLeft(f.file);
			String upp = GetUppDir();
		#ifdef PLATFORM_WIN32
			if(f.file[0] == '\\' || f.file[0] == '/')
				f.file = String(upp[0], 1) + ':' + f.file;
		#endif
			if(!IsFullPath(f.file) && *f.file != '\\' && *f.file != '/') {
				if(cache.wspc_paths.IsEmpty()) {
					::Workspace  wspc;
					wspc.Scan(main);
					for(int i = 0; i < wspc.GetCount(); i++)
						cache.wspc_paths.Add(GetFileDirectory(PackagePath(wspc[i])));
				}
				for(int i = 0; i < cache.wspc_paths.GetCount(); i++) {
					String path = AppendFileName(cache.wspc_paths[i], f.file);
					int q = cache.ff.Find(path);
					if(q >= 0) {
						if(cache.ff[q]) {
							f.file = path;
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
							f.file = path;
							break;
						}
					}
				}
			}
			f.file = FollowCygwinSymlink(f.file);
			if(IsFullPath(f.file) && FileExists(f.file) && IsTextFile(f.file)) {
				while(*s && !IsDigit(*s))
					s++;
				f.lineno = f.linepos = 0;
				CParser p(s);
				if(p.IsInt())
					f.lineno = p.ReadInt();
				if(p.Char(':') && p.IsInt()) 
					f.linepos = p.ReadInt();
				const char *ms = p.GetPtr();
				int pos = int(ms - ~ln);
				f.kind = ln.Find("warning", pos) > 0 ? 2 :
				         ln.Find("error", pos) > 0 ? 1 :				         
				         ln.Find("note", pos) > 0 ? 3 : 4;
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
					if(fi.IsOpen())
					{
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
				if(f.lineno > 0)
					return true;
			}
			f.file.Clear();
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
		int c = a.GetCursor() + d;
		if(c >= 0 && c < a.GetCount())
			a.SetCursor(c);
		else {
			if(d > 0)
				a.GoBegin();
			else
				a.GoEnd();
		}
		return true;
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
			error.Add(f.file, f.lineno,
			          AttrText(f.message)
			          .NormalPaper(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_ERROR).color),
			          RawToValue(f));
		}
	}
	SyncErrorsMessage();
	linking = false;
}

bool IsDarkMismatch()
{
	return IsDark(SColorPaper()) != IsDark(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_NORMAL).color);
}

void Ide::ConsoleLine(const String& line)
{
	if(linking) {
		linking_line.Add(line);
		return;
	}
	ErrorInfo f;
	if(FindLineError(line, error_cache, f)) {
		if(findarg(f.kind, 1, 2) >= 0 || error.GetCount() == 0) {
			Color paper = HighlightSetup::GetHlStyle(f.kind == 1 ? HighlightSetup::PAPER_ERROR
			                                                     : HighlightSetup::PAPER_WARNING).color;
			if(f.kind == 1)
				error_count++;
			else
				warning_count++;
			if(IsDarkMismatch())
				paper = SColorPaper();
			error.Add(f.file, f.lineno,
			          AttrText(f.message).NormalPaper(paper),
			          RawToValue(f));
			SyncErrorsMessage();
			addnotes = true;
			return;
		}
	}
	else {
		f.lineno = Null;
		f.file = Null;
		f.message = TrimLeft(line);
	}
	if(addnotes)
		AddNote(f);
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

void Ide::AddNote(const ErrorInfo& f)
{
	int cnt = error.GetCount();
	if(cnt == 0)
		return;
	ValueArray n = error.Get(cnt - 1, "NOTES");
	n.Add(RawToValue(f));
	error.Set(cnt - 1, "NOTES", n);
}

void Ide::ShowNote()
{
	if(notes.IsCursor())
		GoToError(notes);
}

void Ide::ShowFound()
{
	if(ffound.IsCursor())
		GoToError(ffound);
}

void Ide::ShowError()
{
	notes.Clear();
	if(error.IsCursor()) {
		ValueArray n = error.Get("NOTES");
		for(int i = 0; i < n.GetCount(); i++) {
			const ErrorInfo& f = ValueTo<ErrorInfo>(n[i]);
			notes.Add(f.file, f.lineno, f.message, n[i]);
		}
		GoToError(error);
	}	
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
