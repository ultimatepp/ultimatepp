#ifndef RICHEDIT_H
#define RICHEDIT_H

#include <CtrlLib/CtrlLib.h>

namespace Upp {

#define IMAGECLASS RichEditImg
#define IMAGEFILE <RichEdit/RichEdit.iml>
#include <Draw/iml_header.h>

class RichRuler : public FrameCtrl<Ctrl> {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword);
	virtual void RightDown(Point p, dword);
	virtual void LeftDouble(Point p, dword);
	virtual void MouseMove(Point p, dword);
	virtual void LeftUp(Point, dword);

public:
	struct Marker {
		int   pos, minpos, maxpos;
		Image image;
		bool  top;
		bool  deletable;

		bool operator!=(const Marker& m) {
			return pos != m.pos || minpos != m.minpos || maxpos != m.maxpos ||
			       !image.IsSame(m.image) || top != m.top || deletable != m.deletable;
		}
		Marker() { top = false; deletable = false; }
	};

private:
	int    x0, pgcx, numbers, marks;
	double grid, numbermul;
	Zoom   zoom;

	int    track, pos;
	int    trackdx;
	int    snap;

	Array<Marker> marker;
	int           tabpos;
	int           tabsize;
	int           newtabalign;
	
public:
	Event<>       WhenLeftDouble;
	Event<>       WhenLeftDown;
	Event<>       WhenRightDown;
	Event<>       WhenBeginTrack;
	Event<>       WhenTrack;
	Event<>       WhenEndTrack;

	void          SetLayout(int x, int pgcx, Zoom zoom, double grid,
	                        int numbers = INT_MAX, double numbermul = 1, int marks = INT_MAX,
	                        int snap = 32);

	void          Clear();
	void          Set(int i, const Marker& m);
	void          SetCount(int n);
	int           GetCount() const        { return marker.GetCount(); }
	const Marker& operator[](int i)       { return marker[i]; }

	int           FindMarker(Point p);
	int           GetTrack()              { return track; }
	int           GetPos()                { return pos; }

	void          SetTabs(int pos, int size);

	int           GetNewTabAlign()        { return newtabalign; }

	RichRuler();
	virtual ~RichRuler();
};

enum {
	UNIT_DOT,
	UNIT_POINT,
	UNIT_INCH,
	UNIT_MM,
	UNIT_CM,
	
	UNIT_PIXELMODE,
};

class UnitEdit : public EditField, public Convert {
public:
	virtual Value GetData() const;
	virtual Value Format(const Value& v) const;
	virtual void  SetData(const Value& v);
	virtual bool  Key(dword key, int repcnt);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);

private:
	SpinButtons spin;
	int         unit;
	bool        sgn;

	static String AsText(double d, int unit);
	static String DotAsText(int dot, int unit);
	void Spin(int delta);
	void Read(double& q, int& u) const;
	void SyncFilter();

public:
	UnitEdit& SetUnit(int _unit)                        { unit = _unit; SyncFilter(); return *this; }
	void      Set(int _unit, int d)                     { unit = _unit; SetData(d); }
	UnitEdit& WithSgn(bool b = true);

	typedef UnitEdit CLASSNAME;

	UnitEdit();
};

struct FontHeight : public WithDropChoice<EditDouble> {
	virtual bool Key(dword key, int);
	
	FontHeight()   { MinMax(1, 72); }
};

#define LAYOUTFILE <RichEdit/RichEdit.lay>
#include <CtrlCore/lay.h>

bool EditRichHeaderFooter(String& header_qtf, String& footer_qtf, bool allow_dark, bool dark);

class RichEdit;

class ParaFormatting : public WithParaLayout<StaticRect> {
public:
	DropList n[8];

private:
	UnitEdit tabpos;
	DropList tabtype;
	DropList tabfill;
	bool     keepindent;
	Font     font;
	bool     modified;
	String   header_qtf, footer_qtf;
	bool     dark = false;
	bool     allow_dark = false;

