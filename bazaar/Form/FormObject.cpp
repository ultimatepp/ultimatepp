#include "FormObject.hpp"

FormObject::FormObject() : _Rect(Rect(Point(10, 10), Size(120, 30))), _HAlign(Ctrl::LEFT),
	_VAlign(Ctrl::TOP), _State(FormObject::NONE)
{
	Name = "Noname";
	Set("Variable", "None");
	Set("Frame", "Default frame");
}

FormObject::FormObject(const FormObject& other)
{
	*this   = other;
	_Rect   = other.GetRect();
	_State  = other.GetState();
	_HAlign = other.GetHAlign();
	_VAlign = other.GetVAlign();
}

FormObject::FormObject(const Rect& r)
	: _Rect(r), _HAlign(Ctrl::LEFT), _VAlign(Ctrl::TOP), _State(FormObject::NONE)
{
	Name = "Noname";
	Set("Variable", "None");
	Set("Frame", "Default frame");
}

FormObject::FormObject(int x, int y, int cx, int cy) : _Rect(Rect(Point(x, y), Size(cx, cy))),
	_HAlign(Ctrl::LEFT), _VAlign(Ctrl::TOP), _State(FormObject::NONE)
{
	Name = "Noname";
	Set("Variable", "None");
	Set("Frame", "Default frame");
}

void FormObject::Xmlize(XmlIO xml)
{
	xml.Attr("x", _Rect.left).Attr("y", _Rect.top);

	int cx = _Rect.Width();
	int cy = _Rect.Height();

	xml.Attr("cx", cx).Attr("cy", cy);

	if (xml.IsLoading())
	{
		_Rect.right = _Rect.left + cx;
		_Rect.bottom = _Rect.top + cy;
	}

	xml.Attr("align", _HAlign).Attr("valign", _VAlign);
	XMLConfig::Xmlize(xml);
}
