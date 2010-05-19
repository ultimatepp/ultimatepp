#include "FormView.hpp"

void FormView::CreateObject(Point p, const char* type)
{
	if (!IsLayout())
		return;

	p = DeZoom(p);

	Rect pageRect = GetPageRect();
	Rect objRect;

	if (GetBool("Grid.Binding", true))
		PointToGrid(p);

	if (AsString(type) == "Button")
		objRect = Rect(Point(p.x - pageRect.left, p.y - pageRect.top),
			Size(GetNumber("Grid.CX", 10) * 10, GetNumber("Grid.CY", 10) * 3)
		);
	else
		objRect = Rect(Point(p.x - pageRect.left, p.y - pageRect.top),
			Size(GetNumber("Grid.CX", 10) * 10, GetNumber("Grid.CY", 10) * 2)
		);

	pageRect = Deoffseted(GetPageRect());
	if (objRect.right  > pageRect.right)  objRect.Offset(- objRect.right + pageRect.right, 0);
	if (objRect.bottom > pageRect.bottom) objRect.Offset(0, - objRect.bottom + pageRect.bottom);

	FormObject obj(objRect);
	obj.Set("Type", type);

	if (AsString(type) == "EditField")
		obj.Set("TextAlign", "Left");

	GetObjects()->Add(obj);

	ClearSelection();
	AddToSelection(GetObjectCount() - 1);

	Refresh();
	WhenUpdate.Execute();
	WhenChildSelected.Execute(GetSelected());
	WhenChildCount.Execute(GetObjectCount());
}

void FormView::SetSprings(dword hAlign, dword vAlign)
{
	if (!IsLayout())
		return;

	Vector<int> sel = GetSelected();
	for (int i = 0; i < sel.GetCount(); ++i)
	{
		FormObject* pI = GetObject(sel[i]);
		if (!pI) continue;

		if (hAlign != -1) pI->SetHAlign(_HAlign = hAlign);
		if (vAlign != -1) pI->SetVAlign(_VAlign = vAlign);
	}

	WhenUpdate.Execute();
	Refresh();
}

void FormView::RemoveSelection()
{
	if (!IsLayout())
		return;

	Vector<int> sel = GetSelected();
	Sort(sel);

	for (int i = sel.GetCount() - 1; i >= 0; --i)
		GetObjects()->Remove(sel[i]);

	Refresh();
	WhenChildCount.Execute(GetObjectCount());
}

void FormView::MoveUpObject(int id)
{
	if (!IsLayout())
		return;

	if (id < 0 || id >= GetObjectCount() - 1)
		return;

	GetObjects()->Swap(id, id + 1);
	Refresh();
	WhenChildZ.Execute();
}

void FormView::MoveDownObject(int id)
{
	if (!IsLayout())
		return;

	if (id <= 0 || id >= GetObjectCount())
		return;

	GetObjects()->Swap(id, id - 1);
	Refresh();
	WhenChildZ.Execute();
}

void FormView::MoveToTopObject(int id)
{
	if (!IsLayout())
		return;

	if (id < 0 || id >= GetObjectCount() - 1)
		return;

	for (int i = id; i < GetObjectCount() - 1; ++i)
		GetObjects()->Swap(i, i + 1);
	Refresh();
	WhenChildZ.Execute();
}

void FormView::MoveToBottomObject(int id)
{
	if (!IsLayout())
		return;

	if (id <= 0 || id >= GetObjectCount())
		return;

	for (int i = id; i > 0; --i)
		GetObjects()->Swap(i, i - 1);
	Refresh();
	WhenChildZ.Execute();
}

void FormView::InvertSelection()
{
	if (!IsLayout())
		return;

	for (int i = 0; i < GetObjectCount(); ++i)
	{
		FormObject* p = &(*GetObjects())[i];
		p->GetState() == FormObject::SELECTED
		   ? p->SetState(FormObject::NONE)
		   : p->SetState(FormObject::SELECTED);
	}
	Refresh();
}

void FormView::AlignTopSelection()
{
	if (!IsLayout())
		return;

	int min = Deoffseted(GetSelectionRect()).top;
	Vector<int> sel = GetSelected();

	for (int i = 0; i < sel.GetCount(); ++i)
	{
		FormObject* p = GetObject(sel[i]);
		if (!p) continue;
		p->SetRect( Rect(Point(p->GetRect().left, min), p->GetSize()) );
	}

	WhenChildPos.Execute(sel);
}

void FormView::AlignLeftSelection()
{
	if (!IsLayout())
		return;

	int min = Deoffseted(GetSelectionRect()).left;
	Vector<int> sel = GetSelected();

	for (int i = 0; i < sel.GetCount(); ++i)
	{
		FormObject* p = GetObject(sel[i]);
		if (!p) continue;
		p->SetRect( Rect(Point(min, p->GetRect().top), p->GetSize()) );
	}

	WhenChildPos.Execute(sel);
}

void FormView::AlignRightSelection()
{
	if (!IsLayout())
		return;

	int min = Deoffseted(GetSelectionRect()).right;
	Vector<int> sel = GetSelected();

	for (int i = 0; i < sel.GetCount(); ++i)
	{
		FormObject* p = GetObject(sel[i]);
		if (!p) continue;
		p->SetRect( Rect(Point(min - p->GetRect().Width(), p->GetRect().top), p->GetSize()) );
	}

	WhenChildPos.Execute(sel);
}

void FormView::AlignBottomSelection()
{
	if (!IsLayout())
		return;

	int min = Deoffseted(GetSelectionRect()).bottom;
	Vector<int> sel = GetSelected();

	for (int i = 0; i < sel.GetCount(); ++i)
	{
		FormObject* p = GetObject(sel[i]);
		if (!p) continue;
		p->SetRect( Rect(Point(p->GetRect().left, min - p->GetRect().Height()), p->GetSize()) );
	}

	WhenChildPos.Execute(sel);
}

void FormView::ToggleOutlineDraw(int obj)
{
	if (!IsLayout())
		return;

	FormObject* pI = GetObject(obj);
	if (!pI) return;

	pI->SetBool("OutlineDraw", !pI->GetBool("OutlineDraw", false));
	WhenChildZ.Execute();
}

bool FormView::IsOutlineDraw(int obj)
{
	if (!IsLayout())
		return false;

	FormObject* pI = GetObject(obj);
	if (!pI) return false;

	return pI->GetBool("OutlineDraw", false);
}
