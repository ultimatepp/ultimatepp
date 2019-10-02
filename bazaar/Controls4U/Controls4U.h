#ifndef _Controls4U_Controls4U_h_
#define _Controls4U_Controls4U_h_

#include <PolyXML/PolyXML.h>
#include <Painter/Painter.h>
#include <Functions4U/Functions4U_Gui.h>
#if defined(PLATFORM_WIN32) 
#include "Controls4U/ActiveX.h"
#endif
#include "Controls4U/PainterCanvas.h"
#include "Controls4U/SliderCtrlX.h"
#include "Controls4U/StarIndicator.h"
#include "Controls4U/SplitterButton.h"

using namespace Upp;

double AngAdd(double ang, double val);

class FileSel_ : public FileSel {
public:
	String GetBaseDir()	{return basedir;}	
};

class EditFileFolder : public WithDropChoice<EditString> {
typedef EditFileFolder CLASSNAME;

protected:
	FrameLeft<Button> butBrowseLeft, butLeft, butRight, butUp;
	FrameRight<Button> butBrowseRight, butFolder;
	FrameRight<Button> butGo;
	
	FileSel_ *pfs;
	bool isFile, isLoad;
	String title;
	
	Vector <String> history;
	int histInd;
	
	void InitFs();
	
	void DoLeft(), DoRight(), DoUp();
		
public:
	EditFileFolder() 								{Init();};
	virtual ~EditFileFolder();
	
	void Init();
	void ClearTypes()								{InitFs();	pfs->ClearTypes();}
	void Type(const char *name, const char *ext)	{InitFs();	pfs->Type(name, ext);}
	void ActiveType(int type)						{InitFs();	pfs->ActiveType(type);}	
	void AllFilesType()								{InitFs();	pfs->AllFilesType();}
	void ActiveDir(const String& d) 				{InitFs();	pfs->ActiveDir(d);}
	void MkDirOption(bool b)						{InitFs();	pfs->MkDirOption(b);}
	void BaseDir(const char *dir)					{InitFs();	pfs->BaseDir(dir);}
	String Get() const                           	{return GetData();}
	operator const char *() const					{return Get();}
	const String operator~() const   				{return Get();}
	void Set(const String& s) {
		InitFs();	
		pfs->Set(s); 
		EditString::SetData(s); 
		AddHistory();
	}
	void operator<<=(const char *s)   				{Set(s);}
	void operator<<=(const String& s)   			{Set(s);}
	Callback operator<<=(Callback  action) 			{WhenAction = action; return action;}
	bool IsEmpty()									{return GetData().IsNull();}
	EditFileFolder &NotNull(bool b)					{EditString::NotNull(b);  return *this;}
	EditFileFolder &SelLoad(bool load) 				{isLoad = load; return *this;}
	EditFileFolder &SetTitle(const char *_title)	{title = _title; return *this;}
	EditFileFolder &UseHistory(bool use = true);
	EditFileFolder &UseOpenFolder(bool use = true);
	EditFileFolder &UseUp(bool use = true);
	EditFileFolder &UseBrowse(bool use = true);
	EditFileFolder &UseBrowseRight(bool use = true);
	EditFileFolder &UseGo(bool use = true);
	EditFileFolder &BrowseRightWidth(int w)			{butBrowseRight.Width(w); 	return *this;}
	EditFileFolder &BrowseOpenFolderWidth(int w)	{butFolder.Width(w);  		return *this;}
	EditFileFolder &UseDropping(bool use = true)	{Dropping(use);				return *this;}
	EditFileFolder &BrowseDroppingWidth(int w)		{DropWidth(w);		  		return *this;}
	virtual void SetData(const Value& data);
	
	void DoGo(bool add = true);
	void DoBrowse();
		
	Function<bool ()> WhenChange;
	
