#include "ide.h"

struct FileLine : Moveable<FileLine> {
	String file;
	int    line;
};

struct LngEntry : Moveable<LngEntry>, DeepCopyOption<LngEntry> {
	bool                   added;
	VectorMap<int, String> text;
	Vector<FileLine>       fileline;

	LngEntry(const LngEntry& e, int) { added = e.added; text <<= e.text; fileline <<= e.fileline; }
	LngEntry() {}

	void AddFileLine(CParser& p) {
		FileLine& fl = fileline.Add();
		fl.file = p.GetFileName();
		fl.line = p.GetLine();
	}
};

struct TFile : Moveable<TFile> {
	bool                        dirty;
	bool                        java;
	String                      package, file;
	VectorMap<String, LngEntry> map;
	Vector<int>                 ls;

	void MakeLS() {
		Index<int> lngset;
		lngset.Add(LNG_enUS);
		for(int i = 0; i < map.GetCount(); i++)
			for(int j = 0; j < map[i].text.GetCount(); j++)
				lngset.FindAdd(map[i].text.GetKey(j));
		ls = lngset.PickKeys();
		Sort(ls);
	}

	rval_default(TFile);
	TFile() { dirty = false; }
};

void t_error(CParser& p)
{
	PutConsole(p.GetFileName() + Format("(%d): t_/tt_ works only with plain text literals", p.GetLine()));
}

void LngParseCFile(const String& fn, VectorMap<String, LngEntry>& lng)
{
	String data = LoadFile(fn);
	CParser p(data, fn);
	try {
		while(!p.IsEof()) {
			if((p.Id("t_") || p.Id("tt_")) && p.Char('('))
				if(p.IsString()) {
					String tid = p.ReadString();
					LngEntry& le = lng.GetAdd(tid);
					le.text.GetAdd(LNG_enUS) = GetENUS(tid);
					le.AddFileLine(p);
					le.added = true;
					if(!p.Char(')'))
						t_error(p);
				}
				else
					t_error(p);
			else
				p.SkipTerm();
		}
	}
	catch(CParser::Error e) {
		PutConsole(e);
		ShowConsole();
	}
}

const Index<int>& LngIndex()
{
	static Index<int> l;
	if(l.GetCount() == 0) {
		const int *x = GetAllLanguages();
		while(*x)
			l.Add(*x++);
	}
	return l;
}

bool LngParseTFile(const String& fn, VectorMap<String, LngEntry>& lng)
{
	String data = LoadFile(fn);
	CParser p(data, fn);
	try {
		if(p.Char('#'))
			while(!p.IsEof()) {
				if(p.IsChar2('T', '_'))
					break;
				else
					p.SkipTerm();
			}
		String id;
		while(!p.IsEof()) {
			if(p.Id("T_")) {
				p.PassChar('(');
				id = Null;
				do
					id.Cat(p.ReadString());
				while(p.Char('+'));
				p.PassChar(')');
				lng.GetAdd(id).added = false;
			}
			else {
				if(IsNull(id))
					p.ThrowError("missing T_");
				String lngs = p.ReadId();
				p.PassChar('(');
				if(lngs.GetLength() == 4) {
					int lang = LNG_(ToUpper(lngs[0]), ToUpper(lngs[1]), ToUpper(lngs[2]), ToUpper(lngs[3]));
					if(LngIndex().Find(lang) >= 0) {
						String lt;
						do
							lt.Cat(p.ReadString());
						while(p.Char('+'));
						lng.GetAdd(id).text.GetAdd(lang) = lt;
						p.PassChar(')');
						continue;
					}
				}
				p.ThrowError("invalid language");
			}
		}
	}
	catch(CParser::Error e) {
		PutConsole(e);
		ShowConsole();
		return false;
	}
	return true;
}

