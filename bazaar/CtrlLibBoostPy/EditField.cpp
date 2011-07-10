#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

class EditFieldWrap : public EditField, public wrapper<EditField> {
public:
	typedef EditField C;

	virtual void SetData(const Value& data) { if(override f = this->get_override("setdata")) f(data); C::SetData(data); }
	void default_SetData(const Value& data) { C::SetData(data); }

	virtual Value GetData() const { if(override f = this->get_override("getdata")) return f(); return C::GetData(); }
	Value default_GetData() const { return C::GetData(); }

	virtual void CancelMode() { if(override f = this->get_override("cancelmode")) f(); C::CancelMode(); }
	void default_CancelMode() { C::CancelMode(); }

	virtual void MouseEnter(Point p, dword keyflags) { if(override f = this->get_override("mouseenter")) f(p, keyflags); C::MouseEnter(p, keyflags); }
	void default_MouseEnter(Point p, dword keyflags) { C::MouseEnter(p, keyflags); }

	virtual void MouseMove(Point p, dword keyflags) { if(override f = this->get_override("mousemove")) f(p, keyflags); C::MouseMove(p, keyflags); }
	void default_MouseMove(Point p, dword keyflags) { C::MouseMove(p, keyflags); }

	virtual void LeftDown(Point p, dword keyflags) { if(override f = this->get_override("leftdown")) f(p, keyflags); C::LeftDown(p, keyflags); }
	void default_LeftDown(Point p, dword keyflags) { C::LeftDown(p, keyflags); }

	virtual void LeftDouble(Point p, dword keyflags) { if(override f = this->get_override("leftdouble")) f(p, keyflags); C::LeftDouble(p, keyflags); }
	void default_LeftDouble(Point p, dword keyflags) { C::LeftDouble(p, keyflags); }

	virtual void LeftTriple(Point p, dword keyflags) { if(override f = this->get_override("lefttriple")) f(p, keyflags); C::LeftTriple(p, keyflags); }
	void default_LeftTriple(Point p, dword keyflags) { C::LeftTriple(p, keyflags); }

	virtual void LeftDrag(Point p, dword keyflags) { if(override f = this->get_override("leftdrag")) f(p, keyflags); C::LeftDrag(p, keyflags); }
	void default_LeftDrag(Point p, dword keyflags) { C::LeftDrag(p, keyflags); }

	virtual void LeftUp(Point p, dword keyflags) { if(override f = this->get_override("leftup")) f(p, keyflags); C::LeftUp(p, keyflags); }
	void default_LeftUp(Point p, dword keyflags) { C::LeftUp(p, keyflags); }

	virtual void RightDown(Point p, dword keyflags) { if(override f = this->get_override("rightdown")) f(p, keyflags); C::RightDown(p, keyflags); }
	void default_RightDown(Point p, dword keyflags) { C::RightDown(p, keyflags); }

