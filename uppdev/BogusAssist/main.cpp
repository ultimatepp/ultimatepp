#include <CtrlLib/CtrlLib.h>

template <class T, class S>
struct ATesting : public Moveable<S> {
	void Assign(const ATesting& s);
	void Assign(const T *s, int len);
	void DeepAssign(const ATesting& s);
};

class Testing : public ATesting<char, Testing> {
	typedef ATesting<char, Testing> B;

	Testing(Data *data)                         { ptr = CreateEmpty(data); }
	Testing(int, char *p) : B(0, p)             {}

	Testing Mid(int pos, int length) const      { return Testing(0, B::Mid(pos, length)); }
};

GUI_APP_MAIN
{
	MyHello().Run();
}