String CreateTFile(const VectorMap<String, LngEntry>& map, const Vector<int>& lngset, bool rep, bool obsolete, bool java)
{
	const char *linepfx = (java ? "   + " : "     ");
	int ascflags = (java ? 0 : ASCSTRING_OCTALHI) | ASCSTRING_SMART;

	String out;
	String cfile;
	out << "#ifdef _MSC_VER\r\n#pragma setlocale(\"C\")\r\n#endif";

	for(int i = 0; i < map.GetCount(); i++) {
		if(i) out << "\r\n";
		const LngEntry& e = map[i];
		String nc;
		if(e.fileline.GetCount())
			nc = e.fileline[0].file;
		if(nc != cfile) {
			cfile = nc;
			if(!IsNull(cfile) && !rep)
				out << "\r\n// " << GetFileName(cfile) << "\r\n\r\n";
			if(IsNull(cfile) && obsolete)
				out << "\r\n// Obsolete\r\n\r\n";
		}
		if(!IsNull(cfile) || rep || obsolete) {
			String id = map.GetKey(i);
			out << "T_(" << AsCString(id, 70, linepfx, ascflags) << ")\r\n";
			for(int j = 0; j < lngset.GetCount(); j++) {
				int lang = lngset[j];
				if(rep || lang != LNG_enUS) {
					int q = e.text.Find(lang);
					if(!rep || q >= 0 && !IsNull(e.text[q])) {
						int c = (lang >> 15) & 31;
						if(c) {
							out.Cat(c + 'a' - 1);
							c = (lang >> 10) & 31;
							if(c) {
								out.Cat(c + 'a' - 1);
								c = (lang >> 5) & 31;
								if(c) {
									out.Cat(c + 'A' - 1);
									c = lang & 31;
									if(c) out.Cat(c + 'A' - 1);
								}
							}
						}
						out << '(' << AsCString(q >= 0 ? e.text[q] : String(), 70,
							linepfx, ascflags) << ")\r\n";
					}
				}
			}
		}
	}
	return out;
}

struct LangDlg : WithLangLayout<TopWindow> {
	void Serialize(Stream& s);

	Vector<TFile>& tfile;

	void AddLang();
	void RemoveLang();

	void AddLangAll();
	void RemoveLangAll();

	void EnterFile();
	void EnterText();
	void ToggleWork();

	void LangMenu(Bar& bar);

	bool ShouldWrite(int i)        { return file.Get(i, 1); }

	LangDlg(Vector<TFile>& map);
};

void LangDlg::Serialize(Stream& s)
{
	SerializePlacement(s);
	String f = file.GetKey();
	int l = lang.GetKey();
	String t = text.GetKey();
	s % f % l % t;
	file.FindSetCursor(f);
	lang.FindSetCursor(l);
	text.FindSetCursor(t);
}

void LangDlg::AddLang()
{
	if(!file.IsCursor())
		return;
	WithAddLangLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Add language");
	dlg.lang <<= dlg.Breaker(999);
	TFile& tf = tfile[file.GetCursor()];
	Vector<int>& ls = tf.ls;
	for(;;) {
		int l = ~dlg.lang;
		dlg.ok.Enable((l & 31) && FindIndex(ls, l) < 0);
		switch(dlg.Run()) {
		case IDOK:
			l = ~dlg.lang;
			if(FindIndex(ls, l) < 0) {
				ls.Add(l);
				tf.dirty = true;
				Sort(ls);
				EnterFile();
				lang.FindSetCursor(l);
				return;
			}
			break;
		case IDCANCEL:
			return;
		}
	}
}

void LangDlg::AddLangAll()
{
	WithAddLangLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Add to all");
	if(dlg.Run() != IDOK)
		return;
	int l = ~dlg.lang;
	for(int i = 0; i < tfile.GetCount(); i++) {
		TFile& tf = tfile[i];
		Vector<int>& ls = tf.ls;
		if(FindIndex(ls, l) < 0) {
			ls.Add(l);
			tf.dirty = true;
			Sort(ls);
		}
	}
	EnterFile();
	lang.FindSetCursor(l);
}

