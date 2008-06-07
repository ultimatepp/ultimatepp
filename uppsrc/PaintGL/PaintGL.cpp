#include "PaintGL.h"

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking
#pragma comment( lib, "glaux.lib" )								// Search For GLaux.lib While Linking

PaintGL::PaintGL()
{
	size = Null;
	hbmp = ohbmp = NULL;
	hdc = NULL;
	hrc = NULL;
}

PaintGL::~PaintGL()
{
	Free();
}

void PaintGL::Free()
{
	if(hrc) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		SelectObject(hdc, ohbmp);
		DeleteDC(hdc);
		DeleteObject(hbmp);
		hrc = NULL;
		hbmp = ohbmp = NULL;
		hdc = NULL;
		hbmp = NULL;
	}
	size = Null;
}


void PaintGL::Init(Size sz) {
	Free();

	size = sz;

	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biWidth = ((((int) sz.cx * 8) + 31) & ~31) >> 3;
	bih.biHeight = sz.cy;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;

	hdc = CreateCompatibleDC(NULL);

	void *dummy;

	hbmp = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_PAL_COLORS, &dummy, NULL, 0);

	ohbmp = (HBITMAP)SelectObject(hdc, hbmp);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL/* | PFD_SUPPORT_GDI | PFD_ACCELERATED*/;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	GLuint PixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, PixelFormat, &pfd);
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void PaintGL::Paint(Draw& w, const Rect& r, Callback gl)
{
	if(r.Size() != size)
		Init(r.Size());
	gl();
	glFlush();
	HDC whdc = w.BeginGdi();
	BitBlt(whdc, 0, 0, size.cx, size.cy, hdc, r.left, r.top, SRCCOPY);
	w.EndGdi();
}
