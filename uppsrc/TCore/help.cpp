#include "TCore.h"

NAMESPACE_UPP

#define LLOG(x) // RLOG(x)
#define LDUMP(x) // RDUMP(x)

#define LNGLIST() \
	LNGITEM(LNG_CZECH,   CHARSET_WIN1250) \
	LNGITEM(LNG_ENGLISH, CHARSET_WIN1252) \
//	LNGITEM(LNG_GERMAN,  CHARSET_WIN1252) \

#define LNGITEM(lcode, charset)

static String LNGAsTextShort(int language)
{
	int charset = GetLNGCharset(language);
	String lcode = LNGAsText(language & ~(-1 << 20));
	if(charset != CHARSET_UNICODE)
		lcode << charset;
	return lcode;
}

bool operator == (const HelpTopicInfo& a, const HelpTopicInfo& b)
{
	if(a.textfunc != b.textfunc || a.titlefunc != b.titlefunc)
		return false;
	if(a.language.GetCount() != b.language.GetCount())
		return false;
	for(int l = 0; l < a.language.GetCount(); l++)
	{
		int f = b.language.Find(a.language[l]);
		if(f < 0 || a.title[l] != b.title[f] || a.text[l] != b.text[f])
			return false;
	}
	return true;
}

String GetLangIdent(int language)
{
	switch(language)
	{
#undef LNGITEM
#define LNGITEM(lcode, charset) case lcode: return #lcode;
	LNGLIST()
	default: return "0x" + FormatIntHex(language, 8);
	}
}

byte GetLangStdCharset(int language)
{
	if(IsNull(language) || language == 0)
		return GetDefaultCharset();
	int cs = (language >> 20) & 0xFF;
	if(cs)
		return cs;
	switch(language)
	{
#undef LNGITEM
#define LNGITEM(lcode, charset) case lcode: return charset;
	LNGLIST()
	default: NEVER(); return CHARSET_DEFAULT;
	}
}

static int help_topic_seq = 0;

int  HelpTopicGetSeq()   { return help_topic_seq; }
void HelpTopicTouchSeq() { help_topic_seq++; }

void HelpTopicInfo::Set(int lang, String tit_, String txt_, String tmo_)
{
	String tmo = tmo_, tit = tit_, txt = txt_;
	int f = language.Find(lang);
	if(IsNull(tit) && IsNull(txt))
	{
		if(f < 0)
			return;
		language.Remove(f);
		module.Remove(f);
		title.Remove(f);
		text.Remove(f);
	}
	else
	{
		if(f >= 0)
		{
			if(title[f] == tit && text[f] == txt)
				return;
			title[f] = tit;
			text[f] = txt;
			module[f] = tmo;
		}
		else
		{
			language.Add(lang);
			title.Add(tit);
			text.Add(txt);
			module.Add(tmo);
		}
		if(!IsNull(tmo))
			text_folder = GetFileFolder(tmo);
	}
	HelpTopicTouchSeq();
}

String HelpTopicInfo::GetTitle(int lang) const
{
	int f = language.Find(lang);
	if(f >= 0)
		return title[f];
	return Null;
}

String HelpTopicInfo::GetText(int lang) const
{
	int f = language.Find(lang);
	if(f >= 0)
		return text[f];
	return Null;
}

String HelpTopicInfo::GetTitle() const
{
	String t = GetTitle(GetCurrentLanguage());
	if(IsNull(t))
		t = GetDefaultTitle();
	return t;
}

String HelpTopicInfo::GetText() const
{
	String t = GetText(GetCurrentLanguage());
	if(IsNull(t))
		t = GetDefaultText();
	return t;
}

String HelpTopicInfo::GetDefaultTitle() const
{
	String out;
	if(recursed)
		return "(rekurze v tématu)";
	recursed = true;
	if(titlefunc)
		try
		{
			titlefunc(out);
		}
		catch(Exc e)
		{
			out = NFormat("Chyba: [* \1%s\1]", e);
		}
	recursed = false;
	return out;
}