	virtual void  Serialize(Stream& s) {
		WithDropChoice::Serialize(s);
		if (s.IsLoading()) {
			if (select.GetCount() > 0)
				SetData(select.Get(0));
		}
	}
	virtual void  Jsonize(JsonIO& jio) {
		Vector<String> list;
		if (!jio.IsLoading()) {
			AddHistory();
			for (int i = 0; i < select.GetCount(); ++i)
				list << select.Get(i);
		}
		jio
			("list", list)
		;
		if (jio.IsLoading()) {
			for (int i = 0; i < list.GetCount(); ++i)
				select.Add(list[i]);
			if (list.GetCount() > 0)
				SetData(list[0]);
		}
	}
	virtual void  Xmlize(XmlIO& xio) {
		Vector<String> list;
		if (!xio.IsLoading()) {
			AddHistory();
			for (int i = 0; i < select.GetCount(); ++i)
				list << select.Get(i);
		}
		xio
			("list", list)
		;
		if (xio.IsLoading()) {
			for (int i = 0; i < list.GetCount(); ++i)
				select.Add(list[i]);
			if (list.GetCount() > 0)
				SetData(list[0]);
		}
	}
};

class EditFile : public EditFileFolder {
typedef EditFile CLASSNAME;	
	
public:
	EditFile();
	virtual ~EditFile() {};
	
	using EditFileFolder::operator=;
};

class EditFolder : public EditFileFolder {
typedef EditFolder CLASSNAME;	
	
public:
	EditFolder();
	virtual ~EditFolder() {};
	
	using EditFileFolder::operator=;
};

class ImagePopUp : public Ctrl {
	public:
		Ctrl* ctrl;

		ImagePopUp() {}
		virtual ~ImagePopUp() {};
		
		Point Offset(Point p);

		virtual void  Paint(Draw &w);
		virtual void  LeftDown(Point p, dword flags);
		virtual void  LeftDrag(Point p, dword flags);
		virtual void  LeftDouble(Point p, dword flags);
		virtual void  RightDown(Point p, dword flags);
		virtual void  LeftUp(Point p, dword flags);
		virtual void  MouseWheel(Point p, int zdelta, dword flags);
		virtual void  MouseLeave();
		virtual void  MouseEnter(Point p, dword flags);
		virtual void  MouseMove(Point p, dword flags);
	    virtual Image CursorImage(Point p, dword flags);
	    virtual void  LostFocus();
		void PopUp(Ctrl *owner, int x, int y, int width, int height, Image &_image, int _angle, int _fit);
		virtual void Close();
		
		Image image;
		int angle, fit;
};

class StaticImage : public Ctrl {
typedef StaticImage CLASSNAME;
		
public:
	enum ImageAngle {Angle_0, Angle_90, Angle_180, Angle_270};
	enum ImageFit   {BestFit, FillFrame, NoScale, RepeatToFill};

protected:
	virtual void Paint(Draw& draw);
	virtual void Layout();
	virtual void RightDown(Point pos, dword keyflags);
	virtual void LeftDown(Point pos, dword keyflags);
	virtual void LeftDouble(Point pos, dword keyflags);
	virtual void MouseEnter(Point pos, dword keyflags);
	virtual void MouseLeave();
	
	String fileName;
	Image image, origImage;
	Color background;
	int angle, fit;
	bool useAsBackground;
	ImagePopUp popup;
	bool isPopUp;
	Size szPopUp;

public:
	bool Set(String fileName);
	bool Set(Image image);
	void Clear()							{Set(Image());}
	Image &Get()							{return origImage;}
	void SetData(const Value& data)			{Set(data.ToString());}
	
	void  operator=(String _fileName)      	{Set(_fileName);}
	void  operator=(Image _image)       	{Set(_image);}	

	StaticImage& SetAngle(int _angle);
	StaticImage& SetFit(int _fit)				{fit = _fit; 		  Refresh(); return *this;}
	StaticImage& SetBackground(Color c) 		{background = c; 	  Refresh(); return *this;}
	StaticImage& UseAsBackground(bool b = true)	{useAsBackground = b; Refresh(); return *this;}
	StaticImage& SetPopUp(bool pop = true)		{isPopUp = pop;	return *this;}
	StaticImage& SetPopUpSize(Size sz);
	StaticImage();
	virtual ~StaticImage() {};
	
