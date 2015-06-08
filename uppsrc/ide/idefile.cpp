#include "ide.h"

void Ide::SetupEditor(int f, String hl, String fn)
{
	if(IsNull(hl)) {
		hl = EditorSyntax::GetSyntaxForFilename(fn);
		if(IsNull(hl))		
			hl = EditorSyntax::GetSyntaxForFilename(ToLower(fn));
	}
	switch(f) {
	case 1:  editor.SetFont(font1); break;
	case 2:  editor.SetFont(font2); break;
	case 3:  editor.SetFont(consolefont); break;
	default: editor.SetFont(editorsplit.GetZoom() < 0 && editorsplit.IsHorz() ? veditorfont :
	                        hl == "t" ? tfont : editorfont); break;
	}
	editor.Highlight(hl);
}

void Ide::SetupEditor()
{
	if(!IsActiveFile())
		return;
	Package::File& f = ActiveFile();
	SetupEditor(f.font, f.highlight, GetFileExt(GetActiveFileName()));
}

void Ide::FileCursor()
{
	WorkspaceWork::FileCursor();
	if(IsActiveFile() && !filelist[filelist.GetCursor()].isdir) {
		const Package::File& f = ActiveFile();
		editor.SetEditable(!f.readonly);
		editor.TabSize(f.tabsize > 0 ? f.tabsize : editortabsize);
		SetupEditor();
		String headername;
		if(insert_include)
			for(int i = insert_include == 1 ? 0 : filelist.GetCursor();
			    i >= 0 && i < filelist.GetCount();
			    i += insert_include == 1 ? 1 : -1) {
					String nm = FileName(i);
					if(!IsSeparator(i) && GetFileExt(nm) == ".h") {
						headername = nm;
						break;
					}
			}
		String p = GetActiveFileName();
		if(p != HELPNAME)
			p = GetActiveFilePath();
		EditFile0(p, f.charset ? f.charset : actual.charset ? actual.charset : default_charset,
		          false, headername);
	}
}

bool Ide::HasFileData(const String& file)
{
	return filedata.Find(NormalizePath(file)) >= 0;
}

Ide::FileData& Ide::Filedata(const String& file) {
	return filedata.GetAdd(NormalizePath(file));
}

void DlCharsetD(DropList& d)
{
	d.Add(0, "Default");
	DlCharset(d);
}

void Ide::ChangeFileCharset(const String& name, Package& p, byte charset)
{
	if(IsNull(name))
		return;
	bool sv = false;
	for(int i = 0; i < p.file.GetCount(); i++)
		if(SourcePath(name, p.file[i]) == editfile && p.file[i].charset != charset) {
			p.file[i].charset = charset;
			sv = true;
		}
	if(sv) {
		String pp = PackagePathA(name);
		RealizePath(pp);
		p.Save(pp);
	}
}

void Ide::FileProperties()
{
	if(!IsActiveFile()) return;
	WithFileFormatLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "File properties");
	Package::File& f = ActiveFile();
	DlCharsetD(d.charset);
	d.font.Add(0, "Normal");
	d.font.Add(1, "Small");
	d.font.Add(2, "Special");
	d.font.Add(3, "Console");
	d.highlight.Add(Null, "Default");
	for(int i = 0; i < EditorSyntax::GetSyntaxCount(); i++) {
		String desc = EditorSyntax::GetSyntaxDescription(i);
		if(desc.GetCount())
			d.highlight.Add(EditorSyntax::GetSyntax(i), desc);
	}
	d.tabsize <<= f.tabsize > 0 ? f.tabsize : Null;
	d.tabsize <<= d.Breaker(111);
	d.tabsize.MinMax(1, 100);
	d.charset <<= (int)f.charset;
	d.font <<= f.font;
	d.font <<= d.Breaker(111);
	d.highlight <<= f.highlight;
	d.highlight <<= d.Breaker(111);
	d.line_endings.Add(CRLF, "CRLF");
	d.line_endings.Add(LF, "LF");
	d.line_endings <<= findarg(Nvl(editfile_line_endings, line_endings), LF, DETECT_LF) >= 0 ? LF : CRLF;
	d.line_endings.Enable(findarg(line_endings, DETECT_CRLF, DETECT_LF) >= 0);
	for(;;) {
		switch(d.Run()) {
		case IDCANCEL:
			FlushFile();
			FileCursor();
			return;
		case IDOK:
			int c = filelist.GetCursor();
			FlushFile();
			f.charset = (byte)(int)~d.charset;
			f.tabsize = Nvl((int)~d.tabsize);
			f.font = Nvl((int)~d.font);
			f.highlight = ~d.highlight;
			SavePackage();
			PackageCursor();
			filelist.SetCursor(c);
			editor.ClearUndo();
			if(editfile_line_endings != ~d.line_endings) {
				editfile_line_endings = ~d.line_endings;
				SaveFile(true);
			}
			MakeTitle();
			return;
		}
		if(!IsNull(editfile)) {
			int ts = Nvl((int)~d.tabsize);
			editor.TabSize(ts ? ts : editortabsize);
			SetupEditor(Nvl((int)~d.font), ~d.highlight, editfile);
		}
	}
}

