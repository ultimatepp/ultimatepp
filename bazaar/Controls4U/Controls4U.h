#ifndef _Controls4U_Controls4U_h_
#define _Controls4U_Controls4U_h_

#include <PolyXML/PolyXML.h>
#include <Painter/Painter.h>
#include <Functions4U/Functions4U.h>
#if defined(PLATFORM_WIN32) 
#include "Controls4U/ActiveX.h"
#endif
#include "Controls4U/PainterCanvas.h"
#include "Controls4U/SliderCtrlX.h"
#include "Controls4U/StarIndicator.h"

NAMESPACE_UPP;

double AngAdd(double ang, double val);

class FileSel_ : public FileSel {
public:
	String GetBaseDir()	{return basedir;}	
};

class EditFileFolder : public EditString {
typedef EditFileFolder CLASSNAME;
protected:
	FrameLeft<Button> butBrowse, butLeft, butRight, butUp;
	FrameRight<Button> butGo;
	
	FileSel_ *pfs;
	bool isFile, isLoad;
	String title;
	
	Vector <String> history;
	int histInd;
	
	void InitFs();
		
public:
	EditFileFolder() 								{Init();};
	~EditFileFolder();
	
	void Init();
	void DoLeft(), DoRight(), DoUp();
	void DoBrowse();
	void DoGo(bool add = true);
	void Type(const char *name, const char *ext)	{InitFs();	pfs->Type(name, ext);}
	void AllFilesType()								{InitFs();	pfs->AllFilesType();}
	void ActiveDir(const String& d) 				{InitFs();	pfs->ActiveDir(d);}
	void MkDirOption(bool b)						{InitFs();	pfs->MkDirOption(b);}
	void BaseDir(const char *dir)					{InitFs();	pfs->BaseDir(dir);}
	String Get() const                           	{return GetData();}
	operator const char *() const					{return Get();}
	const String operator~() const   				{return Get();}
	void Set(const String& s)						{InitFs();	pfs->Set(s); EditString::SetData(s);}
	void operator<<=(const String& s)            	{Set(s);}
	EditFileFolder &operator=(const char *s)  	  	{Set(s); return *this;}
	EditFileFolder &operator=(const String& s) 		{Set(s); return *this;}
	bool IsEmpty()									{return GetData().IsNull();}
	EditFileFolder &NotNull(bool b)					{EditString::NotNull(b);  return *this;}
	EditFileFolder &SelLoad(bool load) 				{isLoad = load; return *this;}
	EditFileFolder &SetTitle(const char *_title)	{title = _title; return *this;}
	EditFileFolder &UseHistory(bool use);
	EditFileFolder &UseUp(bool use);
	EditFileFolder &UseBrowse(bool use);
	virtual void SetData(const Value& data);
	//virtual void LostFocus()						{DoGo(true);}
	
	Callback WhenChange;
};

class EditFile : public EditFileFolder {
typedef EditFile CLASSNAME;		
public:
	EditFile();
	using EditFileFolder::operator=;
};

class EditFolder : public EditFileFolder {
typedef EditFolder CLASSNAME;		
public:
	EditFolder();
	using EditFileFolder::operator=;
};

class ImagePopUp : public Ctrl {
	public:
		Ctrl* ctrl;

		ImagePopUp() {}
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
	
	void  operator=(String fileName)      	{Set(fileName);}
	void  operator=(Image image)       		{Set(image);}	

	StaticImage& SetAngle(int _angle);
	StaticImage& SetFit(int _fit)				{fit = _fit; 		  Refresh(); return *this;}
	StaticImage& SetBackground(Color c) 		{background = c; 	  Refresh(); return *this;}
	StaticImage& UseAsBackground(bool b = true)	{useAsBackground = b; Refresh(); return *this;}
	StaticImage& SetPopUp(bool pop = true)		{isPopUp = pop;	return *this;}
	StaticImage& SetPopUpSize(Size sz);
	StaticImage();
	
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
	virtual void GotFocus();
	virtual void LostFocus();
	
	Vector<Image> images;
	Color background;
	
	int id;

public:
	bool  Add(String fileName);
	bool  Add(Image image);
	Image &Get(int id)						{return images[id];}
	Vector<Image> &GetImages()				{return images;}
	void SetActive(int _id)					{id = _id; Refresh();}
	StaticImageSet& SetBackground(Color c) 	{background = c; Refresh(); return *this;}
	void Next()								{id++; if(id >= images.GetCount()) id = 0;}
	
	StaticImageSet();
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
};

class StaticLine : public Ctrl, public CtrlFrame {
typedef StaticLine CLASSNAME;	
public:
	virtual void FrameAddSize(Size& sz) {}
	virtual void FrameLayout(Rect& r) {}
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
};

class StaticArrow : public Ctrl, public CtrlFrame  {
typedef StaticArrow CLASSNAME;	
public:
	virtual void FrameAddSize(Size& sz) {}
	virtual void FrameLayout(Rect& r) {}
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
	~StaticClock(); 
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
	~Meter();
};

class Knob : public Ctrl {
private:
	double value;
	double minv, maxv, keyStep;
	int nminor, nmajor;
	double minorstep, majorstep;
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
	typedef Knob CLASSNAME;

	Callback WhenSlideFinish;
	
	Knob();

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
			Vector<String> files;
			
			if (AcceptFiles(d)) {
				files = GetFiles(d);
				Refresh();
				PromptOK(DeQtf(files[0]));
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
	
	void AddFolder(String folder, String &myFolders, TreeCtrl &folders, int id);
	
public: 
	FileBrowser();
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
	RichTextView about;

	typedef AboutUpp CLASSNAME;

	AboutUpp();
};


class HyperlinkLabel : public Label {
public:
	HyperlinkLabel() {
		NoIgnoreMouse();
		SetInk(LtBlue());
	}
	HyperlinkLabel& SetHyperlink(const char* str) 		{hyperlink = str; return *this;}

private:
	String hyperlink;
	virtual Image CursorImage(Point p, dword keyflags) 	{return Image::Hand();}
	virtual void LeftDown(Point p, dword keyflags) 		{LaunchWebBrowser(hyperlink);}
};

END_UPP_NAMESPACE

#endif