	Callback WhenLeftDouble;
	Callback WhenLeftDown;
	Callback WhenRightDown;
};

class StaticImageSet : public Ctrl {
typedef StaticImageSet CLASSNAME;
		
protected:
	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point pos, dword keyflags);
	virtual void LeftRepeat(Point pos, dword keyflags);
	virtual void LeftUp(Point pos, dword keyflags);
	virtual void MouseMove(Point pos, dword keyflags);
	virtual void GotFocus() 				{Refresh();}
	virtual void LostFocus() 				{Refresh();}
	
	Vector<Image> images;
	Color background;
	
	int id;

public:
	bool Add(String fileName);
	bool Add(Image image);
	void Clear()							{images.Clear(); id = -1;}
	Image &Get(int _id)						{return images[_id];}
	Vector<Image> &GetImages()				{return images;}
	void SetActive(int _id)					{id = _id; Refresh();}
	StaticImageSet& SetBackground(Color c) 	{background = c; Refresh(); return *this;}
	void Next()								{id++; if(id >= images.GetCount()) id = 0;}
	
	StaticImageSet();
	virtual ~StaticImageSet() {};
};

#ifndef flagNOPAINTER

class StaticRectangle : public Ctrl {
typedef StaticRectangle CLASSNAME;	

public:
	virtual void Paint(Draw& draw);
	virtual void MouseEnter(Point p, dword keyflags) 	{WhenMouseEnter(p, keyflags);};
	virtual void MouseLeave() 							{WhenMouseLeave();};
	virtual void LeftDown(Point p, dword keyflags) 		{WhenLeftDown(p, keyflags);};
	virtual void LeftUp(Point p, dword keyflags) 		{WhenLeftUp(p, keyflags);};
	virtual void Layout();
	
protected:
	Color background;
	Color color;
	int width;
	bool isSquare;

public:
	StaticRectangle& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticRectangle& SetColor(Color c) 			{color = c; Refresh(); return *this;}
	StaticRectangle& SetBackground(Color c) 	{background = c; Refresh(); return *this;}
	StaticRectangle& IsSquare(bool is = false) 	{isSquare = is; Refresh(); return *this;}
	Callback2<Point, dword> WhenMouseEnter; 
	Callback WhenMouseLeave;
	Callback2<Point, dword> WhenLeftDown; 
	Callback2<Point, dword> WhenLeftUp; 
	
	StaticRectangle();
	virtual ~StaticRectangle() {};
};

class StaticEllipse : public Ctrl {
typedef StaticEllipse CLASSNAME;
	
public:
	virtual void   Paint(Draw& draw);
	
protected:
	Color background;
	Color color;
	int width;

public:
	StaticEllipse& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticEllipse& SetColor(Color c) 		{color = c; Refresh(); return *this;}
	StaticEllipse& SetBackground(Color c) 	{background = c; Refresh(); return *this;}

	StaticEllipse();
	virtual ~StaticEllipse() {};
};

class StaticFrame : public Ctrl {
typedef StaticFrame CLASSNAME;	

public:
	virtual void Paint(Draw& draw);

protected:
	Color background;

public:
	StaticFrame& SetBackground(Color c) {background = c; Refresh(); return *this;}
	
	StaticFrame();
	virtual ~StaticFrame() {};
};

class StaticLine : public Ctrl, public CtrlFrame {
typedef StaticLine CLASSNAME;
	
public:
	virtual void FrameAddSize(Size& ) {}
	virtual void FrameLayout(Rect& ) {}
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual int OverPaint() const { return 20; }
	
	enum LineOrientation {OrVert, OrHor, OrNW_SE, OrSW_NE};
	
protected:
	int orientation;
	Color color;
	int width;

public:
	StaticLine& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticLine& SetColor(Color c) 			{color = c; Refresh(); return *this;}
	StaticLine& SetOrientation(String o);
	StaticLine& SetOrientation(int o) 		{orientation = o; Refresh(); return *this;}
	