void Ide::ChangeCharset()
{
	if(!IsActiveFile()) return;
	Package::File& f = ActiveFile();
	SaveFile();
	WithCharsetLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Save file with enconding");
	DlCharsetD(dlg.charset);
	dlg.charset <<= editor.GetCharset();
	dlg.ActiveFocus(dlg.charset);
	byte cs;
	for(;;) {
		if(dlg.Run() != IDOK)
			return;
		cs = (byte)(int)~dlg.charset;
		int q = editor.GetInvalidCharPos(cs);
		if(q >= 0) {
			if(PromptYesNo("File contains characters that cannot be saved using selected encoding.&"
			               "Save anyway? (If you choose Yes, they will be replaced with question marks.)"))
								break;
			editor.SetCursor(q);
			return;
		}
		else
			break;
	}
	f.charset = cs;
	SavePackage();
	editor.SetCharset(f.charset);
	SaveFile(true);
	FlushFile();
	FileCursor();
	editor.ClearUndo();
}

void Ide::PosSync()
{
	if(designer || editfile.IsEmpty())
		return;
	for(int i = 0; i < 2; i++)
		if(PathIsEqual(posfile[i], editfile))
			editor.SetPtr(posline[i], posimg[i], i);
		else
			editor.SetPtr(-1, Image(), i);
	editor.SyncTip();
}

bool Ide::IsProjectFile(const String& f) const
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String fn = pk.file[i];
			String path = SourcePath(n, fn);
			if(f == path)
				return true;
		}
	}
	return false;
}

void Ide::ScanFile()
{
	if(IsCppBaseFile()) {
		String s = ~editor;
		StringStream ss(s);
		CodeBaseScanFile(ss, editfile);
	}
}

String ConvertTLine(const String& line, int flag)
{
	String r;
	const char *s = line;
	
	while(*s) {
		if(*s == '\"') {
			CParser p(s);
			p.NoSkipSpaces();
			try {
				r.Cat(AsCString(p.ReadOneString(), INT_MAX, NULL, flag));
			}
			catch(CParser::Error) {
				return line;
			}
			s = p.GetPtr();
		}
		else
			r.Cat(*s++);
	}
	return r;
}

void Ide::SaveFile(bool always)
{
	issaving++;
	SaveFile0(always);
	issaving--;
}

void Ide::SaveEditorFile(Stream& out)
{
	if(GetFileExt(editfile) == ".t") {
		for(int i = 0; i < editor.GetLineCount(); i++) {
			if(i) out.PutCrLf();
			out.Put(ConvertTLine(editor.GetUtf8Line(i), ASCSTRING_OCTALHI));
		}
	}
	else {
		int le = line_endings;
		if(le == DETECT_CRLF)
			le = Nvl(editfile_line_endings, CRLF);
		if(le == DETECT_LF)
			le = Nvl(editfile_line_endings, LF);
		editor.Save(out, editor.GetCharset(), le == LF ? TextCtrl::LE_LF : TextCtrl::LE_CRLF);
	}
}