	RichPara::NumberFormat GetNumbering();
	bool                   IsNumbering();
	int                    ComputeIndent();
	void                   SetMod()                  { modified = true; }

	typedef ParaFormatting CLASSNAME;

public:
	void  Set(int unit, const RichText::FormatInfo& formatinfo, bool baselevel = false);
	dword Get(RichText::FormatInfo& formatinfo);
	void  SetFont(Font fnt)                          { font = fnt; }
	bool  IsChanged() const                          { return IsModifiedDeep() || modified; }
	void  EnableNumbering();
	void  SetupIndent();
	void  EditHdrFtr();
	void  NewHdrFtr();
	void  SyncHdrFtr();

	ParaFormatting(const RichEdit& e);
};

class StyleManager : public WithStylesLayout<TopWindow> {
public:
	ParaFormatting            para;
	FontHeight                height;

private:
	int                       unit;
	ArrayMap<Uuid, RichStyle> style;
	Index<Uuid>               dirty;
	EditString                name;

	void EnterStyle();
	void SaveStyle();
	void SetupFont();
	void SetupFont0();
	void Create();
	void ReloadNextStyles();
	void Remove();

	void GetFont(Font& font);

	void Menu(Bar& bar);

	typedef StyleManager CLASSNAME;

public:
	void     Set(const RichText& text, const Uuid& current = Null);
	void     Set(const char *qtf);
	bool     IsChanged() const;
	void     Get(RichText& text);
	RichText Get();
	String   GetQTF();
	
	void     Setup(const Vector<int>& faces, int aunit = UNIT_DOT);

	StyleManager(const RichEdit& e);
};

void SetupFaceList(DropList& face);

class RichEdit : public Ctrl, private TextArrayOps {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& w);
	virtual bool  Key(dword key, int count);
	virtual void  LeftDown(Point p, dword flags);
	virtual void  MiddleDown(Point p, dword flags);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  LeftDrag(Point p, dword flags);
	virtual void  RightDown(Point p, dword flags);
	virtual void  LeftDouble(Point p, dword flags);
	virtual void  LeftTriple(Point p, dword flags);
	virtual void  MouseMove(Point p, dword flags);
	virtual void  LeftRepeat(Point p, dword flags);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void  CancelMode();
	virtual Image CursorImage(Point p, dword flags);
	virtual Value GetData() const;
	virtual void  SetData(const Value& v);
	virtual void  Serialize(Stream& s);
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();
	virtual String GetSelectionData(const String& fmt) const;
	virtual Point  GetPreedit();
	virtual Font   GetPreeditFont();
	virtual Rect   GetCaret() const;
	virtual void   Skin();