void LangDlg::RemoveLang()
{
	if(file.IsCursor() && lang.IsCursor() && (int)lang.GetKey() != LNG_enUS
	   && PromptOKCancel("Remove selected language version?")) {
		TFile& tf = tfile[file.GetCursor()];
		tf.ls.Remove(lang.GetCursor());
		tf.dirty = true;
		EnterFile();
	}
}

void LangDlg::RemoveLangAll()
{
	WithAddLangLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Remove from all");
	if(file.IsCursor() && lang.IsCursor())
		dlg.lang <<= tfile[file.GetCursor()].ls[lang.GetCursor()];
	if(dlg.Run() != IDOK)
		return;
	int l = ~dlg.lang;
	for(int i = 0; i < tfile.GetCount(); i++) {
		TFile& tf = tfile[i];
		Vector<int>& ls = tf.ls;
		int q = FindIndex(ls, l);
		if(q >= 0) {
			ls.Remove(q);
			tf.dirty = true;
			Sort(ls);
		}
	}
	EnterFile();
	lang.FindSetCursor(l);
}

void LangDlg::LangMenu(Bar& bar)
{
	bar.Add(file.IsCursor(), "Add..", [=] { AddLang(); });
	bar.Add(lang.IsCursor(), "Remove", [=] { RemoveLang(); });
	bar.Separator();
	bar.Add(file.IsCursor(), "Add to all..", [=] { AddLangAll(); });
	bar.Add(lang.IsCursor(), "Remove from all..", [=] { RemoveLangAll(); });
}

struct FontAndColorDisplay : Display {
	Font  font;
	Color color;

	void Paint(Draw& w, const Rect& r, const Value& q,
						Color ink, Color paper, dword s) const {
		w.DrawRect(r, paper);
		String vt = q;
		const char *txt = GetENUS(vt);
		int x = r.left;
		if(txt != ~vt) {
			String id(~vt, txt - 1);
			w.DrawText(x, r.top, id, font, txt[-1] == '\a' ? LtRed : Magenta);
			x += GetTextSize(id, font).cx + 6;
		}
		w.DrawText(x, r.top, AsCString(txt), font, ink);
	}

	FontAndColorDisplay(Font f, Color c) : font(f), color(c) {}
};

void LangDlg::EnterFile()
{
	TFile& tf = tfile[file.GetCursor()];
	Sort(tf.ls);

	lang.Clear();
	for(int i = 0; i < tf.ls.GetCount(); i++)
		lang.Add(tf.ls[i], LNGAsText(tf.ls[i]));
	lang.GoBegin();

	static FontAndColorDisplay normal(ArialZ(11)(), Null);
	static FontAndColorDisplay added(ArialZ(11)().Bold(), Null);
	static FontAndColorDisplay obsolete(ArialZ(11)().Italic(), Null);

	text.Clear();
	for(int i = 0; i < tf.map.GetCount(); i++) {
		text.Add(tf.map.GetKey(i));
		LngEntry& e = tf.map[i];
		Display *d;
		if(e.fileline.GetCount() == 0)
			d = &obsolete;
		else
		if(e.added)
			d = &added;
		else
			d = &normal;
		text.SetDisplay(text.GetCount() - 1, 0, *d);
	}
	text.GoBegin();
}

void LangDlg::EnterText()
{
	source.Clear();
	VectorMap<String, LngEntry>& map = tfile[file.GetCursor()].map;
	int q = text.GetCursor();
	if(q < 0 || q >= map.GetCount())
		return;
	LngEntry& e = map[q];
	for(int i = 0; i < e.fileline.GetCount(); i++) {
		FileLine& f = e.fileline[i];
		source.Add(f.file, f.line, GetFileName(f.file) + " (" + AsString(f.line) + ")");
	}
}

