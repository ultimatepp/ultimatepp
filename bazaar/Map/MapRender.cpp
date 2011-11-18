#include "MapLevel.h"

void MapLevel::Paint(Draw& w)
{
	w.DrawRect(Ctrl::GetSize(), SColorPaper());
	Render(&w);
}

void MapLevel::Render(Draw* w)
{
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i] && _layers[i]->IsShown())
			_layers[i]->Render(w);
	IMapRender::Render(w);
	RenderInfo(w);

	if (IsEditMode())
		for (int i = 0; i < _editModePoints.GetCount(); ++i)
		{
			Point p = ViewToScene(_editModePoints[i]);
			Rect  r(Point(p.x - 4, p.y - 4), Size(8, 8));

			w->DrawRect(r, Black());
			r.Deflate(1, 1);

			w->DrawRect(r, LtRed());
		}
}

void RenderText(Draw* w, Point pos, const char* text)
{
	Size sz = GetTextSize(text, StdFont());
	Rect r(pos, sz + Size(8, 8));
	w->DrawRect(r, Black());
	r.Deflate(1, 1);
	w->DrawRect(r, White());
	w->DrawText(pos.x + 4, pos.y + 4, text);
}

void MapLevel::RenderInfo(Draw* w)
{
	if (!IsEditMode())
		return;

//	RenderText(w, Point(100, 11), "Pos: " + GetMousePos().ToString());
}

void MapLevel::RefreshView()
{
	Refresh();
}
