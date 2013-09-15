#include "SDL20.h"

#ifdef MAIN_CONF
#include <main.conf.h>
#endif

#include <RichText/RichText.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <SDL20/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <SDL20/test.iml>
#include <Draw/iml_source.h>

CONSOLE_APP_MAIN {
	LOG("HERE:");

#ifdef PLATFORM_POSIX
//	SetFileFont(0, "/usr/share/fonts/truetype/droid/DroidSans.ttf");
//	SetFileFont(0, "/usr/share/fonts/truetype/droid/DroidSans-Bold.ttf", FtBOLD);

	SetFileFont(Font::SANSSERIF, "/usr/share/fonts/truetype/droid/DroidSans.ttf");
//	SetFileFont(Font::SANSSERIF, "/usr/share/fonts/truetype/droid/DroidSans-Bold.ttf", FtBOLD);
	
	SetStdFont(Arial(20));
#else
	SetFileFont(0, "C:\\u\\exsrc\\droid\\DroidSans.ttf");
	SetFileFont(0, "C:\\u\\exsrc\\droid\\DroidSans-Bold.ttf", FtBOLD);

	SetFileFont(Font::SANSSERIF, "C:\\u\\exsrc\\droid\\DroidSans.ttf");
	SetFileFont(Font::SANSSERIF, "C:\\u\\exsrc\\droid\\DroidSans-Bold.ttf", FtBOLD);
#endif

	SDL_Init(SDL_INIT_EVERYTHING);

	SDLWindow win;
	win.Create(RectC(100, 100, 1024, 768), "First test");

	{
		SystemDraw w;
		w.Set(win);
		Size sz = Size(1024, 768);
		w.Init(sz);
		
		w.DrawRect(sz, White);
		w.DrawText(10, 10, "Hello world!", Arial(40));
	}

	bool quit = false;
	int i = 0;
	while(!quit) {
		SystemDraw w;
		w.Set(win);
		Size sz = Size(1024, 768);
		w.Init(sz);
		
//		w.DrawText(i++, i, "Hello world!", Arial(40));

		
//		w.DrawImage(300, 300, TestImg::pinkie());

		w.DrawRect(0, 0, 1024, 768, White());
		RichText txt = ParseQTF(LoadFile(GetDataFile("text.qtf")));
		if(1) {
			RTIMING("SystemDraw");
			txt.Paint(Zoom(2, 10), w, 0, 0, sz.cx);
		}
		
		w.DrawRect(100, 100, 200, 200, InvertColor());

		win.Present();	

		SDL_Event e;
		while (SDL_PollEvent(&e)){
			//If user closes he window
			if (e.type == SDL_QUIT)
				quit = true;
			//If user presses any key
			if (e.type == SDL_KEYDOWN)
				quit = true;
			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}
		SDL_Delay(20);
	}

	SDL_Quit();
}
