#include "FormView.hpp"
#include "ScrollContainer.hpp"

void FormView::LeftDouble(Point p, dword keyflags)
{
	int obj = ObjectFromPt(p);
	if (obj >= 0)
	{
		FormObject* pI = GetObject(obj);
		if (pI->Get("Type") == "TabCtrl")
		{
			int	active =  pI->GetNumber("Tab.Active", -1, -1) + 1;
			if (active == pI->GetNumber("Tab.Count", 0, 0)) active = 0;

			pI->SetNumber("Tab.Active", active);
			WhenUpdateTabs.Execute();
		}
	}

	if (GetSelected().GetCount())
		WhenObjectProperties.Execute(GetSelected());
}

void FormView::LeftDown(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	dword tool = IsGroupButton(p);

	if (keyflags & K_SHIFT)
	switch(tool)
	{
		case TOOL_LEFT:
			_leftCur = 2;
			Refresh();
			AlignLeftSelection();
			return;
		case TOOL_TOP:
			_topCur = 2;
			Refresh();
			AlignTopSelection();
			return;
		case TOOL_RIGHT:
			_rightCur = 2;
			Refresh();
			AlignRightSelection();
			return;
		case TOOL_BOTTOM:
			_bottomCur = 2;
			Refresh();
			AlignBottomSelection();
			return;
	}

	int obj = ObjectFromPt(p);

	if (GetSelectionRect().Contains(p) && (GetSelectedCount() > 1) && (!(keyflags & K_CTRL))
		&& (!(keyflags & K_SHIFT)))
	{
		WhenChildSelected.Execute(GetSelected());
		WhenUpdate.Execute();
		return;
	}

	_objectResize = IsObjectButton(p);
	if (!_objectResize)
	{
		if (keyflags & K_SHIFT)
		{
			int id = ObjectFromPt(p);
			if (id < 0) { WhenUpdate.Execute(); return Refresh(); }
			AddToSelection(id);
		}
		else if (keyflags & K_CTRL)
		{
			int id = ObjectFromPt(p);
			if (id < 0) { WhenUpdate.Execute(); return Refresh(); }
			ToggleSelection(id);
		}
		else
		{
			if (Zoom(Offseted(GetSelectionRect().Offseted(-20, -20))).Contains(p))
			{
				WhenChildSelected.Execute(GetSelected());
				return;
			}

			ClearSelection();
			AddToSelection(obj);
			WhenUpdate.Execute();
			IsObjectButton(p);
		}
		WhenChildSelected.Execute(GetSelected());
		return;
	}

	WhenChildSelected.Execute(GetSelected());
	WhenUpdate.Execute();
}

void FormView::MouseMove(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	if (!IsGroupButton(p))
		IsObjectButton(p);
	Ctrl::MouseMove(p, keyflags);
}

void FormView::LeftUp(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	IsGroupButton(p);
	Ctrl::LeftUp(p, keyflags);
}

void FormView::LeftDrag(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	if (ProcessLeftDrag(p, keyflags))
		WhenChildPos.Execute(GetSelected());
	WhenUpdate.Execute();
}

