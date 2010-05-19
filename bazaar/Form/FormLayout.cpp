#include "FormLayout.hpp"

FormLayout::FormLayout()
{
	SetNumber("Form.Width",  400);
	SetNumber("Form.Height", 300);

	SetBool("Form.MaximizeBox", false);
	SetBool("Form.MinimizeBox", false);
	SetBool("Form.Sizeable",    false);
	SetBool("Form.ToolWindow",  false);

	Set("Form.Name", t_("Noname"));

	SetPageRect( Rect(Point(10, 10), Size(400, 300)) );
}

Size FormLayout::GetFormSize()
{
	return Size(GetNumber("Form.Width",  400),
				GetNumber("Form.Height", 400));
}

void FormLayout::Xmlize(XmlIO xml)
{
	xml("content", _Objects);
	XMLConfig::Xmlize(xml);
}

void FormLayout::Clear()
{
	XMLConfig::Clear();
	GetObjects().Clear();
}
