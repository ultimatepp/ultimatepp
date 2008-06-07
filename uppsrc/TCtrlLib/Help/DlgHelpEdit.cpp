#if 0

#include          "TCtrlLibHelp.h"
#pragma hdrstop

#define LAYOUTFILE <TCtrlLib/Help/DlgHelpEdit.lay>
#include <CtrlCore/lay.h>

class DlgHelpRename : public WithHelpRenameLayout<TopWindow>
{
public:
	typedef DlgHelpRename CLASSNAME;
	DlgHelpRename(const VectorMap<String, String>& mod_map);

	bool Run(VectorMap<String, String>& key_map);

private:
	void Find(int start, bool rev);

	TOOL(Existing)
		TOOL(ExistingFind)
		TOOL(ExistingFindPrev)
		TOOL(ExistingFindNext)
		TOOL(ExistingAssign)

	TOOL(Aliased)
		TOOL(AliasedRemove)

private:
	String                           find_text;
	const VectorMap<String, String>& mod_map;
};

RegisterHelpTopicObjectTitle(DlgHelpRename, t_("Map changed identifiers"))

DlgHelpRename::DlgHelpRename(const VectorMap<String, String>& mod_map)
: mod_map(mod_map)
{
	CtrlLayoutOKCancel(*this, DlgHelpRenameHelpTitle());
	HelpTopic("DlgHelpRename");
	unmatched.AutoHideSb();
	unmatched.AddColumn(t_("Loaded file"));
	existing.AutoHideSb();
	existing.AddColumn(t_("Identifier map"));
	existing.WhenBar = THISBACK(ToolExisting);
	aliased.AutoHideSb();
	aliased.AddColumn(t_("Old"));
	aliased.AddColumn(t_("Mapped"));
	aliased.AddColumn(t_("Module"));
	aliased.WhenBar = THISBACK(ToolAliased);
}

bool DlgHelpRename::Run(VectorMap<String, String>& key_map)
{
	Vector<String> lmap = key_map.PickKeys();
	Sort(lmap, GetLanguageInfo());
	Append(unmatched, lmap);
	lmap <<= mod_map.GetKeys();
	Sort(lmap, GetLanguageInfo());
	Append(existing, lmap);
	if(TopWindow::Run() != IDOK)
		return false;
	key_map.Clear();
	for(int i = 0; i < aliased.GetCount(); i++)
		key_map.Add(aliased.Get(i, 0), aliased.Get(i, 1));
	return true;
}

void DlgHelpRename::ToolExisting(Bar& bar)
{
	bar.Add(unmatched.IsCursor() && existing.IsCursor(), t_("Map"), THISBACK(OnExistingAssign))
		.Key(K_INSERT);
	bar.Add(t_("Find"), THISBACK(OnExistingFind))
		.Key(K_CTRL_F);
	bar.Add(t_("Find next"), THISBACK(OnExistingFindNext))
		.Key(K_F3);
	bar.Add(t_("Find previous"), THISBACK(OnExistingFindPrev))
		.Key(K_SHIFT_F3);
}

static bool IdentMatch(const char *mask, const char *name)
{
	for(;;)
		switch(*mask++) {
		case 0: return *name == 0;
		case '*':
			do
				if(IdentMatch(mask, name))
					return true;
			while(*name++);
			return false;
		case '?':
			if(*name++ == 0)
				return false;
			break;
		default:
			if(ToLower(mask[-1]) != ToLower(*name++))
				return false;
			break;
		}
}

RegisterHelpTopicObjectTitle(DlgHelpRenameFind, t_("Find"))

void DlgHelpRename::OnExistingFind()
{
	WithHelpRenameFindLayout<TopWindow> finddlg;
	finddlg.find <<= find_text;
	CtrlLayoutOKCancel(finddlg, DlgHelpRenameFindHelpTitle());
	finddlg.HelpTopic("DlgHelpRenameFind");
	if(finddlg.Run() == IDOK) {
		find_text = ~finddlg.find;
		Find(0, false);
	}
}

void DlgHelpRename::OnExistingFindNext()
{
	Find(existing.GetCursor() + 1, false);
}

void DlgHelpRename::OnExistingFindPrev()
{
	Find(existing.GetCursor() - 1, true);
}

void DlgHelpRename::Find(int start, bool rev)
{
	String ft = "*" + find_text + "*";
	for(int i = start; i >= 0 && i < existing.GetCount(); i += (rev ? -1 : 1))
		if(IdentMatch(ft, String(existing.Get(i, 0)))) {
			existing.SetCursor(i);
			return;
		}
	BeepExclamation();
}

