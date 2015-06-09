#include "ide.h"

FileSel& sSD()
{
	static bool b;
	static FileSel fs;
	if(!b) {
		fs.AllFilesType();
		b = true;
	}
	return fs;
}

void Ide::SerializeFindInFiles(Stream& s) {
	int version = 7;
	s / version;
	s % ff.files;
	ff.files.SerializeList(s);
	s % ff.folder;
	ff.folder.SerializeList(s);
	if(version >= 2)
	{
		s % ff.replace;
		ff.replace.SerializeList(s);
		s % ff.style;
	}
	if(version >= 1)
		s % sSD();
	if(version >= 3 && version < 7) {
		DropList dummy;
		s % dummy;
	}

	if(version >= 4)
		s % ff.samecase;
	if(version >= 5)
		s % ff.regexp;
	if(version >= 6)
		s % ff.workspace;
}

void SearchForFiles(Index<String>& files, String dir, String mask, int readonly, Progress& pi) {
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		if(ff.IsFolder() && *ff.GetName() != '.')
			SearchForFiles(files, AppendFileName(dir, ff.GetName()), mask, readonly, pi);
		else
		if(ff.IsFile() && PatternMatchMulti(mask, ff.GetName())) {
			if(IsNull(readonly) || !!readonly == !!ff.IsReadOnly()) {
				if(pi.StepCanceled()) return;
				files.FindAdd(AppendFileName(dir, ff.GetName()));
			}
		}
		ff.Next();
	}
}

enum {
	WILDANY = 16,
	WILDONE,
	WILDSPACE,
	WILDNUMBER,
	WILDID,
};

bool Match(const char *f, const char *s, bool we, bool ignorecase, int& count) {
	const char *b = s;
	while(*f) {
		if(*f == WILDANY) {
			f++;
			for(;;) {
				if(Match(f, s, we, ignorecase, count)) {
					count += int(s - b);
					return true;
				}
				if(!*s++) break;
			}
			return false;
		}
		else
		if(*f == WILDONE) {
			if(!*s++) return false;
		}
		else
		if(*f == WILDSPACE) {
			if(*s != ' ' && *s != '\t') return false;
			s++;
			while(*s == ' ' || *s == '\t')
				s++;
		}
		else
		if(*f == WILDNUMBER) {
			if(*s < '0' || *s > '9') return false;
			s++;
			while(*s >= '0' && *s <= '9')
				s++;
		}
		else
		if(*f == WILDID) {
			if(!iscib(*s)) return false;
			s++;
			while(iscid(*s)) s++;
		}
		else {
			if(ignorecase ? ToUpper(*s) != ToUpper(*f) : *s != *f) return false;
			s++;
		}
		f++;
	}
	count = int(s - b);
	return we && iscid(*s) ? false : true;
}

void Ide::AddFoundFile(const String& fn, int ln, const String& line, int pos, int count)
{
	ErrorInfo f;
	f.file = fn;
	f.lineno = ln;
	f.linepos = 0;
	f.kind = 0;
	f.message = "\1" + EditorSyntax::GetSyntaxForFilename(fn) + "\1" +
	            AsString(pos) + "\1" + AsString(count) + "\1" + line;
	ffound.Add(fn, ln, f.message, RawToValue(f));
	ffound.Sync();
}

bool Ide::SearchInFile(const String& fn, const String& pattern, bool wholeword, bool ignorecase,
                       int& n, RegExp *regexp) {
	FileIn in(fn);
	if(!in) return true;
	int ln = 1;
	bool wb = wholeword ? iscid(*pattern) : false;
	bool we = wholeword ? iscid(*pattern.Last()) : false;
	int infile = 0;
	while(!in.IsEof()) {
		String line = in.GetLine();
		bool bw = true;
		int  count;
		if(regexp) {
			if(regexp->Match(line))
				AddFoundFile(fn, ln, line, regexp->GetOffset(), regexp->GetLength());
		}
		else
			for(const char *s = line; *s; s++) {
				if(bw && Match(pattern, s, we, ignorecase, count)) {
					AddFoundFile(fn, ln, line, int(s - line), count);
					infile++;
					n++;
					break;
				}
				if(wb) bw = !iscid(*s);
			}
		ln++;
	}

	in.Close();
	int ffs = ~ff.style;
	if(infile && ffs != STYLE_NO_REPLACE)
	{
		EditFile(fn);
		bool doit = true;
		if(ffs == STYLE_CONFIRM_REPLACE)
		{
			editor.SetCursor(0);
			editor.Find(false, true);
			switch(PromptYesNoCancel(NFormat("Replace %d lines in [* \1%s\1]?", infile, fn)))
			{
			case 1:  break;
			case 0:  doit = false; break;
			case -1: return false;
			}
		}
		if(doit)
		{
			editor.SelectAll();
			editor.BlockReplace();
			SaveFile();
			ffound.Add(fn, Null, AsString(infile) + " replacements made");
			ffound.Sync();
		}
	}

	return true;
}

