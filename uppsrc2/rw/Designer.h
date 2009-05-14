#include <CtrlLib/CtrlLib.h>

#define  LAYOUTFILE <rw/Designer.lay>
#include <CtrlCore/lay.h>

#define  IMAGEFILE <rw/rw.iml>
#include <Draw/iml_header.h>

const Display& VarLblDisplay(bool selected);

typedef Ctrl *(*CtrlCreator)();

class AliasMap {
public:
	AliasMap() {}

	static String  StdFileName();

	bool           IsEmpty() const { return obj_source.IsEmpty(); }

	bool           Edit();

	bool           Load(const char *path);
	bool           Save(const char *path);
	void           SetDefault();

	String         Convert(const String& name) const;

public:
	Vector<String> obj_source;
	Vector<String> obj_dest;
};

class SelectedDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const;
};

extern const SelectedDisplay SelDisplay;

class LayoutDesigner : public Ctrl {
public:
	virtual void  Paint(Draw& w);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  LeftRepeat(Point p, dword keyflags);
	virtual void  LeftUp(Point p, dword keyflags);
	virtual void  RightDown(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual bool  Key(dword key, int count);
	virtual void  Layout();
	virtual void  Serialize(Stream& s);

protected:
	enum {
		LEFT,
		CENTER,
		RIGHT,
		TOP     = LEFT,
		BOTTOM  = RIGHT,
		FORM,
		SELECTRECT,

		TRACKERSIZE = 6,

		SLEFT = 0,
		SRIGHT,
		STOP,
		SBOTTOM,
	};

	enum ITEMPOS
	{
		POS_NONE,
		POS_AUTO,
		POS_LEFT,
		POS_CENTER,
		POS_RIGHT,
		POS_SIZE,
		POS_TOP = POS_LEFT,
		POS_BOTTOM = POS_RIGHT,
	};

	struct ItemInfo {
		LogPos         pos;
		String         classname;
		String         variable;
		String         labelid;
		String         helpid;
		Vector<String> label;
		Vector<String> help;
		dword          hotkey;
	};

	struct Form {
		String          name;
		Size            size;
		Array<ItemInfo> item;
	};

	Array<Form> layout;
	int         layouti;
	int         langi;

	DropList         layoutlist;
	DropList         langlist;

	CallbackArg<int>   classset;
	Point              newctrl;

	WithGridLayout<TopWindow> grid;
	bool        usegrid;
	bool        ignoremin;
	bool        qlib2_output;

	WithMatrixLayout<TopWindow> matrix;

	String        filename;
	FileSel       filesel;
	LRUList       LruList;

	AliasMap      alias_map;
	String        alias_map_file;

	Rect          selectrect;
	Point         track;
	Vector<int>   itemi;
	Vector<Point> melta;
	Point         delta;
	bool          moved;
	bool          cltm;

	Ctrl            design;
	Splitter        splital;

	ArrayCtrl       itab;

	Splitter                   props;
	Splitter                   label_help_split;
	Ctrl                       class_var_lbl;
	WithDropChoice<EditString> classname;
	EditString                 variable;
	DocEdit                    label;
	DocEdit                    help;

	struct DesignForm;
	friend struct DesignForm;
	struct DesignForm : public Ctrl {
		virtual void Paint(Draw& w)                               { ld->PaintForm(w); }
		LayoutDesigner *ld;
	};

	DesignForm      form;
	Array<Ctrl>     ctrl;
	Array<Ctrl>     ref_ctrl;
	Array<Ctrl>     mnem_ctrl;

	ScrollBars      sb;

	struct AfterPaint;
	friend struct AfterPaint;

	struct AfterPaint : public Ctrl {
		virtual void Paint(Draw& w)                               { ld->PaintTrack(w); }
		LayoutDesigner *ld;
	};

	AfterPaint  apaint;

	MenuBar     menubar;
	ToolBar     toolbar;
	FrameRight<Label> status;

	static VectorMap<String, CtrlCreator>& ClassMap();

	struct Frame;
	friend struct Frame;
	struct Frame : public TopWindow {
		void WindowClose()       { ld->CloseFile(); }
		virtual bool Key(dword k, int c) { return ld->Key(k, c) ? true : TopWindow::Key(k, c); }
		LayoutDesigner *ld;

		Frame() { WhenClose = callback(this, &Frame::WindowClose); }
	};

	Frame frame;

	static int   GetTrack(int a, int b, int type);
	static Image GetTrackShape(Point track);

	String&   Lng(Vector<String>& x)          { return x.At(langi); }
	String    Lng(const Vector<String>& x) const;
	Form&     ActualForm()                    { return layout[layouti]; }
	const Form& ActualForm() const            { return layout[layouti]; }
	ItemInfo& Item(int i)                     { return ActualForm().item[i]; }
	const ItemInfo& Item(int i) const         { return ActualForm().item[i]; }
	int       ItemCount()                     { return ActualForm().item.GetCount(); }
//G++ bug
	String&   ItemLabel(int i);
	String    ItemLabel(int i) const;
	String&   Help(int i);
	String    Help(int i) const;

	Ctrl     *CreateCtrl(const String& classname);
	Ctrl     *CreateCtrl(const String& classname, Index<String>& recursed);
	Ctrl     *CreateLayoutCtrl(String classname, Index<String>& recursed);

	int    ParseLayoutRef(String cls, String& base) const;
	bool   IsLayoutRef(String cls) const { String dummy; return ParseLayoutRef(cls, dummy) >= 0; }
	void   SynchronizeCtrl(int im);
	void   SynchronizeCtrl(Ctrl& ctrl, const ItemInfo& ii);
	void   SynchronizeCtrl();
	void   SynchronizeCtrls();
	void   SynchronizeMnemonic();
	void   CleanRefCtrl();
	void   AddCtrl(const String& cls, const String& var, const String& lbl, const String& hlp);
	void   SetClass(int im);
	void   UpdateClassList();

	void   LoadCtrl(CParser& p, ItemInfo& ii, int langi = 0);
	String SaveCtrl(const ItemInfo& ii, int i, const String& name, bool lang) const;

	Rect   GetSpringRect(int i);
	Rect   GetTracker(Point track);
	Point  FindTracker(Point p);
	Rect   GetCtrlRect(int i);
	Rect   GetActualRect();
	Rect   GetActual();
	void   RefreshActual();
	int    FindItem(Point p);
	void   DrawHorzSpring(Draw& w, int i);
	void   DrawVertSpring(Draw& w, int i);
	void   PaintTrack(Draw& w);
	void   PaintForm(Draw& w);
	void   StartMoving(Point p);
	void   DoMoving();
	bool   IsMoving();
	bool   GetSpring(int im, int i);
	bool   GetSpring(int i);
	void   SetItemPos(int i, const Rect& r, int chi = -1, bool val = false, bool sync = true);
	void   SetActualPos(const Rect& r, int chi = -1, bool val = false);
	void   ShowHideLayouts();
//	void   SetAutoItemPos(int i, const Rect& r);
	void   WhenITab();
	void   NewClass();
	void   NewVariable();
	void   NewLabel();
	void   NewHelp();
	void   SetFormSize(Size sz);
	void   ClassMenu(Bar& menu);
	void   NewCtrl(int i);

	int    FindSelect(int im);
	void   SelectItem(int im);
	void   SelectOneItem(int im);
	void   UnselectItem(int im);
	bool   ItemSelected(int im)                                { return FindSelect(im) >= 0; }
	void   ClearSelection();
	void   ClearSelection0();
	void   RefreshSelect(int i);
	void   SelectToEnd(int i);

	void   RefreshSelectRect();

	void   Swap(int i1, int i2);
	void   SetLayout(int i, int li);
	void   SetLayout(int i)                                    { SetLayout(i, langi); }
	void   EnterLayout();
	void   EnterLanguage();
	void   Scroll();
	void   ItemPos(int xy_long);
	int    GetItemPosMask() const;

	void   SetupLayoutList();
	void   NewLayout();
	void   RemoveLayout();
	void   RenameLayout();
	void   PrevLayout();
	void   NextLayout();

	bool   SaveLayoutFileAs();

	bool   DoSave();
	bool   PromptSave();
	void   LruOpen(const String& s);
	void   MakeCaption();
	void   OpenFile();
	void   SaveFile();
	void   SaveFileAs();
	void   CloseFile();
	void   NewFile();

#ifdef flagIDERW
	void   CloseDesigner();
#endif

	void   SetupAlias();
	void   SetupGrid();
	void   ToggleGrid();
	void   ToggleIgnoreMin();
	void   ToggleQLIB2Output();
	int    GridX()                           { return usegrid ? max((int)~grid.x, 1) : 1; }
	int    GridY()                           { return usegrid ? max((int)~grid.y, 1) : 1; }

	void   AddItem();
	void   MoveUp();
	void   MoveDown();

	void   DoDuplicate(const Vector<int>& imi, int dx, int dy);
	void   KeyMove(int dx, int dy, int sx, int sy);

	void   Copy();
	void   Cut();
	void   Paste();
	void   Delete();
	void   Duplicate();
	void   SelectAll();

	void   VertCenter();
	void   HorzCenter();
	void   AlignLeft();
	void   AlignRight();
	void   AlignTop();
	void   AlignBottom();
	void   AlignHCenter();
	void   AlignVCenter();
	void   SameWidth();
	void   SameHeight();
	void   SameSize();
	void   MinWidth();
	void   MinHeight();
	void   Matrix();
	void   AlignLbl();

	void   ShowStatus();

	Vector<String> CoreLangs();
	void   AddLang(const String& lang, bool copy);
	void   NewLanguage();
	void   RenameLanguage();
	void   RemoveLanguage();
	void   AddCoreLanguages();
	void   NextLang();
	void   PrevLang();

	void   FileBar(Bar& bar);
	void   LayoutBar(Bar& bar);
	void   SpringsBar(Bar& bar);
	void   AlignBar(Bar& bar);
	void   SizeBar(Bar& bar);
	void   ItemBar(Bar& bar);
	void   OptionsBar(Bar& bar);
	void   EditBar(Bar& bar);
	void   LangBar(Bar& bar);
	void   MainBar(Bar& bar);

	void   Tool(Bar& bar);

	void   SetBar();

public:
	static void Register(const String& classname, CtrlCreator creator);

	void   LoadLayoutFile(const char *s);
	bool   SaveLayoutFile();
	bool   CloseLayoutFile();
	String GetLayoutFile() const;
	String GetFileName() const              { return filename; }

	void   Execute();
	void   OpenWindow();

	LayoutDesigner();

	typedef LayoutDesigner CLASSNAME;
};

template <class T>
class CtrlClassRegistrator {
public:
	static Ctrl *Create()                   { return new T; }

	CtrlClassRegistrator(const char *name)  { LayoutDesigner::Register(name, Create); }
};

class UrgCtrl : public Ctrl {
public:
	virtual void SetLabelText(const char *s);
	virtual void Paint(Draw& w);

protected:
	String label;
	String classname;

public:
	UrgCtrl(const String& classname) : classname(classname) { SetFrame(InsetFrame()); }
};


#define REGCTRLCLASS(x)  static CtrlClassRegistrator<x > MK__s (#x);

void DesignLayout();
