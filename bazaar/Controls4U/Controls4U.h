#ifndef _Controls4U_Controls4U_h_
#define _Controls4U_Controls4U_h_

#include <Painter/Painter.h>
#include "Functions4U/Functions4U.h"
#if defined(PLATFORM_WIN32) 
#include "Controls4U/ActiveX.h"
#endif

#ifndef flagNOPAINTER
class MyBufferPainter : public BufferPainter {
public:
	MyBufferPainter(ImageBuffer& ib, int mode = MODE_ANTIALIASED) : BufferPainter(ib, mode) {};
	void DrawLine(double x1, double y1, double x2, double y2, double width, Color color) {
		Move(x1, y1).Line(x2, y2).Stroke(width, color);
	}
	void DrawRect(double x, double y, double cx, double cy, Color color) {
		Move(x, y).Line(x + cx, y).Line(x + cx, y + cy).Line(x, y + cy).Close();
		Fill(color);
	}
	void DrawRect(const Rect &r, Color color) {
		DrawRect(r.left, r.top, r.right-r.left, r.bottom-r.top, color);
	}	
	void DrawEllipse(double x, double y, double rx, double ry, Color color) {
		Ellipse(x, y, rx, ry);
		Fill(color);
	}
	void DrawEllipse(const Rect& r, Color color) {
		Sizef sz = r.GetSize();
		Ellipse(r.left + sz.cx / 2, r.top + sz.cy / 2, sz.cx / 2, sz.cy / 2);
		Fill(color);
	}
	void DrawCircle(double x, double y, double r, Color color) {
		Ellipse(x, y, r, r);
		Fill(color);
	}
	void DrawCenterText(double x, double y, String text, Font fnt, Color color) {
		Size sz = GetTextSize(text, fnt);
		DrawText((int)(x - sz.cx / 2.), (int)(y - sz.cy / 2.), text, fnt, color);
	}
	void DrawArc(double cx, double cy, double R, double ang0, double ang1, int direction, 
					double width, Color color) {
		if (direction == -1) {
			double c = ang0;
			ang0 = ang1;
			ang1 = c;
		}
		ang0 = ToRad(ang0);
		ang1 = ToRad(ang1);
		double delta = ToRad(1);
		if (ang0 > ang1)
			ang1 += 2*M_PI;
		double x0, y0, x1, y1;
		for (double i = ang0; i < ang1; i += delta) {
			if (i == ang0) {
				x0 = cx + R*cos(i);
				y0 = cy - R*sin(i);
			} else {
				x0 = x1;
				y0 = y1;
			}
			x1 = cx + R*cos(i + delta);
			y1 = cy - R*sin(i + delta);
			DrawLine(x0, y0, x1, y1, width, color);
		}
	}
};
#endif

class EditFileFolder : public EditString {
typedef EditFileFolder CLASSNAME;
protected:
	FrameLeft<Button> butBrowse, butLeft, butRight, butUp;
	FrameRight<Button> butGo;
	bool Key(dword key, int rep);
	
	FileSel fs;
	bool isFile, isLoad;
	String title;
	
public:
	EditFileFolder() 								{Init();};
	void Init();
	void DoLeft(), DoRight(), DoUp();
	void DoBrowse();
	void DoGo();
	void Type(const char *name, const char *ext)	{fs.Type(name, ext);};
	void AllFilesType()								{fs.AllFilesType();};
	void ActiveDir(const String& d) 				{fs.ActiveDir(d);};
	void MkDirOption(bool b)						{fs.MkDirOption(b);};
	String Get() const                           	{return GetData();};	
	String operator~() const                     	{return Get();};
	operator String() const                      	{return Get();}
	void Set(const String& s)						{fs.Set(s); SetData(s);};
	void operator<<=(const String& s)            	{Set(s);};
	void operator=(const String& s)              	{Set(s);};
	bool IsEmpty()									{return GetData().IsNull();};
	EditFileFolder &NotNull(bool b)					{EditString::NotNull(b);  return *this;};
	EditFileFolder &SelLoad(bool load) 				{isLoad = load; return *this;};
	EditFileFolder &SetTitle(const char *_title)	{title = _title; return *this;};
	EditFileFolder &UseHistory(bool use);
	EditFileFolder &UseUp(bool use);
	EditFileFolder &UseBrowse(bool use);
	//void Xmlize(XmlIO xml);
	