	virtual void MiddleDown(Point p, dword keyflags) { if(override f = this->get_override("middledown")) f(p, keyflags); C::MiddleDown(p, keyflags); }
	void default_MiddleDown(Point p, dword keyflags) { C::MiddleDown(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(key, count); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual void GotFocus() { if(override f = this->get_override("gotfocus")) f(); C::GotFocus(); }
	void default_GotFocus() { C::GotFocus(); }

	virtual void LostFocus() { if(override f = this->get_override("lostfocus")) f(); C::LostFocus(); }
	void default_LostFocus() { C::LostFocus(); }

	virtual void Layout() { if(override f = this->get_override("layout")) f(); C::Layout(); }
	void default_Layout() { C::Layout(); }

	virtual Size GetMinSize() const { if(override f = this->get_override("getminsize")) return f(); return C::GetMinSize(); }
	Size default_GetMinSize() const { return C::GetMinSize(); }
};

void EditFieldPassword(EditField& c, bool pwd) { c.Password(pwd); }
void EditFieldAutoFormat(EditField& c, bool b) { c.AutoFormat(b); }
void EditFieldClickSelect(EditField& c, bool b) { c.ClickSelect(b); }
void EditFieldInitCaps(EditField& c, bool b) { c.InitCaps(b); }
void EditFieldMaxChars(EditField& c, int mc) { c.MaxChars(mc); }
void EditFieldNoBackground(EditField& c, bool b) { c.NoBackground(b); }
void EditFieldAlignRight(EditField& c, bool b) { c.AlignRight(b); }

void export_EditField()
{
ONCELOCK
{
	class_<EditField, bases<Ctrl>, boost::noncopyable>("EditField", "Upp EditField")

		.def("setdata", &EditField::SetData, &EditFieldWrap::default_SetData)
		.def("getdata", &EditField::GetData, &EditFieldWrap::default_GetData)
		.def("cancelmode", &EditField::CancelMode, &EditFieldWrap::default_CancelMode)
		.def("mouseenter", &EditField::MouseEnter, &EditFieldWrap::default_MouseEnter)
		.def("mousemove", &EditField::MouseMove, &EditFieldWrap::default_MouseMove)
		.def("leftdown", &EditField::LeftDown, &EditFieldWrap::default_LeftDown)
		.def("leftdouble", &EditField::LeftDouble, &EditFieldWrap::default_LeftDouble)
		.def("lefttriple", &EditField::LeftTriple, &EditFieldWrap::default_LeftTriple)
		.def("leftdrag", &EditField::LeftDrag, &EditFieldWrap::default_LeftDrag)
		.def("leftup", &EditField::LeftUp, &EditFieldWrap::default_LeftUp)
		.def("rightdown", &EditField::RightDown, &EditFieldWrap::default_RightDown)
		.def("middledown", &EditField::MiddleDown, &EditFieldWrap::default_MiddleDown)
		.def("mouseleave", &EditField::MouseLeave, &EditFieldWrap::default_MouseLeave)
		.def("key", &EditField::Key, &EditFieldWrap::default_Key)
		.def("gotfocus", &EditField::GotFocus, &EditFieldWrap::default_GotFocus)
		.def("lostfocus", &EditField::LostFocus, &EditFieldWrap::default_LostFocus)
		.def("layout", &EditField::Layout, &EditFieldWrap::default_Layout)
		.def("getminsize", &EditField::GetMinSize, &EditFieldWrap::default_GetMinSize)

		.def("insert", (int (EditField::*)(int, const String&))&EditField::Insert)
		.def("move", &EditField::Move)
		.def("remove", &EditField::Remove)
		.def("setselection", &EditField::SetSelection)
		.def("getselection", &EditField::GetSelection)
		.def("isselection", &EditField::IsSelection)
		.def("removeselection", &EditField::RemoveSelection)
		.def("cancelselection", &EditField::CancelSelection)

		.def("copy", &EditField::Copy)
		.def("undo", &EditField::Undo)
		.def("cut", &EditField::Cut)
		.def("paste", &EditField::Paste)
		.def("erase", &EditField::Erase)
		.def("selectall", &EditField::SelectAll)

		.def_readwrite("whenenter", &EditField::WhenEnter)

		.add_property("text", &EditField::operator String, (void (EditField::*)(const String&))&EditField::SetText)

		.def("getlength", &EditField::GetLength)
		.def("getchar", &EditField::GetChar)

		.def("getraretrect", (Rect (EditField::*)() const)&EditField::GetCaretRect)
		.def("clear", &EditField::Clear)
		.def("reset", &EditField::Reset)

		.def("error", &EditField::Error)

		.add_property("password", &EditField::IsPassword, &EditFieldPassword)
		.add_property("autoformat", &EditField::IsAutoFormat, &EditFieldAutoFormat)
		.add_property("clickselect", &EditField::IsClickSelect, &EditFieldClickSelect)
		.add_property("initcaps", &EditField::IsInitCaps, &EditFieldInitCaps)
		.add_property("maxchars", &EditField::GetMaxChars, &EditFieldMaxChars)
		.add_property("nobackground", &EditField::IsNoBackground, &EditFieldNoBackground)
		.add_property("alignright", &EditField::IsAlignRight, &EditFieldAlignRight)
	;
}
}

END_UPP_NAMESPACE