	StaticLine();
	virtual ~StaticLine() {};
};

class StaticArrow : public Ctrl, public CtrlFrame  {
typedef StaticArrow CLASSNAME;	
public:
	virtual void FrameAddSize(Size& ) {}
	virtual void FrameLayout(Rect& ) {}
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual int OverPaint() const { return 20; }
	
	enum LineOrientation {OrVert, OrHor, OrNW_SE, OrSW_NE, OrNW_SE_HVH, OrSW_NE_HVH, OrNW_SE_VHV, OrSW_NE_VHV};
	enum ArrowEnds {EndLeft, EndRight, EndLeftRight, NoEnd};

protected:
	int orientation;
	int ends;
	Color color;
	int width;

public:
	StaticArrow& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticArrow& SetColor(Color c) 			{color = c; Refresh(); return *this;}
	StaticArrow& SetOrientation(int o) 		{orientation = o; Refresh(); return *this;}
	StaticArrow& SetOrientation(String o);
	StaticArrow& SetEnds(int e) 			{ends = e; Refresh(); return *this;}
	StaticArrow& SetEnds(String e);

	StaticArrow();
	virtual ~StaticArrow() {};
};

class StaticClock : public Ctrl {
typedef StaticClock CLASSNAME;	
public:
	virtual void   Paint(Draw& draw);
	
	enum HourType 	{No, Square, Rectangle};
	enum NumberType {NoNumber, Small, Big, BigSmall, Big4};
	enum ColorType 	{WhiteType, BlackType};

protected:
	void PaintPtr(BufferPainter &w, double cmx, double cmy, double pos, double m, double d, 
					Color color, double cf);
	int hourType;
	int numberType;
	Image image;
	bool seconds;
	int colorType;
	bool autoMode;

	Time t;		

public:
	StaticClock& SetImage(String fileName);
	StaticClock& SetImage(Image _image)	{image = _image; Refresh(); return *this;}

	StaticClock& SetHourType(int type) 	{hourType = type; Refresh(); return *this;}
	StaticClock& SetNumberType(int type){numberType = type; Refresh(); return *this;}
	StaticClock& SetColorType(int c)	{colorType = c; Refresh(); return *this;}
	StaticClock& Seconds(bool b) 		{seconds = b; Refresh(); return *this;}	
	StaticClock& SetAuto(bool mode = true);
	bool IsAuto() 						{return autoMode;}
	void RefreshValue() {Refresh();};
	
	void SetData(const Value& v);
	void SetTime(const Time& tm = GetSysTime());
	void SetTimeRefresh() {SetTime();};
	void SetTime(int h, int n, int s);

	StaticClock();
	virtual ~StaticClock();	
};

class Meter : public Ctrl {
typedef Meter CLASSNAME;		
public:
	virtual void   Paint(Draw& draw);
	
	enum ColorType {WhiteType, BlackType};
	
	friend void MeterThread(Meter *gui, double value);

protected:
	double value;
	double min;
	double max;
	double peak;
	double step;	
	double angleBegin;
	double angleEnd;
	String text;
	bool clockWise;
	bool number;
	int colorType;
	double speed;
	int sensibility;
	volatile Atomic running, kill;
	
