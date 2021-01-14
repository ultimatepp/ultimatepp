#include <Ole/Ctrl/OleCtrl.h>
#include <CtrlLib/CtrlLib.h>
#include <Esc/Esc.h>

#include "calc_idl.h"

using namespace Upp;

#define LAYOUTFILE "calc.lay"
#include <CtrlCore/lay.h>

class Calculator : public WithCalculatorLayout<OcxRunnableControl>, public DispatchInterface<ICalculator> {
	void Eval();

public:
	virtual void Paint(UPP::Draw& w);

	STDMETHOD(get_Input)(BSTR *result);
	STDMETHOD(put_Input)(BSTR input);
	STDMETHOD(Calculate)();
	STDMETHOD(get_Output)(BSTR *result);

	typedef Calculator CLASSNAME;
	Calculator();
};

OCX_OBJECT(Calculator)

void Calculator::Paint(UPP::Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
}

Calculator::Calculator()
{
	CtrlLayout(*this);
	ok.Ok() <<= THISBACK(Eval);
	ok.Tip("Evaluate given expression");
	NoWantFocus();
}

void Calculator::Eval()
{
	ArrayMap<String, EscValue> global;
	value <<= Evaluate((String)~expr, global).ToString();
}

HRESULT Calculator::get_Input(BSTR *result)
{
	ReturnString(result, ~expr);
	return S_OK;
}

HRESULT Calculator::put_Input(BSTR input)
{
	expr <<= BSTRToString(input);
	return S_OK;
}

HRESULT Calculator::Calculate()
{
	Eval();
	return S_OK;
}

HRESULT Calculator::get_Output(BSTR *result)
{
	ReturnString(result, ~value);
	return S_OK;
}

OCX_APP_MAIN
{
	SetLanguage(LNG_CZECH);
	SetDefaultCharset(CHARSET_WIN1250);
}
