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
				int pos = ms - ~ln;
				f.kind = ln.Find("error", pos) > 0 ? 1 :
				         ln.Find("warning", pos) > 0 ? 2 :
				         ln.Find("note", pos) > 0 ? 3 : 4;
				const char *hs = ms;
				while(!IsLetter(*hs) && *hs)
					hs++;
				f.message = *hs ? hs : ms;
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
	FindLineError(GetConsole().GetLine(GetConsole().GetCursor()));
}

void Ide::FindNextError()
{
	if(findarg(btabs.GetCursor(), BCONSOLE, BFINDINFILES) < 0) {
		int c = error.GetCursor();
		if(c >= 0 && c < error.GetCount() - 1)
			error.SetCursor(c + 1);
		else
			error.GoBegin();
	}
	else {
		int ln = GetConsole().GetLine(GetConsole().GetCursor());
		int l = ln;
		for(l = ln; l < GetConsole().GetLineCount(); l++)
			if(FindLineError(l)) return;
		for(l = 0; l < ln; l++)
			if(FindLineError(l)) return;
	}
}

void Ide::FindPrevError() {
	if(findarg(btabs.GetCursor(), BCONSOLE, BFINDINFILES) < 0) {
		int c = error.GetCursor();
		if(c > 0)
			error.SetCursor(c - 1);
		else
			error.GoEnd();
	}
	else {
		int ln = GetConsole().GetLine(GetConsole().GetCursor());
		int l = ln;
		One<Host> host = CreateHost(false);
		for(l = ln - 2; l >= 0; l--)
			if(FindLineError(l)) return;
		for(l = GetConsole().GetLineCount() - 1; l > ln; l--)
			if(FindLineError(l)) return;
	}
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

bool Ide::FindLineError(int l) {
	ErrorInfo f;
	Console& c = GetConsole();
	FindLineErrorCache cache;
	if(FindLineError(c.GetUtf8Line(l), cache, f)) {
		GoToError(f);
		c.SetSelection(c.GetPos(l), c.GetPos(l + 1));
		if(btabs.GetCursor() != BCONSOLE && btabs.GetCursor() != BFINDINFILES)
			ShowConsole();
		return true;
	}
	return false;
}

void Ide::ConsoleLine(const String& line)
{
	ErrorInfo f;
	if(FindLineError(line, error_cache, f)) {
		if(findarg(f.kind, 1, 2) >= 0 || error.GetCount() == 0)
			error.Add(GetFileName(f.file), f.lineno,
			          AttrText(f.message)
			          .NormalPaper(HighlightSetup::GetHlStyle(f.kind == 1 ? HighlightSetup::PAPER_ERROR
			                                                              : HighlightSetup::PAPER_WARNING).color),
			          RawToValue(f));
		else
			AddNote(f);
	}
}

void Ide::AddNote(const ErrorInfo& f)
{
	int cnt = error.GetCount();
	ValueArray n = error.Get(cnt - 1, "NOTES");
	n.Add(RawToValue(f));
	error.Set(cnt - 1, "NOTES", n);
}

void Ide::ShowNote()
{
	if(notes.IsCursor())
		GoToError(ValueTo<ErrorInfo>(notes.Get("INFO")));
}

void Ide::ShowError()
{
	notes.Clear();
	if(error.IsCursor()) {
		ValueArray n = error.Get("NOTES");
		for(int i = 0; i < n.GetCount(); i++) {
			const ErrorInfo& f = ValueTo<ErrorInfo>(n[i]);
			notes.Add(GetFileName(f.file), f.lineno, f.message, n[i]);
		}
		GoToError(ValueTo<ErrorInfo>(error.Get("INFO")));
	}	
}