void DlgHelpRename::OnExistingAssign()
{
	int u = unmatched.GetCursor(), e = existing.GetCursor();
	if(u >= 0 && e >= 0) {
		int a = aliased.GetCount();
		String ex = existing.Get(0);
		aliased.Add(unmatched.Get(0), ex, mod_map.Get(ex, Null));
		aliased.SetCursor(a);
		unmatched.Remove(u);
		existing.Remove(e);
		if(u > 1)
			unmatched.SetCursor(u < unmatched.GetCount() ? u : u - 1);
		if(e > 1)
			existing.SetCursor(e < existing.GetCount() ? e : e - 1);

	}
}

void DlgHelpRename::ToolAliased(Bar& bar)
{
	bar.Add(t_("Remove alias"), THISBACK(OnAliasedRemove))
		.Key(K_DELETE);
}

void DlgHelpRename::OnAliasedRemove()
{
	int a = aliased.GetCursor();
	if(a >= 0) {
		unmatched.Add(aliased.Get(0));
		existing.Add(aliased.Get(1));
		aliased.Remove(a);
		if(a > 1)
			aliased.SetCursor(a < aliased.GetCount() ? a : a - 1);
		unmatched.SetCursor(unmatched.GetCount() - 1);
		existing.SetCursor(existing.GetCount() - 1);
	}
}

//////////////////////////////////////////////////////////////////////
// DlgHelpLang::

class DlgHelpLang : public WithHelpLangLayout<TopWindow>
{
public:
	typedef DlgHelpLang CLASSNAME;
	DlgHelpLang();

	bool Run(VectorMap<int, String>& ext_lang, int& init_lang);

private:
	void OnExtLangEdit();
	void OnLangFileBrowse();

private:
	DropList lang_edit;
	EditField lang_file;
	ExtendedButton lang_file_browse;
};

bool RunDlgHelpLang(VectorMap<int, String>& ext_lang, int& init_lang) { return DlgHelpLang().Run(ext_lang, init_lang); }

RegisterHelpTopicObjectTitle(DlgHelpLang, t_("Application languages"))

DlgHelpLang::DlgHelpLang()
{
	CtrlLayoutOKCancel(*this, DlgHelpLangHelpTitle());
	HelpTopic("DlgHelpLang");

	ext_lang.AutoHideSb();
	ext_lang.AddColumn(t_("Language code"), 10).SetConvert(lang_edit).Edit(lang_edit);
	ext_lang.AddColumn(t_("Language file"), 30).Edit(lang_file);
	ext_lang.Inserting().Removing().NoAskRemove();
	ext_lang.WhenArrayAction = THISBACK(OnExtLangEdit);

	lang_file.AddFrame(lang_file_browse);
	lang_file_browse <<= THISBACK(OnLangFileBrowse);

	for(const int *p = GetAllLanguages(); *p; p++)
		lang_edit.Add(*p, LNGAsText(*p));
}

bool DlgHelpLang::Run(VectorMap<int, String>& el, int& il)
{
	int i;
	for(i = 0; i < el.GetCount(); i++)
		ext_lang.Add(el.GetKey(i), el[i]);
	OnExtLangEdit();
	init_lang <<= il;
	if(TopWindow::Run() != IDOK)
		return false;
	el.Clear();
	for(i = 0; i < ext_lang.GetCount(); i++)
		el.Add(ext_lang.Get(i, 0), ext_lang.Get(i, 1));
	il = ~init_lang;
	return true;
}

void DlgHelpLang::OnExtLangEdit()
{
	int oi = ~init_lang;
	init_lang.Clear();
	Index<int> app_lang = GetLanguages();
	for(int i = 0; i < ext_lang.GetCount(); i++)
		app_lang.FindAdd(ext_lang.Get(i, 0));
	Index<int> lmask;
	for(int i = 0; i < app_lang.GetCount(); i++) {
		int l = app_lang[i], l0 = SetLNGCharset(l, CHARSET_DEFAULT);
		if(lmask.Find(l0) < 0) {
			lmask.Add(l0);
			init_lang.Add(l, LNGAsText(l0));
		}
	}
	init_lang <<= (init_lang.FindKey(oi) < 0 ? app_lang[0] : oi);
}

void DlgHelpLang::OnLangFileBrowse()
{
	FileSelector fsel;
	fsel.Type(t_("Language files (*.lng)"), t_("*.lng"));
	fsel.DefaultExt("lng");
	fsel.AllFilesType();
	fsel <<= String(~lang_file_browse);
	if(fsel.ExecuteOpen(t_("Language file"))) {
		lang_file_browse <<= ~fsel;
		lang_file_browse.Action();
	}
}

class DlgHelpDirty : public WithHelpDirtyLayout<TopWindow>
{
public:
	typedef DlgHelpDirty CLASSNAME;
	DlgHelpDirty();

	bool                   Run(VectorMap<int, String>& files);

private:
	void                   OnAdd();
	void                   OnAddAll();
	void                   OnRemove();
	void                   OnRemoveAll();
	void                   OnBrowse();
	void                   UpdateGUI();

	bool                   DoAdd();
	bool                   DoRemove();

	bool                   BrowseFile(String& fn, int lang);

private:
	VectorMap<int, String> files;
};

