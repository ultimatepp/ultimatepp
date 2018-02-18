#include "Local.h"

#ifdef GUI_SDL20GL

NAMESPACE_UPP

bool SDLWindow::Create(const Rect& rect, const char *title)
{
	win = SDL_CreateWindow(title, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(),
	                       SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS);
	if(!win)
		return false;
	MemoryIgnoreLeaksBegin();
	glcontext = SDL_GL_CreateContext(win);
	MemoryIgnoreLeaksEnd();
	if(!glcontext) {
		Destroy();
		return false;
	}
	INTERLOCKED {
		static int64 h;
		serial = h++;
	}
	return true;
}

void SDLWindow::Destroy()
{
	if(glcontext) {
		SDL_GL_DeleteContext(glcontext);
		glcontext = NULL;
		GLDraw::ResetCache(); // TODO: Consider not reseting ALL cache data, only specific context
	}
	if(win) {
		SDL_DestroyWindow(win);
		win = NULL;
	}
}

SDLWindow::SDLWindow()
{
	glcontext = NULL;
	win = NULL;
}

SDLWindow::~SDLWindow()
{
	Destroy();
}

END_UPP_NAMESPACE

#endif
