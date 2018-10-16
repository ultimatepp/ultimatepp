#include "GLDraw.h"

#define LTIMING(x) // RTIMING(x)

#define LLOG(x) // DLOG(x)

namespace Upp {
	
GLuint CreateGLTexture(const Image& img, dword flags)
{
	Size sz = img.GetSize();
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
	if(flags & TEXTURE_MIPMAP)
		glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags & TEXTURE_LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags & TEXTURE_LINEAR ?
																flags & TEXTURE_MIPMAP ? GL_LINEAR_MIPMAP_LINEAR
																                       : GL_LINEAR
																                 : GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texture_id;
}

struct ImageGLData {
	Image        img;
	GLuint       texture_id;
	
	void Init(const Image& img, dword flags);
	~ImageGLData();
};

void ImageGLData::Init(const Image& img, dword flags)
{
	LTIMING("CreateTexture");

	texture_id = CreateGLTexture(img, flags);

	LLOG("Texture id created: " << texture_id);
	SysImageRealized(img);
}

static LRUCache<ImageGLData, Tuple<uint64, uint64, dword> > sTextureCache;
static bool sReset;

ImageGLData::~ImageGLData()
{
	SysImageReleased(img);
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

struct ImageGLDataMaker : LRUCache<ImageGLData, Tuple<uint64, uint64, dword> >::Maker {
	Image         img;
	uint64        context;
	dword         flags;

	virtual Tuple<uint64, uint64, dword>  Key() const                      { return MakeTuple(img.GetSerialId(), context, flags); }
	virtual int                           Make(ImageGLData& object) const  { object.Init(img, flags); return img.GetLength(); }
};

int max_texture_memory = 4 * 1024 * 768;
int max_texture_count = 1000;

GLuint GetTextureForImage(dword flags, const Image& img, uint64 context)
{
	sTextureCache.Shrink(max_texture_memory, max_texture_count);
	ImageGLDataMaker m;
	m.img = img;
	m.context = context;
	m.flags = flags;
	return sTextureCache.Get(m).texture_id;
}

};