bool RunDlgHelpDirty(VectorMap<int, String>& files) { return DlgHelpDirty().Run(files); }

RegisterHelpTopicObjectTitle(DlgHelpDirty, t_("Save changes..."))

DlgHelpDirty::DlgHelpDirty()
{
	CtrlLayoutOKCancel(*this, DlgHelpDirtyHelpTitle());
	HelpTopic("DlgHelpDirty");

	dirty_files.AutoHideSb();
	dirty_files.AddIndex();
	dirty_files.AddColumn("Jazyk");

	dirty_files.AddKey(K_RIGHT, add <<= THISBACK(OnAdd));
	dirty_files.AddKey(K_CTRL_RIGHT, add_all <<= THISBACK(OnAddAll));

	save_files.AutoHideSb();
	save_files.AddIndex();
	save_files.AddColumn("Jazyk", 10);
	save_files.AddColumn("Cesta", 40);
	save_files.WhenLeftDouble = THISBACK(OnBrowse);

	save_files.AddKey(K_LEFT, remove <<= THISBACK(OnRemove));
	save_files.AddKey(K_CTRL_LEFT, remove_all <<= THISBACK(OnRemoveAll));
}

bool DlgHelpDirty::Run(VectorMap<int, String>& f)
{
	files = f;
	int t;
	for(t = 0; t < files.GetCount(); t++) {
		int l = files.GetKey(t);
		String ln = LNGAsText(l);
		if(files[t].IsEmpty())
			dirty_files.Add(l, ln);
		else
			save_files.Add(l, ln, files[t]);
	}
	dirty_files.SetCursor(0);
	save_files.SetCursor(0);
	ActiveFocus(dirty_files);

	if(TopWindow::Run() != IDOK)
		return false;
	f.Clear();
	for(t = 0; t < save_files.GetCount(); t++)
		f.Add(save_files.Get(t, 0), save_files.Get(t, 2));
	return true;
}

void DlgHelpDirty::UpdateGUI()
{
	add.Enable(dirty_files.IsCursor());
	add_all.Enable(dirty_files.GetCount() > 0);
	remove.Enable(save_files.IsCursor());
	remove_all.Enable(save_files.GetCount() > 0);
}

void DlgHelpDirty::OnBrowse()
{
	if(!save_files.IsCursor())
		return;
	String fn = save_files.Get(2);
	if(BrowseFile(fn, save_files.Get(0)))
		save_files.Set(2, fn);
}

bool DlgHelpDirty::BrowseFile(String& fn, int lang)
{
	FileSelector fsel;
	fsel.DefaultExt("lng");
	fsel.Type("Jazykové soubory (*.lng)", "*.lng");
	fsel <<= fn;
	if(!fsel.ExecuteSaveAs(NFormat("%s - uložit jako...", LNGAsText(lang))))
		return false;
	fn = ~fsel;
	return true;
}

bool DlgHelpDirty::DoAdd()
{
	int dc = dirty_files.GetCursor();
	if(dc < 0)
		return false;
	int l = dirty_files.Get(0);
	String ln = LNGAsText(l);
	String fn = files.Get(l);
	if(fn.IsEmpty())
		if(!BrowseFile(fn, l))
			return false;
	files.Get(l) = fn;
	dirty_files.Remove(dc);
	dirty_files.SetCursor(dc < dirty_files.GetCount() ? dc : dc - 1);
	save_files.Add(l, ln, fn);
	save_files.SetCursor(save_files.GetCount() - 1);
	UpdateGUI();
	return true;
}

bool DlgHelpDirty::DoRemove()
{
	int sc = save_files.GetCursor();
	if(sc < 0)
		return false;
	int l = save_files.Get(0);
	String ln = LNGAsText(l);
	save_files.Remove(sc);
	save_files.SetCursor(sc < save_files.GetCount() ? sc : sc - 1);
	dirty_files.Add(l, ln);
	dirty_files.SetCursor(dirty_files.GetCount() - 1);
	UpdateGUI();
	return true;
}

void DlgHelpDirty::OnAdd()
{
	DoAdd();
}

void DlgHelpDirty::OnAddAll()
{
	while(DoAdd())
		;
}

void DlgHelpDirty::OnRemove()
{
	DoRemove();
}

void DlgHelpDirty::OnRemoveAll()
{
	while(DoRemove())
		;
}