private:
	virtual int   GetCharAt(int64 i) const                  { return GetChar((int)i); }
	virtual int64 GetTotal() const                          { return GetLength(); }

	int                      viewborder;
	void                    *context;
	Size                     p_size;
	bool                     sizetracking;
	ScrollBar                sb;
	Scroller                 scroller;
	RichRuler                ruler;
	RichText                 text;
	RichText::FormatInfo     formatinfo;
	int                      cursor, anchor;
	bool                     begtabsel;
	RichCaret                cursorc;
	RichPos                  cursorp;
	RichPos                  anchorp;
	int                      tablesel;
	Rect                     cells;
	int                      gx;
	int                      oselh, osell;
	int                      objectpos;
	int                      zsc;
	Rect                     objectrect;
	RichHotPos               tabmove;
	int                      mpos;
	int                      undosteps;
	Rect                     dropcaret;
	bool                     selclick;
	DropList                 face;
	FontHeight               height;
	DataPusher               hyperlink;
	DataPusher               label;
	DataPusher               indexentry;
	FrameRight<Button>       gotolabel, gotoentry;
	MultiButton::SubButton  *setstyle;
	PopUpTable               gototable;
	ColorButton              ink, paper;
	ToolButton               adjustunits;
	DropList                 style;
	DropList                 language;
	Size                     pagesz;
	String                   footer;
	bool                     nolinks;
	bool                     overwrite;
	bool                     useraction, modified;
	bool                     singleline;
	static int               fixedlang;
	RichObject               bar_object;
	String                   bar_fieldparam;
	Rect                     caret;

	WithRichFindReplaceLayout<TopWindow> findreplace;

	bool                     found, notfoundfw;
	bool                     persistent_findreplace;

	VectorMap<String, Value> vars;

	int                      unit;
	Color                    showcodes;
	int                      zoom;
	bool                     spellcheck;
	bool                     paintcarect;

	int                      undoserial;
	bool                     incundoserial;
	
	Vector<int>              ffs;
	
	int                      bullet_indent;
	
	PaintInfo                paint_info;
	bool                     ignore_physical_size;
	
	bool                     pixel_mode = false;
	bool                     dark_content = false;
	bool                     allow_dark_content = false;
	
	bool                     show_zoom = false;
	
	Color                    override_paper = Null;

	static int fh[];

	struct UndoRec {
		int     serial;
		int     cursor;

		virtual void         Apply(RichText& txt) = 0;
		virtual One<UndoRec> GetRedo(const RichText& txt) = 0;

		One<UndoRec> Serial(int s) { serial = s; return this; }

		virtual ~UndoRec() {}
	};

	struct UndoInsert : UndoRec {
		int                  pos;
		int                  length;
		bool                 typing;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoInsert(int pos, int length, bool typing = false);
	};

	struct UndoRemove : UndoRec {
		int                  pos;
		RichText             text;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoRemove(const RichText& txt, int pos, int length);
	};

	struct UndoFormat : UndoRec {
		int                 pos;
		int                 length;
		RichText::Formating format;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoFormat(const RichText& txt, int pos, int length);
	};

	struct UndoStyle : UndoRec {
		Uuid                 id;
		RichStyle            style;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoStyle(const RichText& txt, const Uuid& id);
	};

	struct UndoStyles : UndoRec {
		RichStyles           styles;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoStyles(const RichText& txt);
	};

	struct UndoTableFormat : UndoRec {
		int                 table;
		RichTable::Format   format;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoTableFormat(const RichText& txt, int table);
	};

	struct UndoCreateTable : UndoRec {
		int              table;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoCreateTable(int table) : table(table) {}
	};

	struct UndoDestroyTable : UndoRec {
		int              pos;
		RichTable        table;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoDestroyTable(const RichText& txt, int pos);
	};

	struct UndoRemoveParaSpecial : UndoRec {
		int              table;
		bool             before;
		RichPara::Format format;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoRemoveParaSpecial(const RichText& txt, int table, bool before);
	};

	struct UndoInsertParaSpecial : UndoRec {
		int              table;
		bool             before;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoInsertParaSpecial(int table, bool before) : table(table), before(before) {}
	};

	struct UndoTable : UndoRec {
		int             table;
		RichTable       copy;

		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);

		UndoTable(const RichText& txt, int table);
	};
	
	struct UndoBegSelFix : UndoRec {
		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);
	};

	struct UndoBegSelUnFix : UndoRec {
		virtual void         Apply(RichText& txt);
		virtual One<UndoRec> GetRedo(const RichText& txt);
	};

	BiArray<UndoRec> undo;
	Array<UndoRec>   redo;

	FileSel          imagefs;
	
	struct StyleKey {
		Uuid   styleid;
		String stylename;
		String face;
		int    height;
		Color  ink;
		Color  paper;
		
		StyleKey();
	};
	
	StyleKey   stylekey[20];
	
	Zoom       clipzoom;
	
	double     floating_zoom;
	
	RichPara::CharFormat last_format;
	Image      last_format_img;

	Size       GetZoomedPage() const;
	int        GetPosY(PageY py) const;
	void       GetPagePoint(Point p, PageY& py, int& x);
	int        GetX(int x);
	int        GetSnapX(int x);
	PageY      GetPageY(int y) const;
	int        GetNearestPos(int x, PageY py);
	void       SetSb();
	void       Scroll();
	void       SetZsc();
	Rect       PlaceCaret();
	void       FinishNF();
	void       Finish();
	void       ReadFormat();
	void       ShowFormat();
	void       SetLastCharFormat(const RichPara::CharFormat& fmt);
	void       SetLastCharFormat();
	void       LastCharFormat();
	int        GetMousePos(Point p);
	RichHotPos GetHotPos(Point p);
	int        GetHotSpot(Point p) const;
	Rect       GetObjectRect(int pos) const;
	void       FixObjectRect();
	bool       RemoveBullet(bool backspace);

	void       SetObjectPos(int pos);
	void       AdjustObjectSize();
	void       SetObjectPercent(int p);
	void       SetObjectYDelta(int pt);
	void       SetFace();
	void       SetHeight();

	bool       SelBeg(bool select);
	bool       SelEnd(bool select);
	void       SelCell(int dx, int dy);

	void       Limit(int& pos, int& count);
	bool       InvalidRange(int c1, int c2);
	void       NextUndo()                 { undoserial += incundoserial; incundoserial = false; useraction = true; }
	void       AddUndo(One<UndoRec>&& ur);

	void       BeginRulerTrack();
	void       RulerTrack();
	void       IndentMark();
	void       HighLightTab(int r);
	void       TabAdd(int align);
	void       AddTab();
	void       TabMenu();

	void       Hyperlink();
	void       Label();
	void       IndexEntry();
	void       GotoLbl();
	void       GotoEntry();
	void       GotoType(int type, Ctrl& l);
	void       Goto();

	void       SetInk();
	void       SetPaper();
	void       SetLanguage();
	void       Language();
	void       SetupLanguage(Vector<int>&& lng);

	void       SetBullet(int bullet);

	void       SetupRuler();

	void       ReadStyles();

	void       SetStyle();

	void       EndSizeTracking();

	RichObject GetObject() const;
	void       ReplaceObject(const RichObject& obj);

	static bool   IsW(int c);

	void Insert(int pos, const RichText& text, bool typing = false);
	void Remove(int pos, int len, bool forward = false);
	void SaveFormat(int pos, int count);
	void SaveFormat();
	void ModifyFormat(int pos, const RichText::FormatInfo& fi, int count);
	void SetParaFormat(dword paravalid);

	void SaveStylesUndo();
	void SaveStyleUndo(const Uuid& id);

	bool CursorKey(dword key, int count);

	void ApplyFormat(dword charvalid, dword paravalid = 0);

	void MoveNG(int newpos, bool select);
	void MoveUpDown(int dir, bool select, int firststep = 0);
	void MovePageUpDown(int dir, bool select);
	void MoveWordRight(bool select);
	void MoveWordLeft(bool select);
	void MoveHomeEnd(int dir, bool select);

	void InsertLine();

	void Bold();
	void Italic();
	void Underline();
	void Strikeout();
	void Capitals();
	void SetScript(int script);

	void AlignLeft();
	void AlignRight();
	void AlignCenter();
	void AlignJustify();

	void Style();

	void ParaFormat();
	void ToPara();

	void ZoomView(int d);
	void SetupUnits();
	void SpellCheck();

	void    AddUserDict();
	WString GetWordAtCursor();
	void    GetWordAtCursorPos(int& pos, int& count);

	Rect     GetCaretRect(const RichCaret& pr) const;
	Rect     GetCaretRect() const;

	void     SaveTableFormat(int table);
	void     SaveTable(int table);
	void     InsertTable();
	void     DestroyTable();
	void     TableProps();
	bool     RemoveSpecial(int ll, int hh, bool back);
	bool     InsertLineSpecial();
	void     TableInsertRow();
	void     TableRemoveRow();
	void     TableInsertColumn();
	void     TableRemoveColumn();
	void     SplitCell();
	void     JoinCell();
	void     CellProperties();

	void     SetupFindReplace0();
	void     SetupFindReplace();
	void     OpenFindReplace();
	void     CloseFindReplace();
	int      FindPos(bool back);
	RichText ReplaceText();
	void     Find(bool back);
	void     Replace();
	void     FindReplaceAddHistory();

	void     Reset();

	void     DoRefreshBar();
	void     RefreshBar();

	bool     Accept(PasteClip& d, RichText& clip, String& fmt);
	void     ClipPaste(RichText& clip, const String& fmt);
	bool     InSelection(int& c) const;
	void     RefreshDropCaret();
	void     ZoomClip(RichText& text) const;
	
	void     InsertImage();
	void     InsertDiagram();

	RichObject Adjust(RichObject o);

	void     StyleKeys();
	void     ApplyStyleKey(int i);
	
	void     HeaderFooter();
	bool     EditHeaderFooter(String& header_qtf, String& footer_qtf);

	bool     BegSelTabFix(int& count);
	bool     BegSelTabFix()                        { int dummy; return BegSelTabFix(dummy); }
	void     BegSelTabFixEnd(bool fix);

	Size     GetPhysicalSize(const RichObject& obj);

	struct DisplayDefault : public Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q,
		                   Color ink, Color paper, dword style) const;
	};

	void UserAction();
	Event<>  User(Event<>  cb);

	static void   SpellerAdd(const WString& w, int lang);
	static int    CompareStyle(const Value& a, const Value& b);

	friend class StyleKeysDlg;
	friend class StyleManager;
	friend class ParaFormatting;

	using Ctrl::Accept;