void Ide::SaveFile0(bool always)
{
	if(designer) {
		String fn = designer->GetFileName();
		Time tm = FileGetTime(fn);
		designer->Save();
		if(tm != FileGetTime(fn))
			TouchFile(fn);
		if(IsProjectFile(fn) && ToUpper(GetFileExt(fn)) == ".LAY")
			CodeBaseScanFile(fn);
		return;
	}

	if(editfile.IsEmpty())
		return;

	FileData& fd = Filedata(editfile);
	fd.lineinfo = editor.GetLineInfo();
	fd.lineinforem = editor.GetLineInfoRem();
	fd.editpos = editor.GetEditPos();
	fd.columnline = editor.GetColumnLine(fd.editpos.cursor);
	fd.filetime = edittime;
	if(!editor.IsDirty() && !always)
		return;
	TouchFile(editfile);
#ifdef PLATFORM_POSIX
	FindFile ff(editfile);
	if(ff && !designer && (ff.GetUid() != getuid() || ff.IsSymLink())) {
		for(;;) {
			FileStream out;
			if(out.Open(editfile, FileStream::READWRITE)) {
				SaveEditorFile(out);
				out.SetSize(out.GetPos());
				if(!out.IsError())
					break;
			}
			console.Flush();
			Sleep(200);
			int art = Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
				"Unable to save current file.&"
				"Retry save, ignore it or save file to another location?",
				"Save as...", "Retry", "Ignore");
			if(art < 0)
				break;
			if(art && AnySourceFs().ExecuteSaveAs()) {
				editfile = AnySourceFs();
				MakeTitle();
				PosSync();
				break;
			}
		}
		if(IsDeactivationSave())
			return;
	}
	else
#endif
	{
		String tmpfile = editfile + ".$tmp", outfile = editfile;
		{
			FileOut out(tmpfile);
			if(!out.IsOpen()) {
				if(IsDeactivationSave())
					return;
				Exclamation(NFormat("Error creating temporary file [* \1%s\1].", tmpfile));
				return;
			}
			if(designer)
				designer->Save();
			else
				SaveEditorFile(out);
			out.Close();
			if(out.IsError()) {
				if(IsDeactivationSave())
					return;
				Exclamation(NFormat("Error writing temporary file [* \1%s\1].", tmpfile));
				return;
			}
		}
	
		for(;;) {
			if(editfile != outfile || !FileExists(tmpfile))
				return;
			FileDelete(outfile);
			if(FileMove(tmpfile, outfile))
				break;
			if(IsDeactivationSave())
				return;
			console.Flush();
			int art = Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
				"Unable to save current file.&"
				"Retry save, ignore it or save file to another location?",
				"Save as...", "Retry", "Ignore");
			if(art < 0)
				break;
			if(art && AnySourceFs().ExecuteSaveAs()) {
				outfile = editfile = AnySourceFs();
				MakeTitle();
				PosSync();
				break;
			}
		}
	
	#ifdef PLATFORM_POSIX
		if(ff)
			chmod(editfile, ff.GetMode());
	#endif
	}

	if(!designer) {
		FindFile ff(editfile);
		fd.filetime = edittime = ff.GetLastWriteTime();
	}

	if(editor.IsDirty()) {
		text_updated.Kill();
		if(IsCppBaseFile())
			CodeBaseScanFile(editfile);
	}

	editor.ClearDirty();

	if(ToLower(GetFileExt(editfile)) == ".usc")
		SyncUsc();

	MakeTitle();
}

void Ide::FlushFile() {
	editor.CloseAssist();
	SaveFile();
	editor.assist_active = false;
	if(designer) {
		designer->SaveEditPos();
		designer->DesignerCtrl().SetFrame(NullFrame());
		if(dynamic_cast<TopicEditor *>(&designer->DesignerCtrl()))
			RefreshBrowser();
	}
	else
	if(!editfile.IsEmpty())
		Filedata(editfile).undodata = editor.PickUndoData();
	editfile.Clear();
	editfile_svn = editfile_isfolder = false;
	svn_dirs = SvnDirs(true).GetCount(); // Perhaps not the best place, but should be ok
	editor.Clear();
	editor.Disable();
	editorsplit.Ctrl::Remove();
	editor.SetFrame(NullFrame());
	designer.Clear();
	SetBar();
}

int CharFilterMacro(int c)
{
	return iscid(c) ? c : '_';
}

void Ide::FileRename(const String& nm)
{
	tabs.RenameFile(editfile, nm);
}

bool Ide::FileRemove()
{
	int q = FindIndex(tablru, editfile);
	if(q >= 0)
		tablru.Remove(q);
	q = tabs.GetCursor();
	if(q >= 0) {
		tabs.CloseForce(q, false);
		if(filelist.GetCount())
			return true;
		if(tabs.GetCount())
			TabFile();
		else {
			tabs.Refresh();
			FlushFile();
		}
		return false;
	}
	return true;
}

