#include "CtrlLib/CtrlLib.h"

using namespace Upp;

#define LAYOUTFILE <Puzzle/Puzzle.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE  <Puzzle/Puzzle.iml>
#define IMAGECLASS PuzzleImg
#include <Draw/iml.h>

#define TFILE <Puzzle/Puzzle.t>
#include <Core/t.h>

class Puzzle : public TopWindow {
public:
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword);

private:
	Buffer<int>            box;
	Size                   size;
	Point                  hole;
	int                    moves;
	int                    lang;
	VectorMap<String, int> score;

	int& Get(int x, int y)           { return box[size.cx * y + x]; }

	bool Move(int x, int y);
	void ShowStatus();
	void Init();

	void About();
	void Scores();
	void Setup();
	void New();

	void GameMenu(Bar& bar);
	void MainMenu(Bar& bar);

	MenuBar   menu;
	StatusBar status;

	typedef Puzzle CLASSNAME;

public:
	void Xmlize(XmlIO xml);
	void Generate();

	Puzzle();
};

bool Puzzle::Move(int x, int y)
{
	if(x >= 0 && x < size.cx && y >= 0 && y < size.cy &&
	   (y == hole.y && abs(x - hole.x) == 1 || x == hole.x && abs(y - hole.y) == 1)) {
	       Swap(Get(x, y), Get(hole.x, hole.y));
	    hole = Point(x, y);
	    Refresh();
		return true;
	}
	return false;
}

void Puzzle::ShowStatus()
{
	String txt = t_("Moves: ");
	txt << moves;
	status = txt;
}

void Puzzle::Generate()
{
	int n = size.cx * size.cy;
	box.Alloc(n);
	n--;
	for(int i = 0; i < n; i++)
		box[i] = i + 1;
	box[n] = 0;
	hole = size - 1;
	n = 20 * size.cx * size.cy;
	while(n > 0)
		n -= Move(hole.x + (rand() % 3) - 1, hole.y + (rand() % 3) - 1);
	moves = 0;
	ShowStatus();
	SetRect(GetWorkArea().CenterRect(AddFrameSize(size * 32)));
}

void Puzzle::Paint(Draw& w)
{
	w.DrawRect(GetSize(), WhiteGray);
	for(int y = 0; y < size.cy; y++)
		for(int x = 0; x < size.cx; x++) {
			int b = Get(x, y);
			Point p(x * 32, y * 32);
			if(b) {
				w.DrawImage(p.x, p.y, PuzzleImg::BoxImg());
				String txt = AsString(b);
				Size sz = GetTextSize(txt, Arial(20).Bold());
				w.DrawText(p.x + (32 - sz.cx) / 2, p.y + (32 - sz.cy) / 2, txt,
				           Arial(20).Bold(), SLtBlue);
			}
		}
}

void Puzzle::LeftDown(Point p, dword)
{
	if(Move(p.x / 32, p.y / 32)) {
		moves++;
		ShowStatus();
		int n = size.cx * size.cy - 1;
		for(int i = 0; i < n; i++)
			if(box[i] != i + 1)
				return;
		String dim;
		int& sm = score.GetAdd(String().Cat() << size.cx << "x" << size.cy);
		if(moves < sm) {
			sm = moves;
			PromptOK(t_("You win with the new best score!"));
		}
		else
			PromptOK(t_("You win!"));
		Generate();
	}
	else
		BeepExclamation();
}

void Puzzle::About()
{
	PromptOK("[A5 Puzzle]&Using [*^https`:`/`/www`.ultimatepp`.org`/^ U`+`+] technology.");
}

void Puzzle::Scores()
{
	WithScoreLayout<TopWindow> d;
	CtrlLayoutOK(d, t_("Best scores"));
	d.score.AddColumn(t_("Dimension"));
	d.score.AddColumn(t_("Moves"));
	d.score.ColumnWidths("71 48");
	d.score.NoCursor().NoGrid();
	Vector<int> o = GetSortOrder(score.GetKeys());
	for(int i = 0; i < o.GetCount(); i++)
		d.score.Add(score.GetKey(o[i]), score[o[i]]);
	d.Run();
}

void Puzzle::Setup()
{
	WithSetupLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, t_("Puzzle setup"));
	CtrlRetriever r;
	Size sz = size;
	r
		(d.cx, size.cx)
		(d.cy, size.cy)
		(d.lang, lang)
	;
	if(d.Run() == IDOK) {
		r.Retrieve();
		Init();
		if(sz != size)
			Generate();
	}
}

void Puzzle::New()
{
	if(PromptYesNo(t_("Start a new game?")))
		Generate();
}

void Puzzle::GameMenu(Bar& bar)
{
	bar.Add(t_("New game"), THISBACK(New));
	bar.Add(t_("Setup.."), THISBACK(Setup));
	bar.Add(t_("Best scores.."), THISBACK(Scores));
	bar.Separator();
	bar.Add(t_("Exit"), Breaker());
}

void Puzzle::MainMenu(Bar& bar)
{
	bar.Add(t_("Game"), THISBACK(GameMenu));
	bar.Add(t_("About"), THISBACK(About));
}

void Puzzle::Init()
{
	SetLanguage(lang);
	SetDefaultCharset(CHARSET_UTF8);
	menu.Set(THISBACK(MainMenu));
	ShowStatus();
}

void Puzzle::Xmlize(XmlIO xml)
{
	XmlizeLangAttr(xml, lang);
	xml("Scores", score)
	   ("Dimension", size);
	Init();
}

Puzzle::Puzzle()
{
	size = Size(4, 4);
	AddFrame(menu);
	AddFrame(status);
	lang = LNG_ENGLISH;
	Init();
	BackPaint();
}

GUI_APP_MAIN
{
	Puzzle p;
	LoadFromXMLFile(p);
	p.Generate();
	p.Run();
	StoreAsXMLFile(p);
}
