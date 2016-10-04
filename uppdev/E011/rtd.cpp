#include "E011.h"

RealTimeD::RealTimeD() {

	CtrlLayoutCancel(*this, "Real time display");
  return;
}

void RealTimeD::Perform() {

	SetTimeCallback(-250, THISBACK(RealTime), 0);
	Run();
	KillTimeCallback(0);
}

void RealTimeD::RealTime() {
	int i;
	bool r;

	r = true;
	i = SEP1_ReadVar(2);
	if(i >= 0) rt_level = Format("%04X", (int)i);
	else r = "";
	i = SEP1_ReadVar(0);
	if(i >= 0) fd_level = Format("%04X", (int)i);
	else r = "";
	if( SEP1_ReadFlash(9) == 1 ) {
		i = SEP1_ReadVar(1);
		if(i >= 0) display = i;
		else r = false;
	}
	else display = "-";

	if(!r) {
		port.disconnect();
		KillTimeCallback(0);
		Exclamation("Problema di comunicazione. Il collegamento è stato interrotto.");
	}
	return;
}
