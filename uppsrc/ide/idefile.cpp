#include "ide.h"

void Ide::SetupEditor(int f, String hl, String fn)
{
	String ext = ToLower(GetFileExt(fn));
	switch(f) {
	case 1:  editor.SetFont(font1); break;
	case 2:  editor.SetFont(font2); break;
	case 3:  editor.SetFont(consolefont); break;
	default: editor.SetFont(editorsplit.GetZoom() < 0 && editorsplit.IsHorz() ? veditorfont :
	                        ext == ".t" ? tfont : editorfont); break;
	}
	if(IsNull(hl)) {
		if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx" ||
		   ext == ".h" || ext == ".hpp" || ext == ".hh" || ext == ".hxx" || ext == ".icpp")
			editor.Highlight(CodeEditor::HIGHLIGHT_CPP);
		else
		if(ext == ".usc")
			editor.Highlight(CodeEditor::HIGHLIGHT_USC);
		else
		if(ext == ".java")
			editor.Highlight(CodeEditor::HIGHLIGHT_JAVA);
		else
		if(ext == ".t" || ext == ".jt")
			editor.Highlight(CodeEditor::HIGHLIGHT_T);
		else
		if(ext == ".sch")
			editor.Highlight(CodeEditor::HIGHLIGHT_SCH);
		else
		if(ext == ".sql")
			editor.Highlight(CodeEditor::HIGHLIGHT_SQL);
		else
			editor.Highlight(CodeEditor::HIGHLIGHT_NONE);
	}
	else {
		if(hl == "cpp")
			editor.Highlight(CodeEditor::HIGHLIGHT_CPP);
		else
		if(hl == "esc")
			editor.Highlight(CodeEditor::HIGHLIGHT_USC);
		else
		if(hl == "java")
			editor.Highlight(CodeEditor::HIGHLIGHT_JAVA);
		else
			editor.Highlight(CodeEditor::HIGHLIGHT_NONE);
	}
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
	d.highlight.Add("cpp", "C++");
	d.highlight.Add("java", "Java");
	d.highlight.Add("esc", "Esc");
	d.tabsize <<= f.tabsize > 0 ? f.tabsize : Null;
	d.tabsize <<= d.Breaker(111);
	d.tabsize.MinMax(1, 100);
	d.charset <<= (int)f.charset;
	d.font <<= f.font;
	d.font <<= d.Breaker(111);
	d.highlight <<= f.highlight;
	d.highlight <<= d.Breaker(111);
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
		CodeBaseScan(ss, editfile);
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
	if(designer) {
		String fn = designer->GetFileName();
		Time tm = FileGetTime(fn);
		designer->Save();
		if(tm != FileGetTime(fn))
			TouchFile(fn);
		if(IsProjectFile(fn) && ToUpper(GetFileExt(fn)) == ".LAY")
			CodeBaseScanLay(fn);
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
#ifdef PLATFORM_POSIX
	struct stat statbuf;
	int statcode = stat(editfile, &statbuf);
#endif
	TouchFile(editfile);
	String tmpfile = editfile + ".$tmp", outfile = editfile;
	{
		FileOut out(tmpfile);
		if(!out.IsOpen()) {
			Exclamation(NFormat("Error creating temporary file [* \1%s\1].", tmpfile));
			return;
		}
		if(designer)
			designer->Save();
		else
			if(GetFileExt(editfile) == ".t") {
				for(int i = 0; i < editor.GetLineCount(); i++) {
					if(i) out.PutCrLf();
					out.Put(ConvertTLine(editor.GetUtf8Line(i), ASCSTRING_OCTALHI));
				}
			}
			else
				editor.Save(out, editor.GetCharset());		
		out.Close();
		if(out.IsError()) {
			Exclamation(NFormat("Error writing temporary file [* \1%s\1].", tmpfile));
			return;
		}
	}

	Progress progress;
	int time = msecs();
	for(;;) {
		progress.SetTotal(10000);
		progress.SetText(NFormat("Saving '%s'", GetFileName(outfile)));
		if(editfile != outfile || !FileExists(tmpfile))
			return;
		FileDelete(outfile);
		if(FileMove(tmpfile, outfile))
			break;
		console.Flush();
		Sleep(200);
		if(progress.SetPosCanceled(msecs(time) % progress.GetTotal()))
		{
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
			progress.Reset();
		}
	}

#ifdef PLATFORM_POSIX
	if(statcode == 0)
		chmod(editfile, statbuf.st_mode);
#endif

	if(!designer) {
		FindFile ff(editfile);
		fd.filetime = edittime = ff.GetLastWriteTime();
	}

	if(editor.IsDirty())
		ScanFile();

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

void Ide::EditFile0(const String& path, byte charset, bool astext, const String& headername) {
	editor.CheckEdited(false);
	editor.CloseAssist();
	if(path.IsEmpty()) return;
	FlushFile();

	editfile = path;
	editor.SetCharset(charset);
	AddLru();
	
	if(!astext && !(debugger && (PathIsEqual(path, posfile[0]) || PathIsEqual(path, posfile[0])))
	   && editastext.Find(path) < 0 && !IsNestReadOnly(editfile)) {
		for(int i = 0; i < GetIdeModuleCount() && !designer; i++)
			designer = GetIdeModule(i).CreateDesigner(this, path, charset);
		if(designer) {
			editpane.Add(designer->DesignerCtrl().SizePos());
			designer->SetFocus();
			if(filetabs) {
				designer->DesignerCtrl().SetFrame(tabs);
				tabs.SetAddFile(editfile);
			}
			
			MakeTitle();
			SetBar();
			return;
		}
	}

	if(filetabs)
		editor.SetFrame(tabs);
	else
		editor.SetFrame(ViewFrame());
	tabs.SetAddFile(editfile);
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
		if(in)
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
			else
				editor.Load(in, charset);
		editor.SetEditPos(fd.editpos);
		if(!IsNull(fd.columnline) && fd.columnline.y >= 0 && fd.columnline.y < editor.GetLineCount())
			editor.SetCursor(editor.GetColumnLinePos(fd.columnline));
		editor.SetPickUndoData(fd.undodata);
		editor.SetLineInfo(fd.lineinfo);
		editor.SetLineInfoRem(fd.lineinforem);
		if(ff.IsReadOnly() || IsNestReadOnly(editfile))
			editor.SetReadOnly();
		fd.undodata.Clear();
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
	editor.assist_active = IsProjectFile(editfile) && (IsCSourceFile(editfile) || IsCHeaderFile(editfile));
	editor.CheckEdited(true);
	editor.Annotate(editfile);
	editor.SyncNavigator();
}

void Ide::EditAsText()
{
	String path = editfile;
	editastext.FindPut(editfile);
	byte cs = editor.GetCharset();
	FlushFile();
	EditFile0(path, cs);
}

void Ide::EditUsingDesigner()
{
	String path = editfile;
	editastext.RemoveKey(editfile);
	byte cs = editor.GetCharset();
	FlushFile();
	EditFile0(path, cs);
}

void Ide::AddEditFile(const String& path)
{
	actual.file.Add(path);
	if(IsAux())
		SaveLoadPackageNS();
	else
		SaveLoadPackage();
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

	if(IsCppBaseFile()) {
		FileIn in(editfile);
		CodeBaseScan(in, editfile);
	}
	ReloadFile();
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
	fs.Multi(false);
	if(!fs.ExecuteOpen()) return;
	EditFile(fs);
	FileSelected();
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
	QuickTabs h;
	h.Set(tabs);
	tabs.Set(tabs2);
	tabs2.Set(h);
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
		editor.SetLineInfoRem(fd.lineinforem);
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
	else
		tabs2.Set(tabs);
	if(horz)
		editorsplit.Horz(editor2, editor);
	else
		editorsplit.Vert(editor2, editor);
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