	void PaintMarks(BufferPainter &w, double cx, double cy, double R, double ang0, 
					double ang1, int direction, double step, double bigF, Color color);
	void PaintNumbers(BufferPainter &w, double cx, double cy, double R, double a0, 
			double step, int direction, double minv, double maxv, double stepv, double bigF, 
			Color color);
	void PaintHand(BufferPainter &w, double cx, double cy, double R, double val, double bigF,
					int colorType);
	void RefreshValue() {Refresh();};
	
public:
	Meter& SetMin(double v)			{min = value = v; Refresh(); return *this;}
	Meter& SetMax(double v)			{max  = v; Refresh(); return *this;}
	Meter& SetPeak(double v)		{peak = v; Refresh(); return *this;}
	Meter& SetStep(double v) 		{step = v; Refresh(); return *this;}
	Meter& SetAngleBegin(double v) 	{angleBegin = v; Refresh(); return *this;}
	Meter& SetAngleEnd(double v)	{angleEnd = v; Refresh(); return *this;}
	Meter& SetText(String s)		{text = s; Refresh(); return *this;}
	Meter& ClockWise(bool v)		{clockWise = v; Refresh(); return *this;}
	Meter& SetNumber(bool v)		{number = v; Refresh(); return *this;}
	Meter& SetColorType(int c)		{colorType = c; Refresh(); return *this;}
	Meter& SetSpeed(double s)		{speed = s; Refresh(); return *this;} 
	Meter& SetSensibility(int s)	{sensibility = s; Refresh(); return *this;}

	void SetData(const Value& v);

	Meter();
	virtual ~Meter();
};

class Knob : public Ctrl {
typedef Knob CLASSNAME;

private:
	double value;
	double minv, maxv;
	int nminor, nmajor;
	double minorstep, majorstep;
	double keyStep;
	double angleBegin, angleEnd;
	double angleClick;
	int  colorType;
	bool clockWise;
	bool number;
	int mark;
	bool interlocking;
	int style;

	double SliderToClient(Point pos);
	void PaintMarks(BufferPainter &w, double cx, double cy, double R, double begin, double end, double ang0, 
		double ang1, int direction, double minorstep, double bigF, Color color);
	void PaintNumbers(BufferPainter &w, double cx, double cy, double R, double a0, 
		double step, int direction, double minv, double maxv, double stepv, double bigF, Color color);	
	void PaintRugged(BufferPainter &w, double cx, double cy, double angle, double r, 
		double rugg, int numRug, Color &color);
	
	virtual void  Paint(Draw& draw);
	virtual bool  Key(dword key, int repcnt);
	virtual void  LeftDown(Point pos, dword keyflags);
	virtual void  LeftRepeat(Point pos, dword keyflags);
	virtual void  LeftUp(Point pos, dword keyflags);
	virtual void  MouseMove(Point pos, dword keyflags);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  Layout();
	Image img, imgMark;
	
public:
	Callback WhenSlideFinish;
	
	Knob();
	virtual ~Knob() {};

	virtual void  SetData(const Value& value);
	virtual Value GetData() const;

	void Inc(double st);
	void Dec(double st);

	Knob& ClockWise(bool v)			{clockWise = v; Refresh(); return *this;}	

	Knob& SetMin(double v)			{minv = v; value = Upp::max<double>(value, minv); Refresh(); return *this;}
	Knob& SetMax(double v)			{maxv = v; value = Upp::min<double>(value, maxv); Refresh(); return *this;}
	Knob& SetMinorStep(double n) 	{minorstep = n; Refresh(); return *this;}	
	Knob& SetMajorStep(double n) 	{majorstep = n; Refresh(); return *this;}	
	Knob& SetKeyStep(double fs = 1) {keyStep = fs; Refresh(); return *this;}	
	Knob& SetAngleBegin(double v) 	{angleBegin = v; Refresh(); return *this;}
	Knob& SetAngleEnd(double v)		{angleEnd = v; Refresh(); return *this;}
	Knob& SetNumber(bool b)			{number = b; Layout(); Refresh(); return *this;}
	Knob& SetInterlocking(bool b = true)	{interlocking = b; Refresh(); return *this;}

	enum ColorType {SimpleWhiteType, SimpleBlackType, WhiteType, BlackType};
	Knob& SetColorType(int c)	{colorType = c; Layout(); Refresh(); return *this;}
	enum Mark {NoMark, Line, Circle};
	Knob& SetMark(int c)		{mark = c; Layout(); Refresh(); return *this;}
	enum Style {Simple, Rugged};
	Knob& SetStyle(int c)		{style = c; Layout(); Refresh(); return *this;}	
};

