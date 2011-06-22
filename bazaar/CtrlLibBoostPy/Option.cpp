#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void OptionBlackEdge(Option& o, bool b) { o.BlackEdge(b); }
void OptionSwitchImage(Option& o, bool b) { o.SwitchImage(b); }
void OptionThreeState(Option& o, bool b) { o.ThreeState(b); }
void OptionShowLabel(Option& o, bool b) { o.ShowLabel(b); }
void OptionNotNull(Option& o, bool b) { o.NotNull(b); }

void export_Option()
{
ONCELOCK
{
	class_<Option, bases<Pusher>, boost::noncopyable>("Option", "Upp Option")
		.add_property("blackedge", &Option::IsBlackEdge, &OptionBlackEdge)
		.add_property("switchimage", &Option::IsSwitchImage, &OptionSwitchImage)
		.add_property("threestate", &Option::IsThreeState, &OptionThreeState)
		.add_property("showlabel", &Option::IsShowLabel, &OptionShowLabel)
		.add_property("notnull", &Option::IsNotNull, &OptionNotNull)
	;
}
}

END_UPP_NAMESPACE
