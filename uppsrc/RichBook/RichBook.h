#ifndef _RichBook_RichBook_h
#define _RichBook_RichBook_h

#include <RichText/RichText.h>

NAMESPACE_UPP

RichPara::FieldType& FieldTypeVar();
Id                   FieldTypeVarID();

class RichIndexEntry : DeepCopyOption<RichIndexEntry>
{
public:
	RichIndexEntry() {}
	RichIndexEntry(const RichIndexEntry& rti, bool deep) : reflist(rti.reflist, deep) {}

public:
	Vector<String> reflist;
};

RichPara::FieldType& FieldTypeIndexEntry();
Id                   FieldTypeIndexEntryID();
String               EncodeIndexEntry(const RichIndexEntry& idx);
RichIndexEntry       DecodeIndexEntry(String encoded_indexentry);

class RichBookSection {
public:
	RichBookSection();

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

class RichPrintSection : public RichBookSection {
public:
	RichPrintSection(const RichBookSection& s, pick_ RichText& t)
		: RichBookSection(s), text(t) {}

	Vector<Drawing>      Print(Size pagesize, Gate2<int, int> progress, int& progress_pos) const;

public:
	RichText text;
};

class RichTocEntry {
public:
	RichTocEntry(String topic = Null, int level = 1, bool numbered = true, bool appendix = false, WString text = Null)
		: topic(topic), level(level), numbered(numbered), text(text) {}

	String topic;
	int level;
	bool numbered;
	bool appendix;
	WString text;
};

class RichToc : DeepCopyOption<RichToc> {
public:
	RichToc() {}
	RichToc(const RichToc& rtoc, int deep) : entries(rtoc.entries, deep) {}

	void Add(String topic, int level, bool numbered = true, bool appendix = false, WString text = Null)
	{ entries.Add(new RichTocEntry(topic, level, numbered, appendix, text)); }

	Array<RichTocEntry> entries;
};

void MakeOnlineToc(RichText& out, const RichToc& toc);
void MakeStdToc(RichText& out, const RichToc& toc, int print_width);
void MakeTocTopics(RichText& out, const RichToc& toc);
//RichText MakeStdIndex(

class RichBook {
public:
	RichBook(Size page_size = Size(3968, 6074));

	RichPrintSection&        AddSection();

	Vector<Drawing>          Print(Size pagesize, Gate2<int, int> progress);

public:
	Size                     page_size;
	RichBookSection          default_section;
	RichStyles               default_styles;
	enum { TOC_DEPTH = 4 };
	Array<RichPrintSection>  sections;
};

void   CreateHelpBook(RichBook& book);
String CreateHelpPDF(RichBook& book);
String CreateHelpRTF(RichBook& book, byte charset);

class RichBookHtml {
public:
	RichBookHtml();

	void     AddFile(String url, String data);
	void     AddImage(String url, String data);
	String   GetURLFile(String url) const;

public:
	String output_path;
	enum { STYLE_DEEP, STYLE_FLAT, STYLE_ONEFILE };
	int style;
	VectorMap<String, String> files;
	VectorMap<String, String> images;
};

void CreateHelpHtml(const RichBook& book, RichBookHtml& html);

Callback1<RichBook&>& DefaultBook();

void CreateDefaultBook();
void CreateDefaultPDF();
void CreateDefaultRTF();
void CreateDefaultHTML();

void HelpBookMenu(Bar& bar);

END_UPP_NAMESPACE

#endif