bool FormView::ProcessLeftDrag(Point p, dword keyflags)
{
	if (!IsLayout())
		return false;

	RectTracker tr(*this);
	Rect r;
	Rect s = GetSelectionRect();

	Vector<int> sel = GetSelected();

	if (sel.GetCount() > 1 && Zoom(s).Contains(p))
	{
		int dx = p.x - (s.left + s.Width() / 2);
		int dy = p.y - (s.top + s.Height() / 2);

		r = Rect(Point(p.x - s.Width() / 2, p.y - s.Height() / 2), s.GetSize());
		r.Offset(-dx, -dy);

		tr.Dashed().Animation();
		Rect backup = r;
		r = DeZoom(tr.Track(Zoom(r), ALIGN_CENTER, ALIGN_CENTER));

		if (GetBool("Grid.Binding", true))
			RectToGrid(r);

		Rect pageRect = GetPageRect();

		if (r.right > pageRect.right) r.Offset(- r.right + pageRect.right, 0);
		if (r.bottom > pageRect.bottom) r.Offset(0, - r.bottom + pageRect.bottom);

		if (r.left < X(0)) { r = Rect( Point(X(0), r.TopLeft().y), r.GetSize() ); }
		if (r.top  < Y(0)) { r = Rect( Point(r.TopLeft().x, Y(0)), r.GetSize() ); }

		dx = r.left - s.left;
		dy = r.top - s.top;

		bool result = false;
		for (int i = 0; i < sel.GetCount(); ++i)
		{
			FormObject* obj = GetObject(sel[i]);
			if (!obj) continue;
			
			Rect t = obj->GetRect();
			t.Offset(dx, dy);
			obj->SetRect(Rect(Point(t.left, t.top), Size(t.Width(), t.Height())));
			result = true;
		}

		WhenUpdate.Execute();
		Refresh();
		return result;
	}

	if (_frameResize)
	{
		r = Zoom(GetPageRect());
		tr.Dashed().Animation();
		Rect r2 = tr.Track(r, ALIGN_RIGHT, ALIGN_BOTTOM);	

		Size g = Zoom(GetGridSize());
		if (r2.Width()  < g.cx) r2.SetSize(g.cx, r2.GetSize().cy);
		if (r2.Height() < g.cy) r2.SetSize(r2.GetSize().cx, g.cy);
		r2 = DeZoom(r2);

		if (GetBool("Grid.Binding", true))
			RectToGrid(r2);

		if (GetObjectCount() > 0)
		{
			Rect objRect = Deoffseted(GetObjectsRect());

			if (r2.Width() < objRect.left + objRect.Width())
				r2.SetSize(objRect.left + objRect.Width(), r2.GetSize().cy);
			if (r2.Height() < objRect.top + objRect.Height())
				r2.SetSize(r2.GetSize().cx, objRect.top + objRect.Height());
		}

		SetFormSize( Size(r2.Width(), r2.Height()) );
		Refresh();
		WhenUpdate.Execute();
		WhenChildZ.Execute();

		return true;
	}

	if (keyflags & K_SHIFT || keyflags & K_CTRL || sel.GetCount() == 0)
	{
		tr.Dashed();
		r = tr.Track(RectC(p.x, p.y, 0, 0), ALIGN_NULL, ALIGN_NULL);
		if (keyflags & K_SHIFT)
			SelectAllInRect(r, true);
		else if (keyflags & K_CTRL)
			SelectAllInRect(r, true, true);
		else
			SelectAllInRect(r);
		WhenChildSelected.Execute(GetSelected());
		WhenUpdate.Execute();
		return false;
	}

	if (sel.GetCount() == 0 || sel.GetCount() > 1) return false;

	Rect oldRect;
	FormObject* obj = GetObject(sel[0]);
	if (!obj) { WhenChildSelected.Execute(GetSelected()); return false; }

	oldRect = r = Zoom(Offseted(obj->GetRect()));

	Size sz = GetSize();
	if (_objectResize)
	{
		tr.Dashed().Animation();
		r = tr.Track(r, ALIGN_RIGHT, ALIGN_BOTTOM);
	}
	else
	{
		tr.Dashed().Animation();
		r = tr.Track(r, ALIGN_CENTER, ALIGN_CENTER);
	}

	r = DeZoom(r);

	if (GetBool("Grid.Binding", true))
		RectToGrid(r);

	if (!_objectResize)
		r.SetSize(obj->GetRect().Width(), obj->GetRect().Height());

	if (r.GetWidth()  < 20) r.SetSize(20, obj->GetRect().Height());
	if (r.GetHeight() < 20) r.SetSize(obj->GetRect().Width(), 20);

	Rect pageRect = GetPageRect();

	if (r.right > pageRect.right) r.Offset(- r.right + pageRect.right, 0);
	if (r.bottom > pageRect.bottom) r.Offset(0, - r.bottom + pageRect.bottom);

	if (r.left < X(0)) { r = Rect( Point(X(0), r.TopLeft().y), r.GetSize() ); }
	if (r.top  < Y(0)) { r = Rect( Point(r.TopLeft().x, Y(0)), r.GetSize() ); }

	obj->SetRect(Deoffseted(r));
	Refresh();
	WhenChildSelected.Execute(GetSelected());
	return true;
}