void Ide::EditFile0(const String& path, byte charset, bool astext, const String& headername) {
	text_updated.Kill();

	AKEditor();
	editor.CheckEdited(false);
	editor.CloseAssist();
	if(path.IsEmpty()) return;
	FlushFile();

	editfile = path;
	editor.SetCharset(charset);
	AddLru();

	editfile_isfolder = IsFolder(editfile);
	svn_dirs = SvnDirs(true).GetCount(); // Perhaps not the best place, but should be ok
	
	bool candesigner = !astext && !(debugger && (PathIsEqual(path, posfile[0]) || PathIsEqual(path, posfile[0])))
	   && editastext.Find(path) < 0 && !IsNestReadOnly(editfile);
	
	if(candesigner) {
		for(int i = 0; i < GetIdeModuleCount() && !designer; i++)
			designer = GetIdeModule(i).CreateDesigner(this, path, charset);
	}

	if(!designer && editastext.Find(path) < 0 && (FileIsBinary(path) || editashex.Find(path) >= 0))
		designer.Create<FileHexView>().Open(path);

	if(designer) {
		editpane.Add(designer->DesignerCtrl().SizePos());
		designer->RestoreEditPos();
		if(filetabs)
			tabs.SetAddFile(editfile);
		MakeTitle();
		SetBar();
		designer->SetFocus();
		return;
	}

	tabs.SetAddFile(editfile);
	tabs.SetSplitColor(editfile2, Yellow);
	editor.Enable();
	editpane.Add(editorsplit);
	editor.HiliteScope(hilite_scope);
	editor.OverWriteMode(false);
	ActiveFocus(editor);
	FileData& fd = Filedata(editfile);
	FindFile ff(editfile);
	bool tfile = GetFileExt(editfile) == ".t";
	if(ff) {
		edittime = ff.GetLastWriteTime();
		if(edittime != fd.filetime || IsNull(fd.filetime))
			fd.undodata.Clear();
		FileIn in(editfile);
		if(in) {
			if(tfile && editastext.Find(editfile) < 0) {
				String f;
				String ln;
				for(;;) {
					int c = in.Get();
					if(c < 0) {
						f.Cat(ConvertTLine(ln, 0));
						break;
					}
					if(c != '\r') {
						ln.Cat(c);
						if(c == '\n') {
							f.Cat(ConvertTLine(ln, 0));
							ln.Clear();
						}
					}
				}
				editor.Set(f);
				editor.SetCharset(CHARSET_UTF8);
			}
			else {
				int le = editor.Load(in, charset);
				editfile_line_endings = le == TextCtrl::LE_CRLF ? CRLF : le == TextCtrl::LE_LF ? LF : (int)Null;
			}
		}
		else
			Exclamation("Failed to read the file.");
		editor.SetEditPos(fd.editpos);
		if(!IsNull(fd.columnline) && fd.columnline.y >= 0 && fd.columnline.y < editor.GetLineCount())
			editor.SetCursor(editor.GetColumnLinePos(fd.columnline));
		editor.SetPickUndoData(pick(fd.undodata));
		editor.SetLineInfo(fd.lineinfo);
		editor.SetLineInfoRem(pick(fd.lineinforem));
		if(ff.IsReadOnly() || IsNestReadOnly(editfile) || editor.IsTruncated()) {
			editor.SetReadOnly();
			editor.NoShowReadOnly();
		}
		fd.ClearP();
		PosSync();
	}
	else {
		RealizePath(editfile);
		if(GetFileExt(editfile) == ".h") {
			String n = '_' + Filter(String().Cat() << GetActivePackage() << '_' << GetFileTitle(editfile) << "_h_",
			                        CharFilterMacro);
			String s;
			s << "#ifndef " << n << "\r\n";
			s << "#define " << n << "\r\n";
			s << "\r\n";
			s << "#endif\r\n";
			editor <<= s;
			editor.SetCursor(editor.GetPos(2));
		}
		if(IsCSourceFile(editfile) && !IsNull(headername)) {
			editor <<= "#include \"" + headername + "\"\r\n";
			editor.SetCursor(editor.GetPos(1));
		}
		editor.SetCharset(tfile ? CHARSET_UTF8 : charset);
	}
	editor.SetFocus();
	MakeTitle();
	SetBar();
	editor.assist_active = IsProjectFile(editfile) && IsCppBaseFile();
	editor.CheckEdited(true);
	editor.Annotate(editfile);
	editor.SyncNavigator();
	editfile_svn = IsSvnDir(GetFileFolder(editfile));
}