static String FromEdit(const String& s)
{
	String out;
	for(const char *b = s, *p = b; *p; p++)
		if(*p == '\r')
			;
		else if(*p != '\\')
			out.Cat(*p);
		else
			switch(*++p) {
			case 'a':  out << '\a'; break;
			case 'b':  out << '\b'; break;
			case 't':  out << '\t'; break;
			case 'v':  out << '\v'; break;
			case 'f':  out << '\f'; break;
			case 'r':  out << '\r'; break;
			case 'n':  out << '\n'; break;
			case '\\': out << '\\'; break;
			case '\"': out << '\"'; break;
			case 'x': {
					int c = ctoi(*++p);
					if(c < 16) {
						int c2 = ctoi(*++p);
						if(c2 < 16) {
							p++;
							c = c * 16 + c2;
						}
						out.Cat((char)c);
					}
					p--;
				}
				break;

			default:
				if(*p >= '0' && *p <= '7') { // octal sequence
					int c = *p - '0';
					int c2 = *++p - '0';
					if(c2 >= 0 && c2 < 8) {
						c = c * 8 + c2;
						c2 = *++p - '0';
						if(c2 >= 0 && c2 < 8) {
							c = c * 8 + c2;
							p++;
						}
					}
					out.Cat((char)c);
					p--;
				}
				break;
			}

	return out;
}

static String ToEdit(const String& s)
{
	String out;
	for(const char *p = s.Begin(), *e = s.End(); p < e;) {
		const char *b = p;
		while(p < e && (*p == '\n' || (byte)*p >= ' ' && *p != '\\' && *p != (char)127 && *p != (char)255))
			p++;
		out.Cat(b, p - b);
		if(p < e)
			switch(*p++) {
			case '\a': out << "\\a"; break;
			case '\b': out << "\\b"; break;
			case '\t': out << "\\t"; break;
//			case '\n':
			case '\v': out << "\\v"; break;
			case '\f': out << "\\f"; break;
			case '\r': out << "\\r"; break;
			case '\\': out << "\\\\"; break;
			default:   out << "\\" << FormatIntOct((byte)p[-1], 3); break;
			}
	}
	return out;
}

class DlgHelpEdit : public WithHelpEditLayout<TopWindow>
{
public:
	typedef DlgHelpEdit CLASSNAME;
	DlgHelpEdit();

	void           Run();
	virtual bool   HotKey(dword key);

private:
	void           UpdateLang();
	void           UpdateModules(const String& mod);
	void           UpdateTexts(const String& key);
	void           UpdateStrings();
	void           UpdateSrcText();
	void           UpdateDestText();
	void           UpdateTitle();

	void           OnModules();
	void           OnTexts();
	void           OnSrcLang();
	void           OnDestLang();
	void           OnDestText();

	void           OnNew();
	void           OnSave();
	void           OnLoad();
	void           OnExit();
	void           OnExport();

	void           OnPrevDoubtfulText();
	void           OnNextDoubtfulText();

	String         GetModule() const;
	String         GetText() const;
	bool           IsValidLang(int sl, int dl) const { return !IsNull(sl) && !IsNull(dl) && sl != dl; }
	int            GetTextState(const String& key, int sl, int dl) const;

	void           WriteText(const String& key, int lang, const String& text);
	String         ReadText(const String& key, int lang) const;
	String         ReadLoadedText(const String& key, int lang) const;

	bool           CheckDirty();

private:
	enum {
		M_KEY,
		M_NAME,
		M_COUNT,
		M_TRAN,
		M_DIRTY,
	};

	enum {
		T_KEY,
		T_TRAN,
		T_STATE,
	};

	class LangInfo {
	public:
		LangInfo() {}

	public:
		String                    filename;
		VectorMap<String, String> loaded;
		VectorMap<String, String> edited;
	};

	VectorMap<String, String> all_lang_keys;
	VectorMap<String, Vector<String> > module_map;
	Vector<String> file_map;
	ArrayMap<int, LangInfo> languages;
	String last_key;
	int last_lang;
	bool lock_dest;
	FrameLeft<Button> new_lang;
};

void RunDlgHelpEdit() { DlgHelpEdit().Run(); }

RegisterHelpTopicObjectTitle(DlgHelpEdit, "Editor jazykù")

