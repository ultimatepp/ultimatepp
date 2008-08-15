#include "Circles.h"

void CircleView::UpdateView(const Rect& r)
{
	Rect br = doc.GetBoundingRect();
	Refresh(r.Offseted(-(Point)sb));
	sb.SetTotal(br.right, br.bottom);
	SetBar();
}

void CircleView::Scroll()
{
	Refresh();
}

void CircleView::Layout()
{
	sb.SetPage(sb.GetReducedViewSize());
}

void CircleView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorPaper);
	Point p = -(Point)sb;
	for(int i = 0; i < doc.GetCount(); i++) {
		DUMP(doc[i].GetRect());
		w.DrawEllipse(doc[i].GetRect().Offseted(p), Blue, 1, Black);
	}
}

void CircleView::LeftDown(Point p, dword)
{
	doc.AddCircle(p + sb, 50);
}

void CircleView::RightDown(Point p, dword)
{
	p += sb;
	for(int i = 0; i < doc.GetCount(); i++)
		if(doc[i].GetRect().Contains(p)) {
			doc.RemoveCircle(i);
			return;
		}
}

void CircleView::Close()
{
	doc.RemoveView(this);
}

void CircleView::FileBar(Bar& bar)
{
	bar.Add("Open new view", callback(&doc, &CircleDoc::NewView));
}

void CircleView::EditBar(Bar& bar)
{
	bar.Add(doc.IsUndo(), "Undo", CtrlImg::undo(), callback(&doc, &CircleDoc::Undo));
	bar.Add(doc.IsRedo(), "Redo", CtrlImg::redo(), callback(&doc, &CircleDoc::Redo));
}

void CircleView::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileBar));
	bar.Add("Edit", THISBACK(EditBar));
}

void CircleView::MainBar(Bar& bar)
{
	FileBar(bar);
	bar.Separator();
	EditBar(bar);
}

void CircleView::SetBar()
{
	tool.Set(THISBACK(MainBar));
}

CircleView::CircleView(CircleDoc& doc)
	: doc(doc)
{
	AddFrame(menu);
	AddFrame(tool);
	AddFrame(FieldFrame());
	AddFrame(sb);
	sb = Point(0, 0);
	sb.WhenScroll = THISBACK(Scroll);
	Sizeable().Zoomable();
	menu.Set(THISBACK(MainMenu));
	SetBar();
}
