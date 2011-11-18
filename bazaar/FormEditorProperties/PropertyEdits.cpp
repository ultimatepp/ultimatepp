#include "PropertyEdits.h"

#define TEXT_DEFAULT_LEFT  5
#define TEXT_DEFAULT_RIGHT 120
#define EDIT_DEFAULT_LEFT  120
#define EDIT_DEFAULT_RIGHT 5

// Edit field for text
CEditField::CEditField()
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.WhenAction = THISBACK(Notify);
}

CEditField::CEditField(int (*filter)(int))
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.WhenAction = THISBACK(Notify);
	_Edit.SetFilter(filter);
}

void CEditField::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Edit.GetStdSize().cy);
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

// Button
CButton::CButton(const char* label, const Callback& call)
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);

	_Edit.SetLabel(label);
	_Edit.WhenPush = call;
}

void CButton::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, EditField().GetStdSize().cy);
	_Edit.TopPosZ(cy, EditField().GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

// Edit field for numbers
CEditIntSpin::CEditIntSpin(int min, int max)
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.Min(min).Max(max);
	_Edit.WhenAction = THISBACK(Notify);
}

void CEditIntSpin::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Edit.GetStdSize().cy);
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

void CEditIntSpin::SetData(const Value& data)
{
	if (data.GetType() == INT_V)
		_Edit.SetData(data);
	else
		_Edit.SetData(10);
}

// Edit field for double-type
CEditDouble::CEditDouble(double min, double max)
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.Min(min).Max(max);
	_Edit.WhenAction = THISBACK(Notify);
}

void CEditDouble::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Edit.GetStdSize().cy);
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

void CEditDouble::SetData(const Value& data)
{
	_Edit.SetData(ScanDouble(data.ToString()));
}

// Edit field for boolean
CEditBool::CEditBool()
{
	_Edit.HSizePosZ(10, 10);
	_Edit.WhenAction = THISBACK(Notify);
}

void CEditBool::Set(Ctrl& c, int& cy)
{
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Edit);
}

void CEditBool::SetData(const Value& data)
{
	if (data.GetType() == BOOL_V || data.GetType() == INT_V)
		_Edit.Set(data);
}

// Dropping list
CDropList::CDropList(const VectorMapEx<String, Value>& list, int defaultIndex)
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit .HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit .WhenAction = THISBACK(Notify);

	for (int i = 0; i < list.GetCount(); ++i)
		_Edit.Add(list.GetKey(i), list[i]);
	if (_Edit.GetCount()) _Edit.SetIndex(defaultIndex);
}

void CDropList::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Edit.GetStdSize().cy);
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

void CDropList::SetData(const Value& data)
{
	bool found = false;
	for (int i = 0; i < _Edit.GetCount(); ++i)
		if (_Edit.GetValue(i) == data)
		{
			_Edit.SetIndex(i);
			found = true;
			break;
		}
	if (!found)
		_Edit.SetData(data);
}

// Edit field for text
CEditColor::CEditColor()
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.WhenAction = THISBACK(Notify);
}

void CEditColor::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Edit.GetStdSize().cy);
	_Edit.TopPosZ(cy, _Edit.GetStdSize().cy);
	cy += _Edit.GetStdSize().cy;
	c.Add(_Label);
	c.Add(_Edit);
}

// Edit field for simple doc
CDocEdit::CDocEdit()
{
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Edit.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Edit.WhenAction = THISBACK(Notify);
}

void CDocEdit::Set(Ctrl& c, int& cy)
{
	_Label.TopPosZ(cy, _Label.GetStdSize().cy);
	_Edit.TopPosZ(cy, 100);
	cy += 100;
	c.Add(_Label);
	c.Add(_Edit);
}

// Edit field for size
CEditSize::CEditSize(int min, int max)
{
	_CX.Min(min).Max(max);
	_CY.Min(min).Max(max);
	_CX.WhenAction = THISBACK(Notify);
	_CY.WhenAction = THISBACK(Notify);
	_Label.HSizePosZ(TEXT_DEFAULT_LEFT, TEXT_DEFAULT_RIGHT);
	_Size.HSizePosZ(EDIT_DEFAULT_LEFT, EDIT_DEFAULT_RIGHT);
	_Size.Add(_CX.SizePos());
	_Size.Add(_CY.SizePos());
}

void CEditSize::Set(Ctrl& c, int& cy)
{
	c.Add(_Label.TopPosZ(cy, _CY.GetStdSize().cy));
	c.Add(_Size.TopPosZ(cy, _CY.GetStdSize().cy));

	cy += _CY.GetStdSize().cy;
}

void CEditSize::SetData(const Value& data)
{
	if (data.GetType() == SIZE_V)
	{
		Size sz = data;
		_CX.SetData(sz.cx);
		_CY.SetData(sz.cy);
	}
}

// Edit field for point
void CEditPoint::SetData(const Value& data)
{
	if (data.GetType() == POINT_V)
	{
		Point p = data;
		_CX.SetData(p.x);
		_CY.SetData(p.y);
	}
}