String HelpTopicInfo::GetDefaultText() const
{
	String out;
	if(recursed)
		return "(rekurze v tématu)";
	recursed = true;
	if(textfunc)
		try
		{
			textfunc(out);
		}
		catch(Exc e)
		{
			out = NFormat("Chyba: [* \1%s\1]\n\nVytvoøená èást textu:\n%s", e, out);
		}
	recursed = false;
	return out;
}

String HelpFormatDPP(const char *space, const char *nesting, const char *topic)
{
	return NFormat("dpp://%[::]~s//%[::]~s//%s", space, nesting, topic);
}

bool HelpParseDPP(const char *path, String& space, String& nesting, String& topic)
{
	if(path[0] == 'd' && path[1] == 'p' && path[2] == 'p' && path[3] == ':')
		path += 4;
	if(path[0] == '/' && path[1] == '/')
		path += 2;
	const char *b = path;
	while(*path && !(path[0] == '/' && path[1] == '/'))
		path++;
	space = String(b, path - b == 2 && b[0] == ':' && b[1] == ':' ? b : path);
	if(path[0] == '/' && path[1] == '/')
		path += 2;
	b = path;
	while(*path && !(path[0] == '/' && path[1] == '/'))
		path++;
	nesting = String(b, path - b == 2 && b[0] == ':' && b[1] == ':' ? b : path);
	if(path[0] == '/' && path[1] == '/')
		path += 2;
	topic = path;
	return !IsNull(space) || !IsNull(nesting) || !IsNull(topic);
}

GLOBAL_VAR(HelpTopicInfoMap, HelpTopicMap)

void HelpTopicAdd(const char *drl, const char *decl_module, Callback1<String&> titlefunc, Callback1<String&> textfunc)
{
	ASSERT(decl_module && *decl_module);
//	String module = GetFileTitle(raw_module);
	HelpTopicInfoMap& map = HelpTopicMap();
	HelpTopicInfo& info = map.GetAdd(drl);
	info.added = true;
/*	if(info.titlefunc && titlefunc && info.titlefunc != titlefunc)
		RLOG("[" << drl << "]: titlefunc mismatch, decl_module = " << decl_module);
	if(info.textfunc && textfunc && info.textfunc != textfunc)
		RLOG("[" << drl << "]: textfunc mismatch, decl_module = " << decl_module);
	if(decl_module && *decl_module && !IsNull(info.decl_module) && info.decl_module != decl_module)
		RLOG("[" << drl << "]: declaration module mismatch - '" << decl_module << "' and '" << info.decl_module << "'");
*/	if(titlefunc)
		info.titlefunc = titlefunc;
	if(textfunc)
		info.textfunc = textfunc;
	info.decl_module = Nvl(decl_module, info.decl_module);
	HelpTopicTouchSeq();
}

void HelpTopicSet(const char *drl, const char *text_module, int language, const char *title, String text)
{
	HelpTopicInfoMap& map = HelpTopicMap();
	int f = map.Find(drl);
	if(f >= 0)
	{
		if(text_module && *text_module && !IsNull(map[f].text_folder))
		{
			String tfo = GetFileFolder(text_module);
			if(tfo != map[f].text_folder)
				RLOG("[" << drl << "] text folder mismatch: " << tfo << " / " << map[f].text_folder);
		}
	}
	else
	{
		f = map.GetCount();
		HelpTopicInfo& info = map.Add(drl);
		HelpTopicTouchSeq();
	}
	map[f].Set(language, title, text, text_module);
}

void HelpTopicSet(const char *space, const char *nesting, const char *topic, const char *text_module, int language, const char *title, String text)
{
	HelpTopicSet(HelpFormatDPP(space, nesting, topic), text_module, language, title, text);
}

void HelpTopicSet(const char *space, const char *nesting, const char *topic, const char *text_module, const char *language, const char *title, String text)
{
	HelpTopicSet(space, nesting, topic, text_module, LNGFromText(language), title, text);
}

const HelpTopicInfo& HelpTopicGet(String topic)
{
	HelpTopicInfoMap& map = HelpTopicMap();
	int f = map.Find(topic);
	if(f >= 0)
		return map[f];
	static HelpTopicInfo empty;
	return empty;
}

