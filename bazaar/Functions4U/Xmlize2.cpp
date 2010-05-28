#ifdef flagGUI

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <Functions4U/Functions4U.h>
#include <Functions4U/Functions4U_Gui.h>

//#include "Xmlize2.h"

NAMESPACE_UPP

template <> void Xmlize(XmlIO xml, GridCtrl& r) {
	Vector<Vector<Value> > data;
	
	if(xml.IsLoading()) {
		xml("data", data);
		SetGridData(r, data);
	} else {
		data = GetGridData(r);
		xml("data", data);
	}
}

END_UPP_NAMESPACE

#endif