void LangDlg::ToggleWork()
{
	if(file.GetCount()) {
		bool q = !(int)file.Get(0, 1);
		for(int i = 0; i < file.GetCount(); i++)
			file.Set(i, 1, q);
	}
}

LangDlg::LangDlg(Vector<TFile>& tfile)
:	tfile(tfile)
{
	CtrlLayoutOKCancel(*this, "Translation files");
	file.AddColumn("File");
	HeaderCtrl::Column& m = file.AddColumn().Ctrls<Option>().HeaderTab();
	m.SetImage(IdeImg::work());
	m.WhenAction = [=] { ToggleWork(); };
	file.WhenEnterRow = [=] { EnterFile(); };
	file.ColumnWidths("144 27");

	lang.AddIndex();
	lang.AddColumn("Versions");
	lang.WhenBar = [=] (Bar& bar) { LangMenu(bar); };

	text.AddColumn("Text");
	text.WhenEnterRow = [=] { EnterText(); };
	
	help << [=] { LaunchWebBrowser("https://www.ultimatepp.org/srcdoc$Core$i18n_en-us.html"); };
	
	source.AddIndex();
	source.AddIndex();
	source.AddColumn("Source");
	source.WhenLeftDouble = Breaker(IDYES);

	for(int i = 0; i < tfile.GetCount(); i++)
		file.Add(tfile[i].package + '/' + tfile[i].file, 1);
	file.GoBegin();

	Icon(IdeImg::Language());
	Sizeable().Zoomable();
}

class ExportTrDlg : public WithExportTrLayout<TopWindow> {
	SelectDirButton of;
	typedef ExportTrDlg CLASSNAME;

public:
	ExportTrDlg();
};

ExportTrDlg::ExportTrDlg()
{
	CtrlLayoutOKCancel(*this, "Export .tr file");

	DlCharsetD(charset);
	of.Attach(folder);

	charset <<= CHARSET_UTF8;
	lang <<= LNG_ENGLISH;
}

String ExportTr(const Vector<TFile>& tfile, int& cs)
{
	ExportTrDlg dlg;
	LoadFromGlobal(dlg, "tr-export");
	int c = dlg.Run();
	StoreToGlobal(dlg, "tr-export");
	if(c != IDOK)
		return Null;
	int lng = ~dlg.lang;
	cs = ~dlg.charset;
	String fn = AppendFileName(~dlg.folder, LNGAsText(lng) + "." + CharsetName(cs) + ".tr");
	FileOut out(fn);
	if(!out) {
		Exclamation("Unable to open the output file:&[* \1" + fn);
		return Null;
	}
	out << "LANGUAGE " << AsCString(LNGAsText(SetLNGCharset(lng, cs))) << ";\r\n";
	for(int i = 0; i < tfile.GetCount(); i++) {
		const TFile& m = tfile[i];
		out << "// " << m.package << "/" << m.file << "\r\n";
		for(int j = 0; j < m.map.GetCount(); j++)
			out << AsCString(m.map.GetKey(j), 70) << ",\r\n"
			    << "\t" << AsCString(ToCharset(cs, m.map[j].text.Get(lng, ""), CHARSET_UTF8),
			                         60, "\t", ASCSTRING_SMART)
			    << ";\r\n\r\n";
	}
	return fn;
}

