#ifndef FORM_HPP
#define FORM_HPP

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#include "Container.hpp"
#include "FormLayout.hpp"

class Form : public TopWindow
{
	typedef Form CLASSNAME;

public:
	 Form();
	~Form();

	bool Load(const String& file);
	bool Layout(const String& layout, Font font = StdFont());
	bool Generate(Font font = StdFont());

	bool Exit(const String& action);
	bool Acceptor(const String& action);
	bool Rejector(const String& action);

	Ctrl* GetCtrl(const String& var);
	Value GetData(const String& var);

	String ExecuteForm();

	String Script;
	Callback3<const String&, const String&, const String& > SignalHandler;

	ArrayMap<String, Ctrl>& GetCtrls() { return _Ctrls; }
	const ArrayMap<String, Ctrl>& GetCtrls() const { return _Ctrls; }

	void Clear(bool all = true);

	bool IsLayout() { return _Current != -1; }
	int HasLayout(const String& layout);

	void Xmlize(XmlIO xml) { xml("layouts", _Layouts); }

	Vector<FormLayout>& GetLayouts() { return _Layouts; }
	const Vector<FormLayout>& GetLayouts() const { return _Layouts; }

protected:
	void OnAction(const String& action);
	bool SetCallback(const String& action, Callback c);

	Vector<String> _Acceptors;
	Vector<String> _Rejectors;
	Vector<FormLayout> _Layouts;
	ArrayMap<String, Ctrl> _Ctrls;

private:
	int _Current;
	String _File;
};

#endif // .. FORM_HPP