#endif

class FileBrowser : public StaticRect {
typedef FileBrowser CLASSNAME;	

public:
	struct EditStringLostFocus : public EditString {
		String file;
		Callback WhenChange;
		
		virtual void LostFocus() {
			ValueArray values = GetData();
			if (!IsNull(file))
				if (file != values[0]) {
					if(WhenChange)
						WhenChange();
				}
			EditString::LostFocus();
		}
		virtual void SetData(const Value& data) {
			ValueArray values = data;
			file = values[0];
			EditString::SetData(data);
		}
		virtual bool Key(dword key, int rep) {
			if (key == K_ESCAPE)
				file = Null;
			return EditString::Key(key, rep);
		}
	};
	
protected:
	EditFolder folder;
	
	void OpenSelExt(bool forceOpen);
	void OpenSel();	
	
private:
	struct TreeCtrlPlus : public TreeCtrl {
		virtual bool Key(dword key, int count) {
			if (key == K_F5) {
				Ctrl *q = Ctrl::GetParent()->GetParent()->GetParent();
				FileBrowser *f = dynamic_cast<FileBrowser *>(q);
				f->folder.DoGo();
				return true;
			} else if (key == K_ENTER) {
				Vector<int> sel = GetSel();
				if (sel.GetCount() > 0) 
					Open(sel[0], true);
				return true;
			} else
				return TreeCtrl::Key(key, count);
		}
	};
	struct ArrayCtrlExternDrop : public ArrayCtrl {
	public:
		EditStringLostFocus *textFileName;
	private:
		virtual bool Key(dword key, int count) {
			if (key == K_DELETE) {
				if (GetSelectCount() > 1) {
					if(PromptYesNo(Format(t_("Do you want to send the %d files to the trash bin?"), 
								GetSelectCount()))) {
						for (int i = 0; i < GetCount(); ++i) {			
							if (IsSelected(i)) {
								ValueArray va = GetColumn(i, 0);
								String fileName = va[0];
								bool isFolder = DirectoryExists(fileName);
							    if (!FileToTrashBin(fileName))
									Exclamation(Format(t_("%s \"%s\" cannot be sent to the trash bin"), 
										isFolder ? t_("Folder") : t_("File"), DeQtf(fileName)));
							}
						}
					}
				} else {
					ValueArray va = GetColumn(GetCursor(), 0);
					String fileName = va[0];
					bool isFolder = DirectoryExists(fileName);
					if(PromptYesNo(Format(t_("Do you want to send %s \"%s\" to the trash bin?"), 
								isFolder ? t_("folder") : t_("file"), DeQtf(fileName))))
						if (!FileToTrashBin(fileName))
							Exclamation(Format(t_("%s \"%s\" cannot be sent to the trash bin"), 
								isFolder ? t_("Folder") : t_("File"), DeQtf(fileName)));
				}
				Ctrl *q = GetParent()->GetParent();
				FileBrowser *f = dynamic_cast<FileBrowser *>(q);
				f->folder.DoGo();
				return true;
			} else if (key == K_F5) {
				Ctrl *q = GetParent()->GetParent();
				FileBrowser *f = dynamic_cast<FileBrowser *>(q);
				f->folder.DoGo();
				return true;
			} else if ((key == K_UP || key == K_DOWN) && textFileName->HasFocus())
				return ArrayCtrl::Key(K_ENTER, count);
			else if (key == K_F2) {
				DoEdit();
				return true;
			} else
				return ArrayCtrl::Key(key, count);
		}
		virtual void DragAndDrop(Point p, PasteClip& d) {
			Vector<String> fls;
			
			if (AcceptFiles(d)) {
				fls = GetFiles(d);
				Refresh();
				PromptOK(DeQtf(fls[0]));
			} else
				ArrayCtrl::DragAndDrop(p, d);
		}
		typedef ArrayCtrlExternDrop CLASSNAME;
	};
	friend struct ArrayCtrlExternDrop;

private:
	EditStringLostFocus textFileName;
	Splitter pack;
	StaticRect foldersRect;
	Label foldersLabel;
	TreeCtrlPlus folders;
	ArrayCtrlExternDrop files;
	bool browseFiles;
	bool noDoOpen;
	bool forceOpenTree;
	String fileNameSelected;
	bool readOnly;
	EXT_FILE_FLAGS flags;
	bool acceptDragAndDrop;
	