int CharFilterFindFileMask(int c)
{
	return ToUpper(ToAscii(c));
}

void Ide::FindFileName() {
	const Workspace& wspc = IdeWorkspace();

	WithFindFileLayout<TopWindow> ffdlg;
	CtrlLayoutOKCancel(ffdlg, "Find file");
	ffdlg.list.AutoHideSb();
	ffdlg.list.AddColumn("Package");
	ffdlg.list.AddColumn("File");
	ffdlg.list.WhenLeftDouble = ffdlg.Acceptor(IDOK);
	ffdlg.mask.NullText("Search");
	ffdlg.mask.SetText(find_file_search_string);
	ffdlg.mask.SelectAll();
	ffdlg.mask.SetFilter(CharFilterFindFileMask);
	ffdlg.mask <<= ffdlg.Breaker(IDYES);
	for(;;) {
		ffdlg.list.Clear();
		String mask = ~ffdlg.mask;
		for(int p = 0; p < wspc.GetCount(); p++) {
			String packname = wspc[p];
			const Package& pack = wspc.GetPackage(p);
			for(int f = 0; f < pack.file.GetCount(); f++) {
				String fn = pack.file[f];
				if(ToUpper(packname).Find(mask) >= 0 || ToUpper(fn).Find(mask) >= 0)
					ffdlg.list.Add(packname, pack.file[f]);
			}
		}
		if(ffdlg.list.GetCount() > 0)
			ffdlg.list.SetCursor(0);
		switch(ffdlg.Run()) {
		case IDCANCEL:
			return;
		case IDOK:
			if(ffdlg.list.IsCursor()) {
				find_file_search_string = ffdlg.mask;
				EditFile(SourcePath(ffdlg.list.Get(0),  ffdlg.list.Get(1)));
				return;
			}
			break;
		case IDYES:
			break;
		}
	}
}

void Ide::FindInFiles(bool replace) {
	CodeEditor::FindReplaceData d = editor.GetFindReplaceData();
	CtrlRetriever rf;
	rf(ff.find, d.find)
	  (ff.replace, d.replace)
	  (ff.ignorecase, d.ignorecase)
	  (ff.samecase, d.samecase)
	  (ff.wholeword, d.wholeword)
	  (ff.wildcards, d.wildcards)
	  (ff.regexp, d.regexp)
	;
	ff.Sync();
	if(String(ff.folder).IsEmpty())
		ff.folder <<= GetUppDir();
	ff.style <<= STYLE_NO_REPLACE;
	ff.Sync();
	ff.itext = editor.GetI();
	ff.Setup(replace);	
	
	int c = ff.Execute();

	rf.Retrieve();
	editor.SetFindReplaceData(d);
	

	if(c == IDOK) {
		ffound.HeaderTab(2).SetText("Source line");
		Renumber();
		ff.find.AddHistory();
		ff.files.AddHistory();
		ff.folder.AddHistory();
		ff.replace.AddHistory();
		Progress pi("Found %d files to search.");
		pi.AlignText(ALIGN_LEFT);
		Index<String> files;
		if(ff.workspace) {
			const Workspace& wspc = GetIdeWorkspace();
			for(int i = 0; i < wspc.GetCount(); i++)
				SearchForFiles(files, GetFileFolder(PackagePath(wspc[i])),
					           ~ff.files, ~ff.readonly, pi);
		}
		else
			SearchForFiles(files, NormalizePath((String)ff.folder, GetUppDir()),
				           ~ff.files, ~ff.readonly, pi);
		if(!pi.Canceled()) {
			String pattern;
			RegExp rx, *regexp = NULL;
			if(ff.regexp) {
				rx.SetPattern(~ff.find);
				regexp = &rx;
				pattern = "dummy";
			}
			else
			if(ff.wildcards) {
				String q = ~ff.find;
				for(const char *s = q; *s; s++)
					if(*s == '\\') {
						s++;
						if(*s == '\0') break;
						pattern.Cat(*s);
					}
					else
					switch(*s) {
					case '*': pattern.Cat(WILDANY); break;
					case '?': pattern.Cat(WILDONE); break;
					case '%': pattern.Cat(WILDSPACE); break;
					case '#': pattern.Cat(WILDNUMBER); break;
					case '$': pattern.Cat(WILDID); break;
					default:  pattern.Cat(*s);
					}
			}
			else
				pattern = ~ff.find;
			pi.SetTotal(files.GetCount());
			ShowConsole2();
			ffound.Clear();
			pi.SetPos(0);
			int n = 0;
			for(int i = 0; i < files.GetCount(); i++) {
				pi.SetText(files[i]);
				if(pi.StepCanceled()) break;
				if(!IsNull(pattern)) {
					if(!SearchInFile(files[i], pattern, ff.wholeword, ff.ignorecase, n, regexp))
						break;
				}
				else {
					ErrorInfo f;
					f.file = files[i];
					f.lineno = 1;
					f.linepos = 0;
					f.kind = 0;
					f.message = files[i];
					ffound.Add(f.file, 1, f.message, RawToValue(f));
					ffound.Sync();
					n++;
				}
			}
			if(!IsNull(pattern))
				ffound.Add(Null, Null, AsString(n) + " occurrence(s) have been found.");
			else
				ffound.Add(Null, Null, AsString(n) + "  matching file(s) have been found.");
			ffound.HeaderTab(2).SetText(Format("Source line (%d)", ffound.GetCount()));
		}
	}
}

