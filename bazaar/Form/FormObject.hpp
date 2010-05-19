#ifndef FORM_EDITOR_HPP
#define FORM_EDITOR_HPP

#include <CtrlCore/CtrlCore.h>
using namespace Upp;

#include "XMLConfig.h"
#include "Macro.hpp"

class FormObject : public XMLConfig, Moveable<FormObject>
{
	typedef FormObject CLASSNAME;

	PARAMETER(Rect,  Rect);
	PARAMETER(dword, State);
	PARAMETER(dword, HAlign);
	PARAMETER(dword, VAlign);

public:
	FormObject();
	FormObject(const FormObject& other);
	FormObject(const Rect& r);
	FormObject(int x, int y, int cx, int cy);

	void Xmlize(XmlIO xml);
	Size GetSize() const { return _Rect.GetSize(); }
	bool Intersects(const FormObject& obj) { return _Rect.Intersects(obj.GetRect()); }
	int  GetCenterX() const { return _Rect.left + _Rect.Width()  / 2; }
	int  GetCenterY() const { return _Rect.top  + _Rect.Height() / 2; }

	bool IsSelected() { return _State == FormObject::SELECTED; }
	enum { NONE, SELECTED };
};

#endif // .. FORM_EDITOR_HPP