void Ide::EditFileAssistSync()
{
	ScanFile();
	editor.Annotate(editfile);
	editor.SyncNavigator();
}

void Ide::TriggerAssistSync()
{
	if(editor.GetLength() < 500000) // Sanity
		text_updated.KillSet(1000, THISBACK(EditFileAssistSync));
}

void Ide::EditAsHex()
{
	String path = editfile;
	editastext.RemoveKey(editfile);
	editashex.FindPut(editfile);
	byte cs = editor.GetCharset();
	FlushFile();
	EditFile0(path, cs);
}

void Ide::EditAsText()
{
	String path = editfile;
	editashex.RemoveKey(editfile);
	editastext.FindPut(editfile);
	byte cs = editor.GetCharset();
	FlushFile();
	EditFile0(path, cs);
}

void Ide::EditUsingDesigner()
{
	String path = editfile;
	editashex.RemoveKey(editfile);
	editastext.RemoveKey(editfile);
	byte cs = editor.GetCharset();
	FlushFile();
	EditFile0(path, cs);
}

void Ide::AddEditFile(const String& path)
{
	actual.file.AddPick(Package::File(path));
	if(IsAux())
		SaveLoadPackageNS(false);
	else
		SaveLoadPackage(false);
	ShowFile(package.GetCount() - 1);
	filelist.SetCursor(filelist.GetCount() - 1);
}

void Ide::EditFile(const String& p)
{
	if(p.IsEmpty()) {
		FlushFile();
		return;
	}
	if(p == HELPNAME) {
		if(designer && designer->GetFileName() == p)
			return;
		package.FindSetCursor(METAPACKAGE);
		filelist.FindSetCursor(HELPNAME);
		return;
	}

	String path = NormalizePath(p);
	if(designer ? path == designer->GetFileName() : path == editfile)
		return;

	FlushFile();
	if(path.IsEmpty())
		return;

	for(int i = 0; i < package.GetCount(); i++) {
		String pkg = package[i].name;
		Package p;
		p.Load(PackagePathA(pkg));
		for(int j = 0; j < p.file.GetCount(); j++)
			if(PathIsEqual(SourcePath(pkg, p.file[j]), path)) {
				package.FindSetCursor(pkg);
				ShowFile(j);
				filelist.FindSetCursor(p.file[j]);
				return;
			}
		if(GetFileExt(path) == ".tpp" && PathIsEqual(SourcePath(pkg, GetFileName(path)), path)) {
			filelist.KillCursor();
			package.KillCursor();
			package.SetCursor(i);
			AddEditFile(GetFileName(path));
			return;
		}
	}
	filelist.KillCursor();
	package.KillCursor();
	package.SetCursor(package.GetCount() - 2);
	AddEditFile(path);
}

bool Ide::IsCppBaseFile()
{
	return IsProjectFile(editfile) && (IsCSourceFile(editfile) || IsCHeaderFile(editfile) ||
	                                   ToUpper(GetFileExt(editfile)) == ".SCH");
}

void Ide::CheckFileUpdate()
{
	if(editfile.IsEmpty() || !IsForeground() || designer) return;
	FindFile ff(editfile);
	if(!ff) return;
	FileTime tm = ff.GetLastWriteTime();
	if(tm == edittime) return;
	edittime = tm;
	if(editor.IsDirty() && !Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
		"Current file was changed outside the IDE, but was also edited inside it.&"
		"Would you like to reload the file or to keep changes made in the IDE ?",
		"Reload", "Keep")) return;

	if(IsCppBaseFile())
		CodeBaseScanFile(editfile);
	ReloadFile();
}

static void GetLineIndex(String file, HashBase& hash, Vector<String>& lines)
{
	const char *p = file;
	while(*p)
	{
		while(*p && *p != '\n' && (byte)*p <= ' ')
			p++;
		const char *b = p;
		while(*p && *p++ != '\n')
			;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		String s(b, e);
		hash.Add(GetHashValue(s));
		lines.Add(s);
	}
}