DlgHelpEdit::DlgHelpEdit()
{
	last_lang = Null;
	lock_dest = false;

	CtrlLayoutCancel(*this, "");
	Sizeable().MaximizeBox();

	modules.AutoHideSb();
	modules.AddIndex();
	modules.AddColumn("Modul", 30);
	modules.AddColumn("Poèet", 10);
	modules.AddColumn("Pøeklad", 10);
	modules.WhenEnterRow = modules.WhenKillCursor = THISBACK(OnModules);

	texts.AutoHideSb();
	texts.AddColumn("Kód textu", 40);
	texts.AddColumn("Pøeklad", 10);
	texts.AddIndex();
	texts.WhenEnterRow = texts.WhenKillCursor = THISBACK(OnTexts);

	src_text.SetReadOnly();

	src_lang <<= THISBACK(OnSrcLang);
	dest_lang <<= THISBACK(OnDestLang);
	dest_lang.AddFrame(new_lang);
	new_lang.SetImage(CtrlImg::plus());
	new_lang <<= THISBACK(OnNew);

	dest_text <<= THISBACK(OnDestText);

	save <<= THISBACK(OnSave);
	load <<= THISBACK(OnLoad);
	cancel <<= WhenClose = THISBACK(OnExit);

	Append(languages, GetLanguages());

	Vector<String> lmod = GetLanguageModules();
	Index<String> file_index;
	Sort(lmod, GetLanguageInfo());
	int i;
	for(i = 0; i < lmod.GetCount(); i++) {
		String mod = lmod[i];
		String l = GetLanguageFile(mod);
		const char *b = l.Begin(), *e = l.End();
		if(*b == '\"' || *b == '<')
			b++;
		if(e > b && (e[-1] == '\"' || e[-1] == '>'))
			e--;
		String fn(b, e);
		String ext = GetFileExt(fn);
		if(!CompareNoCase(ext, ".lng")) {
			file_index.Add(mod);
			module_map.Add(GetFileTitle(fn));
		}
	}

	VectorMap<String, String> rmap = GetLanguageMap();
	all_lang_keys <<= rmap;
	for(i = 0; i < rmap.GetCount(); i++) {
		int p = file_index.Find(rmap[i]);
		if(p >= 0)
			module_map[p].Add(rmap.GetKey(i));
	}
	file_map = file_index.PickKeys();

	UpdateLang();
	if(languages.GetCount() >= 2)
		dest_lang <<= languages.GetKey(1);
}

void DlgHelpEdit::Run()
{
	UpdateModules(Null);
	TopWindow::Run();
}

void DlgHelpEdit::UpdateStrings()
{
	String mod = GetModule(), key = GetText();
	UpdateModules(mod);
	UpdateTexts(key);
	UpdateSrcText();
	UpdateDestText();
}

void DlgHelpEdit::UpdateModules(const String& mod)
{
	String mod_key = mod;
	if(mod_key.IsEmpty() && modules.IsCursor())
		mod_key = modules.Get(M_KEY);
	int sc = modules.GetCursorSc();
	modules.Clear();
	modules.Add();
	int total = 0, ttran = 0;
	int sl = ~src_lang, dl = ~dest_lang;
	bool vl = IsValidLang(sl, dl);
	bool is_dirty = false;
	const LangInfo *lmap = NULL;
	if(vl)
		lmap = &languages.Get(dl);
	for(int i = 0; i < module_map.GetCount(); i++) {
		String modkey = module_map.GetKey(i);
		const Vector<String>& modkeys = module_map[i];
		int ntran = 0;
		bool mod_dirty = false;
		for(int t = 0; t < modkeys.GetCount(); t++) {
			String key = modkeys[t];
			if(GetTextState(key, sl, dl) == 1)
				ntran++;
			if(lmap && lmap -> edited.Find(key) >= 0)
				mod_dirty = true;
		}
		String sntran;
		if(!modkeys.IsEmpty() && vl)
			sntran << iscale(ntran, 100, modkeys.GetCount()) << '%';
		String modname = modkey;
		if(mod_dirty)
			modname << " (*)";
		modules.Add(modkey, modname, modkeys.GetCount(), sntran, mod_dirty);
		total += modkeys.GetCount();
		ttran += ntran;
		if(mod_dirty)
			is_dirty = true;
	}
	modules.Set(0, M_NAME, is_dirty ? "(všechny) (*)" : "(všechny)");
	modules.Set(0, M_COUNT, total);
	modules.Set(0, M_DIRTY, is_dirty);
	if(total > 0 && vl)
		modules.Set(0, M_TRAN, NFormat("%d%%", iscale(ttran, 100, total)));
	modules.HeaderTab(0).SetText(NFormat("Moduly (%d)", module_map.GetCount()));
	if(!modules.FindSetCursor(mod_key))
		modules.SetCursor(0);
	modules.ScCursor(sc);
}

int DlgHelpEdit::GetTextState(const String& key, int sl, int dl) const
{
	if(!IsValidLang(sl, dl) || key.IsEmpty())
		return -1;
	String stxt = ReadText(key, sl);
	String dtxt = ReadText(key, dl);
	if(dtxt.IsEmpty())
		return 0;
	else if(dtxt == stxt) {
		for(const char *s = stxt; *s; s++)
			if(IsLetter(*s))
				return 2;
		return 1;
	}
	else if(dtxt.GetLength() * 3 < stxt.GetLength())
		return 2;
	else
		return 1;
}

