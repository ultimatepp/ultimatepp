#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

FrameInfo frameInfo;
SystemDraw* SystemDraw::systemDraw = NULL;
float SystemDraw::blurStrength = 0;
float SystemDraw::grayStrength = 0;

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
	GuiLock __;
	drawing_clip = clip;
}

void SystemDraw::Reset() {
	systemDraw = this;
	cloff.SetCount(32);
	mstack.SetCount(32);
	ci = 0;
	cn = 0;
	cd = 0;
	mi = 0;
	drawing_offset = Point(0, 0);
	alpha = 255;
	r = g = b = a = 255;
	angle = 0;
	scale = 1;
	image_coloring = true;
	projection_mode = 0;
}

SystemDraw::SystemDraw() {
	GuiLock __;
	Reset();
}

SystemDraw::SystemDraw(Size sz) {
	GuiLock __;
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

void SystemDraw::Clear(bool ontransforms)
{
	GuiLock __;
	if(ontransforms && (angle == 0 && scale == 1))
		return;
#if CLIP_MODE == 0
	glDisable(GL_SCISSOR_TEST);
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#if CLIP_MODE == 0
	glEnable(GL_SCISSOR_TEST);
#endif
}

void SystemDraw::ViewPort(int width, int height)
{
	GuiLock __;
	glViewport(0, 0, (GLsizei) width < 0 ? drawing_size.cx : width, (GLsizei) height < 0 ? drawing_size.cy : height);
}

void SystemDraw::OrthogonalView(bool clear_modelview)
{
	GuiLock __;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, drawing_size.cx, drawing_size.cy, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	if(clear_modelview)
		glLoadIdentity();
}

void SystemDraw::PerspectiveView(bool clear_modelview)
{
	GuiLock __;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, GetAspect(), 1.f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	if(clear_modelview)
		glLoadIdentity();
}

void SystemDraw::ApplyTransforms()
{
	GuiLock __;
	float dx = (float) drawing_size.cx / 2;
	float dy = (float) drawing_size.cy / 2;
	glTranslatef(dx, dy, 0.f);
	glRotatef(angle, 0, 0, 1);
	glScalef(scale, scale, 1);
	glTranslatef(-dx, -dy, 0.f);
}

void SystemDraw::PushContext()
{
	GuiLock __;
	MatrixStack& m = mstack[mi++];	
	glGetDoublev(GL_PROJECTION_MATRIX, m.projection_matrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, m.modelview_matrix);
	m.projection_mode = projection_mode;
}

void SystemDraw::PopContext()
{
	GuiLock __;
	MatrixStack& m = mstack[--mi];
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(m.projection_matrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m.modelview_matrix);
	projection_mode = m.projection_mode;
}

SystemDraw::~SystemDraw() {
}

END_UPP_NAMESPACE

#endif
