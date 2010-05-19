#ifndef FORM_LAYOUT_HPP
#define FORM_LAYOUT_HPP

#include "FormObject.hpp"

class FormLayout : public XMLConfig, public Moveable<FormLayout>
{
	typedef FormLayout CLASSNAME;

	PARAMETER(Rect, PageRect);
	PARAMETER(Vector<FormObject>, Objects);

public:
	FormLayout();

	FormLayout(const FormLayout& other)
	{
		*this <<= other;
	}

	FormLayout& operator<<=(const FormLayout& other)
	{
		WhenProgressBar = other.WhenProgressBar;
		_PageRect  = other.GetPageRect();
		_Objects <<= other.GetObjects();
		Name  = other.Name;
		Path  = other.Path;
		Map <<= other.Map;
		return *this;
	}

	void Clear();
	Size GetFormSize();
	virtual void Xmlize(XmlIO xml);
};

#endif // .. FORM_LAYOUT_HPP
