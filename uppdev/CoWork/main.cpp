#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS MyImg
#define IMAGEFILE  <CoWork/Img.iml>
#include <Draw/iml.h>

using namespace Upp;

class App : public TopWindow {
public:
	typedef App CLASSNAME;
	virtual void LeftDown(Point, dword);
	virtual void Paint(Draw& w);

private:
	String text;
	bool   cowork;
	int    phase;

/* extra stuff */
	CoWork co;
	bool dorun;
	
	TreeCtrl tGroups2;
	void TestDropInsert( int parent, int ii, PasteClip& d );
	void TestDrag();
	void TestTree();
	void DoRefresh();
	void Start();
/* ...... */

	void   SyncTitle();

public:
	typedef App CLASSNAME;

	App& Text(const String& t)     { text = t; Refresh(); return *this; }

	App();

	//extra stuff
	~App() { dorun = false; }
};




/* extra stuff */

void App::Start()
{
	for(int i = 0; i < 10; i++)
	{
		co & THISBACK(DoRefresh);	
	}
}

void App::TestTree() {

	TreeCtrl & tree = tGroups2;

	Vector<int> parent, parent2;
	parent.Add(0);
	tree.SetRoot(Image(), "The Tree");
	for(int i = 1; i < 100; i++) {
		parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
		            FormatIntRoman(i, true)));
		if((rand() & 3) == 0)
			tree.Open(parent.Top());
	}

	tree.Open(0);
	tree.WhenDropInsert = THISBACK(TestDropInsert);
	tree.WhenDrag = THISBACK(TestDrag);
	tree.MultiSelect();
}

void App::TestDropInsert(int parent, int ii, PasteClip& d)
{
	/*
	if(AcceptInternal<TreeCtrl>(d, "mytreedrag")) {
		tGroups2.InsertDrop(parent, ii, d);
		tGroups2.SetFocus();
		return;
	}
	if(AcceptText(d)) {
		tGroups2.SetCursor(tGroups2.Insert(parent, ii, Image(), GetString(d)));
		tGroups2.SetFocus();
		return;
	}
	*/
}

void App::TestDrag()
{
	if(tGroups2.DoDragAndDrop(InternalClip(tGroups2, "mytreedrag"),
	                       tGroups2.GetDragSample()) == DND_MOVE)
	{
		tGroups2.RemoveSelection();
	}
	int a = 0;
}

void App::DoRefresh()
{
	while(dorun)
	{
		Refresh();
		Sleep(1);
	}
}

/* ..... */


App::App()
{
	SetRect(0,0,472,320);
	BackPaint();
	Zoomable().Sizeable();
	cowork = true;
	phase = 0;
	SyncTitle();

	/* extra stuff */	
	Add(tGroups2.LeftPos(0,100).VSizePos());
	TestTree();
//	PostCallback(THISBACK(Start));
	
	
	/* ... */
}

void App::SyncTitle()
{
	Title(cowork ? "Now using all CPU cores (click the image for single core)"
	             : "Now using single CPU core (click the image to use all cores)");
}

void App::LeftDown(Point, dword)
{
	cowork = !cowork;
	SyncTitle();
}

void DoLine(RGBA *line, int phase, int y, Size sz)
{
	Point c = sz / 2; //center is rotation point
	c = Point(	int(sin((double)phase / 131) * c.x/2 + c.x), 
				int(cos((double)phase / 127) * c.y/2 + c.y)
			);

	int yy = (y - c.y) * (y - c.y);

	for(int x = 0; x < sz.cx; x++) { //for the x points
		double d = (x - c.x) * (x - c.x) + yy;
		line[x] = 
		//GrayColor((int)(120 + 120 * sin(d / 1000 - (double)phase / 5)));
		Blend(Black, LtBlue, (int)(120 + 120 * sin(d / 3000 - (double)phase / 5)));
	}
}

struct EXT
{
	RGBA *line;
	int y;
	int phase;
	Size sz;
	Point c;
	Color col;
};

void DoLine2(EXT e)
{
	int yy = (e.y - e.c.y) * (e.y - e.c.y);
	for(int x = 0; x < e.sz.cx; x++) { //for the x points
		double d = (x - e.c.x) * (x - e.c.x) + yy;
		e.line[x] = 
		//GrayColor((int)(120 + 120 * sin(d / 1000 - (double)phase / 5)));
		Blend(Black, e.col, (int)(120 + 120 * sin(d / 2000 - (double)e.phase / 5)));
	}
}


void App::Paint(Draw& w)
{
	Size sz = GetSize();
	ImageBuffer ib(sz);

/*
	if(cowork) {
		CoWork co;
		for(int y = 0; y < sz.cy; y++)
			co & callback4(DoLine, ib[y], phase, y, sz); //calculate each line separately
	}
	else
		for(int y = 0; y < sz.cy; y++)
			DoLine(ib[y], phase, y, sz); //calculate each line separately
*/

/*
	EXT e;
	e.phase = phase;
	e.sz = sz;
	Point c = e.sz / 2; //center is rotation point
	//e.col = Color(0, 69, 149);
	e.col = HsvColorf((float)(phase%360)/360.0f, 1, 1);
	if(1)
	{
	e.c = Point(	int(sin((double)e.phase / 131) * c.x/2 + c.x), 
				int(cos((double)e.phase / 127) * c.y/2 + c.y)
			);
	}
	else
		e.c = c;
	
	if(cowork) {
		CoWork co;
		for(int y = 0; y < sz.cy; y++)
		{
			e.line = ib[y]; e.y = y;
			co & callback1(DoLine2, e); //calculate each line separately
		}
	}
	else
		for(int y = 0; y < sz.cy; y++)
		{
			e.line = ib[y]; e.y = y;
			DoLine2(e); //calculate each line separately
		}
*/
	w.DrawImage(0, 0, ib);

	const Image & im = MyImg::dfmaudio();
	Size isz = im.GetSize();
	Point pp = isz/2;
//	w.DrawImage(e.c.x-pp.x, e.c.y-pp.y, im);
	w.DrawImage(0+(phase%200), 0, im);
	//Refresh(); // is done in the cowork
	phase++;
}

GUI_APP_MAIN
{
	App app;
	a
	App().Run();
}