	void SortByColumn(int col);
	
	void FoldersWhenOpen(int id); 
	void FoldersWhenClose(int id); 
	void FoldersWhenSel(); 
	void FoldersWhenLeftDouble();
	
	void FilesEnterRow();
	void FilesWhenLeftDouble();
	void FilesWhenSel();
	void FilesList(String folderName, bool &thereIsAFolder);
	
	void FolderWhenChange();
	
	void FileNameWhenChanged();
	
	void AddFolder(String folder, String &myFolders, int id);
	
public: 
	FileBrowser();
	virtual ~FileBrowser() {};
	
 	String GetFile();
 	String operator~()    {return GetFile();}	
 	String GetFolder();
	FileBrowser &SetReadOnly(bool set = true) {readOnly = set; return *this;};
	FileBrowser &SetUseTrashBin(bool set = true) {flags = (set ? EXT_FILE_FLAGS(flags | USE_TRASH_BIN) : EXT_FILE_FLAGS(flags & ~USE_TRASH_BIN)); return *this;};
	FileBrowser &SetBrowseLinks(bool set = true) {flags = (set ? EXT_FILE_FLAGS(flags | BROWSE_LINKS) : EXT_FILE_FLAGS(flags & ~BROWSE_LINKS)); return *this;};
	FileBrowser &SetDeleteReadOnly(bool set = true) {flags = (set ? EXT_FILE_FLAGS(flags | DELETE_READ_ONLY) : EXT_FILE_FLAGS(flags & ~DELETE_READ_ONLY)); return *this;};
//	FileBrowser &SetAskBeforeDelete(bool set = true) {set ? flags |= ASK_BEFORE_DELETE : flags &= ~ASK_BEFORE_DELETE; return *this;};
	FileBrowser &SetDragAndDrop(bool set = true) {acceptDragAndDrop = set; return *this;};
	FileBrowser &SetBrowseFiles(bool set = true) {browseFiles = set; return *this;};
	
	//Callback WhenOpened;	
	Callback WhenTreeSelected;
	Callback WhenTreeDblClick;
	Callback WhenSelected;
};

struct AboutUpp : StaticRect {
typedef AboutUpp CLASSNAME;

	AboutUpp();
	virtual ~AboutUpp() {};
	
	RichTextView about;
};


class HyperlinkLabel : public Label {
typedef HyperlinkLabel CLASSNAME;

public:
	HyperlinkLabel() {
		NoIgnoreMouse();
		SetInk(LtBlue());
	}
	virtual ~HyperlinkLabel() {};
	
	HyperlinkLabel& SetHyperlink(const char* str) 		{hyperlink = str; return *this;}

private:
	String hyperlink;
	virtual Image CursorImage(Point , dword ) 	{return Image::Hand();}
	virtual void LeftDown(Point , dword ) 		{LaunchWebBrowser(hyperlink);}
};

class BarDisplay : public Display {
typedef BarDisplay CLASSNAME;	
	
public:
	BarDisplay() : ink(SColorText), value(0), align(ALIGN_LEFT) {}
	virtual ~BarDisplay() {};