void FormView::RightDown(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	if (keyflags & K_CTRL || keyflags & K_SHIFT)
	{
//		UpdateTools();
//		_popup.SetRect( Rect(GetRect().Offseted(p).TopLeft(), Size(200, 60) ) );
//		_popup.AddFrame(_tools);
//		if(!_popup.IsPopUp())
//			_popup.PopUp(this,false,true,false,true);
		return;
	}

	if (p == Point(-1, -1))
	{
		ClearSelection();
		Refresh();
		WhenChildSelected.Execute(GetSelected());
		return;
	}

	OverrideCursor(_cursor);
	MenuBar menu;

	int id = ObjectFromPt(p);
	if (id < 0)
	{
		ClearSelection();
		PointToGrid(p);
		Refresh();
		WhenChildSelected.Execute(GetSelected());
		menu.Add(t_("Add object..."), THISBACK1(AddObjectMenu, p));
		menu.Execute();
		return;
	}
	else
	{
		if (!IsSelected(id))
		{
			ClearSelection();
			AddToSelection(id);
			Refresh();
			WhenUpdate.Execute();
		}
	}

	Vector<int> sel = GetSelected();

	if (!sel.GetCount())
	{
		AddToSelection(id);
		Refresh();
	}

	if (keyflags & K_CTRL)
		return;

	CreateObjectMenu(menu, id);

	menu.Execute();
	Refresh();
}

void FormView::RightDrag(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	ProcessRightDrag(p, keyflags);
	WhenChildPos.Execute(GetSelected());
}

void FormView::ProcessRightDrag(Point p, dword keyflags)
{
	if (!IsLayout())
		return;

	if (!(keyflags & K_CTRL || keyflags & K_SHIFT))
		return;

	Vector<int> sel = GetSelected();

	if (sel.GetCount() == 0)
		return;

	Rect s = GetSelectionRect();

	int dx = p.x - (s.left + s.Width() / 2);
	int dy = p.y - (s.top + s.Height() / 2);

	Rect r = Rect(Point(p.x - s.Width() / 2, p.y - s.Height() / 2), s.GetSize());
	r.Offset(-dx, -dy);
	RectTracker tr(*this);

	tr.Dashed().Animation();
	r = DeZoom(tr.Track(Zoom(r), ALIGN_CENTER, ALIGN_CENTER));

	if (GetBool("Grid.Binding", true))
		RectToGrid(r);

	Rect pageRect = GetPageRect();
	if (r.right > pageRect.right) r.Offset(- r.right + pageRect.right, 0);
	if (r.bottom > pageRect.bottom) r.Offset(0, - r.bottom + pageRect.bottom);

	dx = r.left - s.left;
	dy = r.top - s.top;

	Vector<int> copies;

	for (int i = sel.GetCount() - 1; i >= 0; --i)
	{
		FormObject* pI = GetObject(sel[i]);
		if (!pI) continue;

		Rect t = pI->GetRect();
		t.Offset(dx, dy);
		FormObject obj(*pI);
		obj.SetRect(RectC(t.left, t.top, t.Width(), t.Height()));
		copies << GetObjectCount();
		GetObjects()->Add(obj);
	}

	ClearSelection();
	for (int i = 0; i < copies.GetCount(); ++i)
		AddToSelection( copies[i] );
	WhenUpdate.Execute();
	WhenChildSelected.Execute(GetSelected());
	WhenChildCount.Execute(GetObjectCount());
}

void FormView::MouseLeave()
{
	OverrideCursor(_cursor);
}