Vector<String> HelpTopicEnumSpace()
{
	const HelpTopicInfoMap& map = HelpTopicMap();
	Index<String> spaces;
	for(int i = 0; i < map.GetCount(); i++)
	{
		String space, nesting, topic;
		HelpParseDPP(map.GetKey(i), space, nesting, topic);
		spaces.FindAdd(space);
	}
	Vector<String> splist = spaces.PickKeys();
	Sort(splist, StdLess<String>());
	return splist;
}

Vector<String> HelpTopicEnumNesting(String sp)
{
	const HelpTopicInfoMap& map = HelpTopicMap();
	Index<String> nest;
	for(int i = 0; i < map.GetCount(); i++)
	{
		String space, nesting, topic;
		HelpParseDPP(map.GetKey(i), space, nesting, topic);
		if(IsNull(sp) || sp == space)
			nest.FindAdd(nesting);
	}
	Vector<String> nelist = nest.PickKeys();
	Sort(nelist, StdLess<String>());
	return nelist;
}

Vector<String> HelpTopicEnumDeclModules()
{
	Index<String> modmap;
	const HelpTopicInfoMap& map = HelpTopicMap();
	for(int i = 0; i < map.GetCount(); i++)
		modmap.FindAdd(map[i].decl_module);
	Vector<String> mm = modmap.PickKeys();
	Sort(mm, StdLess<String>());
	return mm;
}

static void CatUnder(String& out)
{
	if(!out.IsEmpty() && *out.Last() != '_')
		out.Cat('_');
}

static void AdjustCat(String& out, const char *in, int maxlen)
{
	for(; *in && out.GetLength() < maxlen; in++)
		if(IsAlNum(*in) || *in == '_' || *in == '-' || *in == '$')
			out.Cat(*in);
		else
			CatUnder(out);
}

String HelpTopicTextModuleTitle(String space, String nesting, String topic, Index<String>& used_names)
{
	String title;
	AdjustCat(title, space, 8);
	CatUnder(title);
	if(!IsNull(nesting))
	{
		AdjustCat(title, nesting, 16);
		CatUnder(title);
	}
	AdjustCat(title, topic, 32);
	String base = title;
	int i = 0;
	while(used_names.Find(title) >= 0)
		title = base + FormatInt(++i);
	return title;
}

static String NormalizePathCase(String s)
{
#ifdef PLATFORM_WIN32
	return ToLower(s);
#else
	return s;
#endif
}

static GLOBAL_VAR(String, HelpHostDir__)

String HelpTopicHostDir() { return HelpHostDir__(); }

void HelpTopicSetHostDir(const char *dir) { HelpHostDir__() = dir; }

static String GetTextFolder(String nesting, String decl_module, String text_folder)
{
	if(nesting == "host")
		return HelpTopicHostDir();
	if(!IsNull(decl_module))
		return NormalizePathCase(GetFileFolder(decl_module));
	String tmod = text_folder;
	if(!stricmp(GetFileNamePos(tmod), "doc.dpp"))
		tmod = GetFileFolder(tmod);
	return NormalizePathCase(tmod);
}

Vector<String> HelpTopicEnumTextFolders()
{
	Index<String> out;
	const HelpTopicInfoMap& map = HelpTopicMap();
	for(int i = 0; i < map.GetCount(); i++)
	{
		String drl = map.GetKey(i);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		const HelpTopicInfo& info = map[i];
		String folder = GetTextFolder(nesting, info.decl_module, info.text_folder);
		if(out.Find(folder) < 0)
		{
			out.Add(folder);
			LLOG("HelpTopicSave: folder = " << folder);
		}
	}
	Vector<String> list = out.PickKeys();
	Sort(list, StdLess<String>());
	return list;
}

int HelpTopicGetFolderDirty(String text_folder, const HelpTopicInfoMap& diff)
{
	const HelpTopicInfoMap& map = HelpTopicMap();
	static HelpTopicInfo empty;
	int dirty = 0;
	for(int i = 0; i < map.GetCount(); i++)
	{
		String drl = map.GetKey(i);
		const HelpTopicInfo& topicinfo = map[i];
		int old = diff.Find(drl);
		const HelpTopicInfo& oldinfo = (old >= 0 ? diff[old] : empty);
		if(topicinfo == oldinfo)
			continue;
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		if(nesting == "sys")
			continue;
		String folder = GetTextFolder(nesting, topicinfo.decl_module, topicinfo.text_folder);
		if(folder == text_folder)
			dirty++;
	}
	return dirty;
}

