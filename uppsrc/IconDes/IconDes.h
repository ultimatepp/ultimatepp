#ifndef _IconDes_IconDes_h_
#define _IconDes_IconDes_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <RichEdit/RichEdit.h>
#include <plugin/bmp/bmp.h>

namespace Upp {

#define IMAGECLASS IconDesImg
#define IMAGEFILE <IconDes/IconDes.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <IconDes/IconDes.lay>
#include <CtrlCore/lay.h>

#define KEYNAMESPACE IconDesKeys
#define KEYGROUPNAME "Icon designer"
#define KEYFILE      <IconDes/IconDes.key>
#include             <CtrlLib/key_header.h>

class AlphaCtrl : public Ctrl {
public:
	virtual void Layout();
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);

private:
	Image img;
	Color color;
	int   alpha;
	bool  mask;
	int   maskvalue;

	void MakeImage();
	void DoPoint(Point p);

public:
	void SetColor(Color c);
	void Set(int a);
	int  Get() const;
	void Mask(bool m);
	bool IsMask() const                   { return mask; }

	AlphaCtrl();
};

class RGBACtrl : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);

private:
	Size  cbox;
	Point cs;
	enum { COLORCOUNT = 216 + 18 + 18 };

	Color         color;
	ColorRampCtrl ramp;
	AlphaCtrl     alpha;
	EditString    text;
	FrameRight<Button> setcolor;
	Ptr<Ctrl>     subctrl;

	Color GetColor(int i) const;
	void  Ramp();
	void  Alpha();
	void  SyncText();
	int   DoLayout(Size sz, bool set);

public:
	typedef RGBACtrl CLASSNAME;

	void Set(RGBA c);
	void MaskSet(int a);
	RGBA Get() const;
	
	Color GetColor() const;
	int   GetAlpha() const;

	void Mask(bool b);

	void SubCtrl(Ctrl *c);
	
	int  GetHeight(int cx);

	RGBACtrl();
};

struct IconShow : public Ctrl {
	Image image;
	bool  show_small;
	bool  show_other;

	void Paint(Draw& w);

	IconShow() { BackPaint(); }
};

void   FloodFill(ImageBuffer& img, RGBA color, Point pt, const Rect& rc, int tolerance);
void   InterpolateImage(Image& img, const Rect& _rc);
void   MirrorHorz(Image& img, const Rect& rect);
void   MirrorVert(Image& img, const Rect& rect);
String PackImlData(const Vector<ImageIml>& image);
Image  DownSample3x(const Image& src);
Image  DownSample2x(const Image& src);

/*
struct IconDraw : ImagePainter {
	IconDraw(Size sz) : ImagePainter(sz, MODE_NOAA) {}
};
*/

struct IconDraw : NilDraw, DDARasterizer {
	RGBA        docolor;
	ImageBuffer image;
	
	virtual void PutHorz(int x, int y, int cx);
	virtual void PutVert(int x, int y, int cy);

	virtual void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	
	operator Image() { return image; }
	
	IconDraw(Size sz) { image.Create(sz); Cy(sz.cy); }
};

class IconDes : public Ctrl {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  MouseEnter(Point, dword)                 { SyncStatus(); }
	virtual void  MouseLeave()                             { SyncStatus(); }
	virtual void  LeftUp(Point p, dword keyflags);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void  RightDown(Point p, dword keyflags);
	virtual bool  Key(dword key, int count);
	virtual Image CursorImage(Point p, dword keyflags);

private:
	struct Slot {
		String          name;
		Image           image;
		Image           base_image;
		Image           selection;
		Point           pastepos;
		Image           paste_image;
		String          undo;
		String          redo;
		bool            exp;
		dword           flags = 0;

		Slot();
	};
	
	enum {
		PASTE_TRANSPARENT, PASTE_OPAQUE, PASTE_BACK
	};

	Array<Slot>  slot;
	Slot         dummy;

	int          magnify;
	int          pen;
	Point        startpoint;
	Rect         m1refresh;
	void        (IconDes::*tool)(Point p, dword flags);
	bool         doselection = false;
	bool         selectrect = false;
	int          paste_mode;
	bool         show_other = false;
	bool         show_small = false;
	bool         show_grid2 = false;

	ScrollBars   sb;
	Scroller     scroller;
	ToolBar      toolbar;

	SplitterFrame  leftpane;
	SplitterFrame  bottompane;

	ParentCtrl     imgs;
	ArrayCtrl      ilist;
	EditString     search;

	RGBACtrl       rgbactrl;
	IconShow       iconshow;
	Image          cursor_image;
	Image          fill_cursor, fill_cursor2, fill_cursor3, antifill_cursor;
	bool           single_mode;
	ParentCtrl     single;
	Label          info;
	Button         resize;

	Array<Slot>    removed;

	FrameRight<Label> status;

	int syncinglist = 0;
	
	Rect           rect;

	struct TextDlg : WithIconDesTextLayout<TopWindow> {
		typedef TextDlg CLASSNAME;
		
		Font GetFont();
		
		TextDlg();
	};
	
	TextDlg        textdlg;

	void  PenSet(Point p, dword flags);

	void  LineTool(Point p, dword f);
	void  FreehandTool(Point p, dword f);

	void  EllipseTool0(Point p, dword flags, Color inner);
	void  EllipseTool(Point p, dword f);
	void  EmptyEllipseTool(Point p, dword f);

	void  RectTool0(Point p, dword f, bool empty);
	void  RectTool(Point p, dword f);
	void  EmptyRectTool(Point p, dword f);

	void  HotSpotTool(Point p, dword f);

