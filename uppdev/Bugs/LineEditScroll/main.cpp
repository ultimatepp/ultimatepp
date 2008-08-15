#include "LineEditScroll.h"

class ScrollBugWindow : public TopWindow {
public:
	typedef ScrollBugWindow CLASSNAME;
	ScrollBugWindow();

private:
	void Animate();

private:
	LineEdit editor;
	Vector<char> matrix;
	enum { WIDTH = 80, HEIGHT = 160 };
	Point pos;
	int sync_ticks;
	TimeCallback tc_anim;
};

ScrollBugWindow::ScrollBugWindow()
{
	matrix.SetCount(WIDTH * HEIGHT, ' ');
	pos = Point(0, 0);
	sync_ticks = msecs();
	Add(editor.SizePos());
	Sizeable().Zoomable();
	Title("Line editor scrolling bug");
	tc_anim.Set(-10, THISBACK(Animate));
}

void ScrollBugWindow::Animate()
{
	String rowfmt = NFormat("Row #%d: ", pos.y + 1);
	rowfmt.Cat('*', pos.y + 1);
	rowfmt << " \\";
	matrix[pos.y * WIDTH + pos.x] = (pos.x < rowfmt.GetLength() ? rowfmt[pos.x] : ' ');
	if(pos.x & 1) {
		if(--pos.y < 0) {
			pos.y = 0;
			pos.x++;
		}
	}
	else {
		if(++pos.y >= HEIGHT) {
			pos.y = HEIGHT - 1;
			pos.x++;
		}
	}
	if(pos.x >= WIDTH) {
		Fill(matrix.Begin(), matrix.End(), ' ');
		pos = Point(0, 0);
	}
	if(msecs(sync_ticks) >= 500) {
		String text;
		for(int i = 0; i < HEIGHT; i++) {
			text.Cat(matrix.GetIter(i * WIDTH), WIDTH);
			text.Cat('\n');
		}
		editor <<= text;
		sync_ticks = msecs();
	}
}

GUI_APP_MAIN
{
	ScrollBugWindow().Run();
}
