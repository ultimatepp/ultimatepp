#include "PropEdit.h"

NAMESPACE_UPP

void PropEditCtrl::UpdateCtrl()
{
	ClearMap();
	am.Clear();
	if(!ctrl) return;

	//init local accessor map
	//prefer the dynamic access
	//fall back to the static accessors from Props<>
	GetAccessorMapI* ami = dynamic_cast<GetAccessorMapI*>(ctrl);
	if(ami) am <<= ami->GetAccessorMap();
	else if(!Props<Ctrl>::SetupAccessorMap(*ctrl, am)) return;

	SetMap(&am);

	for(int i = 0; i < avae.GetCount(); i++)
	{
		ValueAccessorEdit& c = avae[i];
		if(ValueAccessorCtrl* vac = dynamic_cast<ValueAccessorCtrl*>(&c))
		{
			//LogPosCtrl needs instance infos to live show changes
			if(LogPosCtrl* ple = dynamic_cast<LogPosCtrl*>(~vac->oc))
				ple->Set(*ctrl);
		}
	}
}

void PropEditCtrl::OnAction()
{
	if(!ctrl) return;
	ctrl->UpdateActionRefresh(); //propagate user action
}

//

PropEdit::PropEdit()
{
	cktype = CKOKCANCEL;
	SetRect(pec.GetRect());
	Add(pec.HSizePos().VSizePos(0,20));
	//WhenDeactivate = THISBACK1(RejectBreak, int(IDCANCEL));
}

END_UPP_NAMESPACE
