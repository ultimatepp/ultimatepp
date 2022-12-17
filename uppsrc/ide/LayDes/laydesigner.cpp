#include "LayDes.h"

LayDesigner::LayDesigner(const String& hlStyles)
{
	designer.hlStyles = hlStyles;
	parent.Add(designer.DesignerCtrl().SizePos());
}
