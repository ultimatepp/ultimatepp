#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Bombs : public TopWindow {
public:
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword flags);
	virtual void RightDown(Point p, dword flags);

private:
	Size         level;
	int          cx, cy;
	int          normal_cells, bombs;
	Buffer<byte> field;
	MenuBar      menu;
	StatusBar    status;

	byte& Field(int x, int y) { return field[x + y * cx]; }

	enum {
		HIDDEN = 16,
		BOMB = 32,
		MARK = 64,
		EXPLODED = 128,
	};
	
	int UNIT = 30;

	void About();

	void File(Bar& menu);
	void Game(Bar& menu);
	void Menu(Bar& menu);

	void ShowStatus();
	void Level(Size sz);

	void Uncover(int x, int y);
	void Generate();
	void UncoverAll();

public:
	typedef Bombs CLASSNAME;
	Bombs();
};

void Bombs::Generate()
{
	cx = level.cx;
	cy = level.cy;
	field.Alloc(cx * cy);
	for(int i = cx * cy - 1; i >= 0; i--)
		field[i] = (rand() & 15) < 3 ? HIDDEN|BOMB : HIDDEN;
	normal_cells = 0;
	for(int x = 0; x < cx; x++)
		for(int y = 0; y < cy; y++)
			if((Field(x, y) & BOMB) == 0) {
				normal_cells++;
				for(int xx = -1; xx <= 1; xx++)
					for(int yy = -1; yy <= 1; yy++)
						if((xx || yy) && x + xx >= 0 && x + xx < cx && y + yy >= 0 && y + yy < cy &&
						   (Field(x + xx, y + yy) & BOMB))
							Field(x, y)++;
			}
	bombs = cx * cy - normal_cells;
	Rect r = GetRect();
	r.SetSize(AddFrameSize(UNIT * cx, UNIT * cy));
	SetRect(r);
	ShowStatus();
	Refresh();
}

void Bombs::UncoverAll()
{
	for(int i = cx * cy - 1; i >= 0; i--)
		field[i] &= ~HIDDEN;
	Refresh();
}

void Bombs::Paint(Draw& w)
{
	for(int x = 0; x < cx; x++)
		for(int y = 0; y < cy; y++) {
			byte f = Field(x, y);
			w.DrawRect(x * UNIT, y * UNIT + UNIT - 1, UNIT, 1, SBlack);
			w.DrawRect(x * UNIT + UNIT - 1, y * UNIT, 1, UNIT, SBlack);
			w.DrawRect(x * UNIT, y * UNIT, UNIT - 1, UNIT - 1,
			           (f & (HIDDEN|MARK)) ? SLtGray : f & BOMB ? SLtRed : SWhite);
			String txt;
			Color ink = SBlack;
			Color cross = Null;
			if(f & MARK) {
				txt = "M";
				ink = SLtRed;
				if((f & (HIDDEN|BOMB)) == BOMB) {
					ink = SLtBlue;
					cross = SLtRed;
				}
			}
			else
			if(!(f & HIDDEN)) {
				if(f & BOMB)
					txt = "B";
				else {
					f = f & 15;
					txt = String(f + '0', 1);
					ink = f == 0 ? SLtGreen : f == 1 ? SLtBlue : SBlack;
				}
			}
			Size tsz = GetTextSize(txt, Roman(2 * UNIT / 3));
			w.DrawText(x * UNIT + (UNIT - tsz.cx) / 2, y * UNIT + (UNIT - tsz.cy) / 2,
			           txt, Roman(2 * UNIT / 3), ink);
			if(f & EXPLODED)
				cross = SLtBlue;
			w.DrawLine(x * UNIT, y * UNIT, x * UNIT + UNIT - 1, y * UNIT + UNIT - 1, 1, cross);
			w.DrawLine(x * UNIT, y * UNIT + UNIT - 1, x * UNIT + UNIT - 1, y * UNIT, 1, cross);
		}
}

void Bombs::Uncover(int x, int y)
{
	if(x >= 0 && x < cx && y >= 0 && y < cy) {
		byte& f = Field(x, y);
		if((f & (HIDDEN|MARK)) == HIDDEN) {
			if(f & BOMB) {
				f |= EXPLODED;
				normal_cells = 0;
				UncoverAll();
				return;
			}
			if((f &= ~HIDDEN) == 0)
				for(int xx = -1; xx <= 1; xx++)
					for(int yy = -1; yy <= 1; yy++)
						if(xx || yy)
							Uncover(x + xx, y + yy);
			normal_cells--;
			if(normal_cells == 0) {
				UncoverAll();
				PromptOK("[*@4A6 Nice!]&You have found all the bombs!");
			}
		}
	}
}

void Bombs::LeftDown(Point p, dword flags)
{
	if(!normal_cells)
		return;
	p /= UNIT;
	Uncover(p.x, p.y);
	Refresh();
	ShowStatus();
}

void Bombs::RightDown(Point p, dword flags)
{
	if(!normal_cells)
		return;
	p /= UNIT;
	if(Field(p.x, p.y) & HIDDEN) {
		Field(p.x, p.y) ^= MARK;
		Refresh();
	}
}

void Bombs::ShowStatus()
{
	status = Format("%d bombs, %d cells remaining", bombs, normal_cells);
}

void Bombs::Level(Size sz)
{
	level = sz;
}

void Bombs::About()
{
	PromptOK("[*A9/ uBombs]&[A5 Ultimate`+`+ example]");
}

void Bombs::File(Bar& menu)
{
	menu.Add("Exit", Breaker(IDOK));
	menu.Separator();
	menu.Add("About..", THISBACK(About));
}

void Bombs::Game(Bar& menu)
{
	menu.Add("Restart", THISBACK(Generate));
	menu.Separator();
	menu.Add("Easy", THISBACK1(Level, Size(10, 10)))
	    .Check(level.cx == 10);
	menu.Add("Medium", THISBACK1(Level, Size(15, 15)))
	    .Check(level.cx == 15);
	menu.Add("Difficult", THISBACK1(Level, Size(25, 20)))
	    .Check(level.cx == 25);
}

void Bombs::Menu(Bar& menu)
{
	menu.Add("File", THISBACK(File));
	menu.Add("Game", THISBACK(Game));
}

#define IMAGECLASS BombsImg
#define IMAGEFILE  <Bombs/bombs.iml>
#include <Draw/iml.h>

Bombs::Bombs()
{
	UNIT = DPI(30);
	level = Size(10, 10);
	AddFrame(menu);
	menu.Set(THISBACK(Menu));
	AddFrame(status);
	AddFrame(InsetFrame());
	Title("uBombs");
	Icon(BombsImg::Small());
	Generate();
}

GUI_APP_MAIN
{
	Bombs().Run();
}