Vector<int> HelpTopicEnumLang()
{
	Index<int> lngmap;
	const HelpTopicInfoMap& map = HelpTopicMap();
	for(int i = 0; i < map.GetCount(); i++)
	{
		const HelpTopicInfo& topic = map[i];
		for(int l = 0; l < topic.language.GetCount(); l++)
			lngmap.FindAdd(topic.language[l]);
	}
	Vector<int> lng = lngmap.PickKeys();
	Sort(lng);
	return lng;
}

Vector<String> HelpTopicList(String chksp, String chkne)
{
	Vector<String> out;
	const HelpTopicInfoMap& map = HelpTopicMap();
	if(IsNull(chksp) && IsNull(chkne))
		out <<= map.GetKeys();
	else
	{
		for(int i = 0; i < map.GetCount(); i++)
		{
			String space, nesting, topic;
			HelpParseDPP(map.GetKey(i), space, nesting, topic);
			if((IsNull(chksp) || chksp == Nvl(space, "::")) && (IsNull(chkne) || chkne == Nvl(nesting, "::")))
				out.Add(map.GetKey(i));
		}
	}
	Sort(out, GetLanguageInfo());
	return out;
}

Vector<String> HelpTopicListTextFolder(String text_folder)
{
	Vector<String> drls;
	const HelpTopicInfoMap& map = HelpTopicMap();
	for(int t = 0; t < map.GetCount(); t++)
	{
		const HelpTopicInfo& topicinfo = map[t];
		String drl = map.GetKey(t);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		String outdir = GetTextFolder(nesting, topicinfo.decl_module, topicinfo.text_folder);
		if(text_folder.IsVoid() || !ComparePath(outdir, text_folder))
			drls.Add(drl);
	}
	return drls;
}

String HelpTopicSave(Vector<String>& saved_files, String text_folder, HelpTopicInfoMap& diff, String out_folder, bool skip_file_write)
{
	Vector<String> drls = HelpTopicListTextFolder(IsNull(out_folder) ? text_folder : String::GetVoid());
	Index<String> used_names;
	HelpTopicInfoMap& map = HelpTopicMap();
	String out;
	bool first = true;
	String dir_dph;
	for(int t = 0; t < drls.GetCount(); t++) {
		String drl = drls[t];
		const HelpTopicInfo& topicinfo = HelpTopicGet(drl);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		String outdir = AppendFileName(Nvl(out_folder, text_folder), "doc.dpp");
		String title = HelpTopicTextModuleTitle(space, nesting, topic, used_names);
		for(int l = 0; l < topicinfo.language.GetCount(); l++)
			if(!IsNull(topicinfo.title[l]) || !IsNull(topicinfo.text[l])) {
				String file;
				file << title << '_' << LNGAsTextShort(topicinfo.language[l]) << ".dpx";
				String path = AppendFileName(outdir, file);
				dir_dph << "#include \"" << path << "\"\n";
				if(!skip_file_write) {
					String lng = LNGAsText(topicinfo.language[l]);
					String out;
					out << "HELP_TOPIC(" << AsCString(space)
						<< ", " << AsCString(nesting)
						<< ", " << AsCString(topic)
						<< ", " << AsCString(lng)
						<< ", " << AsCString(topicinfo.title[l]) << ")\n";
					String text = topicinfo.text[l];
					String ctext;
					const char *p = text;
					while(*p) {
						const char *b = p;
						enum { CHUNK = 100 };
						while(*p && *p++ != '\n' && p - b < CHUNK)
							;
						if(!IsNull(ctext))
							ctext << "\n\t";
						ctext << AsCString(String(b, p));
						if(ctext.GetLength() >= 5000)
						{
							out << "HELP_TEXT(\n\t" << ctext << ")\n";
							ctext = Null;
						}
					}
					if(!IsNull(ctext))
						out << "\tHELP_TEXT(\n\t" << ctext << ")\n";
					out << "HELP_END\n";
					if(first) {
						first = false;
						RealizePath(path);
					}
					if(!IsSameTextFile(LoadFile(path), out)) {
						if(!SaveFileBackup(path, out))
							throw Exc(NFormat("Nelze uložit soubor '%s'.", path));
						saved_files.Add(path);
					}
				}
				diff.GetAdd(drl) <<= topicinfo;
			}
	}
	return dir_dph;
}

