NAMESPACE_UPP

class HelpInit
{
public:
	HelpInit(void (*fn)());
	static void Run();
};

#define HELPINITBLOCK \
static void MK__s(); \
static HelpInit COMBINE(MK__s, _init)(&MK__s); \
static void MK__s()

String               GetLangIdent(int language);
byte                 GetLangStdCharset(int language);

class HelpTopicInfoDeep
{
public:
	HelpTopicInfoDeep() : added(false), recursed(false) {}

	String             decl_module;
	String             text_folder;

	Callback1<String&> titlefunc;
	Callback1<String&> textfunc;

	bool               added;
	mutable bool       recursed;
};

class HelpTopicInfo : public HelpTopicInfoDeep, MoveableAndDeepCopyOption<HelpTopicInfo>
{
public:
	HelpTopicInfo() {}
	HelpTopicInfo(const HelpTopicInfo& h, int deep)
		: HelpTopicInfoDeep(h), language(h.language, deep)
		, title(h.title, deep), text(h.text, deep), module(h.module, deep) {}

	void               Set(int language, String title, String text, String text_module = Null);
	String             GetTitle(int language) const;
	String             GetText(int language) const;
	String             GetTitle() const;
	String             GetText() const;
	String             GetDefaultTitle() const;
	String             GetDefaultText() const;

	Index<int>         language;
	Vector<String>     title;
	Vector<String>     text;
	Vector<String>     module;
};

bool        operator == (const HelpTopicInfo& a, const HelpTopicInfo& b);
inline bool operator != (const HelpTopicInfo& a, const HelpTopicInfo& b) { return !(a == b); }

typedef ArrayMap<String, HelpTopicInfo> HelpTopicInfoMap;

HelpTopicInfoMap&    HelpTopicMap();
void                 HelpTopicAdd(const char *drl, const char *decl_module, Callback1<String&> titlefunc, Callback1<String&> textfunc);
Vector<String>       HelpTopicEnumSpace();
Vector<String>       HelpTopicEnumNesting(String space = Null);
Vector<String>       HelpTopicEnumDeclModules();
String               HelpTopicTextModule(String decl_module, String drl, int language);
Vector<String>       HelpTopicEnumTextFolders();
int                  HelpTopicGetFolderDirty(String text_folder, const HelpTopicInfoMap& diff);
Vector<int>          HelpTopicEnumLang();
Vector<String>       HelpTopicList(String space = Null, String nesting = Null);
Vector<String>       HelpTopicListTextFolder(String text_folder);
String               HelpTopicSave(Vector<String>& saved_files, String text_folder, HelpTopicInfoMap& diff, String out_folder, bool skip_file_write = false);
void                 HelpTopicLoad(String text_module, String data, Index<String> *topics_loaded = NULL);
void                 HelpTopicSet(const char *drl, const char *text_module, int language, const char *title, String text);
void                 HelpTopicSet(const char *space, const char *nesting, const char *topic, const char *text_module, int language, const char *title, String text);
void                 HelpTopicSet(const char *space, const char *nesting, const char *topic, const char *text_module, const char *language, const char *title, String text);
const HelpTopicInfo& HelpTopicGet(String drl);
int                  HelpTopicGetSeq();
void                 HelpTopicTouchSeq();
String               HelpTopicHostDir();
void                 HelpTopicSetHostDir(const char *dir);

String               HelpFormatDPP(const char *space, const char *nesting, const char *topic);
bool                 HelpParseDPP(const char *path, String& space, String& nesting, String& topic);

inline String        HelpAppDPP(const char *topic)    { return HelpFormatDPP("AppDoc", "", topic); }
inline String        HelpHostDPP(const char *topic)   { return HelpFormatDPP("AppDoc", "host", topic); }

typedef void (*HelpAutoIndexProc)(Index<WString>& entries, Vector<String>& drls);

Vector<HelpAutoIndexProc>&       HelpAutoIndexMap();
void                             RegisterHelpAutoIndex(HelpAutoIndexProc haip);

#define RegisterHelpTopicRawInfoTag(tag, drl, dclmod, tfn, xfn) \
	static void COMBINE(HelpTopicInfo, tag)() { HelpTopicAdd(drl, dclmod, tfn, xfn); } \
	static HelpInit COMBINE(HelpTopicInfoCallinit, tag)(&COMBINE(HelpTopicInfo, tag));

#define RegisterHelpTopicInfoTag(tag, top, dclmod, tfn, xfn) \
	RegisterHelpTopicRawInfoTag(tag, "dpp://AppDoc//:://" top, dclmod, tfn, xfn)

#define RegisterHelpTopicInfo(top, dclmod, tfn, xfn) \
	RegisterHelpTopicInfoTag(MK__s, top, dclmod, tfn, xfn)

#define RegisterHelpTopicInfoTitleTag(tag, top, dclmod, tit, xfn) \
	static void COMBINE(HelpTopicInfoTitle, tag)(String& out) { out.Cat(tit); } \
	RegisterHelpTopicInfoTag(tag, top, dclmod, callback(&COMBINE(HelpTopicInfoTitle, tag)), xfn)

#define RegisterHelpTopicInfoTitle(top, dclmod, tit, xfn) \
	RegisterHelpTopicInfoTitleTag(MK__s, top, dclmod, tit, xfn)

#define RegisterHelpTopic(top) RegisterHelpTopicInfo(top, __FILE__, CNULL, CNULL)

#define RegisterHelpTopicTitle(top, tit) RegisterHelpTopicInfoTitle(top, __FILE__, tit, CNULL)

#define RegisterHelpTopicObjectTitle(cls, title) \
static void COMBINE(cls, HelpTitleProc)(String& out) { out.Cat(title); } \
inline String COMBINE(cls, HelpTitle)() { String temp; COMBINE(cls, HelpTitleProc)(temp); return temp; } \
RegisterHelpTopicInfo(ASSTRING(cls), __FILE__, callback(&COMBINE(cls, HelpTitleProc)), CNULL)

#define RegisterHelpTopicHost(top, tit, gen) \
	static void COMBINE(HelpTopicInfoTitle, MK__s)(String& out) { out.Cat(tit); } \
	RegisterHelpTopicRawInfoTag(MK__s, "dpp://AppDoc//host//" top, __FILE__, callback(&COMBINE(HelpTopicInfoTitle, MK__s)), gen)

#define RegisterHelpTopicSys(top, tit, gen) \
	static void COMBINE(HelpTopicInfoTitle, MK__s)(String& out) { out.Cat(tit); } \
	RegisterHelpTopicRawInfoTag(MK__s, "dpp://AppDoc//sys//" top, __FILE__, callback(&COMBINE(HelpTopicInfoTitle, MK__s)), gen)

#define HELP_TOPIC_BEGIN(topic_id, language, title) \
	HelpTopicSet("dpp://AppDoc//:://" topic_id, __FILE__, language, title, String().Cat()

#define HELP_TOPIC(namespace_id, nesting_id, topic_id, language, title) \
	HelpTopicSet(namespace_id, nesting_id, topic_id, __FILE__, language, title, String().Cat()

#define HELP_TOPIC_TEXT(text) << text
#define HELP_TOPIC_END() );

#define HELP_TEXT(text) << text
#define HELP_END );

END_UPP_NAMESPACE
