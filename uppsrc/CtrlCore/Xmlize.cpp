#include "CtrlCore.h"

NAMESPACE_UPP

void Ctrl::Xmlize(XmlIO xml)
{
	Value v = GetData();
	UPP::Xmlize(xml, v);
	SetData(v);
}

END_UPP_NAMESPACE
