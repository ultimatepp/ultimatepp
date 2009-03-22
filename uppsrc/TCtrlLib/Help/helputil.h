NAMESPACE_UPP

inline String        HelpDPPStyle()                   { return HelpHostDPP("style"); }
inline String        HelpDPPTopics()                  { return HelpHostDPP("topics"); }
inline String        HelpDPPTopicsAuto()              { return HelpHostDPP("topics_auto"); }
inline String        HelpDPPIndex()                   { return HelpHostDPP("index"); }
inline String        HelpDPPObjects()                 { return HelpHostDPP("objects"); }

String               MQTFStrip(const char *text);
WString              MQTFStrip(const wchar *text);
void                 MQTFCat(RichPara& para, const char *mqtf_text, const RichPara::CharFormat& basefmt);

RichObjectType      *RichObjectTypeEmpty();
RichObjectType      *RichObjectTypeDrawing();
RichObjectType      *RichObjectTypeGIF();
RichObjectType      *RichObjectTypeFormula();

String               GetRichObjectTypeDrawingData(const Drawing& drawing);
String               GetRichObjectTypeGIFData(const Image& image);

void                 CatRichText(RichText& text, const RichText& text2);
void                 AppendQTF(RichText& text, String qtf);
const Display&       RichTextDisplay();

void                 RegisterHelpFieldTypes();

RichPara::FieldType& FieldTypeLink();
Id                   FieldTypeLinkID();
String               EncodeLink(String target, String text, String style);
void                 DecodeLink(String link, String& target, String& text, String& style);

class RichTextSection
{
public:
	RichTextSection();

	void                 MakeDefault();
	void                 Override(RichTextSection& old) const;

	Rect                 GetPageRect(Size pagesize) const;
	Rect                 GetTextRect(Size pagesize, int columnindex) const;
	Size                 GetTextSize(Size pagesize) const;

public:
	bool                 nested;
	int                  columns;
	int                  firstpage;
	Rect                 margin;
	int                  header_space;
	int                  footer_space;
	int                  column_space;
	String               header[3], footer[3];
	RichPara::CharFormat charformat;
};

RichPara::FieldType& FieldTypeSection();
Id                   FieldTypeSectionID();
String               EncodeSection(const RichTextSection& sec);
RichTextSection      DecodeSection(String sect);

class RichTextIndex
{
public:
	RichTextIndex() {}

public:
	Vector<String> reflist;
};

RichPara::FieldType& FieldTypeIndex();
Id                   FieldTypeIndexID();
String               EncodeIndex(const RichTextIndex& idx);
RichTextIndex        DecodeIndex(String packed);

struct PrintSection : RichTextSection
{
	RichText text;
};

Array<PrintSection>  RichTextSplitSections(const RichText& text, Gate2<int, int> progress);
Vector<Drawing>      SectionPrintBook(Array<PrintSection>& sections, Size pagesize, VectorMap<String, Value>& vars, Gate2<int, int> progress);

void                 CreateHelpBook();
String               CreateHelpPDF();
String               CreateHelpRTF();

String               AsCStringWrap(const char *s, int linemax, const char *prefix);

void                 RegisterHelpStdItems();
void                 RegisterHelpMenuItems(Callback1<Bar&> barproc, String ident, String module);

const RichText::FormatInfo& NoFormat();
RichText             HelpTopicExpand(String style_qtf, String title_qtf, String topic_qtf,
	String title_style_name, bool nested, int depth, const RichText::FormatInfo& formatinfo,
	String inline_link_style, Gate2<int, int> progress, int& progress_step);
RichText             HelpTopicExpand(String drl, String title_style_name,
	const RichText::FormatInfo& formatinfo = NoFormat(), Gate2<int, int> progress = false);

struct HelpIndexFormattedEntry
{
	int            nesting; // -1 = new letter, 0, 1 ... = normal nesting
	WString        full_text;
	WString        text;
	String         topics; // topic;topic;topic;!important_topic;!important_topic
};

void                           HelpTopicGetIndex(Index<WString>& entries, Vector<String>& drls, Gate2<int, int> progress = CNULL);
Array<HelpIndexFormattedEntry> HelpTopicFormatIndex();

class RichObjectDisplay : public Display
{
public:
	RichObjectDisplay(const RichObjectType *type = NULL, void *context = NULL) : type(type), context(context) {}

	void                  SetType(const RichObjectType *t) { type = t; }
	const RichObjectType *GetType() const                  { return type; }

	void                  SetContext(void *t)              { context = t; }
	 void                *GetContext() const               { return context; }

	virtual void          Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const;
	virtual Size          GetStdSize(const Value& v) const;

private:
	const RichObjectType *type;
	void                 *context;
};

END_UPP_NAMESPACE