int LocateLine(String old_file, int old_line, String new_file)
{
	HashBase old_hash, new_hash;
	Vector<String> old_lines, new_lines;
	GetLineIndex(old_file, old_hash, old_lines);
	GetLineIndex(new_file, new_hash, new_lines);
	if(old_line <= 0)
		return 0;
	if(old_line >= old_lines.GetCount())
		return new_lines.GetCount();
	String line = old_lines[old_line];
	//int hash = old_hash[old_line]; Mirek: unused
	//int fore_count = old_lines.GetCount() - old_line - 1;
	int best_match = 0, best_value = 0;
	for(int r = 0; r < 10 && !best_value; r++)
	{
		int src = (r & 1 ? old_line + (r >> 1) + 1 : old_line - (r >> 1));
		if(src < 0 || src >= old_lines.GetCount())
			continue;
		dword hash = old_hash[src];
		for(int i = new_hash.Find(hash); i >= 0; i = new_hash.FindNext(i))
			if(new_lines[i] == old_lines[src])
			{
				int max_back = min(i, src);
				int max_fore = min(new_lines.GetCount() - i, old_lines.GetCount() - src) - 1;
				if(max_back + max_fore <= best_value)
					continue;
				int back = 1;
				while(back <= max_back && new_hash[i - back] == old_hash[src - back]
					&& new_lines[i - back] == old_lines[src - back])
					back++;
				int fore = 1;
				while(fore < max_fore && new_hash[i + fore] == old_hash[src + fore]
					&& new_lines[i + fore] == old_lines[src + fore])
					fore++;
				if(back + fore > best_value)
				{
					best_value = back + fore;
					best_match = minmax(i, 0, new_lines.GetCount());
				}
			}
	}
	return best_match;
}

void Ide::ReloadFile()
{
	if(editfile.IsEmpty())
		return;
	String fn = editfile;
	String data = ~editor;
	int ln = editor.GetCursorLine();
	editfile.Clear();
	int sc = filelist.GetSbPos();
	EditFile0(fn, editor.GetCharset());
	filelist.SetSbPos(sc);
	int l = LocateLine(data, ln, ~editor);
	editor.SetCursor(editor.GetPos(l));
}

void Ide::EditAnyFile() {
	FileSel& fs = AnySourceFs();
#if 0
	fs.Multi(false);
	if(!fs.ExecuteOpen()) return;
	EditFile(fs);
	FileSelected();
#endif
	if(fs.ExecuteOpen())
		for(int i = 0; i < fs.GetCount(); i++) {
			EditFile(fs[i]);
			FileSelected();
		}
}

void Ide::DragAndDrop(Point, PasteClip& d)
{
	if(AcceptFiles(d)) {
		Vector<String> f = GetFiles(d);
		for(int i = 0; i < f.GetCount(); i++)
			if(FileExists(f[i])) {
				EditFile(f[i]);
				FileSelected();
				editor.SetFocus();
			}
	}
}

void Ide::AddLru()
{
	if(editfile.IsEmpty() || tabi) return;
	LruAdd(tablru, editfile, 200);
}

static String sExFiles(const char *fn, const char **ext, int cnt)
{
	for(int i = 0; i < cnt; i++) {
		String f = ForceExt(fn, ext[i]);
		if(FileExists(f))
			return f;
	}
	return Null;
}

String Ide::GetOpposite()
{
	static const char *cpp[] = { ".c", ".cpp", ".cc", ".cxx" };
	static const char *hdr[] = { ".h", ".hpp", ".hh", ".hxx" };
	if(IsNull(editfile) || designer)
		return Null;
	String ext = GetFileExt(editfile);
	for(int i = 0; i < __countof(cpp); i++)
		if(ext == cpp[i])
			return sExFiles(editfile, hdr, __countof(hdr));
	for(int i = 0; i < __countof(hdr); i++)
		if(ext == hdr[i])
			return sExFiles(editfile, cpp, __countof(cpp));
	return Null;
}

void Ide::GoOpposite()
{
	String fn = GetOpposite();
	if(!IsNull(fn))
		EditFile(fn);
}