void DlgHelpEdit::UpdateTexts(const String& key)
{
	String sel_key = key;
	if(sel_key.IsEmpty() && texts.IsCursor())
		sel_key = texts.Get(T_KEY);
	int sc = texts.GetCursorSc();
	texts.Clear();
	if(modules.IsCursor()) {
		Vector<String> list;
		String mod = modules.Get(M_KEY);
		int b = 0, e = module_map.GetCount();
		if(!mod.IsEmpty())
			e = 1 + (b = module_map.Find(mod));
		ASSERT(b >= 0);
		int i;
		for(i = b; i < e; i++)
			list.Append(module_map[i]);
		int sl = ~src_lang, dl = ~dest_lang;
		for(i = 0; i < list.GetCount(); i++) {
			String key = list[i];
			String tran;
			Color c = Black;
			int state = GetTextState(key, sl, dl);
			switch(state) {
			case 0: tran = "ne"; break;
			case 1: tran = "ano"; break;
			case 2: tran = "???"; break;
			}
			texts.Add(key, tran, state);
		}
	}
	String ttit = "Texty";
	if(texts.GetCount() > 0)
		ttit << " (" << texts.GetCount() << ")";
	texts.HeaderTab(0).SetText(ttit);
	if(!texts.FindSetCursor(sel_key))
		texts.SetCursor(0);
	texts.ScCursor(sc);
}

void DlgHelpEdit::UpdateTitle()
{
	String title;
	if(modules.IsCursor() && texts.IsCursor())
		title << (String)texts.Get(T_KEY) << " - editor jazykù";
	else
		title = DlgHelpEditHelpTitle();
	Title(title.ToWString());
}

void DlgHelpEdit::UpdateLang()
{
	int os = ~src_lang, od = ~dest_lang;
	src_lang.Clear();
	dest_lang.Clear();
	for(int i = 0; i < languages.GetCount(); i++) {
		int l = languages.GetKey(i);
		String ln = LNGAsText(l);
		if(IsNull(os)) os = l;
		if(IsNull(od)) od = l;
		src_lang.Add(l, ln);
		dest_lang.Add(l, ln);
	}
	src_lang <<= os;
	dest_lang <<= od;
}

void DlgHelpEdit::UpdateSrcText()
{
	int sl = ~src_lang;
	bool en = modules.IsCursor() && texts.IsCursor() && !IsNull(sl);
	String s;
	if(en) {
		src_text.SetFont(Courier(14));
//		src_text.Charset(GetLangStdCharset(sl));
		String key = texts.Get(T_KEY);
		s = ReadText(key, sl);
	}
	src_text.Enable(en);
	src_text <<= ToEdit(s);
}

void DlgHelpEdit::OnDestText()
{
	if(!IsNull(last_lang) && !last_key.IsEmpty()) {
		lock_dest = true;
		WriteText(last_key, last_lang, FromEdit(~dest_text));
		lock_dest = false;
	}
}

void DlgHelpEdit::UpdateDestText()
{
	if(lock_dest)
		return;
	int dl = ~dest_lang;
	bool en = modules.IsCursor() && texts.IsCursor()
	&& !IsNull(dl) && dl != (int)~src_lang;
	String s;
	if(en) {
		dest_text.SetFont(Courier(14));
//		dest_text.Charset(GetLangStdCharset(dl));
		String key = texts.Get(0);
		s = ReadText(key, dl);
		last_key = key;
		last_lang = dl;
	}
	dest_text.Enable(en);
	dest_text <<= ToEdit(s);
	dest_text.SetCursor(dest_text.GetLength());
}

void DlgHelpEdit::OnModules()
{
	UpdateTexts(Null);
}

void DlgHelpEdit::OnTexts()
{
	UpdateTitle();
	UpdateSrcText();
	UpdateDestText();
}

void DlgHelpEdit::OnSrcLang()
{
	UpdateStrings();
}

void DlgHelpEdit::OnDestLang()
{
	UpdateStrings();
}

bool DlgHelpEdit::HotKey(dword key)
{
	int mod = modules.GetCursor();
	int txt = texts.GetCursor();
	switch(key) {
	case K_ALT_LEFT:
		if(mod > 0)
			modules.SetCursor(mod - 1);
		break;

	case K_ALT_RIGHT:
		if(mod + 1 < modules.GetCount())
			modules.SetCursor(mod + 1);
		break;

	case K_ALT_UP:
		if(txt > 0)
			texts.SetCursor(txt - 1);
		break;

	case K_ALT_DOWN:
		if(txt + 1 < texts.GetCount())
			texts.SetCursor(txt + 1);
		break;

	case K_F4:
		OnNextDoubtfulText();
		break;

	case K_SHIFT_F4:
		OnPrevDoubtfulText();
		break;

	case K_CTRL_N:
		OnNew();
		break;

	case K_CTRL_O:
		OnLoad();
		break;

	case K_CTRL_S:
		OnSave();
		break;

	case K_CTRL_R:
		OnExport();
		break;

	default:
		return TopWindow::HotKey(key);
	}

	dest_text.SetWantFocus();
	return true;
}

String DlgHelpEdit::GetModule() const
{
	if(modules.IsCursor())
		return modules.Get(M_KEY);
	return String::GetVoid();
}

String DlgHelpEdit::GetText() const
{
	if(texts.IsCursor())
		return texts.Get(T_KEY);
	return String::GetVoid();
}