void Ide::SyncT(int kind)
{
	console.Clear();
	String filepath = GetActiveFilePath();
	SaveFile();

	TFile repository;
	LngParseTFile(ConfigFile("repository.t"), repository.map);

	if(kind == 1) {
		FileSel fs;
		fs.ActiveDir(AnySourceFs().GetActiveDir());
		fs.Type("Runtime translation file (*.tr)", "*.tr");
		fs.AllFilesType();
		LoadFromGlobal(fs, "fs-tr");
		bool b = fs.ExecuteOpen("Import translation file");
		StoreToGlobal(fs, "fs-tr");
		if(!b) return;
		String tr = LoadFile(~fs);
		CParser p(tr);
		try {
			while(!p.IsEof()) {
				p.PassId("LANGUAGE");
				int lang = LNGFromText(p.ReadString());
				if(!lang)
					p.ThrowError("Invalid language");
				p.PassChar(';');
				byte cs = GetLNGCharset(lang);
				lang &= LNGC_(255, 255, 255, 255, 0);
				while(p.IsString()) {
					String id = p.ReadString();
					p.PassChar(',');
					repository.map.GetAdd(id).text.GetAdd(lang) = ToCharset(CHARSET_UTF8, p.ReadString(), cs);
					p.PassChar(';');
				}
			}
		}
		catch(CParser::Error& e) {
			PutConsole(e);
			Exclamation("Failed: \1" + e);
			return;
		}
	}

	Vector<TFile> tfile;
	Vector<int> mainsT;

	Progress pi;
	const Workspace& wspc = IdeWorkspace();
	pi.SetTotal(wspc.GetCount());
	for(int iPackage = 0; iPackage < wspc.GetCount(); iPackage++) {
		const Package& pk = wspc.GetPackage(iPackage);
		String n = wspc[iPackage];
		pi.SetText("Scanning " + n);
		if(pi.StepCanceled()) return;
		VectorMap<String, LngEntry> pmap;
		for(int iFile = 0; iFile < pk.file.GetCount(); iFile++) {
			String file = SourcePath(n, pk.file[iFile]);
			LngParseCFile(SourcePath(GetActivePackage(), file), pmap);
		}
		bool localT = false;
		for(int iFile = 0; iFile < pk.file.GetCount(); iFile++) {
			String file = SourcePath(n, pk.file[iFile]);
			String ext = GetFileExt(file);
			if(ext == ".t" || ext == ".jt") {
				VectorMap<String, LngEntry> tmap(pmap, 0);
				if(LngParseTFile(file, tmap)) {
					TFile& tf = tfile.Add();
					tf.java = (ext == ".jt");
					tf.package = n;
					tf.file = pk.file[iFile];
					tf.map = pick(tmap);
					tf.MakeLS();
					// mark that we've found a local translation file
					localT = true;
					
					// store index of main package translation(s) file(s)
					if(iPackage == 0)
						mainsT.Add(tfile.GetCount() - 1);
				}
			}
		}
		// if no local translation file(s), we append translation to
		// main package one(s), if any
		if(!localT && iPackage > 0)
		{
			for(int i = 0; i < mainsT.GetCount(); i++)
			{
				VectorMap<String, LngEntry> &map = tfile[mainsT[i]].map;
				for(int iEntry = 0; iEntry < pmap.GetCount(); iEntry++)
				{
					int idx = map.Find(pmap.GetKey(iEntry));
					if(idx < 0)
					{
						map.Add(pmap.GetKey(iEntry), clone(pmap[iEntry]));
						idx = map.GetCount() - 1;
					}
					LngEntry &entry = map[idx];
					if(!entry.fileline.GetCount())
					{
						LngEntry &pEntry = pmap[iEntry];
						for(int iLc = 0; iLc < pEntry.fileline.GetCount(); iLc++)
						{
							FileLine &fl = entry.fileline.Add();
							String file = GetFileName(pEntry.fileline[iLc].file);
							file = "PACKAGE '" + GetFileName(n) + "' FILE '" + file + "'";
							fl.file = file;
							fl.line = pEntry.fileline[iLc].line;
						}
					}
				}
			}
		}
	}

	if(kind == 2) {
		for(int i = 0; i < tfile.GetCount(); i++) {
			TFile& tf = tfile[i];
			for(int ii = 0; ii < tf.map.GetCount(); ii++) {
				LngEntry& tle = tf.map[ii];
				LngEntry& rle = repository.map.GetAdd(tf.map.GetKey(ii));
				for(int l = 0; l < tle.text.GetCount(); l++)
					if(!IsNull(tle.text[l]))
						rle.text.GetAdd(tle.text.GetKey(l)) = tle.text[l];
			}
		}
	
		int cs;
		String fn = ExportTr(tfile, cs);
		if(!IsNull(fn)) {
			EditFile(fn);
			int c = filelist.GetCursor();
			FlushFile();
			ActiveFile().charset = (byte)cs;
			SavePackage();
			PackageCursor();
			if(c >= 0)
				filelist.SetCursor(c);
			editor.ClearUndo();
		}
		return;
	}

	LangDlg dlg(tfile);
	LoadFromWorkspace(dlg, "Tdlg");
	switch(dlg.Execute()) {
	case IDCANCEL:
		return;
	case IDYES:
		if(dlg.source.IsCursor()) {
			EditFile(dlg.source.Get(0));
			editor.SetCursor(editor.GetPos64(editor.GetLineNo((int)dlg.source.Get(1) - 1)));
			editor.CenterCursor();
			editor.SetFocus();
			StoreToWorkspace(dlg, "Tdlg");
			return;
		}
	}
	StoreToWorkspace(dlg, "Tdlg");

	FlushFile();

	for(int i = 0; i < tfile.GetCount(); i++) {
		TFile& tf = tfile[i];
		for(int ii = 0; ii < tf.map.GetCount(); ii++) {
			int q = repository.map.Find(tf.map.GetKey(ii));
			if(q >= 0) {
				LngEntry& tle = tf.map[ii];
				LngEntry& rle = repository.map[q];
				for(int l = 0; l < tf.ls.GetCount(); l++) {
					q = rle.text.Find(tf.ls[l]);
					if(q >= 0)
						tle.text.GetAdd(tf.ls[l]) = rle.text[q];
				}
			}
		}
	}

	pi.SetTotal(tfile.GetCount());
	for(int i = 0; i < tfile.GetCount(); i++) {
		TFile& tf = tfile[i];
		String td = CreateTFile(tf.map, tf.ls, false, !dlg.remove, tf.java);
		String fp = SourcePath(tf.package, tf.file);
		pi.SetText("Storing " + tf.package + '/' + tf.file);
		if(pi.StepCanceled())
			break;
		if(dlg.ShouldWrite(i) && LoadFile(fp) != td)
			::SaveFile(fp, td);
	}

	repository.MakeLS();
	::SaveFile(ConfigFile("repository.t"), CreateTFile(repository.map, repository.ls, true, false, false));
	FileCursor();
}