void Ide::PassEditor()
{
	editorsplit.NoZoom();
	SyncEditorSplit();
	SetupEditor();
	editfile2 = editfile;
	editor2.SetFont(editor.GetFont());
	editor2.Highlight(editor.GetHighlight());
	editor2.LoadHlStyles(editor.StoreHlStyles());
	byte charset = editor.GetCharset();
	editor2.CheckEdited(false);
	editor2.Set(editor.Get(charset), charset);
	editor2.SetEditPosSb(editor.GetEditPos());
	editor2.CheckEdited();
	editor.SetFocus();
	editor.ScrollIntoCursor();
}

void Ide::ClearEditedFile()
{
	editor.ClearEdited();
}

void Ide::ClearEditedAll()
{
	ClearEditedFile();
	for(int i = 0; i < filedata.GetCount(); i++) {
		LineInfo li = editor.GetLineInfo();
		LineInfoRem lir = editor.GetLineInfoRem();
		FileData& fd = Filedata(filedata.GetKey(i));
		editor.SetLineInfo(fd.lineinfo);
		editor.SetLineInfoRem(pick(fd.lineinforem));
		ClearEditedFile();
		fd.lineinfo = editor.GetLineInfo();
		fd.lineinforem = editor.GetLineInfoRem();
		editor.SetLineInfo(li);
	}
}

void Ide::SplitEditor(bool horz)
{
	if(editorsplit.GetZoom() < 0)
		CloseSplit();
	
	if(horz)
		editorsplit.Horz(editor2, editor);
	else
		editorsplit.Vert(editor2, editor);
	
	tabs.SetSplitColor(editfile, Yellow);
	PassEditor();
}

void Ide::SwapEditors()
{
	String f = editfile2;
	CodeEditor::EditPos p = editor2.GetEditPos();
	if(editorsplit.GetFirstChild() == &editor)
		if(editorsplit.IsVert())
			editorsplit.Vert(editor2, editor);
		else
			editorsplit.Horz(editor2, editor);
	else
		if(editorsplit.IsVert())
			editorsplit.Vert(editor, editor2);
		else
			editorsplit.Horz(editor, editor2);
	PassEditor();
	EditFile(f);
	editor.SetEditPos(p);
}

void Ide::CloseSplit()
{
	editorsplit.Vert(editor, editor2);
	editorsplit.Zoom(0);
	editfile2.Clear();
	tabs.ClearSplitColor();
	SyncEditorSplit();
	editor.SetFocus();
	SetupEditor();
}

void Ide::KeySplit(bool horz)
{
	if(editorsplit.GetZoom() >= 0)
		SplitEditor(horz);
	else
		CloseSplit();
}

void Ide::SyncEditorSplit()
{
	editor.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::split()).Tip("Split (Ctrl+[-])");
	editor.topsbbutton <<= THISBACK1(SplitEditor, false);
	editor.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::vsplit()).Tip("Split (Ctrl+[\\])");
	editor.topsbbutton1 <<= THISBACK1(SplitEditor, true);
	editor2.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::split()).Tip("Split (Ctrl+[-])");
	editor2.topsbbutton <<= THISBACK1(SplitEditor, false);
	editor2.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::vsplit()).Tip("Split (Ctrl+[\\])");
	editor2.topsbbutton1 <<= THISBACK1(SplitEditor, true);
	if(editorsplit.GetZoom() >= 0)
		return;
	if(editorsplit.IsVert()) {
		editor.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Tip("Close (Ctrl+[-])");
		editor.topsbbutton <<= THISBACK(CloseSplit);
		editor2.topsbbutton.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Tip("Close (Ctrl+[-])");
		editor2.topsbbutton <<= THISBACK(CloseSplit);
	}
	else {
		editor.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Tip("Close (Ctrl+[\\])");
		editor.topsbbutton1 <<= THISBACK(CloseSplit);
		editor2.topsbbutton1.ScrollStyle().SetMonoImage(IdeImg::closesplit()).Tip("Close (Ctrl+[\\])");
		editor2.topsbbutton1 <<= THISBACK(CloseSplit);
	}
}

bool Ide::HotKey(dword key)
{
	if(designer && designer->DesignerCtrl().HotKey(key))
		return true;
	return TopWindow::HotKey(key);
}

String Ide::IdeGetFileName()
{
	return editfile;
}

String Ide::IdeGetNestFolder()
{
	Vector<String> w = GetUppDirs();
	for(int i = 0; i < w.GetCount(); i++)
		if(editfile.StartsWith(w[i]))
			return w[i];
	return Null;
}
