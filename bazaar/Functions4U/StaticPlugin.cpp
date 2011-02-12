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
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == name) {
			Plugins()[i].Delete(data);
			return;
		}
	}
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == "") {
			Plugins()[i].Delete(data);
			return;
		}
	}	
}
	
bool StaticPlugin::Init(const char *_name) {
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == _name) {
			data = Plugins()[i].New();
			name = _name;
			return true;
		}
	}
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == "") {
			data = Plugins()[i].New();
			return false;
		}
	}
	return false;
}

