#include <Core/Core.h>

using namespace Upp;

#include "StaticPlugin.h"


Array<StaticPlugin::PluginData>& StaticPlugin::Plugins() {
	static Array<PluginData> x;
	return x;
};

StaticPlugin::StaticPlugin() {
	data = 0;
}
StaticPlugin::~StaticPlugin() {
	End();
}

void StaticPlugin::End() {
	if (!data)
		return;
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == name && Plugins()[i].type == type && Plugins()[i].instance == instance) {
			Plugins()[i].Delete(data);
			data = 0;
			return;
		}
	}
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == "" && Plugins()[i].type == type && Plugins()[i].instance == instance) {
			Plugins()[i].Delete(data);
			data = 0;
			return;
		}
	}	
}




