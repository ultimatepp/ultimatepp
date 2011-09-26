#include <CtrlCore/CtrlCore.h>
//#include <WinGl/Fonts.brc>

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

int64 Resources::currentSerialId = -1;
ArrayMap<int64, Texture> Resources::textures;
VectorMap<String, OpenGLFont> Resources::fonts;
Vector<DragRect> SystemDraw::dragRect;
FrameInfo frameInfo;

float GetFps()
{
	static float fps = 0.0f;
	static dword updateInterval = 1000;
	static dword timeSinceLastUpdate = 0;
	static dword frameCount = 0;
	static dword currentTick;
	static dword lastTick;
	static bool  isFirst = true;

	if(isFirst)
	{
		currentTick = GetTickCount();
		lastTick = currentTick;
		isFirst = false;
	}

	frameCount++;
	currentTick = GetTickCount();

	dword elapsed = currentTick - lastTick;

	lastTick = currentTick;
	timeSinceLastUpdate += elapsed;

	if (timeSinceLastUpdate > updateInterval)
	{
		if (timeSinceLastUpdate)
		{
			fps = (frameCount / float(timeSinceLastUpdate)) * 1000.f;
			frameCount = 0;
			timeSinceLastUpdate -= updateInterval;
		}
	}

	return fps;
}

void Texture::AddPart(int64 serialId, const Image& img)
{
}

int64 Resources::Bind(const Image& img, bool linear)
{
	int64 serialId = img.GetSerialId();

	if(!Bind(serialId))
		return serialId;

	int textureNumber = textures.Find(serialId);

	if(textureNumber < 0)
	{
		textures.Add(serialId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	}

	return serialId;
}

bool Resources::Bind(int64 serialId, bool force)
{
	if(!force && serialId == currentSerialId)
		return false;
	
	currentSerialId = serialId;
	glBindTexture(GL_TEXTURE_2D, (GLuint) serialId);
	return true;
}

OpenGLFont& Resources::GetFont(const char* fontName, int fontHeight)
{
	int n = fonts.Find(fontName);
	OpenGLFont* fgl = NULL;
	if(n >= 0)
		fgl = &fonts[n];
	else
	{
		fgl = &fonts.Add(fontName);
		fgl->Load(fontName);
	}
	
	fgl->scale = (fontHeight * 96.f / 72.f) / 72.f;
	return *fgl;
}

OpenGLFont& Resources::GetFontBrc(const char* fontName, const byte* fontDef,  const byte** imagesData, const int* imagesSize, int imagesCount, int fontHeight)
{
	int n = fonts.Find(fontName);
	OpenGLFont* fgl = NULL;
	if(n >= 0)
		fgl = &fonts[n];
	else
	{
		fgl = &fonts.Add(fontName);
		fgl->LoadBrc(fontDef, imagesData, imagesSize, imagesCount);
	}
	
	fgl->scale = (fontHeight * 96.f / 72.f) / 72.f;
	return *fgl;
}

OpenGLFont& Resources::GetFont(const Font& font)
{
	if(font.IsBold())
		return GetFontBrc("TahomaB", tahomaBFontDef, (const byte**) tahomaBFontImg, tahomaBFontImg_length, tahomaBFontImg_count, font.GetHeight());
	else
		return GetFontBrc("TahomaN", tahomaNFontDef, (const byte**) tahomaNFontImg, tahomaNFontImg_length, tahomaNFontImg_count, font.GetHeight());

	//return GetFont(bold ? "tahoma14b.fnt" : "tahoma14.fnt");
	/*return GetFontBrc(
	    bold ? "tahoma14b.fnt" : "tahoma14.fnt",
		bold ? resTahoma14Fnt : resTahoma14BoldFnt,
		bold ? resTahoma14Img : resTahoma14BoldImg);*/
}

dword SystemDraw::GetInfo() const
{
	return 0;
}

Size SystemDraw::GetPageSize() const
{
	return Size(0, 0);
}

Size SystemDraw::GetNativeDpi() const
{
	return Size(96, 96);
}

void SystemDraw::BeginNative()
{
}

void SystemDraw::EndNative()
{
}

int SystemDraw::GetCloffLevel() const
{
	return ci;
}

void SystemDraw::InitClip(const Rect& clip)
{
	drawing_clip = clip;
}

void SystemDraw::Reset() {
	cloff.SetCount(20);
	ci = 0;
	cn = 0;
	cd = 0;
	drawing_offset = Point(0, 0);
	alpha = 255;
	angle = 0.f;
	image_coloring = true;
}

SystemDraw::SystemDraw() {
	Reset();
}

SystemDraw::SystemDraw(Size sz) {
	Reset();
	drawing_clip = sz;
	drawing_size = sz;
	clip = sz;
	Init();
}

void SystemDraw::Init()
{
	//glVertexPointer(2, GL_FLOAT, 0, vtx);
	//glTexCoordPointer(2, GL_FLOAT, 0, crd);	
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_ALPHA_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		
#if CLIP_MODE == 0
	glEnable(GL_SCISSOR_TEST);
#elif CLIP_MODE == 1
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
#elif CLIP_MODE == 2
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glClearStencil(0);
#endif
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void SystemDraw::Clear()
{
#if CLIP_MODE == 0
	glDisable(GL_SCISSOR_TEST);
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#if CLIP_MODE == 0
	glEnable(GL_SCISSOR_TEST);
#endif
}

void SystemDraw::FlatView(bool applyTransform)
{
	glViewport(0, 0, (GLsizei) drawing_size.cx, (GLsizei) drawing_size.cy);
	float aspect = drawing_size.cx / (float) drawing_size.cy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, drawing_size.cx, drawing_size.cy, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(applyTransform)
	{
		float dx = (float) drawing_size.cx / 2;
		float dy = (float) drawing_size.cy / 2;
		glTranslatef(dx, dy, 0.f);
		glRotatef(angle, 0, 0, 1);
		glScalef(scale, scale, 1);
		glTranslatef(-dx, -dy, 0.f);
	}
}

void SystemDraw::PushContext()
{
}

void SystemDraw::PopContext()
{
	FlatView();
}

SystemDraw::~SystemDraw() {
}

END_UPP_NAMESPACE

#endif