	Callback WhenChange;
};

class EditFile : public EditFileFolder {
typedef EditFile CLASSNAME;		
public:
	EditFile();
};

class EditFolder : public EditFileFolder {
typedef EditFolder CLASSNAME;		
public:
	EditFolder();
};

class StaticImage : public Ctrl {
typedef StaticImage CLASSNAME;		
public:
	enum ImageAngle {Angle_0, Angle_90, Angle_180, Angle_270};
	enum ImageFit   {BestFit, FillFrame, NoScale, RepeatToFill};

protected:
	virtual void Paint(Draw& draw);
	virtual void Layout();
	
	String fileName;
	Image image, origImage;
	Color background;
	int angle, fit;
	bool useAsBackground;

public:
	bool  Set(String fileName);
	bool  Set(Image image);

	void  operator=(String fileName)      	{Set(fileName);}
	void  operator=(Image image)       		{Set(image);}	

	StaticImage& SetAngle(int _angle);
	StaticImage& SetFit(int _fit)				{fit = _fit; 		  Refresh(); return *this;}
	StaticImage& SetBackground(Color c) 		{background = c; 	  Refresh(); return *this;}
	StaticImage& UseAsBackground(bool b = true)	{useAsBackground = b; Refresh(); return *this;}
	StaticImage();
};

#ifndef flagNOPAINTER

class StaticRectangle : public Ctrl {
typedef StaticRectangle CLASSNAME;	
public:
	virtual void Paint(Draw& draw);
	virtual void MouseEnter(Point p, dword keyflags) {WhenMouseEnter(p, keyflags);};
	virtual void MouseLeave() {	WhenMouseLeave();};
	virtual void LeftDown(Point p, dword keyflags) {WhenLeftDown(p, keyflags);};
	virtual void LeftUp(Point p, dword keyflags) {WhenLeftUp(p, keyflags);};
	
protected:
	Color background;
	Color color;
	int width;

public:
	StaticRectangle& SetWidth(int w) 		{width = w; Refresh(); return *this;}
	StaticRectangle& SetColor(Color c) 		{color = c; Refresh(); return *this;}
	StaticRectangle& SetBackground(Color c) {background = c; Refresh(); return *this;}
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
	void PaintPtr(MyBufferPainter &w, double cmx, double cmy, double pos, double m, double d, 
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
	int speed;
	int sensibility;
	volatile Atomic running, kill;
	
	void PaintMarks(MyBufferPainter &w, double cx, double cy, double R, double ang0, 
					double ang1, int direction, double step, double bigF, Color color);
	double AngAdd(double ang, double val);
	void PaintNumbers(MyBufferPainter &w, double cx, double cy, double R, double a0, 
			double step, int direction, double minv, double maxv, double stepv, double bigF, 
			Color color);
	void PaintHand(MyBufferPainter &w, double cx, double cy, double R, double val, double bigF,
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
	Meter& SetSpeed(int s)			{speed = s; Refresh(); return *this;} 
	Meter& SetSensibility(int s)	{sensibility = s; Refresh(); return *this;}

	void SetData(const Value& v);

	Meter();
	~Meter();
};

#endif

class FileBrowser : public StaticRect {
typedef FileBrowser CLASSNAME;	
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
	EditString textFileName;
	Splitter pack;
	StaticRect foldersRect;
	Label foldersLabel;
	TreeCtrlPlus folders;
	ArrayCtrlExternDrop files;
	bool browseFiles;
	bool noDoOpen;
	bool forceOpenTree;
	int fileFlags;
	
	void SortByColumn(int col);
	
	void OpenDir(int id); 
	void CloseDir(int id); 
	void FileSelectedTree(); 
	void FileSelected(String &fileName);
	void OpenFileFilesList();
	void FolderChanged();
	void FilesEnterRow();
	
	void ListFiles(String folderName, bool &thereIsAFolder);
	void AddMyFolder(String folder, String &myFolders, TreeCtrl &folders, int id);
	
public: 
	FileBrowser();
	
	FileBrowser &SetBrowseFiles(bool b);
 	String GetFile();
 	String GetFolder();
 	String operator~()    {return GetFile();}
	void SetFlags(int flags) {fileFlags = flags;};
	int GetFlags() {return fileFlags;}; 	
 	
////////////////////////
	// Parameters
	
	bool foldersInFileList; 	
 	
};

#endif