void DlgHelpEdit::OnPrevDoubtfulText()
{
	for(int row = texts.GetCursor() - 1; row >= 0; row--) {
		int state = texts.Get(row, T_STATE);
		if(state == 0 || state == 2) {
			texts.SetCursor(row);
			return;
		}
	}
	BeepQuestion();
}

void DlgHelpEdit::OnNextDoubtfulText()
{
	for(int row = texts.GetCursor() + 1, n = texts.GetCount(); row < n; row++) {
		int state = texts.Get(row, T_STATE);
		if(state == 0 || state == 2) {
			texts.SetCursor(row);
			return;
		}
	}
	BeepQuestion();
}

void DlgHelpEdit::WriteText(const String& key, int lang, const String& text)
{
	ASSERT(!IsNull(lang) && !key.IsEmpty());
	if(text == ReadText(key, lang))
		return;
	LangInfo& llist = languages.Get(lang);
	bool ddirty = false;
	int f = llist.edited.Find(key);
	if(text == ReadLoadedText(key, lang)) {
		if(f >= 0) {
			llist.edited.Remove(f);
			ddirty = true;
		}
	}
	else {
		if(f < 0) {
			llist.edited.Add(key, text);
			ddirty = true;
		}
		else
			llist.edited[f] = text;
	}
	int new_state = GetTextState(key, ~src_lang, lang);
	if(new_state != (int)texts.Get(T_STATE) || ddirty) {
		UpdateStrings();
	}
}

String DlgHelpEdit::ReadText(const String& key, int lang) const
{
	if(IsNull(lang) || key.IsEmpty())
		return String::GetVoid();
	const LangInfo& llist = languages.Get(lang);
	int f;
	if((f = llist.edited.Find(key)) >= 0)
		return llist.edited[f];
	if((f = llist.loaded.Find(key)) >= 0)
		return llist.loaded[f];
	return txtGet(key, lang);
}

String DlgHelpEdit::ReadLoadedText(const String& key, int lang) const
{
	if(IsNull(lang) || key.IsEmpty())
		return String::GetVoid();
	const LangInfo& llist = languages.Get(lang);
	int f;
	if((f = llist.loaded.Find(key)) >= 0)
		return llist.loaded[f];
	return txtGet(key, lang);
}

void DlgHelpEdit::OnSave()
{
	CheckDirty();
}

RegisterHelpTopicObjectTitle(DlgHelpOpen, "Nový jazyk")

void DlgHelpEdit::OnNew()
{
	WithHelpOpenLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, DlgHelpOpenHelpTitle());
	dlg.HelpTopic("DlgHelpOpen");
	const int *pl = GetAllLanguages();
	for(; *pl; pl++)
		if(languages.Find(*pl) < 0) {
			String ln = LNGAsText(*pl);
			if(!ln.IsEmpty())
				dlg.lang.Add(*pl, LNGAsText(*pl));
		}
	if(dlg.lang.GetCount() == 0) {
		Exclamation("Žádné další jazyky nejsou k dispozici.");
		return;
	}
	dlg.lang.SetIndex(0);
	if(dlg.Run() == IDOK) {
		int l = ~dlg.lang;
		ASSERT(!IsNull(l));
		languages.Add(l);
		UpdateLang();
		dest_lang <<= l;
		UpdateStrings();
	}
}

void DlgHelpEdit::OnLoad()
{
	int dl = ~dest_lang;
	WithHelpOpenLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Naèíst soubor");
	dlg.HelpTopic("DlgHelpOpen");
	const int *pl = GetAllLanguages();
	for(; *pl; pl++) {
		String ln = LNGAsText(*pl);
		if(!ln.IsEmpty()) {
			if(IsNull(dl))
				dl = *pl;
			dlg.lang.Add(*pl, ln);
		}
	}
	dlg.lang <<= dl;
	if(dlg.Run() != IDOK)
		return;

	dl = ~dlg.lang;
	String lang = LNGAsText(dl);
	int lf = languages.Find(dl);
	if(lf >= 0 && !languages[lf].edited.IsEmpty())
		switch(PromptYesNoCancel(NFormat("Chcete stornovat provedené zmìny v mutaci [* \1%s\1]?", lang))) {
		case 1: {
				languages[lf].edited.Clear();
				UpdateStrings();
			}
			break;

		case 0:
			break;

		default:
			return;
		}

	FileSelector fsel;
	fsel.DefaultExt("lng");
	fsel.Type("Jazykové soubory (*.lng)", "*.lng");
	if(lf >= 0)
		fsel <<= languages[lf].filename;
	if(!fsel.ExecuteOpen(lang + " - naèíst soubor..."))
		return;
	String data = LoadFile(~fsel);
	if(data.IsVoid()) {
		Exclamation(NFormat("Nelze naèíst soubor [* \1%s\1].", ~fsel));
		return;
	}
	try {
		VectorMap<String, String> lmap = FileToLanguage(data);
		VectorMap<String, String> alias_map;
		Index<String> alias_all;
		int i;
		for(i = 0; i < lmap.GetCount(); i++)
			if(all_lang_keys.Find(lmap.GetKey(i)) < 0)
				alias_map.Add(lmap.GetKey(i));
		alias_all <<= alias_map.GetIndex();
		if(!alias_map.IsEmpty()) {
			if(!DlgHelpRename(all_lang_keys).Run(alias_map))
				return;
		}
		LangInfo& linfo = languages.GetAdd(dl);
		linfo.filename = ~fsel;
		for(i = 0; i < lmap.GetCount(); i++) {
			String k = lmap.GetKey(i);
			if(alias_all.Find(k) < 0)
				linfo.loaded.GetAdd(k, lmap[i]);
			else if(!(k = alias_map.Get(k, Null)).IsEmpty())
				linfo.loaded.GetAdd(k, lmap[i]);
		}
		UpdateLang();
		dest_lang <<= dl;
	}
	catch(CParser::Error e) {
		Exclamation(NFormat("Chyba v souboru [* \1%s\1]: %s", ~fsel, e));
	}

	UpdateStrings();
}