	virtual void Paint(Draw& w, const Rect& r, const Value& , Color, Color, dword) const {
		int width = int(r.Width()*value);
		w.DrawRect(r.left, r.top, width, r.bottom, color);
		w.DrawRect(r.left + width, r.top, r.right - width, r.bottom, paper);
		if (!text.IsEmpty()) {
			Size sz = GetTextSize(text, StdFont());
		    int xtext;
		    switch (align) {
		    case ALIGN_LEFT:	xtext = 0;							break;
		    case ALIGN_CENTER:	xtext = (r.GetWidth() - sz.cx)/2;	break;
		    default:			xtext = r.GetWidth() - sz.cx;
		    }
	    
			w.DrawText(r.left + xtext, r.top + (r.Height() - StdFont().Info().GetHeight())/2,
			           text, StdFont(), ink);
		}
	}
	BarDisplay &SetValue(double _value) {value = _value;return *this;}		// Between 0 and 1
	BarDisplay &SetText(const char *str){text = str;	return *this;}
	BarDisplay &SetColor(Color _color, Color _paper = Null, Color _ink = SColorText) {
		color = _color; 
		ink = _ink;	
		paper = _paper;	
		return *this;
	}
	BarDisplay &SetAlign(int a) {align = a; return *this;}
	
private:
	Color color, ink, paper;
	String text;
	double value;
	int align;
};

class TextDisplay : public Display {
typedef TextDisplay CLASSNAME;
	
public:
	TextDisplay() : ink(SColorText), align(ALIGN_LEFT) {}
	virtual ~TextDisplay() {};
	
	virtual void Paint(Draw& w, const Rect& r, const Value&, Color, Color, dword) const {
	    w.DrawRect(r.left, r.top, r.right, r.bottom, paper);
	    
	    Size sz = GetTextSize(text, StdFont());
	    int xtext;
	    switch (align) {
	    case ALIGN_LEFT:	xtext = 0;							break;
	    case ALIGN_CENTER:	xtext = (r.GetWidth() - sz.cx)/2;	break;
	    default:			xtext = r.GetWidth() - sz.cx;
	    }
		w.DrawText(r.left + xtext, r.top + (r.Height() - StdFont().Info().GetHeight()) / 2,
			           text, StdFont(), ink);	
	}
	TextDisplay &SetText(const char *str) 					{text = str;					return *this;}
	TextDisplay &SetColor(Color _ink, Color _paper = Null)	{ink = _ink;	paper = _paper;	return *this;}
	TextDisplay &SetInk(Color _ink)							{ink = _ink;					return *this;}
	TextDisplay &SetPaper(Color _paper)						{paper = _paper;				return *this;}
	TextDisplay &SetAlign(int a)             				{align = a; 					return *this;}
	
private:
	Color ink, paper;
	String text;
	int align;
};

class InfoCtrlBar : public InfoCtrl {
typedef InfoCtrlBar CLASSNAME;

public:
	InfoCtrlBar() {
		Set(PaintRect(bar));
		SetColor(SColorFace);
	}
	virtual ~InfoCtrlBar() {};
	
	InfoCtrlBar &SetValue(double value) 											{bar.SetValue(value);				return *this;}	
	InfoCtrlBar &SetText(const char *str) 											{bar.SetText(str);					return *this;}
	InfoCtrlBar &SetColor(Color color, Color paper = Null, Color ink = SColorText)	{bar.SetColor(color, paper, ink);	return *this;}
	InfoCtrlBar &SetAlign(int a)             										{bar.SetAlign(a);					return *this;}
	
private:
	BarDisplay bar;
};

class InfoCtrlText : public InfoCtrl {
typedef InfoCtrlText CLASSNAME;

public:
	InfoCtrlText() {Set(PaintRect(text));}
	virtual ~InfoCtrlText() {};

	InfoCtrlText &SetText(const char *str) 					{text.SetText(str);			return *this;}
	InfoCtrlText &SetColor(Color ink, Color paper = Null)	{text.SetColor(ink, paper);	return *this;}
	InfoCtrlText &SetInk(Color ink)							{text.SetInk(ink);			return *this;}
	InfoCtrlText &SetPaper(Color paper)						{text.SetPaper(paper);		return *this;}
	InfoCtrlText &SetAlign(int a)             				{text.SetAlign(a);			return *this;}
	
private:
	TextDisplay text;
};
	
	
#endif
