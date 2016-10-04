#include "GlDraw.h"

#define LLOG(x) LOG(x)

namespace Upp {
	
struct ImageGLData {
//	Image        img;
	GLuint       texture_id;
	
	void Init(const Image& img);
	~ImageGLData();
};

void ImageGLData::Init(const Image& img)
{
	Size sz = img.GetSize();

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	LLOG("Texture id created: " << texture_id);
//	SysImageRealized(img); // ?
}

static LRUCache<ImageGLData, int64> sTextureCache;
static bool sReset;

ImageGLData::~ImageGLData()
{
//	SysImageReleased(img); // ?
	if(!sReset)
		glDeleteTextures(1, &texture_id);
	LLOG("Texture id deleted: " << texture_id);
}

void GLDraw::ClearCache()
{
	sReset = false;
	sTextureCache.Clear();
}

void GLDraw::ResetCache()
{
	sReset = true;
	sTextureCache.Clear();
	sReset = false;
}

struct ImageGLDataMaker : LRUCache<ImageGLData, int64>::Maker {
	Image         img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(ImageGLData& object) const  { object.Init(img); return img.GetLength(); }
};


void GLDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	if(img.GetLength() == 0)
		return;

	LLOG("PutImage " << img.GetSerialId() << ' ' << p.x << ", " << p.y << ", "<< img.GetSize());
	LLOG("SysImage cache pixels " << sTextureCache.GetSize() << ", count " << sTextureCache.GetCount());
	ImageGLDataMaker m;
	m.img = img;
	ImageGLData& sd = sTextureCache.Get(m);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sd.texture_id);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	if(src == img.GetSize()) {
		Rect r(p, img.GetSize());
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0, 0);
			glVertex2i(r.left, r.top);
			glTexCoord2f(1, 0);
			glVertex2i(r.right, r.top);
			glTexCoord2f(0, 1);
			glVertex2i(r.left, r.bottom);
			glTexCoord2f(1, 1);
			glVertex2i(r.right, r.bottom);
		glEnd();
	}
	else {
		Sizef iszf = img.GetSize();
		Rect s = src & img.GetSize();
		Rect r(p, s.GetSize());
		Rectf h;
		h.left = (double)s.left / iszf.cx;
		h.right = (double)s.right / iszf.cx;
		h.top = (double)s.top / iszf.cy;
		h.bottom = (double)s.bottom / iszf.cy;
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(h.left, h.top);
			glVertex2i(r.left, r.top);
			glTexCoord2f(h.right, h.top);
			glVertex2i(r.right, r.top);
			glTexCoord2f(h.left, h.bottom);
			glVertex2i(r.left, r.bottom);
			glTexCoord2f(h.right, h.bottom);
			glVertex2i(r.right, r.bottom);
		glEnd();
	}

   	glDisable(GL_TEXTURE_2D);

	sTextureCache.Shrink(4 * 1024 * 768, 1000);
}

void GLDraw::SetColor(Color c)
{
	glColor3f(c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

void GLDraw::PutRect(const Rect& r, Color color)
{
	if(color == InvertColor()) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
		SetColor(color);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(r.left, r.top);
		glVertex2i(r.right, r.top);
		glVertex2i(r.left, r.bottom);
		glVertex2i(r.right, r.bottom);
	glEnd();
	if(color == InvertColor())
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLDraw::InitGL(Size sz)
{
	SDraw::Init(sz);

	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

};
