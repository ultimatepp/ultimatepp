#include "FormView.hpp"

FormView::FormView()
{
	TransparentBackPaint();

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				_colors << Color(i * 64 - 1, j * 64 - 1, k * 64 - 1);

	_objectResize = false;
	_frameResize = false;

	_showInfo  = 0;
	_leftCur   = 0;
	_topCur    = 0;
	_rightCur  = 0;
	_bottomCur = 0;

	SetHAlign(-1);
	SetVAlign(-1);

	_Layout = -1;
	_container = NULL;

	SetFont(StdFont());

	_toolTop[0]    = FormViewImg::ToolTop();
	_toolTop[1]    = FormViewImg::ToolTopH();
	_toolTop[2]    = FormViewImg::ToolTopP();
	_toolLeft[0]   = FormViewImg::ToolLeft();
	_toolLeft[1]   = FormViewImg::ToolLeftH();
	_toolLeft[2]   = FormViewImg::ToolLeftP();
	_toolRight[0]  = FormViewImg::ToolRight();
	_toolRight[1]  = FormViewImg::ToolRightH();
	_toolRight[2]  = FormViewImg::ToolRightP();
	_toolBottom[0] = FormViewImg::ToolBottom();
	_toolBottom[1] = FormViewImg::ToolBottomH();
	_toolBottom[2] = FormViewImg::ToolBottomP();

	_cursor = OverrideCursor(Image());
	OverrideCursor(_cursor);

	SetBool("View.Coloring", false);
	SetBool("Grid.Visible", true);
	SetBool("Grid.Binding", true);
	SetNumber("Grid.CX", 10);
	SetNumber("Grid.CY", 10);
}

void FormView::New()
{
	_Layouts.Clear();
	_Layouts.Add().Set("Form.Name", t_("Default"));
	SelectLayout(0);

	WhenUpdateLayouts.Execute();
}

void FormView::Xmlize(XmlIO xml)
{
	for (int i = 0; i < GetLayoutCount(); ++i)
		GetLayout(i)->Compression(IsCompressed());
	xml("layouts", GetLayouts());
}

bool FormView::SaveAll(const String& file, bool compression)
{
	if (compression)
	{
		String s = StoreAsXML(*this, "form");
		return SaveFile(file, ZCompress(s));
	}
	return StoreAsXMLFile(*this, "form", file);
}

bool FormView::LoadAll(const String& file, bool compression)
{
	if (compression)
	{
		String s;
		s = ZDecompress(LoadFile(file));
		return LoadFromXML(*this, s);
	}
	return LoadFromXMLFile(*this, file);
}

void FormView::Layout()
{
	FormLayout *l = GetCurrentLayout();
	if (!l) return;
	
	SetPageRect(
		Rect(Point(GetPageRect().left, GetPageRect().top),
			Size(l->GetNumber("Form.Width",  400),
			     l->GetNumber("Form.Height", 300)))
	);
}

FormObject* FormView::GetObject(int id)
{
	if (!IsLayout())
		return NULL;

	if (id < 0 || id >= GetObjectCount())
		return NULL;

	return &((*GetObjects())[id]);
}

int FormView::ObjectFromPt(Point p)
{
	if (!IsLayout()) return -1;

	for (int i = GetObjectCount() - 1; i >= 0; --i)
		if (Zoom(Offseted( (*GetObjects())[i].GetRect() )).Contains(p))
			return i;
	return -1;
}

bool FormView::IsSelected(int id)
{
	if (!IsLayout())
		return false;

	if (id < 0 || id >= GetObjectCount())
		return false;
	
	return (*GetObjects())[id].GetState() == FormObject::SELECTED;
}

Vector<int> FormView::GetSelected()
{
	Vector<int> result;
	
	if (!IsLayout())
		return result;

	for (int i = GetObjects()->GetCount() - 1; i >= 0; --i)
		if ((*GetObjects())[i].GetState() == FormObject::SELECTED) result << i;
	return result;
}

int FormView::GetSelectedCount()
{
	if (!IsLayout())
		return 0;

	int count = 0;
	for (int i = GetObjects()->GetCount() - 1; i >= 0; --i)
		if ((*GetObjects())[i].GetState() == FormObject::SELECTED) count++;
	return count;
}

Rect FormView::GetObjectsRect()
{
	if (!IsLayout() || !GetObjectCount())
		return Offseted(Rect(Point(0, 0), GetGridSize()));

	Rect r;

	for (int i = 0; i < GetObjectCount(); i++)
	{
		FormObject *pI = GetObject(i);
		if (!pI) continue;

		Rect s = pI->GetRect();
		if (i == 0) r = s;

		if (r.top    > s.top)    r.top    = s.top;
		if (r.left   > s.left)   r.left   = s.left;
		if (r.right  < s.right)  r.right  = s.right;
		if (r.bottom < s.bottom) r.bottom = s.bottom;
	}

	return Offseted(r);
}

Rect FormView::GetSelectionRect()
{
	Rect r;
	if (!IsLayout()) return Offseted(r);
	Vector<int> sel = GetSelected();

	for (int i = 0; i < sel.GetCount(); i++)
	{
		FormObject *pI = GetObject( sel[i] );
		if (!pI) continue;

		Rect s = pI->GetRect();
		if (i == 0) r = s;

		if (r.top    > s.top)    r.top    = s.top;
		if (r.left   > s.left)   r.left   = s.left;
		if (r.right  < s.right)  r.right  = s.right;
		if (r.bottom < s.bottom) r.bottom = s.bottom;
	}

	return Offseted(r);
}

void FormView::AddToSelection(int id)
{
	if (!IsLayout())
		return;

	FormObject *pI = GetObject(id);
	if (!pI) return;

	pI->SetState(FormObject::SELECTED);
	Refresh();
}

void FormView::ToggleSelection(int id)
{
	if (!IsLayout())
		return;

	FormObject *pI = GetObject(id);
	if (!pI) return;

	pI->GetState() == FormObject::SELECTED
		? pI->SetState(FormObject::NONE)
		: pI->SetState(FormObject::SELECTED);

	Refresh();
}

void FormView::ClearSelection()
{
	if (!IsLayout())
		return;

	for (int i = GetObjects()->GetCount() - 1; i >= 0; --i)
		(*GetObjects())[i].SetState(FormObject::NONE);
	WhenUpdate.Execute();
	Refresh();
}

void FormView::SelectAllInRect(Rect r, bool add, bool toggle)
{
	if (!IsLayout())
		return;

	if (!add) ClearSelection();

	for (int i = GetObjectCount() - 1; i >= 0; --i)
		if (r.Contains(Zoom(Offseted(GetObject(i)->GetRect()))))
			toggle ? ToggleSelection(i) : AddToSelection(i);

	Refresh();
}

int FormView::HasLayout(const String& layout)
{
	for (int i = 0; i < _Layouts.GetCount(); ++i)
		if (_Layouts[i].Get("Form.Name") == layout)
			return i;
	return -1;
}
