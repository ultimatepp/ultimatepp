#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

//a dirty hack to expose protected virtuals
//more see bellow
class PusherExpose : public Pusher
{
public:
	typedef Pusher P;

#if 0 // only worked for gcc, without the cast hack bellow
	using P::RefreshPush;
	using P::RefreshFocus;
	using P::PerformAction;
#endif

	inline virtual void RefreshPush() { return P::RefreshPush(); }
	inline virtual void RefreshFocus() { return P::RefreshFocus(); }
	inline virtual void PerformAction() { return P::PerformAction(); }

	inline void KeyPush() { P::KeyPush(); }
	inline bool IsPush() const { return P::IsPush(); }
	inline bool IsKeyPush() { return P::IsKeyPush(); }
	inline bool FinishPush() { return P::FinishPush(); }
};

class PusherWrap : public Pusher, public wrapper<Pusher> {
public:
	typedef Pusher C;

	virtual void CancelMode() { if(override f = this->get_override("cancelmode")) f(); C::CancelMode(); }
	void default_CancelMode() { C::CancelMode(); }

	virtual void LeftDown(Point p, dword keyflags) { if(override f = this->get_override("leftdown")) f(p, keyflags); C::LeftDown(p, keyflags); }
	void default_LeftDown(Point p, dword keyflags) { C::LeftDown(p, keyflags); }

	virtual void MouseMove(Point p, dword keyflags) { if(override f = this->get_override("mousemove")) f(p, keyflags); C::MouseMove(p, keyflags); }
	void default_MouseMove(Point p, dword keyflags) { C::MouseMove(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	virtual void LeftRepeat(Point p, dword keyflags) { if(override f = this->get_override("leftrepeat")) f(p, keyflags); C::LeftRepeat(p, keyflags); }
	void default_LeftRepeat(Point p, dword keyflags) { C::LeftRepeat(p, keyflags); }

	virtual void LeftUp(Point p, dword keyflags) { if(override f = this->get_override("leftup")) f(p, keyflags); C::LeftUp(p, keyflags); }
	void default_LeftUp(Point p, dword keyflags) { C::LeftUp(p, keyflags); }

	virtual void GotFocus() { if(override f = this->get_override("gotfocus")) f(); C::GotFocus(); }
	void default_GotFocus() { C::GotFocus(); }

	virtual void LostFocus() { if(override f = this->get_override("lostfocus")) f(); C::LostFocus(); }
	void default_LostFocus() { C::LostFocus(); }

	virtual void State(int reason) { if(override f = this->get_override("state")) f(reason); C::State(reason); }
	void default_State(int reason) { C::State(reason); }

	virtual String GetDesc() const { if(override f = this->get_override("getdesc")) return f(); return C::GetDesc(); }
	String default_GetDesc() const { return C::GetDesc(); }

	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(key, count); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual dword GetAccessKeys() const { if(override f = this->get_override("getaccesskeys")) return f(); return C::GetAccessKeys(); }
	dword default_GetAccessKeys() const { return C::GetAccessKeys(); }

	virtual void AssignAccessKeys(dword key) { if(override f = this->get_override("assignaccesskeys")) f(key); C::AssignAccessKeys(key); }
	void default_AssignAccessKeys(dword key) { C::AssignAccessKeys(key); }

	virtual bool HotKey(dword key) { if(override f = this->get_override("hotkey")) return f(key); return C::HotKey(key); }
	bool default_HotKey(dword key) { return C::HotKey(key); }

	//own
	virtual void RefreshPush() { if(override f = this->get_override("refreshpush")) f(); C::RefreshPush(); }
	void default_RefreshPush() { C::RefreshPush(); }

	virtual void RefreshFocus() { if(override f = this->get_override("refreshfocus")) f(); C::RefreshFocus(); }
	void default_RefreshFocus() { C::RefreshFocus(); }

	virtual void PerformAction() { if(override f = this->get_override("performaction")) f(); C::PerformAction(); }
	void default_PerformAction() { C::PerformAction(); }
};

void PusherSetLabel(Pusher& c, const String& s) { c.SetLabel(s); }
void PusherClickFocus(Pusher& c, bool b) { c.ClickFocus(b); }

void export_Pusher()
{
ONCELOCK
{
	class_<PusherWrap, bases<Ctrl>, boost::noncopyable>("Pusher", "Upp Pusher")
		.def("cancelmode", &Pusher::CancelMode, &PusherWrap::default_CancelMode)
		.def("leftdown", &Pusher::LeftDown, &PusherWrap::default_LeftDown)
		.def("mousemove", &Pusher::MouseMove, &PusherWrap::default_MouseMove)
		.def("mouseleave", &Pusher::MouseLeave, &PusherWrap::default_MouseLeave)
		.def("leftrepeat", &Pusher::LeftRepeat, &PusherWrap::default_LeftRepeat)
		.def("leftup", &Pusher::LeftUp, &PusherWrap::default_LeftUp)
		.def("gotfocus", &Pusher::GotFocus, &PusherWrap::default_GotFocus)
		.def("lostfocus", &Pusher::LostFocus, &PusherWrap::default_LostFocus)
		.def("state", &Pusher::State, &PusherWrap::default_State)
		.def("key", &Pusher::Key, &PusherWrap::default_Key)
		.def("getaccesskeys", &Pusher::GetAccessKeys, &PusherWrap::default_GetAccessKeys)
		.def("assignaccesskeys", &Pusher::AssignAccessKeys, &PusherWrap::default_AssignAccessKeys)
		.def("hotkey", &Pusher::HotKey, &PusherWrap::default_HotKey)

		.def("refreshpush", (void (Pusher::*)())&PusherExpose::RefreshPush, &PusherWrap::default_RefreshPush)
		.def("refreshfocus", (void (Pusher::*)())&PusherExpose::RefreshFocus, &PusherWrap::default_RefreshFocus)
		.def("performaction", (void (Pusher::*)())&PusherExpose::PerformAction, &PusherWrap::default_PerformAction)

		.def("keypush", (void (Pusher::*)())&PusherExpose::KeyPush)
		.def("ispush", (bool (Pusher::* const)())&PusherExpose::IsPush)
		.def("iskeypush", (bool (Pusher::*)())&PusherExpose::IsKeyPush)
		.def("finishpush", (bool (Pusher::*)())&PusherExpose::FinishPush)

		.add_property("clickfocus", &Pusher::IsClickFocus, &PusherClickFocus)
		.add_property("label", &Pusher::GetLabel, &PusherSetLabel)

		.def("pseudopush", &Pusher::PseudoPush)
		.def("getvisualstate", &Pusher::GetVisualState)
		
		.def_readwrite("whenpush", &Pusher::WhenPush)
		.def_readwrite("whenrepeat", &Pusher::WhenRepeat)
	;
}
}

END_UPP_NAMESPACE
