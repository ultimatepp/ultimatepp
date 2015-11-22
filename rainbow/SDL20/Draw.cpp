#include "Local.h"

#ifdef GUI_SDL20

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

struct RectSDL {
	SDL_Rect sr;
	
	operator SDL_Rect *() { return &sr; }
	
	RectSDL(const Rect& r)
	{
		sr.x = r.left;
		sr.y = r.top;
		sr.w = r.GetWidth();
		sr.h = r.GetHeight();
	}
};

SDL_Texture *TextureFromImage(SDL_Renderer *renderer, const Image& m)
{
	Size isz = m.GetSize();
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                                         SDL_TEXTUREACCESS_STATIC, isz.cx, isz.cy);
	if(texture) {
		SDL_UpdateTexture(texture, RectSDL(isz), ~m, isz.cx * sizeof(RGBA));
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
	return texture;
}

struct ImageSysData {
	Image        img;
	SDL_Texture *texture;
	
	void Init(SDL_Renderer *renderer, const Image& img);
	~ImageSysData();
};

void ImageSysData::Init(SDL_Renderer *renderer, const Image& m)
{
	img = m;
	texture = TextureFromImage(renderer, img);
	SysImageRealized(img);
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	SDL_DestroyTexture(texture);
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Maker {
	Image         img;
	int64         wserial;
	SDL_Renderer *renderer;

	virtual int64  Key() const                      { return img.GetSerialId(); } // TODO! add ren_serial
	virtual int    Make(ImageSysData& object) const { object.Init(renderer, img); return img.GetLength(); }
};

void SystemDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	if(img.GetLength() == 0 || !win)
		return;
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << p.x << ", " << p.y << ", "<< img.GetSize());
	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	m.renderer = win->ren;
	m.wserial = win->serial;
	ImageSysData& sd = cache.Get(m);
	{
		RTIMING("SDL_RenderCopy");
		SDL_RenderCopy(win->ren, sd.texture, RectSDL(src), RectSDL(Rect(p, src.GetSize())));
	}
	cache.Shrink(4 * 1024 * 768, 1000); // Cache shrink must be after Paint because of PaintOnly!
}

void SystemDraw::PutRect(const Rect& r, Color color)
{
	if(win && !IsNull(color)) {
		SDL_SetRenderDrawColor(win->ren, color.GetR(), color.GetG(), color.GetB(), 255); // Optimize?
		SDL_RenderFillRect(win->ren, RectSDL(r));
	}
}

void SystemDraw::Set(SDLWindow& win_)
{
	win = win_.ren ? &win_ : NULL;
}

SystemDraw::SystemDraw()
{
	win = NULL;
}

SystemDraw::~SystemDraw()
{
}

END_UPP_NAMESPACE

#endif
