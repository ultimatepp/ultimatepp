#include "ide.h"

void Ide::SerializeFf(Stream& s) {
	s % ff.find;
	ff.find.SerializeList(s);
	s % ff.wholeword % ff.ignorecase % ff.wildcards;
	s % ff.replace;
	ff.replace.SerializeList(s);
}

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
	int version = 2;
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
}

void SearchForFiles(Vector<String>& files, String dir, String mask, Progress& pi) {
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		if(ff.IsFolder())
			SearchForFiles(files, AppendFileName(dir, ff.GetName()), mask, pi);
		else
		if(ff.IsFile() && PatternMatchMulti(mask, ff.GetName())) {
			if(pi.StepCanceled()) return;
			files.Add(AppendFileName(dir, ff.GetName()));
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

bool Match(const char *f, const char *s, bool we, bool ignorecase) {
	while(*f) {
		if(*f == WILDANY) {
			f++;
			for(;;) {
				if(Match(f, s, we, ignorecase))
					return true;
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
	return we && iscid(*s) ? false : true;
}

bool Ide::SearchInFile(const String& fn, const String& pattern, bool wholeword, bool ignorecase,
					   int& n) {
	FileIn in(fn);
	if(!in) return true;
	int ln = 1;
	bool wb = wholeword ? iscid(*pattern) : false;
	bool we = wholeword ? iscid(*pattern.Last()) : false;
	int infile = 0;
	while(!in.IsEof()) {
		String line = in.GetLine();
		bool bw = true;
		for(const char *s = line; *s; s++) {
			if(bw && Match(pattern, s, we, ignorecase)) {
				console << fn << Format("(%d):", ln) << line << "\n";
				console.Sync();
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
			switch(PromptYesNoCancel(NFormat("Replace %d lines in %s?", infile, fn)))
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
			console << NFormat("%s: %d replacements made\n", fn, infile);
			console.Sync();
		}
	}

	return true;
}

static bool RawFileMatch(const char *pattern, const char *file, const char *& endptr)
{
	for(char c; (c = *pattern++) != 0;)
		if(c == '*')
		{
			do
				if(RawFileMatch(pattern, file, endptr))
					return true;
			while(*file++);
			endptr = pattern;
			return false;
		}
		else if(c == '?')
		{
			if(*file++ == 0)
			{
				endptr = pattern - 1;
				return false;
			}
		}
		else if(ToLower(c) != ToLower(*file++))
		{
			endptr = pattern - 1;
			return false;
		}
	endptr = file;
	return true;
}

static const char *FindFileMatch(const char *pattern, const char *file, String& cont)
{
	const char *endptr;
	if(!RawFileMatch(pattern, file, endptr))
		return endptr;
	if(cont.IsVoid())
	{
		cont = endptr;
		return NULL;
	}
	const char *p = cont;
	while(*p && ToLower(*p) == ToLower(*endptr++))
		p++;
	cont.Trim(int(p - cont.Begin()));
	return NULL;
}

void Ide::FindFileName() {
	const Workspace& wspc = IdeWorkspace();

	WithFindFileLayout<TopWindow> ffdlg;
	CtrlLayoutOKCancel(ffdlg, "Find file");
	ffdlg.list.AutoHideSb();
	ffdlg.list.AddColumn("Package");
	ffdlg.list.AddColumn("File");
	ffdlg.list.WhenLeftDouble = ffdlg.Acceptor(IDOK);
	ffdlg.mask <<= ffdlg.Breaker(IDYES);
	int prev = 0;
	for(;;)
	{
		ffdlg.list.Clear();
		String mask = ffdlg.mask.GetText().ToString();
		String rem = String::GetVoid();
		bool simple = (mask.Find('/') < 0 && mask.Find('\\') < 0);
		const char *best_err = NULL;
		for(int p = 0; p < wspc.GetCount(); p++) {
			String packname = wspc[p];
			const Package& pack = wspc.GetPackage(p);
			for(int f = 0; f < pack.file.GetCount(); f++) {
				String fn = pack.file[f];
				String rr = rem;
				const char *errptr = FindFileMatch(mask, fn, rr);
				if(errptr && simple) {
					const char *b = fn.Begin(), *e = fn.End();
					while(e > b && e[-1] != '\\' && e[-1] != '/')
						e--;
					if(b != e) {
						rr = rem;
						errptr = FindFileMatch(mask, e, rr);
					}
				}
				if(!errptr) {
					rem = rr;
					ffdlg.list.Add(packname, pack.file[f]);
				}
				else if(!best_err || errptr > best_err)
					best_err = errptr;
			}
		}
		if(ffdlg.list.GetCount() == 0) {
			int px = int(best_err - mask.Begin());
			ffdlg.mask.SetSelection(px, ffdlg.mask.GetLength());
			BeepMuteExclamation();
		}
		else if(!rem.IsEmpty() && prev < mask.GetLength()) {
			int l, h;
			ffdlg.mask.GetSelection(l, h);
			if(l == mask.GetLength()) {
				ffdlg.mask.SetText((mask + rem).ToWString());
				ffdlg.mask.SetSelection(l, l + rem.GetLength());
			}
		}
		prev = mask.GetLength();
		if(ffdlg.list.GetCount() > 0)
			ffdlg.list.SetCursor(0);

		switch(ffdlg.Run()) {
		case IDCANCEL:
			return;
		case IDOK:
			if(ffdlg.list.IsCursor()) {
				EditFile(SourcePath(ffdlg.list.Get(0),  ffdlg.list.Get(1)));
				return;
			}
			break;
		case IDYES:
			break;
		}
	}
}

void Ide::FindInFiles() {
	StringStream ss;
	editor.SerializeFind(ss);
	ss.Open(ss.GetResult());
	SerializeFf(ss);
	if(String(ff.folder).IsEmpty())
		ff.folder <<= GetUppDir();
	ff.style <<= STYLE_NO_REPLACE;
	ff.itext = editor.GetI();
	int c = ff.Execute();
	ss.Create();
	SerializeFf(ss);
	ss.Open(ss.GetResult());
	editor.SerializeFind(ss);
	if(c == IDOK && !String(ff.find).IsEmpty()) {
		Renumber();
		ff.find.AddHistory();
		ff.files.AddHistory();
		ff.folder.AddHistory();
		ff.replace.AddHistory();

		Progress pi("Found %d files to search.");
		Vector<String> files;
		SearchForFiles(files, NormalizePath((String)ff.folder, GetUppDir()), ~ff.files, pi);
		if(!pi.Canceled()) {
			String pattern;
			if(ff.wildcards) {
				String q = ~ff.find;
				for(const char *s = q; *s; s++)
					if(*s == '\\') {
						s++;
						if(*s == '\0') break;
						q.Cat(*s);
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
			ShowConsole();
			console.Clear();
			pi.SetPos(0);
			int n = 0;
			for(int i = 0; i < files.GetCount(); i++) {
				pi.SetText(files[i]);
				if(pi.StepCanceled()) break;
				if(!SearchInFile(files[i], pattern, ff.wholeword, ff.ignorecase, n))
					break;
			}
			console << Format("%d occurrence(s) have been found.\n", n);
		}
	}
	SetErrorEditor();
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

void Ide::InsertWildcard(int c) {
	iwc = c;
}

void Ide::FindWildcard() {
	int l, h;
	ff.find.GetSelection(l, h);
	iwc = 0;
	FindWildcardMenu(THISBACK(InsertWildcard), ff.find.GetPushScreenRect().TopRight(), false);
	if(iwc) {
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

void Ide::ConstructFindInFiles() {
	ff.find.AddButton().SetMonoImage(CtrlImg::smallright()) <<= THISBACK(FindWildcard);
	ff.files <<= String("*.cpp *.h *.hpp *.c *.C *.cxx *.cc");
	ff.files.AddList((String)"*.cpp *.h *.hpp *.c *.C *.cxx *.cc");
	ff.files.AddList((String)"*.txt");
	ff.files.AddList((String)"*.*");
	ff.folder.AddButton().SetMonoImage(CtrlImg::smalldown()) <<= THISBACK(FindStdDir);
	ff.folder.AddButton().SetMonoImage(CtrlImg::smallright()) <<= THISBACK(FindFolder);
	editor.PutI(ff.find);
	editor.PutI(ff.replace);
	CtrlLayoutOKCancel(ff, "Find In Files");
}

bool Ide::FindInFiles::Key(dword key, int count)
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