void Ide::ConvertST()
{
	if(!PromptYesNo("Convert project to t_ translations?"))
		return;

	FlushFile();

	TFile repository;
	LngParseTFile(ConfigFile("repository.t"), repository.map);

	Progress pi;
	const Workspace& wspc = IdeWorkspace();
	pi.SetTotal(wspc.GetCount());
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		pi.SetText("Converting " + n);
		if(pi.StepCanceled()) return;
		VectorMap<String, LngEntry> pmap;
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String file = SourcePath(n, pk.file[i]);
			String data = LoadFile(file);
			CParser p(data);
			String r;
			const char *b = data;
			while(!p.IsEof()) {
				p.Spaces();
				const char *w = p.GetPtr();
				if(p.Id("s_") && p.Char('(') && p.IsId()) {
					String id = p.ReadId();
					int q = repository.map.Find(id);
					if(q >= 0) {
						LngEntry& e = repository.map[q];
						q = e.text.Find(LNG_enUS);
						if(q >= 0) {
							r.Cat(b, w);
							r.Cat("t_(");
							r.Cat(AsCString(e.text[q]));
							b = p.GetPtr();
						}
					}
				}
				else
					p.SkipTerm();
			}
			r.Cat(b);
			if(r != data)
				::SaveFile(file, r);
		}
	}

	FileCursor();
}
