#ifndef _SDLCtrl_demo_h_
#define _SDLCtrl_demo_h_

#define	MAXPOINT  1000
#define	MAXSCROLL 9
	
struct ScreenPoint {
	int x, y;
	int sx, sy;
	int col;
};	
struct ScreenFont {
	int x0, y0;
	int x1, y1;
};
	
class SDLExample : public SDLCtrl {
typedef SDLExample CLASSNAME;	
public:
	SDLExample();
	
	void Demo();
	virtual void Layout();
	
	bool done;
	
private:
	void PutLetter(int x, int y, int n, int col, double ampl);
	void WriteStr(int x, int y, const char *str, int col, double ampl);
	int LengthStr(const char *str);
	void DrawPoints(ScreenPoint *p);
	void MovePoints(ScreenPoint *p, int width, int height);
	void Blur(int width, int height);
	void SetupPalette(SDL_Surface * screen);
		
	double sintab[2048];
	ScreenPoint points[MAXPOINT];
	SDLSurface fntbmp;
	SDLSurface surf;
	
	bool demoInitialized;
	int j;
	int k;
	int len0;
	int len1;
	int xmax;
	int x0;
	int x1;
};

#endif
