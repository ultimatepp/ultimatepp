#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

FrameInfo frameInfo;
SystemDraw* SystemDraw::systemDraw = NULL;

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
	systemDraw = this;
	cloff.SetCount(30);
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
