#include "TCtrlLibCalc.h"
#pragma hdrstop

NAMESPACE_UPP

#define LAYOUTFILE <TCtrlLib/Calc/DlgCalc.lay>
#include           <CtrlCore/lay.h>

class DlgCalc : public WithCalcLayout<TopWindow>
{
public:
	typedef DlgCalc CLASSNAME;
	DlgCalc();

	static ConfigItem& config();
	void        Serialize(Stream& stream);
	void        Run(CalcContext& context);

private:
	void        CmdStore();

private:
	CalcContext      *calc_context;
	Value             result_val;
};

CONFIG_ITEM(DlgCalc::config, "DlgCalc", 1, 1, 1)

void RunDlgCalc(CalcContext& context) { DlgCalc().Run(context); }

RegisterHelpTopicObjectTitle(DlgCalc, t_("Calculator"))

DlgCalc::DlgCalc()
{
	calc_context = NULL;
	CtrlLayoutOKCancel(*this, DlgCalcHelpTitle());
	HelpTopic("DlgCalc");
	result.SetReadOnly();
	result.SetConvert(CFormatConvert());
	store <<= THISBACK(CmdStore);
}

void DlgCalc::Serialize(Stream& stream)
{
	exp.SerializeList(stream);
}

void DlgCalc::Run(CalcContext& context)
{
	calc_context = &context;
	ReadConfigSelf();
	CalcContext::Nesting level(context);
	while(TopWindow::Run() == IDOK) {
		try {
			WaitCursor wait;
			CalcNodePtr node = CalcParser().Scan((String)~exp);
			result_val = node -> Calc(context);
			result <<= context.FormatNull(result_val);
			exp.AddHistory();
		}
		catch(Exc e) {
			result <<= e;
			BeepExclamation();
		}
		exp.SetFocus();
		exp.SetSelection();
	}
	WriteConfigSelf();
	calc_context = NULL;
}

void DlgCalc::CmdStore()
{
	WithCalcStoreLayout<TopWindow> stdlg;
	CtrlLayoutOKCancel(stdlg, NFormat("Store %vt", result_val));
	stdlg.var.SetFilter(CharFilterCalcIdent);
	stdlg.list.AddColumn("Variable");
	stdlg.list.AddColumn("Value");
	const CalcSymbols& tos = calc_context->stack.Top();
	Vector<int> order = GetSortOrder(tos.var_index);
	for(int i = 0; i < order.GetCount(); i++) {
		int o = order[i];
		stdlg.list.Add(tos.var_index[o], tos.var_value[o]);
	}
	if(stdlg.Run() == IDOK)
		calc_context->Set(~stdlg.var, result_val, true);
}

END_UPP_NAMESPACE
