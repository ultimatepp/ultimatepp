#include "ide.h"
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
	int version = 8;
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
	if(version >= 6) {
		Option dummy;
		s % dummy;
	}
	if(version >= 8) {
		s % ff.where;
	}
}

void SearchForFiles(Index<String>& files, String dir, String mask, int readonly, Time since, Progress& pi) {
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		if(ff.IsFolder() && *ff.GetName() != '.')
			SearchForFiles(files, AppendFileName(dir, ff.GetName()), mask, readonly, since, pi);
		else
		if(ff.IsFile() && PatternMatchMulti(mask, ff.GetName())) {
			if((IsNull(readonly) || !!readonly == !!ff.IsReadOnly()) &&
			   (IsNull(since) || ff.GetLastWriteTime() >= since)) {
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
	ListLineInfo f;
	f.file = fn;
	f.lineno = ln;
	f.linepos = pos + 1;
	f.len = count;
	f.kind = 0;
	f.line = line;
	f.message = "\1" + EditorSyntax::GetSyntaxForFilename(fn) + "\1" +
	            AsString(pos) + "\1" + AsString(count) + "\1" + (line.GetCount() > 5000 ? line.Mid(0, 5000) : line);
	FFound().Add(fn, ln, f.message, RawToValue(f));
}

bool Ide::SearchInFile(const String& fn, const String& pattern, bool wholeword, bool ignorecase, RegExp *regexp) {
	FileIn in(fn);
	if(!in) return true;
	int ln = 1;
	bool wb = wholeword ? iscid(*pattern) : false;
	bool we = wholeword ? iscid(*pattern.Last()) : false;
	int infile = 0;
	bool sync = false;
	while(!in.IsEof()) {
		String line = in.GetLine();
		bool bw = true;
		int  count;
		if(regexp) {
			const char *s = line;
			while(regexp->Match(s)) {
				AddFoundFile(fn, ln, line, regexp->GetOffset() + int(s - line), regexp->GetLength());
				sync = true;
				s += regexp->GetLength();
			}
		}
		else
			for(const char *s = line; *s; s++) {
				if(bw && Match(pattern, s, we, ignorecase, count)) {
					AddFoundFile(fn, ln, line, int(s - line), count);
					sync = true;
					infile++;
					s += count;
				}
				if(wb) bw = !iscid(*s);
			}
		ln++;
	}

	if(sync)
		FFound().Sync();

	in.Close();
	int ffs = ~ff.style;
	if(infile && ffs != STYLE_NO_REPLACE)
	{
		EditFile(fn);
		if(!editor.IsReadOnly()) {
			bool doit = true;
			if(ffs == STYLE_CONFIRM_REPLACE)
			{
				editor.SetCursor(0);
				editor.Find(false, true);
				switch(PromptYesNoCancel(Format("Replace %d lines in [* \1%s\1]?", infile, fn)))
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
				FFound().Add(fn, Null, AsString(infile) + " replacements made");
				FFound().Sync();
			}
		}
	}

	return true;
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
	WriteList(ff.find, d.find_list);
	WriteList(ff.replace, d.replace_list);
	ff.Sync();
	if(IsNull(~ff.folder))
		ff.folder <<= GetUppDir();
	ff.style <<= STYLE_NO_REPLACE;
	ff.Sync();
	ff.itext = editor.GetI();
	ff.Setup(replace);
	
	int c = ff.Execute();

	ff.find.AddHistory();
	ff.replace.AddHistory();

	rf.Retrieve();
	d.find_list = ReadList(ff.find);
	d.replace_list = ReadList(ff.replace);
	editor.SetFindReplaceData(d);
	
	if(c == IDOK) {
		SaveFile();

		NewFFound();

		FFound().HeaderTab(2).SetText("Source line");
		Renumber();
		ff.find.AddHistory();
		ff.files.AddHistory();
		ff.folder.AddHistory();
		ff.replace.AddHistory();
		Progress pi("Found %d files to search.");
		pi.AlignText(ALIGN_LEFT);
		Index<String> files;
		Time since = Null;
		if(!IsNull(ff.recent))
			since = ToTime(GetSysDate() - (int)~ff.recent);
		int where = ~ff.where;
		Vector<String> dirs;
		if(where == 0) {
			const Workspace& wspc = GetIdeWorkspace();
			for(int i = 0; i < wspc.GetCount(); i++)
				dirs << PackageDirectory(wspc[i]);
		}
		if(where == 1)
			dirs.Append(GetUppDirsRaw());
		if(where == 2 || where == 4)
			for(String h : GetAllNests())
				dirs << NormalizePath(h);
		if(where == 3 || where == 4)
			dirs.Append(Split(GetExternalIncludePath(), ';'));
		if(where == 5)
			dirs << NormalizePath(~~ff.folder, GetUppDir());

		for(String d : dirs)
			SearchForFiles(files, d, ~ff.files, ~ff.readonly, since, pi);

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
			FFound().Clear();
			pi.SetPos(0);
			for(int i = 0; i < files.GetCount(); i++) {
				pi.SetText(files[i]);
				if(pi.StepCanceled()) break;
				if(!IsNull(pattern)) {
					if(!SearchInFile(files[i], pattern, ff.wholeword, ff.ignorecase, regexp))
						break;
				}
				else {
					ListLineInfo f;
					f.file = files[i];
					f.lineno = 1;
					f.linepos = 0;
					f.kind = 0;
					f.message = files[i];
					FFound().Add(f.file, 1, f.message, RawToValue(f));
					FFound().Sync();
				}
			}
			FFoundFinish(!IsNull(pattern));
		}
	}
}

void Ide::FFoundSetIcon(const Image& m)
{
	FFound();
	ffound[0]->icon = m;
}

void Ide::FFoundFinish(bool replace)
{
	ArrayCtrl& ff = FFound();
	FFound().HeaderTab(2).SetText(Format("Source line (%d)", ff.GetCount()));
	int ii = btabs.GetCursor();
	ffound[0]->freplace.Show(ff.GetCount());
	BTabs(); // to update the found text
	btabs.SetCursor(ii);
}

void Ide::FindFileAll(const Vector<Tuple<int64, int>>& f)
{
	NewFFound();
	for(auto pos : f) {
		editor.CachePos(pos.a);
		int linei = editor.GetLinePos64(pos.a);
		WString ln = editor.GetWLine(linei);
		AddFoundFile(editfile, linei + 1, ln.ToString(), Utf8Len(~ln, (int)pos.a), Utf8Len(~ln + pos.a, pos.b));
	}
	FFoundFinish();
}
	
void Ide::FindString(bool back)
{
	if(!editor.FindString(back))
		BeepMuteExclamation();
}

void Ide::TranslateString()
{
	if(editor.IsReadOnly()) return;
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
	String hub = GetHubDir();
	Vector<String> pd;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		pd.Add(PackageDirectory(wspc[i]));
	for(String d : GetUppDirs())
		if(!IsHubDir(d) || FindMatch(pd, [&](const String& q) { return q.StartsWith(d); }) >= 0)
			menu.Add(d, [=] { FindSetStdDir(d); });
	menu.Add(GetHubDir(), [=] { FindSetStdDir(GetHubDir()); });
	menu.Execute(&ff.folder, ff.folder.GetPushScreenRect().BottomLeft());
}

void Ide::FindFolder()
{
	if(!sSD().ExecuteSelectDir()) return;
	ff.folder <<= ~sSD();
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
	ff.ignorecase << [=] { ff.Sync(); };
	ff.samecase <<= true;
	ff.Sync();
}

void FindInFilesDlg::Sync()
{
	replace.Enable((int)~style);
	bool b = !regexp;
	wildcards.Enable(b);
	ignorecase.Enable(b);
	wholeword.Enable(b);
	folder.Enable((int)~where == 5);
	samecase.Enable(ignorecase && b);
}

FindInFilesDlg::FindInFilesDlg()
{
	regexp <<= style <<= THISBACK(Sync);
	readonly.Add(Null, "All files");
	readonly.Add(0, "Writable");
	readonly.Add(1, "Read only");
	readonly <<= Null;
	recent <<= Null;
	recent.Add(Null, "All");
	recent.Add(1, "1 Day");
	recent.Add(3, "3 Days");
	recent.Add(7, "7 Days");
	recent.Add(14, "14 Days");
	recent.Add(32, "28 Days");
	where <<= THISBACK(Sync);
	where <<= 0;
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

void SetupError(ArrayCtrl& error, const char *s);

Ide::FoundList::FoundList()
{
	SetupError(*this, "Source");
	ColumnWidths("207 41 834");
	ColumnAt(0).SetDisplay(Single<FoundFileDisplay>());
	ColumnAt(2).SetDisplay(Single<FoundDisplay>());
	WhenBar = [=](Bar& bar) { TheIde()->FFoundMenu(*this, bar); };
	WhenLeftClick = WhenSel = [=] { TheIde()->ShowFound(*this); };
	freplace.SetLabel("Replace");
	HeaderObject() << freplace.RightPosZ(0, 80).VSizePos();
	freplace.Hide();
	freplace << [=] { TheIde()->ReplaceFound(*this); };
	freplace.SetImage(IdeImg::textfield_rename());
}

void Ide::NewFFound()
{
	if(ffound[0] && ffound[0]->GetCount())
		for(int i = __countof(ffound) - 1; i > 0; i--)
			ffound[i] = pick(ffound[i - 1]);
	FFound();
	SetBottom(BFINDINFILES1);
}
	
ArrayCtrl& Ide::FFound()
{
	if(!ffound[0])
		ffound[0].Create<FoundList>();
	return *ffound[0];
}

void Ide::CopyFound(ArrayCtrl& list, bool all)
{
	String txt;
	for(int i = 0; i < list.GetCount(); i++) {
		if(all)
			txt << list.Get(i, 0) << " (" << list.Get(i, 1) << "): ";
		String h = list.Get(i, 2);
		if(*h == '\1')
			h = Split(~h + 1, '\1', false).Top();
		txt << h << "\r\n";
	}
	WriteClipboardText(txt);
}

void Ide::FFoundMenu(ArrayCtrl& list, Bar& bar)
{
	ArrayCtrl *l = &list;
	bar.Add("Copy text", [=] { CopyFound(*l, false); });
	bar.Add("Copy all", [=] { CopyFound(*l, true); });
}

INITBLOCK {
	RegisterGlobalConfig("Ide::ReplaceFound");
}

void Ide::ReplaceFound(ArrayCtrl& list)
{
	Index<String> ch;
	for(int i = 0; i < list.GetCount() && ch.GetCount() < 6; i++) {
		Value v = list.Get(i, "INFO");
		if(v.Is<ListLineInfo>()) {
			const ListLineInfo& f = ValueTo<ListLineInfo>(v);
			if(*f.message == '\1') {
				Vector<String> h = Split(~f.message + 1, '\1', false);
				if(h.GetCount() > 3 && f.linepos > 0)
					ch.FindAdd(h[3].Mid(f.linepos - 1, f.len));
			}
		}
	}

	WithReplaceResultsLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Replace found items");
	LoadFromGlobal([&](Stream& s) { dlg.text.SerializeList(s); }, "Ide::ReplaceFound");
	for(String s : ch)
		dlg.text.FindAddList(s);
	if(ch.GetCount())
		dlg.text <<= ch[0];
	int c = dlg.Execute();
	dlg.text.AddHistory();
	StoreToGlobal([&](Stream& s) { dlg.text.SerializeList(s); }, "Ide::ReplaceFound");
	if(c != IDOK)
		return;

	String replace = ~dlg.text;

	replace_in_files = true; // allow .lay etc... to be edited as text, do not update things

	String fn = editfile;
	AssistEditor curtain; // to prevent editor visual changes while doing the work
	PassEditor(curtain);
	FlushFile();
	editor_p.Add(curtain.SizePos());

	Index<String> errors;
	// file - line - [origline, [pos, len, listi, replaced]]
	VectorMap<String, VectorMap<int, Tuple<String, Vector<Tuple<int, int, int>>, bool>>> files;
	for(int i = 0; i < list.GetCount(); i++) {
		Value v = list.Get(i, "INFO");
		bool err = true;
		if(v.Is<ListLineInfo>()) {
			const ListLineInfo& f = ValueTo<ListLineInfo>(v);
			if(*f.message == '\1') {
				Vector<String> h = Split(~f.message + 1, '\1', false);
				if(h.GetCount() > 3) {
					if(f.linepos > 0) { // some lines are ignored
						auto& x = files.GetAdd(NormalizePath(f.file)).GetAdd(f.lineno - 1);
						x.a = h[3];
						x.b.Add(MakeTuple(f.linepos - 1, f.len, i));
						x.c = false;
					}
					err = false;
				}
			}
		}
		if(err)
			errors.FindAdd(~list.Get(i, 0) + " " + ~list.Get(i, 1));
	}
	
	Progress pi;
	pi.SetTotal(files.GetCount());
	for(const auto& file : ~files) {
		pi.SetText(GetFileName(file.key));
		if(pi.StepCanceled())
			break;
		LoadFileSilent(file.key);
		editor.NextUndo();
		editor.MoveHome();
		for(int li = 0; li < file.value.GetCount(); li++) {
			file.value[li].c = true; // this file was replaced (not canceled)
			bool err = false;
			String& orig = file.value[li].a;
			String replaced = orig;
			Sort(file.value[li].b, [](const auto& a, const auto& b) { return a.a < b.a; });
			int pos = 0;
			for(auto& r : file.value[li].b) {
				int p = r.a + pos;
				if(p >= 0 && p + r.b < replaced.GetCount()) {
					replaced.Remove(p, r.b);
					replaced.Insert(p, replace);
					r.a = p;
					pos += replace.GetCount() - r.b;
					r.b = replace.GetCount();
				}
				else
					err = true;
			}
			int linei = file.value.GetKey(li);
			if(!err && editor.GetUtf8Line(linei) == orig && !editor.IsReadOnly() && !editor.IsView()) {
				int pos = editor.GetPos(linei);
				editor.Remove(pos, editor.GetLineLength(linei));
				editor.Insert(pos, replaced);
				
			}
			else
				err = true;
			if(err) {
				errors.FindAdd(String() << file.key << " " << linei + 1);
				for(auto& r : file.value[li].b)
					r.c = -1; // do not change this line in the results list
			}
			else
				orig = replaced;
		}
		editor.MoveEnd();
	}

	replace_in_files = false;

	EditFile(fn);

	if(errors.GetCount())
		Exclamation("Some instances could not be replaced:&&\1" + Join(errors.GetKeys(), "\n"));

	for(const auto& file : ~files)
		for(const auto& line : ~file.value)
			if(line.value.c) // file was replaced
				for(const auto& r : line.value.b) {
					int i = r.c; // source line in the list
					if(i >= 0 && i < list.GetCount()) {
						Value v = list.Get(i, "INFO");
						if(v.Is<ListLineInfo>()) {
							const ListLineInfo& f0 = ValueTo<ListLineInfo>(v);
							ListLineInfo f = f0;
							if(*f.message == '\1') {
								String l = line.value.a;
								f.linepos = r.a + 1;
								f.len = r.b;
								f.message = "\1" + EditorSyntax::GetSyntaxForFilename(f.file) + "\1" +
		                                    AsString(r.a) + "\1" + AsString(r.b) + "\1" +
		                                    (l.GetCount() > 5000 ? l.Mid(0, 5000) : l);
								list.Set(i, 2, f.message);
								list.Set(i, 3, RawToValue(f));
							}
						}
					}
				}
}
