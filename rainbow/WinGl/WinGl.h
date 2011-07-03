#ifndef _WinGl_WinGl_h_
#define _WinGl_WinGl_h_

#define GLEW_STATIC
#include <WinGl/glew.h>
#include <WinGl/wglew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <Painter/Painter.h>

#define GUI_WINGL

NAMESPACE_UPP

extern bool glEndSession;
extern HWND glHWND;
extern HDC    hDC;
extern HGLRC  hRC;

void ActivateGLContext();
void DestroyGL();
LRESULT CALLBACK glWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#define GUI_FB

float GetFps();

#define CLIP_MODE 2
/*
0 - Scissor clip
1 - Plane clip
2 - Stencil clip
3 - Manual clip
*/

struct OpenGLFont : Moveable<OpenGLFont>
{
	float scaleW;
	float scaleH;
	float lineHeight;
	float base;
	
	bool texturesUpdated;

	float vtx[1024 * 4 * 2];
	float crd[1024 * 4 * 2];

	struct CharInfo : Moveable<CharInfo>
	{
		int id;
		float x;
		float y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;
		int page;
	};

	Vector<CharInfo> chars;
	VectorMap<int, VectorMap<int, float>> kerns;
	Vector<String> files;
	Array<const byte> compiledFiles;
	Vector<int64> pages;

	OpenGLFont() : texturesUpdated(false)
	{}

	~OpenGLFont()
	{}

	void LoadBrc(const byte* xml, const byte* image);
	void Load(const String& fileName);
	void Parse(const char* xml, bool parsePages);
	void UpdateTextures();
	void BuildVertices();

};

struct Texture : Moveable<Texture>
{
	Size sz;
	Point curpos;	
	VectorMap<int64, Rect> parts;
	void AddPart(int64 serialId, const Image& img);
};

struct Resources
{
	static int64 currentSerialId;
	static ArrayMap<int64, Texture> textures;
	static VectorMap<String, OpenGLFont> fonts;
	static int64 Bind(const Image& img, bool linear = false);
	static bool Bind(int64 serialId, bool force = false);
	static OpenGLFont& GetFont(const char* fontName);
	static OpenGLFont& GetFontBrc(const char* fontName, const byte* fontDef, const byte* fontImage);
	static OpenGLFont& StdFont(bool bold = false);
};

class SystemDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Size  GetPageSize() const;
	bool    CanSetSurface()                         { return false; }

	void PlaneEquation(double eq[4], float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void SetClipRect(const Rect& r);
	void ScissorClip(const Rect& r);
	void PlaneClip(const Rect& r);
	void StencilClip(const Rect& r, int mode = 0);
	void SetClip(const Rect& r, int mode = 0);

	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;
	virtual Rect GetPaintRect() const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	
	virtual Size GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;

private:

	friend class  FontInfo;
	friend class  Font;

	OpenGLFont fi;
	
	struct Cloff : Moveable<Cloff> {
		bool clipping;
		Point org;
		Rect  drawing_clip;
	};
	
	float current_color[4];
	float vtx[8];
	float crd[8];
	void SetVec(float* v, float sx, float sy, float dx, float dy);
	void SetVec(float* v, int sx, int sy, int dx, int dy);
	
	void SaveCurrentColor();
	void RestoreLastColor();
	
public:
	Rect         drawing_clip;
	Size         drawing_size;
	Point 		 drawing_offset;
	Rect		 clip;
	float        alpha;
	float        angle;
	
private:
	Array<Cloff> cloff;
	int ci;
	int cn;
	int cd;

	void Reset();

protected:

	SystemDraw();
	void InitClip(const Rect& clip);
	void Init();

public:

	static void Flush()        { /*glFlush();*/ }	
	Point    GetOffset() const { return drawing_offset; }
	SystemDraw(HDC hdc, Size sz = Size(0, 0));
	virtual ~SystemDraw();
	void FlatView();
	void Clear();
	void PushContext();
	void PopContext();
};

struct BackDraw__ : public SystemDraw {
	BackDraw__() {}
};

class BackDraw : public BackDraw__ { // Dummy only, as we are running in GlobalBackBuffer mode
	Size        size;
	Draw       *painting;
	Point       painting_offset;
	ImageBuffer ib;
	
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

public:
	void  Put(SystemDraw& w, int x, int y)             {}
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy)         {}
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
	void Destroy()                                     {}

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
};

struct ImageDraw__ {
	ImageBuffer    image;
	ImageBuffer    alpha;
	
	ImageDraw__(int cx, int cy) : image(cx, cy), alpha(cx, cy) {}
};

class ImageDraw : private ImageDraw__, public BufferPainter {
	BufferPainter  alpha_painter;
	bool           has_alpha;

	Image Get(bool pm) const;

public:
	Draw& Alpha();

	operator Image() const               { return Get(true); }
	
	Image GetStraight() const            { return Get(false); }
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
};

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern);

bool GlIsWaitingEvent();
bool GlProcessEvent(bool *quit);
void GlSleep(int ms);
bool GlEndSession();

class PrinterJob { // Dummy only...
	NilDraw             nil;
	Vector<int>         pages;

public:
	Draw&               GetDraw()                       { return nil; }
	operator            Draw&()                         { return GetDraw(); }
	const Vector<int>&  GetPages() const                { return pages; }
	int                 operator[](int i) const         { return 0; }
	int                 GetPageCount() const            { return 0; }

	bool                Execute()                       { return false; }

	PrinterJob& Landscape(bool b = true)                { return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage)    { return *this; }
	PrinterJob& PageCount(int n)                        { return *this; }
	PrinterJob& CurrentPage(int currentpage)            { return *this; }
	PrinterJob& Name(const char *_name)                 { return *this; }

	PrinterJob(const char *name = NULL)                 {}
	~PrinterJob()                                       {}
};

END_UPP_NAMESPACE

#define GUIPLATFORM_KEYCODES_INCLUDE <WinGl/Keys.h>
#define GUIPLATFORM_CTRL_TOP_DECLS
#define GUIPLATFORM_CTRL_DECLS_INCLUDE <WinGl/Ctrl.h>
#define GUIPLATFORM_PASTECLIP_DECLS
#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <WinGl/Top.h>

//#include <CtrlCore/stdids.h>
#include "vkcodes.h" //FIXME

#define GUIPLATFORM_INCLUDE_AFTER <WinGl/After.h>


#endif