void Ide::FindString(bool back)
{
	if(!editor.FindString(back))
		BeepMuteExclamation();
}

void Ide::TranslateString()
{
	int l, h;
	if(editor.GetSelection(l, h)) {
		editor.Insert(l, "t_(");
		editor.Insert(h + 3, ")");
		editor.SetCursor(h + 4);
		FindString(false);
	}
}

void Ide::InsertWildcard(const char *s) {
	iwc = s;
}

void Ide::FindWildcard() {
	int l, h;
	ff.find.GetSelection(l, h);
	iwc = 0;
	FindWildcardMenu(THISBACK(InsertWildcard), ff.find.GetPushScreenRect().TopRight(), false, NULL, ff.regexp);
	if(iwc.GetCount()) {
		ff.wildcards = true;
		ff.find.SetFocus();
		ff.find.SetSelection(l, h);
		ff.find.RemoveSelection();
		ff.find.Insert(iwc);
	}
}

void Ide::FindSetStdDir(String n)
{
	ff.folder <<= n;
}

void Ide::FindStdDir()
{
	String n = GetFileFolder(editfile);
	MenuBar menu;
	if(!IsNull(n))
		menu.Add(n, THISBACK1(FindSetStdDir, n));
	Vector<String> d = GetUppDirs();
	for(int i = 0; i < d.GetCount(); i++)
		menu.Add(d[i], THISBACK1(FindSetStdDir, d[i]));
	menu.Execute(&ff.folder, ff.folder.GetPushScreenRect().BottomLeft());
}

void Ide::FindFolder()
{
	if(!sSD().ExecuteSelectDir()) return;
	ff.folder <<= ~sSD();
}

void Ide::SyncFindInFiles()
{
	ff.samecase.Enable(ff.ignorecase);
}

void Ide::ConstructFindInFiles() {
	ff.find.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Wildcard") <<= THISBACK(FindWildcard);
	static const char *defs = "*.cpp *.h *.hpp *.c *.m *.C *.M *.cxx *.cc *.mm *.MM *.icpp *.sch *.lay *.rc";
	ff.files <<= String(defs);
	ff.files.AddList((String)defs);
	ff.files.AddList((String)"*.txt");
	ff.files.AddList((String)"*.*");
	ff.folder.AddButton().SetMonoImage(CtrlImg::smalldown()).Tip("Related folders") <<= THISBACK(FindStdDir);
	ff.folder.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Select folder") <<= THISBACK(FindFolder);
	editor.PutI(ff.find);
	editor.PutI(ff.replace);
	CtrlLayoutOKCancel(ff, "Find In Files");
	ff.ignorecase <<= THISBACK(SyncFindInFiles);
	ff.samecase <<= true;
	SyncFindInFiles();
}

void FindInFilesDlg::Sync()
{
	replace.Enable((int)~style);
	bool b = !regexp;
	wildcards.Enable(b);
	ignorecase.Enable(b);
	wholeword.Enable(b);
	folder.Enable(!workspace);
	folder_lbl.Enable(!workspace);
}

FindInFilesDlg::FindInFilesDlg()
{
	regexp <<= style <<= THISBACK(Sync);
	readonly.Add(Null, "All files");
	readonly.Add(0, "Writable");
	readonly.Add(1, "Read only");
	readonly <<= Null;
	workspace <<= THISBACK(Sync);
}

void FindInFilesDlg::Setup(bool replacing)
{
	Title(replacing ? "Find and replace in files" : "Find in files");
	replace_lbl.Show(replacing);
	style.Show(replacing);
	replace_lbl2.Show(replacing);
	replace.Show(replacing);
	Size sz = GetLayoutSize();
	if(!replacing)
		sz.cy -= replace.GetRect().bottom - folder.GetRect().bottom;
	Rect r = GetRect();
	r.SetSize(sz);
	SetRect(r);
	ActiveFocus(find);
}

bool FindInFilesDlg::Key(dword key, int count)
{
	if(key == K_CTRL_I) {
		if(find.HasFocus()) {
			find <<= itext;
			return true;
		}
		if(replace.HasFocus()) {
			replace <<= itext;
			return true;
		}
	}
	return TopWindow::Key(key, count);
}