protected:
	enum {
		TIMEID_ENDSIZETRACKING = Ctrl::TIMEID_COUNT,
		TIMEID_REFRESHBAR,
		TIMEID_COUNT
	};

public:
	virtual void  PasteFilter(RichText& txt, const String& fmt);
	virtual void  Filter(RichText& txt);

	static double DotToPt(int dot, int unit = UNIT_DOT);
	static int    PtToDot(double pt, int unit = UNIT_DOT);
	static Bits   SpellParagraph(const RichPara& p);
	static void   FixedLang(int lang)              { fixedlang = lang; }

	Event<>                  WhenRefreshBar; // Toolbar should be updated
	Event<>                  WhenStartEvaluating;
	Event<String&, WString&> WhenHyperlink;
	Event<String&>           WhenLabel;
	Event<String&>           WhenIndexEntry;
	Event<Bar&>              WhenBar; // Context menu
	Event<>                  WhenSel;
	Gate<const String&>      WhenIsLink;
	Event<const String&>     WhenLink;

	void   StdBar(Bar& menu);

	void   SerializeSettings(Stream& s);

	Zoom   GetZoom() const;

	int    GetCursor() const                       { return cursor; }
	int    GetAnchor() const                       { return anchor; }

	void     Select(int pos, int count);
	bool     IsSelection() const;
	bool     GetSelection(int& l, int& h) const;
	RichText GetSelection(int maxlen = INT_MAX) const;
	void     SetSelection(int l, int h)            { Select(l, h - l); }
	bool     RemoveSelection(bool back = false);
	void     CancelSelection();

	void   BeginOp()                               { NextUndo(); }

	int                  GetLength() const         { return text.GetLength(); }
	void                 PasteText(const RichText& text);
	void                 RemoveText(int count);
	RichText             CopyText(int pos, int count) const;
	RichText::FormatInfo GetFormatInfo() const     { return formatinfo; }
	void                 ApplyFormatInfo(const RichText::FormatInfo& fi);
	int                  GetChar(int pos) const    { return text[pos]; }
	int                  operator[](int pos) const { return text[pos]; }

	Rect   GetTextRect() const;

	void   Undo();
	void   Redo();

	void   ScrollToCursor()                     { Finish(); }

	void   Move(int newpos, bool select = false);

	void   Copy();
	void   Cut();
	void   Paste();
	void   InsertObject(int type);

	void   Styles();

	bool   Print();
	void   DoPrint()                             { Print(); }

	void   StyleTool(Bar& bar, int width = Zx(120));
	void   FaceTool(Bar& bar, int width = Zx(130));
	void   HeightTool(Bar& bar, int width = Zx(50));
	void   BoldTool(Bar& bar, dword key = K_CTRL_B);
	void   ItalicTool(Bar& bar, dword key = K_CTRL_I);
	void   UnderlineTool(Bar& bar, dword key = K_CTRL_U);
	void   StrikeoutTool(Bar& bar, dword key = 0);
	void   CapitalsTool(Bar& bar, dword key = 0);
	void   SuperscriptTool(Bar& bar, dword key = 0);
	void   SubscriptTool(Bar& bar, dword key = 0);
	void   InkTool(Bar& bar);
	void   PaperTool(Bar& bar);
	void   LastFormatTool(Bar& bar, dword key = K_CTRL_D);
	void   LanguageTool(Bar& bar, int width = Zx(60));
	void   HyperlinkTool(Bar& bar, int width = Zx(180), dword key = 0, const char *tip = NULL);
	void   SpellCheckTool(Bar& bar);
	void   IndexEntryTool(Bar& bar, int width = Zx(80), dword key = 0, const char *tip = NULL);

	void   LeftTool(Bar& bar, dword key = K_CTRL_L);
	void   RightTool(Bar& bar, dword key = K_CTRL_R);
	void   CenterTool(Bar& bar, dword key = K_CTRL_E);
	void   JustifyTool(Bar& bar, dword key = K_CTRL_J);
	void   RoundBulletTool(Bar& bar, dword key = 0);
	void   RoundWhiteBulletTool(Bar& bar, dword key = 0);
	void   BoxBulletTool(Bar& bar, dword key = 0);
	void   BoxWhiteBulletTool(Bar& bar, dword key = 0);
	void   TextBulletTool(Bar& bar, dword key = 0);
	void   ParaFormatTool(Bar& bar, dword key = 0);
	void   LabelTool(Bar& bar, int width = Zx(80), dword key = 0, const char *tip = NULL);
	void   ToParaTool(Bar& bar, dword key = K_CTRL_K);

	void   UndoTool(Bar& bar, dword key = K_CTRL_Z);
	void   RedoTool(Bar& bar, dword key = K_SHIFT_CTRL_Z);
	void   CutTool(Bar& bar, dword key = K_CTRL_X);
	void   CopyTool(Bar& bar, dword key = K_CTRL_C);
	void   PasteTool(Bar& bar, dword key = K_CTRL_V);
	void   PastePlainTextTool(Bar& bar, dword key = K_CTRL_V|K_SHIFT);
	void   ObjectTool(Bar& bar, dword key = 0);
	void   LoadImageTool(Bar& bar, dword key = 0);
	void   InsertDiagramTool(Bar& bar, dword key = 0);
	void   FindReplaceTool(Bar& bar, dword key = K_CTRL_F);

	void   InsertTableTool(Bar& bar, dword key = K_CTRL_F12);
	void   TablePropertiesTool(Bar& bar, dword key = K_SHIFT_F12);
	void   InsertTableRowTool(Bar& bar, dword key = 0);
	void   RemoveTableRowTool(Bar& bar, dword key = 0);
	void   InsertTableColumnTool(Bar& bar, dword key = 0);
	void   RemoveTableColumnTool(Bar& bar, dword key = 0);
	void   SplitJoinCellTool(Bar& bar, dword key = 0);
	void   CellPropertiesTool(Bar& bar, dword key = 0);

	void   PrintTool(Bar& bar, dword key = K_CTRL_P);

	void   FontTools(Bar& bar);
	void   ParaTools(Bar& bar);
	void   EditTools(Bar& bar);
	void   TableTools(Bar& bar);

	void   InsertImageTool(Bar& bar);
	void   StyleKeysTool(Bar& bar);
	
	void   HeaderFooterTool(Bar& bar);

	void   DefaultBar(Bar& bar, bool extended = true);

	void            SetVar(const String& id, const Value& v) { vars.GetAdd(id) = v; }
	Value           GetVar(const String& id) const           { return vars.Get(id, Value()); }
	void            EvaluateFields();

	bool            GotoLabel(Gate<const WString&> match);
	bool            GotoLabel(const String& lbl);
	void            BeginPara();
	void            NextPara();
	void            PrevPara();

	void            Clear();
	void            Pick(RichText pick_ t);
	void            SetQTF(const char *qtf)                { Pick(ParseQTF(qtf, 0, context)); }
	const RichText& Get() const                            { return text; }
	String          GetQTF(byte cs = CHARSET_UTF8) const   { return AsQTF(text, cs); }
	void            ApplyStylesheet(const RichText& r);
	void            SetPage(const Size& sz)                { pagesz = sz; Finish(); }
	Size            GetPage()                              { return pagesz; }
	bool            IsDarkContent() const;
	void            SetupDark(ColorPusher& c) const;

	RichEdit&       NoRuler()                              { RemoveFrame(ruler); return *this; }
	RichEdit&       SingleLine(bool b = true)              { singleline = b; return *this; }
	RichEdit&       FontFaces(const Vector<int>& face);
	RichEdit&       ViewBorder(int cx)                     { viewborder = cx; Refresh(); return *this; }
	RichEdit&       ShowCodes(Color c)                     { showcodes = c; Refresh(); return *this; }
	RichEdit&       Unit(int u)                            { unit = u; Refresh(); return *this; }
	RichEdit&       SpellCheck(bool b)                     { spellcheck = b; Refresh(); return *this; }
	RichEdit&       SetZoom(int z)                         { zoom = z; Refresh(); return *this; }
	RichEdit&       SetContext(void *ctx)                  { context = ctx; Refresh(); return *this; }
	void           *GetContext() const                     { return context; }
	RichEdit&       ClipZoom(Zoom z)                       { clipzoom = z; return *this; }
	RichEdit&       ClipZoom(int m, int d)                 { clipzoom = Zoom(m, d); return *this; }
	Zoom            GetClipZoom() const                    { return clipzoom; }
	RichEdit&       BulletIndent(int i)                    { bullet_indent = i; return *this; }
	RichEdit&       PersistentFindReplace(bool b = true)   { persistent_findreplace = b; return *this; }
	RichEdit&       Floating(double zoomlevel_ = 1);
	RichEdit&       NoFloating()                           { return Floating(Null); }
	RichEdit&       SetPaintInfo(const PaintInfo& pi)      { paint_info = pi; return *this; }
	RichEdit&       IgnorePhysicalObjectSize(bool b = true){ ignore_physical_size = b; return *this; }
	RichEdit&       PixelMode();
	RichEdit&       DarkContent(bool b = true);
	RichEdit&       AllowDarkContent(bool b = true);
	RichEdit&       OverridePaper(Color p);

	struct UndoInfo {
		int              undoserial;
		BiArray<UndoRec> undo;
		Array<UndoRec>   redo;
		
		void Clear()     { undo.Clear(); redo.Clear(); undoserial = 0; }
	};

	struct PosInfo {
		int  cursor;
		int  anchor;
		bool begtabsel;
		int  zsc;

		void Serialize(Stream& s);
	};

	UndoInfo PickUndoInfo();
	void     SetPickUndoInfo(UndoInfo pick_ f);

	PosInfo  GetPosInfo() const;
	void     SetPosInfo(const PosInfo& pos);

	void     SetFooter(const String& s)                   { footer = s; }
	void     PrintNoLinks(bool b = true)                  { nolinks = b; }

	Event<>  WhenLeftUp;
	
	typedef RichEdit CLASSNAME;

	RichEdit();
	virtual ~RichEdit();
};

class RichEditWithToolBar : public RichEdit {
public:
	virtual void Skin();

private:
	ToolBar  toolbar;
	bool     extended;
	void RefreshBar();
	void TheBar(Bar& bar);

public:
	RichEditWithToolBar& Extended(bool b = true) { extended = b; return *this; }
	RichEditWithToolBar& NoExtended()            { return Extended(false); }

	typedef RichEditWithToolBar CLASSNAME;

	RichEditWithToolBar();
};

void AppendClipboard(RichText&& txt);

#include "DiagramEditor.h"

}

#endif