void DlgHelpEdit::OnExit()
{
	if(CheckDirty())
		Break(IDOK);
}

bool DlgHelpEdit::CheckDirty()
{
	VectorMap<int, String> dirty_lang;
	int l;
	for(l = 0; l < languages.GetCount(); l++)
		if(!languages[l].edited.IsEmpty())
			dirty_lang.Add(languages.GetKey(l), languages[l].filename);
	if(dirty_lang.IsEmpty())
		return true;
	if(!RunDlgHelpDirty(dirty_lang))
		return false;
	for(l = 0; l < dirty_lang.GetCount(); l++) {
		String fn = dirty_lang[l];
		int lang = dirty_lang.GetKey(l);
		LangInfo& linfo = languages.Get(lang);
		VectorMap<String, String> langmap;
		langmap <<= linfo.loaded;
		for(int e = 0; e < linfo.edited.GetCount(); e++)
			langmap.GetAdd(linfo.edited.GetKey(e)) = linfo.edited[e];

		if(!SaveFile(fn, LanguageToFile(langmap))) {
			Exclamation(NFormat("Nelze uložit verzi [* \1%s\1] do souboru [* \1%s\1].", LNGAsText(lang), fn));
			return false;
		}

		linfo.loaded = langmap;
		linfo.edited.Clear();
		linfo.filename = fn;
	}
	return true;
}

void DlgHelpEdit::OnExport()
{
	Index<String> dirty_keys;
	int i;
	for(i = 0; i < languages.GetCount(); i++) {
		const LangInfo& li = languages[i];
		FindAppend(dirty_keys, li.loaded);
		FindAppend(dirty_keys, li.edited);
	}
	for(i = dirty_keys.GetCount(); --i >= 0;) {
		bool equ = true;
		for(int l = 0; l < languages.GetCount(); l++) {
			int lc = languages.GetKey(l);
			if(ReadText(dirty_keys[i], lc) != String(txtGet(dirty_keys[i], lc))) {
				equ = false;
				break;
			}
		}
		if(equ)
			dirty_keys.Remove(i);
	}
	Vector<int> dirty_modules;
	for(i = 0; i < module_map.GetCount(); i++) {
		const Vector<String>& mod_keys = module_map[i];
		for(int j = 0; j < mod_keys.GetCount(); j++)
			if(dirty_keys.Find(mod_keys[j]) >= 0) {
				dirty_modules.Add(i);
				break;
			}
	}
	if(dirty_modules.IsEmpty()) {
		PromptOK("Všechny jazykové soubory obsahují platná data; export nemá význam.");
		return;
	}

	FileSel fsel;
	if(!fsel.ExecuteSelectDir("Export jazykových souborù..."))
		return;

	String path = ~fsel;
	for(i = 0; i < dirty_modules.GetCount(); i++) {
		int mod = dirty_modules[i];
		String modfile = AppendFileName(path, ForceExt(file_map[mod], ".lng"));
		const Vector<String>& keys = module_map[mod];
		StringStream output;

		for(int l = 0; l < languages.GetCount(); l++) {
			int lc = languages.GetKey(l);
			output << (l ? "#elif" : "#if") << " LANGUAGE == " << GetLangIdent(lc) << "\n\n";
			for(int k = 0; k < keys.GetCount(); k++) {
				String key = keys[k];
				String val = ReadText(key, lc);
				output << "txt(" << key << ", " << AsCStringWrap(val, 60, "\t") << ")\n";
			}
			output << "\n";
		}

		output << "#endif\n";
		if(!SaveFile(modfile, output.GetResult())) {
			Exclamation(NFormat("Nelze uložit soubor [* \1%s\1].", modfile));
			return;
		}
	}

	PromptOK(NFormat("Export zmìnìných souborù byl proveden do složky [* \1%s\1].", path));
}

#endif