void HelpTopicLoad(String text_module, String data, Index<String> *topics_loaded)
{
	const int *x = GetAllLanguages();
	Vector<int> llist;
	while(*x)
		llist.Add(*x++);
	Index<String> lnames;
	while(lnames.GetCount() < llist.GetCount())
		lnames.Add(GetLangIdent(llist[lnames.GetCount()]));
	int language = 0;
	CParser parser(data);
	while(!parser.IsEof()) {
		parser.Spaces();
		if(parser.IsId()) {
			String id = parser.ReadId();
			enum { DECL_NONE, DECL_HTB, DECL_HT, DECL_I } decl = DECL_NONE;
			if(id == "HELP_TOPIC_BEGIN")
				decl = DECL_HTB;
			else if(id == "HELP_TOPIC")
				decl = DECL_HT;
			else if(id == "ITEM")
				decl = DECL_I;
			if(decl != DECL_NONE) {
				parser.PassChar('(');
				String space = "AppDoc", nesting, topic;
				if(decl != DECL_HTB) {
					space = parser.ReadOneString();
					parser.PassChar(',');
					nesting = parser.ReadOneString();
					parser.PassChar(',');
				}
				topic = parser.ReadOneString();
				parser.PassChar(',');
				if(decl == DECL_HTB) {
					String langid = parser.ReadId();
					int lx = lnames.Find(langid);
					if(lx < 0)
						parser.ThrowError(NFormat("invalid language code: %d", language));
					language = llist[lx];
				}
				else {
					String lang = parser.ReadOneString();
					language = LNGFromText(lang);
				}
				String title;
				if(decl != DECL_I) {
					parser.PassChar(',');
					title = parser.ReadOneString();
				}
				parser.PassChar(')');
				parser.Id("EXTERNAL");
				String text;
				while(parser.IsId() && ((id = parser.ReadId()) == "HELP_TOPIC_TEXT" || id == "HELP_TEXT" || id == "TEXT")) {
					parser.PassChar('(');
					text.Cat(parser.ReadString());
					parser.PassChar(')');
				}
				if(id == "HELP_TOPIC_END") {
					parser.PassChar('(');
					parser.PassChar(')');
				}
				else if(id == "END_ITEM" || id == "HELP_END")
					;
				else
					parser.ThrowError(NFormat("HELP_TOPIC_END/END_ITEM expected, found: %s", StringSample(parser.GetPtr(), 10)));
				String drl = HelpFormatDPP(space, nesting, topic);
				HelpTopicSet(drl, text_module, language, title, text);
				if(topics_loaded)
					topics_loaded->FindAdd(drl);
			}
			else
				parser.ThrowError(NFormat("HELP_TOPIC_BEGIN/HELP_TOPIC/ITEM expected, found: %s", id));
		}
		else
			parser.ThrowError(NFormat("identifier expected, found: %s", StringSample(parser.GetPtr(), 20)));
	}
}

GLOBAL_VAR(Vector<HelpAutoIndexProc>, HelpAutoIndexMap)

void RegisterHelpAutoIndex(HelpAutoIndexProc haip)
{
	HelpAutoIndexMap().Add(haip);
}

typedef Segtor<void (*)(), 256> HelpInitList;
GLOBAL_VAR(HelpInitList, GetHelpInitList)

HelpInit::HelpInit(void (*fn)())
{
	GetHelpInitList().Add(fn);
}

void HelpInit::Run()
{
	HelpInitList& m = GetHelpInitList();
	for(int i = 0; i < m.GetCount(); i++)
		m[i]();
	m.Clear();
}

END_UPP_NAMESPACE
