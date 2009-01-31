class Rasterizer {
	struct Cell : MoveableWithSwap<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };

	Rectf                   cliprect;
	double                  x1, y1;
	Buffer< Vector<Cell> >  cell;
	int                     xmax, ymax;
	int                     min_y;
	int                     max_y;
	Size                    sz;

	void  Init();
	Cell *AddCells(int y, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);

public:
	void LineRaw(int x1, int y1, int x2, int y2);
	
	void Move(double x, double y);
	void Line(double x, double y);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	
	void SetClip(const Rectf& rect);

	template <class Target>
	void Render(int y, Target& g);
	
	void Reset();
	
	Rasterizer(int cx, int cy);
};

inline unsigned Alpha(int area)
{
	int cover = area >> 9;
	if(cover < 0) cover = -cover;
/*	if(evenodd) {
		cover &= 511;
		if(cover > 256)
			cover = 512 - cover;
    }*/
	if(cover > 255) cover = 255;
	return (cover + 1) / 2;
}

template <class Target>
void Rasterizer::Render(int y, Target& g)
{
	PAINTER_TIMING("Render");
	const Cell *c, *e;
	if(!BeginRender(y, c, e))
		return;
	g.Start(c->x);
	int cover = 0;
	while(c < e) {
		int x = c->x;
		int area = c->area;
		cover += c->cover;
		c++;
		while(c < e && c->x == x) {
			area += c->area;
			cover += c->cover;
			c++;
		}
		if(area) {
			g.Render(Alpha((cover << (8 + 1)) - area));
			x++;
		}
		if(c < e && c->x > x)
			g.Render(Alpha(cover << (8 + 1)), c->x - x);
    }
}