	void  DoFill(int tolerance);
	void  FillTool(Point p, dword flags);
	void  Fill2Tool(Point p, dword flags);
	void  Fill3Tool(Point p, dword flags);
	void  AntiFillTool(Point p, dword flags);

	void  Text();
	void  PasteText();
	void  CloseText();

	bool         IsCurrent()            { return !IsNull(ilist.GetKey()); }
	Slot&        Current();

	Image&       CurrentImage();
	void         SetCurrentImage(ImageBuffer& ib);
	RGBA         CurrentColor();
	Size         GetImageSize();
	bool         InImage(int x, int y);
	bool         InImage(Point p)       { return InImage(p.x, p.y); }


	bool  IsHotSpot() const             { return tool == &IconDes::HotSpotTool; }

	Image MakeIconDesCursor(const Image& arrow, const Image& cmask);

	void  SyncShow();

	void  RefreshPixel(Point p, int cx = 1, int cy = 1);
	void  RefreshPixel(int x, int y, int cx = 1, int cy = 1);
	Point GetPos(Point p);
	void  Set(Point p, RGBA rgba, dword flags);
	void  ApplyDraw(IconDraw& iw, dword flags);
	void  ApplyImage(Image m, dword flags, bool alpha = false);

	void  SyncImage();
	void  Reset();

	bool  IsPasting()                   { return IsCurrent() && !IsNull(Current().pastepos); }
	void  MakePaste();
	void  Paste(const Image& img);
	void  FinishPaste();

	void  SetSelect(int a);
	Rect  SelectionRect();
	void  Select();
	void  InvertSelect();
	void  CancelSelect();
	void  SelectRect();
	Image Copy(const Rect& r);
	void  Delete();
	void  Move();
	void  MaskSelection();

	void  SetSb();
	void  Scroll();

	void  SetTool(void (IconDes::*tool)(Point p, dword flags));

	void  SetMagnify(int mag);
	void  ZoomIn();
	void  ZoomOut();

	void  SetPen(int pen);

	void  MainToolBar(Bar& bar);
	void  SetBar();

	void  SaveUndo();
	void  Undo();
	void  Redo();

	void  DoPaste();
	void  DoCopy();
	void  DoCut();

	void  ColorChanged();

	void  MaskFill(ImageBuffer& ib, RGBA color, int mask);
	void  SetColor0(RGBA color);
	void  SetColor();
	void  DoDelete();
	void  Interpolate();
	bool  BeginTransform();
	void  MirrorX();
	void  MirrorY();
	void  SymmX();
	void  SymmY();
	void  FreeRotate();
	void  Rotate();
	void  SmoothRescale();
	void  KeyMove(int dx, int dy);

	void  BeginResize();
	void  ResizeUp();
	void  ResizeDown();
	void  ResizeUp2();
	void  ResizeDown2();
	void  Upscale();

	void  PlaceDlg(TopWindow& dlg);
	Image ImageStart();
	void  ImageSet(const Image& m);
	void  BlurSharpen();
	void  Colorize();
	void  Chroma();
	void  Contrast();
	void  Alpha();
	void  Colors();
	void  Smoothen();

	void  Search();
	void  GoTo(int q);
	void  SyncList();
	void  ListCursor();
	void  PrepareImageDlg(WithImageLayout<TopWindow>& dlg);
	void  PrepareImageSizeDlg(WithImageSizeLayout<TopWindow>& dlg);
	void  SyncDlg(WithImageLayout<TopWindow>& dlg);
	dword GetFlags(WithImageLayout<TopWindow>& dlg);
	Slot& ImageInsert(int ii, const String& name, const Image& m, bool exp = false);
	Slot& ImageInsert(const String& name, const Image& m, bool exp = false);
	void  Slice();
	void  InsertImage();
	void  InsertRemoved(int ii);
	void  EditImageSize();
	void  EditImage();
	void  RemoveImage();
	void  Duplicate();
	void  InsertPaste();
	void  InsertFile();
	void  ExportPngs();
	void  InsertIml();
	void  MoveSlot(int d);
	void  ChangeSlot(int d);
	void  DnDInsert(int line, PasteClip& d);
	void  Drag();

	static FileSel& ImgFile();
	static String FormatImageName(const Slot& c);

public:
	virtual void ToolEx(Bar& bar);
	virtual void ListMenuEx(Bar& bar);

	void  ListMenu(Bar& bar);
	void  EditBar(Bar& bar);
	void  SettingBar(Bar& bar);
	void  SelectBar(Bar& bar);
	void  ImageBar(Bar& bar);
	void  DrawBar(Bar& bar);

	struct EditPosSlot : Moveable<EditPosSlot> {
		String          undo;
		String          redo;
		String          selection;
		bool            supersampling;
	};

	struct EditPos {
		Vector<EditPosSlot> slot;
		int                 cursor, sc;
	};

	void   Clear();
	Slot&  AddImage(const String& name, const Image& image, bool exp);
	int    GetCount() const;
	Image  GetImage(int ii) const;
	String GetName(int ii) const;
	bool   GetExport(int ii) const;
	dword  GetFlags(int ii) const;
	bool   FindName(const String& name);

	String GetCurrentName() const;
	
	void    SyncStatus();

	EditPos GetEditPos();
	void    SetEditPos(const EditPos& o);

	void    SerializeSettings(Stream& s);
	
	void    SingleMode();
	bool    IsSingleMode() const                  { return single_mode; }

	typedef IconDes CLASSNAME;

	IconDes();
};

struct ImlImage : ImageIml {
	String name;
	bool   exp;
};

bool   LoadIml(const String& data, Array<ImlImage>& img, int& format);
String SaveIml(const Array<ImlImage>& iml, int format, const String& eol = "\r\n");

void SetRes(Image& m, int resolution);

}

#endif
